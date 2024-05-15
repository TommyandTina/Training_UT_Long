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
* File Name :    r_osal_common.h
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL Wrapper Common Header
***********************************************************************************************************************/

#ifndef OSAL_COMMON_H
#define OSAL_COMMON_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "rcar-xos/osal/r_osal.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Macro definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_COM_008
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_DEBUG
 * @brief debug print.
***********************************************************************************************************************/
#define OSAL_DEBUG(...)

/*******************************************************************************************************************//**
 * @def OSAL_STATIC
 * @brief static definition
***********************************************************************************************************************/
#if !defined(OSAL_STATIC)
   #define OSAL_STATIC      static
#endif

/*******************************************************************************************************************//**
 * @def OSAL_MAX_TV_NSEC
 * @brief Max value of nano second
***********************************************************************************************************************/
#define OSAL_MAX_TV_NSEC    (1000000000L - 1L)

/*******************************************************************************************************************//**
 * @def OSAL_MSECTONSEC
 * @brief Mask value for converting milli second to nano second
***********************************************************************************************************************/
#define OSAL_MSECTONSEC     (1000000L)

/*******************************************************************************************************************//**
 * @def OSAL_SECTOMSEC
 * @brief Mask value for converting second to milli second
***********************************************************************************************************************/
#define OSAL_SECTOMSEC      (1000L)

/*******************************************************************************************************************//**
 * @def OSAL_SECTONSEC
 * @brief Mask value for converting second to nano second
***********************************************************************************************************************/
#define OSAL_SECTONSEC      (1000000000L)

/*******************************************************************************************************************//**
 * @def OSAL_INNER_TIMEOUT
 * @brief Mask value for timeout used in osal internal exclusive control
***********************************************************************************************************************/
#define OSAL_INNER_TIMEOUT ((osal_milli_sec_t)500U)
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_COM_016]
***********************************************************************************************************************/

/*======================================================================================================================
 Type definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * @typedef e_osal_init_status_t
 * @brief Initialize status.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_osal_init_status
 * @brief Initialize status.
***********************************************************************************************************************/
typedef enum e_osal_init_status
{
    OSAL_MANAGER_STATE_UNINITIALIZED = 0,
    OSAL_MANAGER_STATE_INITIALIZED   = 1,
    OSAL_MANAGER_STATE_MEMOPENED     = 2
} e_osal_init_status_t;

/** @} OSAL_WRAPPER */

#endif /* OSAL_COMMON_H */
/*======================================================================================================================
End of File
======================================================================================================================*/
