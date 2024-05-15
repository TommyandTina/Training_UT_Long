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
* File Name :    r_osal_os_dev_cfg_tbl_v4h.h
* Version :      0.1.0
* Product Name : OSAL
* Device(s) :    R-Car V4H
* Description :  Device Configuration file for R-Car V4H
***********************************************************************************************************************/

#ifndef OSAL_OS_DEV_CFG_V4H_H
#define OSAL_OS_DEV_CFG_V4H_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <device.h>
#include <devicetree.h>

#include "target/common/pma/hw/r_pma_hwa_id.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/
/*======================================================================================================================
 Private macro definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @def DT_MACRO
 * @brief DTS Macro
***********************************************************************************************************************/
/* Get the device type from dtsi */
#define DT_DEVICE_TYPE(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, device_type), \
    (DT_PROP(node_id, device_type)),("invalid"))

#define DT_ADDR(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, reg), \
    (DT_REG_ADDR(node_id)),(0x00000000))

#define DT_SIZE(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, reg), \
    (DT_REG_SIZE(node_id)),(0x0000))

/* Get the number of Interrupt from dtsi */
#define DT_IRQ_NUM(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, interrupts), \
    (DT_NUM_IRQS(node_id)), (0))

/* Get the axi bus name from dtsi */
#define DT_AXIBUS(node_id) COND_CODE_1(DT_NODE_HAS_PROP(node_id, ipmmu_id), \
    (DT_PROP(node_id, ipmmu_id)), ("invalid"))

/* Check the clock proparty */
#define DT_HAS_POST(node_id) COND_CODE_1( DT_NODE_HAS_PROP(node_id, clocks), \
    (DT_POST(node_id)), (OSAL_PM_POSTCKCR_INVALID))

/* Get the data of postckcr from dtsi */
#define DT_POST(node_id) COND_CODE_0( DT_CLOCKS_CELL_BY_IDX(node_id, 0, domain), \
    (DT_CLOCKS_CELL_BY_IDX(node_id, 0, module)), (OSAL_PM_POSTCKCR_INVALID))

/* Get the utlb_assignment from dtsi */
#define DT_UTLB(node_id, prop, idx) DT_PHA_BY_IDX(node_id, prop, idx, utlb),

#define DT_UTLB_NUM(node_id) DT_PROP_LEN(node_id, iommus)

#define DT_UTLB_DATA(node_id) COND_CODE_1( DT_NODE_HAS_PROP(node_id, iommus), \
    ( { DT_UTLB_NUM(node_id), {DT_FOREACH_PROP_ELEM(node_id, iommus, DT_UTLB)} }, ), \
    ( {0, {0}}, ) )

/* Get the device information from dtsi for creating config table */
#define NODE_PROPERTIES(node_id) { NULL, DT_LABEL(node_id), DT_DEVICE_TYPE(node_id), \
    DT_ADDR(node_id), DT_SIZE(node_id), NULL, DT_IRQ_NUM(node_id), \
    R_PMA_HWA_ID_NONE, DT_HAS_POST(node_id), DT_AXIBUS(node_id), NULL },
/*======================================================================================================================
 Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @var gs_osal_utlb_table
 * osal device configuration : utlb_assignment structure
***********************************************************************************************************************/
st_osal_utlb_assignment_info_t gs_osal_utlb_table[] =
{
#ifndef OSAL_ZEPHYR_UT
    DT_FOREACH_CHILD(DT_NODELABEL(osal_device), DT_UTLB_DATA)
#else
    {1, {0}},
    {0, {0}},
#endif
};

/***********************************************************************************************************************
 * XOS3_OSAL_ZEPHYR_CD_CD_IO_005
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @var gs_osal_device_info_v4h
 * @brief osal device configuration : v4 device information structure
***********************************************************************************************************************/
/* PRQA S 3218 1 # Definition within a function reduces readability and maintainability, so no change. */
st_osal_device_info_t gs_osal_device_info_v4h[] =
{
#ifndef OSAL_ZEPHYR_UT
    DT_FOREACH_CHILD(DT_NODELABEL(osal_device), NODE_PROPERTIES)
#else
    {
        .id             = "ut_dummy_1",
        .type           = "ut_dummy",
        .address        = 0xfe860000,
        .range          = 0x10000,
        .irq_channels   = 1,
        .pma_id         = R_PMA_HWA_ID_NONE,
        .axi_bus_name   = "ut_axi_dummy"
    },
    {
        .id             = "fbc_ut_dummy_1",
        .type           = "fbc_ut_dummy",
        .address        = 0xfe860000,
        .range          = 0x10000,
        .irq_channels   = 0,
        .pma_id         = R_PMA_HWA_ID_NONE,
        .axi_bus_name   = "ut_axi_dummy"
    },
#endif
    {.id = OSAL_DEVICE_INVALID_ID}
};
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_IO_031]
 * [Covers: XOS3_OSAL_ZEPHYR_UD_DD_NON_002]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

#endif /* OSAL_OS_DEV_CFG_V4H_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
