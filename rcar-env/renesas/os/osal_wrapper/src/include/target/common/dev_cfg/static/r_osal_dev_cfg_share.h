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
* Description :  OSAL Device Configurator Public Header (share OSAL Configurator, OS IF)
***********************************************************************************************************************/

#ifndef OSAL_DEV_CFG_SHARE_H
#define OSAL_DEV_CFG_SHARE_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal_configuration/target/common/r_osal_dev_cfg_info.h"
#include "target/common/pma/r_pma.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @def OSAL_UTLB_ASSIGN_MAX_NUM
 * @brief Max value of the uTLB assignment number.
***********************************************************************************************************************/
#define OSAL_UTLB_ASSIGN_MAX_NUM    (64)

/*******************************************************************************************************************//**
 * @def OSAL_DEV_CFG_INTERRUPT_NONE_INDEX
 * @brief None index
***********************************************************************************************************************/
#define OSAL_DEV_CFG_INTERRUPT_NONE_INDEX   (-1)

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef e_osal_pm_postckcr_t
 * @brief PM POSTCKCR definitions.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_osal_pm_postckcr
 * @brief PM POSTCKCR definitions.
***********************************************************************************************************************/
typedef enum e_osal_pm_postckcr
{
    OSAL_PM_POSTCKCR  = 0,     /*!< Power-On Self Test Clock Frequency Control Register   (POSTCKCR)  */
    OSAL_PM_POST2CKCR = 2,     /*!< Power-On Self Test 2 Clock Frequency Control Register (POST2CKCR) */
    OSAL_PM_POST3CKCR = 3,     /*!< Power-On Self Test 3 Clock Frequency Control Register (POST3CKCR) */
    OSAL_PM_POST4CKCR = 4,     /*!< Power-On Self Test 4 Clock Frequency Control Register (POST4CKCR) */
    OSAL_PM_POSTCKCR_INVALID = 5,
} e_osal_pm_postckcr_t;

/***********************************************************************************************************************
 * @typedef st_osal_interrupt_info_t
 * @brief structure to hold interrupt information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_interrupt_info
 * @brief structure to hold interrupt information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_interrupt_info
{
    uint64_t    irq_number;                             /*!< IRQ number */
} st_osal_interrupt_info_t;

/***********************************************************************************************************************
 * @typedef st_osal_utlb_assignment_info_t
 * @brief structure to hold utlb assignment information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_utlb_assignment_info
 * @brief structure to hold utlb assignment information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_utlb_assignment_info
{
    uint32_t    table_size;                             /*!< The size of the assignment number table */
    int32_t     table[OSAL_UTLB_ASSIGN_MAX_NUM];        /*!< The assignment number table */
} st_osal_utlb_assignment_info_t;

/***********************************************************************************************************************
 * @typedef st_osal_device_info_t
 * @brief structure to hold device information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_device_info
 * @brief structure to hold device information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_device_info
{
    const char                      *group_name;        /*!< group name */
    const char                      *id;                /*!< device identifier */
    const char                      *type;              /*!< device type */
    uintptr_t                       address;            /*!< Start address of register */
    size_t                          range;              /*!< Register access range */
    st_osal_interrupt_info_t        *interrupt;         /*!< A pointer to the irq number that groups multiple channels together */
    size_t                          irq_channels;       /*!< Number of irq channels */
    e_pma_hwa_id_t                  pma_id;             /*!< Power Manager Agent identifier */
    e_osal_pm_postckcr_t            postckcr;           /*!< POSTCKCR of Power Manager of the device */
    const char                      *axi_bus_name;      /*!< AXI bus name associated with the device */
    st_osal_utlb_assignment_info_t  *utlb_assignment;   /*!< A pointer to the uTLB assignment that groups multiple information together */
} st_osal_device_info_t;

/***********************************************************************************************************************
 * @typedef st_osal_device_control_t
 * @brief structure to hold device control information
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_device_control
 * @brief structure to hold device control information
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_device_control
{
    uint64_t                         handle_id;         /*!< Fixed value: OSAL_DEVICE_HANDLE_ID */
    bool                             active;            /*!< The device handle active flag (true: opened device handle, false: closed device handle) */
    osal_os_mutex_t                  inner_mtx;         /*!< Internal mutex object */
    osal_milli_sec_t                 inner_timeout;     /*!< Waiting time of lock inner_mtx */
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                              fd;                /*!< File descriptor */
    uintptr_t                        base_addr;         /*!< A process's address space that mapped a device's physical address */
    const struct st_osal_device_info *dev_info;         /*!< Device configuration */
    int32_t                          *interrupt_index;  /*!< Index of interrupt thread information table corresponding to device */
    st_pma_handle_t                  pma_handle;        /*!< Handle of Power Manager Agent */
    bool                             pm_wait_state;     /*!< Power manager wait status flag (true: waiting, false: non-waiting) */
    osal_axi_bus_id_t                axi_bus_id;        /*!< AXI bus ID corresponding to the device */
} st_osal_device_control_t;

/***********************************************************************************************************************
 * @typedef st_osal_device_config_addr_t
 * @brief structure to device configuration array
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_osal_device_config_addr
 * @brief structure to device configuration array
***********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the another module reference the member after calling. */
typedef struct st_osal_device_config_addr
{
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                              fd;            /*!< File descriptor */
    uintptr_t                        virtual_addr;  /*!< A process's address space that mapped a device's physical address */
    const struct st_osal_device_info *p_dev_info;   /*!< Device configuration */
} st_osal_device_config_addr_t;

/** @} OSAL_WRAPPER */

#endif /* OSAL_DEV_CFG_SHARE_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
