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
* File Name :    r_osal_dev_cfg_control.h
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL device configuration for eMCOS header file
***********************************************************************************************************************/

#ifndef OSAL_DEV_CFG_CONTROL_H
#define OSAL_DEV_CFG_CONTROL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <pthread.h>
#include <mcos/uring.h>

#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
#include "rcar-xos/memory_allocator/r_meml.h"

#include "target/emcos/pma/r_pma.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Public_Defines_DevConfig_control Device Configurator Public macro definitions (control)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_INVALID_ID
 * Invalid Device ID.
***********************************************************************************************************************/
#define OSAL_DEVICE_INVALID_ID          (NULL)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_DEVICE_MAX_NUM
 * Max value of the device control table.
***********************************************************************************************************************/
#define OSAL_DCFG_DEVICE_MAX_NUM        (384)

/*******************************************************************************************************************//**
 * @def OSAL_AXI_BUS_NAME_MAX_LENGTH
 * Length of the AXI bus name.
***********************************************************************************************************************/
#define OSAL_AXI_BUS_NAME_MAX_LENGTH    (31UL)

/*******************************************************************************************************************//**
 * @def OSAL_DCFG_PDEV_GROUP_MAX_NUM
 * Max value of the pdev entry group name.
***********************************************************************************************************************/
#define OSAL_DCFG_PDEV_GROUP_MAX_NUM    (31UL)

/*******************************************************************************************************************//**
 * @def OSAL_BUS_DOMAIN_SIZE
 * Size of the AXI bus domain.
***********************************************************************************************************************/
#define OSAL_BUS_DOMAIN_SIZE            (32U)

/*******************************************************************************************************************//**
 * @def OSAL_DOMAIN_UTLB_MAX_STRINGS
 * Lenght of Domain and Utlb strings(AXI bus name).
***********************************************************************************************************************/
#define OSAL_DOMAIN_UTLB_MAX_STRINGS    (224U)

/*******************************************************************************************************************//**
 * @def OSAL_AXI_BUS_NAME_LAST
 * Value indicating the last data.
***********************************************************************************************************************/
#define OSAL_AXI_BUS_NAME_LAST          (-1)

/*******************************************************************************************************************//**
 * @def OSAL_INVALID_UTLB
 * uTLB Invalid value
***********************************************************************************************************************/
#define OSAL_INVALID_UTLB               (-2)

/*******************************************************************************************************************//**
 * @def OSAL_UTLB_ASSIGN_MAX_NUM
 * Max value of the uTLB assignment number.
***********************************************************************************************************************/
#define OSAL_UTLB_ASSIGN_MAX_NUM        (64)

/*******************************************************************************************************************//**
 * @def OSAL_MEMORY_INNER_TIMEOUT
 * Value for timeout used in memory manager internal exclusive control.
***********************************************************************************************************************/
#define OSAL_MEMORY_INNER_TIMEOUT       (500L)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_MEM_4K
 * Minimum size of Continuous Memory.
***********************************************************************************************************************/
#define OSAL_MMNGR_MEM_4K               (4U * 1024U)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_GUARD_PAGE_SIZE
 * Guard area size.
***********************************************************************************************************************/
#define OSAL_MMNGR_GUARD_PAGE_SIZE      (4096U)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_GUARD_PAGE_PATTERN
 * Pattern value to be set in the guard area.
***********************************************************************************************************************/
#define OSAL_MMNGR_GUARD_PAGE_PATTERN   (0xAAU)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_BUFFER_UNDERRUN
 * Error bit value when underflow occurs.
***********************************************************************************************************************/
#define OSAL_MMNGR_BUFFER_UNDERRUN      (1U << 0)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_BUFFER_OVERRUN
 * Error bit value when overflow occurs.
***********************************************************************************************************************/
#define OSAL_MMNGR_BUFFER_OVERRUN       (1U << 1)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_MEM_RESERVE_SIZE
 * Reserve area size of Continuous Memory.
 * (((Guard Page Size * 2) + Alignment-adjusted Library Management Size) * Max Allowed Allocations)
***********************************************************************************************************************/
#define OSAL_MMNGR_MEM_RESERVE_SIZE     (((4096U * 2U) + 4096U) * 1024U)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_GB_UNIT
 * Gigabyte unit.
***********************************************************************************************************************/
#define OSAL_MMNGR_GB_UNIT              (1024 * 1024 * 1024)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_MB_UNIT
 * Megabyte unit.
***********************************************************************************************************************/
#define OSAL_MMNGR_MB_UNIT              (1024 * 1024)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_KB_UNIT
 * kilobyte unit.
***********************************************************************************************************************/
#define OSAL_MMNGR_KB_UNIT              (1024)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_DEVICE_NAME
 * device name.
***********************************************************************************************************************/
#define OSAL_MMNGR_DEVICE_NAME          "/pdev/mem"

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_HANDLE_ID
 * Handle id for device.
***********************************************************************************************************************/
#define OSAL_DEVICE_HANDLE_ID    (0x4F53414C5F444556ULL) /* ascii code: OSAL_DEV */

/*******************************************************************************************************************//**
 * @def OSAL_MEMORY_HANDLE_ID
 * Handle id for memory manager object.
***********************************************************************************************************************/
#define OSAL_MEMORY_HANDLE_ID    (0x4F53414C4D4E4752ULL) /* ascii code: OSALMNGR */

/*******************************************************************************************************************//**
 * @def OSAL_BUFFER_HANDLE_ID
 * Handle id for memory buffer object.
***********************************************************************************************************************/
#define OSAL_BUFFER_HANDLE_ID    (0x4F53414C42554646ULL) /* ascii code: OSALBUFF */

/*******************************************************************************************************************//**
 * @def OSAL_INVALID_DEVICE_HANDLE_ID
 * Invalid Device Handle ID
***********************************************************************************************************************/
#define OSAL_INVALID_DEVICE_HANDLE_ID   (0U)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_STATE_NONE
 * Memory Manager status none.
***********************************************************************************************************************/
#define OSAL_MMNGR_STATE_NONE              (0U)

/*******************************************************************************************************************//**
 * @def OSAL_MMNGR_STATE_INIT
 * Memory Manager status initialized.
***********************************************************************************************************************/
#define OSAL_MMNGR_STATE_INIT              (1U)

/*******************************************************************************************************************//**
 * @def OSAL_POWER_MS_SLEEP_TIME
 * Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_POWER_MS_SLEEP_TIME     (1)

/*******************************************************************************************************************//**
 * @def OSAL_POWER_MS_SLEEP_RETRY
 * Sleep time[ms].
***********************************************************************************************************************/
#define OSAL_POWER_MS_SLEEP_RETRY     (1000)

/*******************************************************************************************************************//**
 * @def R_PMA_HWA_ID_NONE
 * PMA Non ID.
***********************************************************************************************************************/
#if !defined(R_PMA_HWA_ID_NONE)
#define R_PMA_HWA_ID_NONE   (R_PMA_HWA_ID_NUM)
#endif

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Public_Enum_DevConfig_info Device Configurator Public Enumration (info)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_osal_pm_postckcr_t
 * PM POSTCKCR definitions.
***********************************************************************************************************************/
typedef enum e_osal_pm_postckcr
{
    OSAL_PM_POSTCKCR  = 0,     /*!< Power-On Self Test Clock Frequency Control Register   (POSTCKCR)  */
    OSAL_PM_POST2CKCR = 2,     /*!< Power-On Self Test 2 Clock Frequency Control Register (POST2CKCR) */
    OSAL_PM_POST3CKCR = 3,     /*!< Power-On Self Test 3 Clock Frequency Control Register (POST3CKCR) */
    OSAL_PM_POST4CKCR = 4,     /*!< Power-On Self Test 4 Clock Frequency Control Register (POST4CKCR) */
    OSAL_PM_POSTCKCR_INVALID,
} e_osal_pm_postckcr_t;

/*******************************************************************************************************************//**
 * @enum e_osal_mem_mmap_phase_t
 * Memory Manager Open mmap phase ID.
***********************************************************************************************************************/
typedef enum e_osal_mem_mmap_phase
{
    OSAL_MEM_MMAP_PHASE_DEVCTL,      /*!< posix_devctl() phase failed. */
    OSAL_MEM_MMAP_PHASE_GETMAX,      /*!< R_OSAL_DCFG_GetMaxConfigOfMemory() phase failed. */
    OSAL_MEM_MMAP_PHASE_MMAP,        /*!< mmap() phase failed. */
    OSAL_MEM_MMAP_PHASE_MEML_INIT,   /*!< Memory Allocator Library Initialize phase failed. */
    OSAL_MEM_MMAP_PHASE_CLOSE,       /*!< close() phase failed. */
    OSAL_MEM_MMAP_PHASE_INVALID,
} e_osal_mem_mmap_phase_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Public_Types_DevConfig_control Device Configurator Public type definitions (control)
 *
 * @{
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_interrupt_info_t
 * structure to hold interrupt information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_interrupt_info
{
    uint64_t    irq_number;     /*!< IRQ number */

}st_osal_interrupt_info_t;

/*******************************************************************************************************************//**
 * @struct st_osal_utlb_assignment_info_t
 * structure to hold utlb assignment information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_utlb_assignment_info
{
    uint32_t    table_size;                         /*!< The size of the assignment number table */
    int32_t     table[OSAL_UTLB_ASSIGN_MAX_NUM];    /*!< The assignment number table */
}st_osal_utlb_assignment_info_t;

/*******************************************************************************************************************//**
 * @struct st_osal_device_info_t
 * structure to hold device information
 * XOS3_OSAL_EMCOS_CD_CD_IO_039
 * [Covers: XOS3_OSAL_EMCOS_UD_DD_IO_065]
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_device_info
{
    const char                       *pdev_group;       /*!< pdev group name */
    const char                       *id;               /*!< Device of unique ID */
    const char                       *type;             /*!< Device type */

    uintptr_t                        address;             /*!< Start address of register */
    size_t                           range;               /*!< Register address range */

    st_osal_interrupt_info_t         *interrupt;          /*!< Array of Interrupt information of the device */
    size_t                           irq_channels;        /*!< Number of interrupts */

    e_pma_hwa_id_t                   pma_id;            /*!< ID that uniquely represents the Power Manager of the device */
    e_osal_pm_policy_t               initial_policy;    /*!< Initial policy of Power Manager of the device */
    e_osal_pm_postckcr_t             postckcr;          /*!< POSTCKCR of Power Manager of the device */

    const char                       *axi_bus_name;     /*!< AXI bus name associated with the device */
    st_osal_utlb_assignment_info_t   *utlb_assignment;  /*!< A pointer to the uTLB assignment */

}st_osal_device_info_t;


/*******************************************************************************************************************//**
 * @struct st_osal_device_control_t
 * structure to control device.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_device_control
{
    uint64_t                         handle_id;             /*!< Fixed value: OSAL_DEVICE_HANDLE_ID */
    bool                             active;                /*!< The device handle active flag (true: opened device handle, false: closed device handle) */

    pthread_mutex_t                  inner_mtx;             /*!< Internal mutex object */
    osal_milli_sec_t                 inner_timeout;         /*!< Waiting time of lock inner_mtx */

    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                              fd;                    /*!< File descriptor */
    uintptr_t                        base_addr;             /*!< A process's address space that mapped a device's physical address */
    uint32_t                         len;                   /*!< Length of the device's physical address */
    const struct st_osal_device_info *dev_info;             /*!< Device configuration */
    int32_t                          *interrupt_index;      /*!< Index of interrupt thread information table */
    uint32_t                         irq_channels;          /*!< Number of interrupts */

    st_pma_handle_t                  pma_handle;            /*!< Handle of Power Manager Agent */
    bool                             pm_wait_state;         /*!< Power manager wait status flag (true: waiting, false: non-waiting) */

    osal_axi_bus_id_t                axi_bus_id;            /*!< AXI bus ID corresponding to the device */

}st_osal_device_control_t;

/*******************************************************************************************************************//**
 * @struct osal_pdev_entry_t
 * structure to pdev information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_pdev_entry
{
    const char                       *name;                 /*!< pdev entry name */
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                              fd;                    /*!< File descriptor */
    uintptr_t                        start_addr;            /*!< A process's address space that device's physical address(start) */
    uintptr_t                        end_addr;              /*!< A process's address space that device's physical address(end) */
    uintptr_t                        maped_start_addr;      /*!< A process's address space that mapped address(start) */
    uring_t                          uring;                 /*!< Information structure for the "uring" API */
    pthread_t                        thread_id;             /*!< pdev of interrupt thread for eMCOS */
    /* TODO: Change from fixed length to variable length. */
    struct st_osal_interrupt_thread_control *control_table[32]; /*!< interrupt thread control table */
}st_osal_pdev_entry_t;

/*******************************************************************************************************************//**
 * @struct st_pdev_entry_t
 * structure to pdev informations
***********************************************************************************************************************/
typedef struct st_pdev_entry
{
    size_t                           num;                   /*!< number of entry */
    st_osal_pdev_entry_t             *entry;                /*!< entry */
}st_pdev_entry_t;

/*******************************************************************************************************************//**
 * @struct st_osal_axi_bus_list_t
 * structure to hold AXI bus information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_axi_bus_list
{
    char    axi_bus_name[OSAL_AXI_BUS_NAME_MAX_LENGTH]; /*!< AXI Bus name */

}st_osal_axi_bus_list_t;

/*******************************************************************************************************************//**
 * @struct st_osal_axi_bus_control_t
 * structure to control axi bus.
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_axi_bus_control
{
    uint64_t                    num;                /*!< number of registrations with "axi_bus_list" */
    st_osal_axi_bus_list_t      *axi_bus_list;      /*!< AXI Bus information */

}st_osal_axi_bus_control_t;

/*******************************************************************************************************************//**
 * @struct st_osal_axi_bus_control_t
 * structure to control axi bus.
***********************************************************************************************************************/
typedef struct st_osal_axi_bus_name_info
{
    char    domain_name[OSAL_BUS_DOMAIN_SIZE];         /*!< AXI Bus domain name */
    int32_t utlb;                                      /*!< uTLB number  */
    char    *dev_names;                                /*!< Associated Device Name */
    size_t  dev_names_length;                          /*!< Length of "dev_names" */
} st_osal_axi_bus_name_info_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mem_buf_ptr_self_t
 * structure to buffer information area
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mem_buf_ptr_self
{
    struct st_osal_memory_buffer_obj mbuff_obj;     /*!< this area is exposed to interface as memory buffer object*/
    uint64_t                         handle_id;     /*!< Fixed value: OSAL_BUFFER_HANDLE_ID */
    size_t                           buffer_id;     /*!< ID of buffer */

    pthread_mutex_t                  inner_mtx;     /*!< Internal mutex object */
    osal_milli_sec_t                 inner_timeout; /*!< Waiting time of lock inner_mtx */

    size_t                           size;          /*!< User-specified size when allocating */
    size_t                           lib_mngt_size; /*!< Library management area size */

    uintptr_t                        buffer_pa;     /*!< Physical address of the buffer used by the user */
    uintptr_t                        chunk_pa;      /*!< Physical address of the allocated chunk */
    size_t                           chunk_size;    /*!< Size of chunk */

    uint8_t                          *p_chunk_va;   /*!< Virtual address of the allocated chunk */
    uint8_t                          *p_buffer_va;  /*!< Virtual address of the buffer used by the user */

}st_osal_mem_buf_ptr_self_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mem_monitor_desc_t
 * structure to monitor information area
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_mem_monitor_desc
{
    e_osal_mmngr_event_t            event_id;       /*!< Event ID */
    fp_osal_memory_mngr_cb_t        *p_handler;     /*!< Function pointer of memory manager callback */
    void                            *p_arg;         /*!< Argument of the callback */

}st_osal_mem_monitor_desc_t;

/*******************************************************************************************************************//**
 * @struct st_osal_mem_mmngr_ptr_self_t
 * structure to memory manager control table
***********************************************************************************************************************/
typedef struct st_osal_mem_mmngr_ptr_self
{
    struct st_osal_memory_manager_obj mmngr_obj;        /*!< this area is exposed to interface as memory manager object */
    uint64_t                          handle_id;        /*!< Fixed value: OSAL_MEMORY_HANDLE_ID */

    pthread_mutex_t                   inner_mtx;        /*!< Internal mutex object */
    osal_milli_sec_t                  inner_timeout;    /*!< Waiting time of lock inner_mtx */

    st_meml_manage_t                  meml_handle;      /*!< Handle of the Memory Allocator Library */
    st_osal_mmngr_config_t            config;           /*!< Memory Manager configuration */

    uintptr_t                         physical_addr;    /*!< Physical Address of the continuous memory */
    void                              *p_virtual_addr;  /*!< Virtual Address of the continuous memory */
    size_t                            size;             /*!< Size of the continuous memory */

    st_osal_mem_buf_ptr_self_t        *p_buffers;       /*!< Pointer to the structure of memory buffer */
    size_t                            num_buffers;      /*!< The number of buffers */
    size_t                            use_buffer_total; /*!< Size of the user allocate total. */
    st_osal_mem_monitor_desc_t        *p_monitor;       /*!< Pointer to the structure of monitor */
    size_t                            num_monitor;      /*!< The number of monitor */

    osal_memory_region_idx_t          region_idx;       /*!< Memory region ID */

}st_osal_mem_mmngr_ptr_self_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS_DEV_CFG
 * @defgroup OSAL_Public_Function_DevConfig Device Condifurator Public function definitions (control)
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
* @brief            Init Device Configuration.
* @return           void
***********************************************************************************************************************/
void R_OSAL_DCFG_Init(void);

/*******************************************************************************************************************//**
* @brief            Deinit Device Configuration.
* @return           void
***********************************************************************************************************************/
void R_OSAL_DCFG_Deinit(void);

/*******************************************************************************************************************//**
* @brief            Get Device SoC type.
* @return           uint64_t  SoC type identifier
***********************************************************************************************************************/
uint64_t R_OSAL_DCFG_GetSocType(void);

/*******************************************************************************************************************//**
* @brief            Get number of device control information.
* @return           size_t  number of device
***********************************************************************************************************************/
size_t R_OSAL_DCFG_GetNumOfDevice(void);

/*******************************************************************************************************************//**
* @brief            Get device control information.
* @param[in]        num      To set the device number
* @param[out]       pp_info  To set the address of device control structure pointer
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetDeviceInfo(uint32_t num, st_osal_device_control_t **pp_info);

/*******************************************************************************************************************//**
* @brief            Get number of pdev entry.
* @return           size_t  number of pdev entry
***********************************************************************************************************************/
size_t R_OSAL_DCFG_GetNumOfPdevEntry(void);

/*******************************************************************************************************************//**
* @brief            Get pdev entry name.
* @param[in]        num      To set the entry number
* @param[out]       pp_info  To set the address of string
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
* @retval           OSAL_RETURN_FAIL Unknown SoC
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetPdevEntryName(uint32_t num, const char **name);

/*******************************************************************************************************************//**
* @brief            Get PMA configuration.
* @param[out]       p_config    The pointor of power manager agent configuration
* @return           e_osal_return_t
* @retval           void
***********************************************************************************************************************/
void R_OSAL_DCFG_GetPMAConfiguration(st_pma_config_t * const p_config);

/*******************************************************************************************************************//**
* @brief            Get axi bus information.
* @param[out]       pp_info     The pointor of axi bus control information
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetAxiBusInfo(st_osal_axi_bus_control_t **pp_info);

/*******************************************************************************************************************//**
* @brief            Get max configuration of the memory.
* @param[out]       pp_config   The pointor of memory manager configuration
* @param[in]        region_idx   Memory region ID
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetMaxConfigOfMemory(st_osal_mmngr_config_t * const p_config,
                                                 osal_memory_region_idx_t region_idx);

/*******************************************************************************************************************//**
* @brief            Get default memory region informations.
* @return           osal_memory_region_idx_t  default memory region id
***********************************************************************************************************************/
osal_memory_region_idx_t R_OSAL_DCFG_GetDefaultRegionInfo(void);

/*******************************************************************************************************************//**
* @brief            Get number of memory region informations.
* @return           size_t  number of pdev entry
***********************************************************************************************************************/
size_t R_OSAL_DCFG_GetNumOfMemoryRegionInfo(void);

/*******************************************************************************************************************//**
* @brief            Get region information of the memory.
* @param[out]       pp_info   The pointor of memory region information
* @param[in]        region_idx   Memory region ID
* @return           e_osal_return_t
* @retval           OSAL_RETURN_OK   Success
* @retval           OSAL_RETURN_PAR  Parameter error
***********************************************************************************************************************/
e_osal_return_t R_OSAL_DCFG_GetMemoryRegionInfo(st_osal_mmngr_region_info_t * const pp_info,
                                                osal_memory_region_idx_t region_idx);
/** @} */

#endif /* OSAL_DEV_CFG_CONTROL_H */
