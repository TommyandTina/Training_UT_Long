/***********************************************************************************************************************
* Copyright [2020-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
 * XOS3_OSAL_NOS_AD_HED_COM_012
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    r_osal_def.h
* Version :      3.7.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL public header file
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_010]
***********************************************************************************************************************/

#ifndef OSAL_DEF_H
#define OSAL_DEF_H

/* ################################################################################### */
/* Proposal: Enforce C99 ANSI C standard compatibility in header */

#if !defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* Compiler is compatible to C99 standard */
#pragma error "Compiler must be compatible with at least C99 standard"
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

/* ################################################################################### */

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_COM_009
***********************************************************************************************************************/
/***********************************************************************************************************************
 * Macro Definitions
 * Note that the prefix of macro definition is "OSAL_".
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_017]
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @defgroup OSAL_Defines Definition
 * OSAL has macro, enumeration and typedef definition. For detail refer to each section.
 * 
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @defgroup macro_definitions macro definitions
 * macro definition for OSAL API
 * 
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_COM_010
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_THREAD_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_THREAD_HANDLE_INVALID ((osal_thread_handle_t)NULL) /*!< invalid thread handle */

/*******************************************************************************************************************//**
 * @def OSAL_MQ_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_MQ_HANDLE_INVALID     ((osal_mq_handle_t)NULL)     /*!< invalid message queue handle */

/*******************************************************************************************************************//**
 * @def OSAL_MUTEX_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_MUTEX_HANDLE_INVALID  ((osal_mutex_handle_t)NULL)  /*!< invalid mutex handle */

/*******************************************************************************************************************//**
 * @def OSAL_COND_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_COND_HANDLE_INVALID   ((osal_cond_handle_t)NULL)     /*!< invalid condition variable handle */

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_DEVICE_HANDLE_INVALID ((osal_device_handle_t)NULL)   /*!< invalid device handle */

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_TYPE_NAME_MAX_LENGTH
 * 
 **********************************************************************************************************************/
#define OSAL_DEVICE_TYPE_NAME_MAX_LENGTH (28UL)   /*!< max length of device type name */

/*******************************************************************************************************************//**
 * @def OSAL_DEVICE_NAME_MAX_LENGTH
 * 
 **********************************************************************************************************************/
#define OSAL_DEVICE_NAME_MAX_LENGTH   (31UL)   /*!< max length of device name */

/*******************************************************************************************************************//**
 * @def OSAL_AXI_BUS_ID_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_AXI_BUS_ID_INVALID ((osal_axi_bus_id_t)0LL) /*!< This definition is reserved value for variables of 
                                                          * osal_axi_bus_id_t type. This value corresponds to "invalid" 
                                                          * of the axi id name. */

/*******************************************************************************************************************//**
 * @def OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY
 * 
 **********************************************************************************************************************/
#define OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY ((osal_axi_bus_id_t)1LL) /*!< This definition is reserved value for variables of 
                                                                  * osal_axi_bus_id_t type. This value corresponds to 
                                                                  * "mm(ipa)" of the axi id name. 
                                                                  * Note: In Case IOMMU/IPMMU is not active, 
                                                                  * this equals physical bus addresses. */

/*******************************************************************************************************************//**
 * @def OSAL_MEMORY_BUFFER_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_MEMORY_BUFFER_HANDLE_INVALID ((osal_memory_buffer_handle_t)NULL) /*!< invalid memory buffer handle */

/*******************************************************************************************************************//**
 * @def OSAL_MEMORY_MANAGER_HANDLE_INVALID
 * 
 **********************************************************************************************************************/
#define OSAL_MEMORY_MANAGER_HANDLE_INVALID ((osal_memory_manager_handle_t)NULL) /*!< invalid memory manager handle */

/*******************************************************************************************************************//**
 * @def OSAL_MEMORY_MANAGER_CFG_UNLIMITED
 * 
 **********************************************************************************************************************/
#define OSAL_MEMORY_MANAGER_CFG_UNLIMITED                                                                              \
    ((size_t)SIZE_MAX - 1U) /*!< Unlimited value to be used in the MMNGR config structure */

/** @} */

/** @} */
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_018]
***********************************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OSAL_DEF_H */
/*======================================================================================================================
End of File
======================================================================================================================*/

