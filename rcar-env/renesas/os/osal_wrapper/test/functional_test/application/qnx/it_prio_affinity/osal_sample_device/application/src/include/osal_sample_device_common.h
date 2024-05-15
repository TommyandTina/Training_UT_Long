/***********************************************************************************************************************
* Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    osal_sample_device_common.h
* Version :      3.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Header file of Sample application to use OSAL IO/Interrupt/Power API
***********************************************************************************************************************/
#ifndef OSAL_SAMPLE_DEVICE_COMMON_H_
#define OSAL_SAMPLE_DEVICE_COMMON_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include "rcar-xos/osal/r_osal.h"

#ifndef OSAL_SAMPLE_NO_LOGS
#define OSAL_SAMPLE_INFO(...) {printf("%s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#define OSAL_SAMPLE_ERR(...)  {printf("error: %s(%d):", __func__, __LINE__); printf(__VA_ARGS__);}
#else
#define OSAL_SAMPLE_INFO(...) {}
#define OSAL_SAMPLE_ERR(...)  {}
#endif

#if !defined(SUPPORT_AXI_BUS_NAME) && !defined(UNSUPPORT_INTERRUPT)
#define UNSUPPORT_INTERRUPT
#endif

/**
 * @page OSAL_Sample_Device_Definition_page OSAL Sample Device Definition
 * @section OSAL_Sample_Device_Definition_section OSAL Sample Device Definition about each SoC
 * Describe the Config of the device used by OSAL Sample Device
 * @snippet osal_sample_device_common.h OSAL_Sample_Device_Defined_value
 */
// [OSAL_Sample_Device_Defined_value]
/* Timeout */
#define TIMEOUT 1000 /* 1000ms */
#define SAMPLE_PM_POLICY OSAL_PM_POLICY_CG

#define OSAL_RESOURCE_ID 0xf000U

/* Message Queue */
typedef int32_t mq_msg_t;
#define MQ_ID_NO1 OSAL_RESOURCE_ID + 0U
#define MAX_NUM_OF_MSG 2
#define MSG_SIZE sizeof(mq_msg_t)
#define MSG_VALUE 0

#if defined(OSAL_SOC_S4) /* R-Car S4 */
/* Device type */
#define SAMPLE_DEVICE_TYPE     "rtdmac_control0"
#define SAMPLE_DEVICE_TYPE_SUB "rtdmac_descriptor0"
#define FBA_SAMPLE_DEVICE_SUFFIX_NAME "ca550" /* full device id: fba_ca550 */
/* Device ID */
#define SAMPLE_DEVICE_ID "rtdmac_control0_00"
/* IO read/write data */
#define SAMPLE_SINGLE_BLOCK_OFFSET 0x0028
#define SAMPLE_WRITE_DATA32 0x00000055U
/* IO block read/write data */
#define SAMPLE_BLOCK_SIZE 16U
#define SAMPLE_MULTI_BLOCK_OFFSET 0x0000
#define SAMPLE_BLOCK_WRITE_DATA32 (uint32_t[]){0xAAAA0000U, 0xAAAA0000U, 0x000000AAU, 0x00000000U}
/* clock divider */
#define SAMPLE_CLOCK_DIVIDER 0xBU
/* Power lowest state */
#define SAMPLE_PM_LOWEST_STATE OSAL_PM_STATE_CG
#elif defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4M) /* R-Car V4H */
/* Device type */
#define SAMPLE_DEVICE_TYPE "ims"
#define FBA_SAMPLE_DEVICE_SUFFIX_NAME "ims0" /* full device id: fba_ims0 */
/* Device ID */
#define SAMPLE_DEVICE_ID "ims_00"
/* IO read/write data */
#define SAMPLE_SINGLE_BLOCK_OFFSET 0x00C0
#define SAMPLE_WRITE_DATA32 0x000000FFU
/* IO block read/write data */
#define SAMPLE_BLOCK_SIZE 16U
#define SAMPLE_MULTI_BLOCK_OFFSET 0x0034
#define SAMPLE_BLOCK_WRITE_DATA32 (uint32_t[]){0x00000000U, 0x00000000U, 0x00000040U, 0x00000100U}
/* clock divider */
#define SAMPLE_CLOCK_DIVIDER 0xBU
/* Power lowest state */
#define SAMPLE_PM_LOWEST_STATE OSAL_PM_STATE_CG
#elif defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) /* R-Car V3x */
/* Device type */
#define SAMPLE_DEVICE_TYPE "ims"
#define FBA_SAMPLE_DEVICE_SUFFIX_NAME "ims0" /* full device id: fba_ims0 */
/* Device ID */
#define SAMPLE_DEVICE_ID "ims_00"
/* IO read/write data */
#define SAMPLE_SINGLE_BLOCK_OFFSET 0x00C0
#define SAMPLE_WRITE_DATA32 0x000000FFU
/* IO block read/write data */
#define SAMPLE_BLOCK_SIZE 16U
#define SAMPLE_MULTI_BLOCK_OFFSET 0x0034
#define SAMPLE_BLOCK_WRITE_DATA32 (uint32_t[]){0x00000000U, 0x00000000U, 0x00000040U, 0x00000100U}
/* clock divider */
#define SAMPLE_CLOCK_DIVIDER 0xBU
/* Power lowest state */
#define SAMPLE_PM_LOWEST_STATE OSAL_PM_STATE_CG
#endif

#if !defined(UNSUPPORT_INTERRUPT)
#if defined(OSAL_SOC_S4) /* R-Car S4 */
#define SAMPLE_INT_SUB_DEVICE_TYPE "rtdmac_descriptor0"     /* In case S4, related device for interrupt generation */
#define SAMPLE_INT_SUB_DEVICE_ID  "rtdmac_descriptor0_00"   /*In case S4, related device for interrupt generation */
/* IRQ channel */
#define IRQ_CHANNEL 0
/* Memory buffer */
#define SAMPLE_BUFFER_SIZE 64U
#define SAMPLE_BUFFER_ALIGNMENT 64U
#define SAMPLE_DUMMY_BUFFER_DATA_1 2908U
#define SAMPLE_DUMMY_BUFFER_DATA_2 0U
#define SAMPLE_BUFFER_ADDR_INDEX_1 0U
#define SAMPLE_BUFFER_ADDR_INDEX_2 8U
/* rtdmac register setting */
#define SAMPLE_REG_RDMSAR   0x0000
#define SAMPLE_REG_RDMDAR   0x0004
#define SAMPLE_REG_RDMTCR   0x0008
#define SAMPLE_REG_RDMCHCRB 0x001C
#define SAMPLE_REG_RDMCHCR  0x000C
#define SAMPLE_REG_RDMOR    0x0060
#define SAMPLE_CLEAR_REGISTER       0x0100
#define SAMPLE_TRANSFER_COUNT_VALUE     0x00000001U
#define SAMPLE_TRANSFER_STEP_VALUE      0x00000000U
#define SAMPLE_TRANSFER_RDMCHCR_VALUE   0x10040415U
#define SAMPLE_TRANSFER_RDMOR_VALUE     0x0301U
#define SAMPLE_CLEAR_REGISTER_VALUE     0x00000001U
#elif defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V3U) || defined(OSAL_SOC_V3H1) || defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3M2) || defined(OSAL_SOC_V4M) /* R-Car V3x, V4H */
#define SAMPLE_INT_SUB_DEVICE_TYPE ""
#define SAMPLE_INT_SUB_DEVICE_ID ""
/* IRQ channel */
#define IRQ_CHANNEL 0
/* Memory buffer */
#define SAMPLE_BUFFER_SIZE 64U
#define SAMPLE_BUFFER_ALIGNMENT 64U
#define SAMPLE_DUMMY_BUFFER_DATA_1 0x8f000000U
#define SAMPLE_DUMMY_BUFFER_DATA_2 0U
#define SAMPLE_BUFFER_ADDR_INDEX_1 0U
#define SAMPLE_BUFFER_ADDR_INDEX_2 8U
/* IMR register setting */
#define SAMPLE_IMR_DLSAR        0x0030
#define SAMPLE_IMR_IMR          0x0018
#define SAMPLE_IMR_CR           0x0008
#define SAMPLE_CLEAR_REGISTER   0x0010
#define SAMPLE_IMR_IMR_VALUE        0x00000000U
#define SAMPLE_IMR_CR_VALUE         0x00000001U
#define SAMPLE_CLEAR_REGISTER_VALUE 0x00000001U
#define SAMPLE_IMR_DL               0x8f000000U
#endif
#endif /* end of !UNSUPPORT_INTERRUPT */

#if defined(OSAL_SOC_S4) /* R-Car S4 */
/* AXI bus name */
#define SAMPLE_AXI_BUS_NAME "rt1_00-15"
#elif defined(OSAL_SOC_V4H) || defined(OSAL_SOC_V4M) /* R-Car V4H */
/* AXI bus name */
#define SAMPLE_AXI_BUS_NAME "vi0_10-11_14-15_18-19_22-23_26-27"
#elif defined(OSAL_SOC_V3U)
/* AXI bus name */
#define SAMPLE_AXI_BUS_NAME "vi0_10-11_14-15"
#elif defined(OSAL_SOC_V3M2)
/* AXI bus name */
#define SAMPLE_AXI_BUS_NAME "vi0_02-03"
#elif defined(OSAL_SOC_V3H2) || defined(OSAL_SOC_V3H1)
/* AXI bus name */
#define SAMPLE_AXI_BUS_NAME "vc0_00-06"
#endif

/**
 * @ingroup osal_sample_device
 * @defgroup OSAL_SAMPLE_DEVICE OSAL sample device common type definitions
 * @{
 */
/**
 * @typedef osal_sample_device_config_t
 * @brief This type is the typedef definition of struct #osal_sample_device_config.
 */
/**
 * @struct osal_sample_device_config
 * @brief This struct contains the configuration of the sample device
 */
typedef struct osal_sample_device_config
{
    char* device_list;              /*!< List of available devices corresponding with a device type in the current system */
    size_t num_of_bytes;            /*!< The number of bytes written in the device list */
    size_t num_of_devices;          /*!< The number of available devices in the current system */
} osal_sample_device_config_t;

/**
 * @typedef osal_sample_device_arg_callback_t
 * @brief This type is the typedef definition of struct #osal_sample_device_arg_callback
 */
/**
 * @struct osal_sample_device_arg_callback
 * @brief This struct is the argument of the callback function
 */
typedef struct osal_sample_device_arg_callback {
    osal_mq_handle_t mq_handle;     /*!< The handle of message queue */
} osal_sample_device_arg_callback_t;

/**
 * @typedef osal_sample_device_callback_ret_t
 * @brief This type is the typedef definition of struct #osal_sample_device_callback_ret
 */
/**
 * @struct osal_sample_device_callback_ret
 * @brief This struct is used for error handling of the interrupt context
 */
typedef struct osal_sample_device_callback_ret {
    e_osal_return_t osal_ret;   /*!< The return value of OSAL API called from the interrupt context */
    uint32_t error_line;        /*!< The number of line occurred error from the interrupt context */
    uint32_t interrupt_cnt;     /*!< The number of times the interrupt occurred */
} osal_sample_device_callback_ret_t;
/** @} */

// [OSAL_Sample_Device_Defined_value]

#endif /* End of File */
