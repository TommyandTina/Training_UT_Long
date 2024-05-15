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
* File Name :    r_osal_version.c
* Version :      0.5.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper version function for eMCOS
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include "target/emcos/r_osal_common.h"

/******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER_EMCOS
 * @defgroup OSAL_Private_Defines_Version Version macro definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def OSAL_WRAPPER_VERSION_MAJOR
 * Major version.
***********************************************************************************************************************/
#define OSAL_WRAPPER_VERSION_MAJOR   (0)

/*******************************************************************************************************************//**
 * @def OSAL_WRAPPER_VERSION_MINOR
 * Minor version.
***********************************************************************************************************************/
#define OSAL_WRAPPER_VERSION_MINOR   (5)

/*******************************************************************************************************************//**
 * @def OSAL_WRAPPER_VERSION_PATCH
 * Patch version.
***********************************************************************************************************************/
#define OSAL_WRAPPER_VERSION_PATCH   (0)

/** @} */

/***********************************************************************************************************************
* Start of function R_OSAL_GetVersion()
* XOS3_OSAL_EMCOS_CD_CD_COM_014
* [Covers: XOS3_OSAL_EMCOS_UD_DD_COM_003]
***********************************************************************************************************************/
const st_osal_version_t* R_OSAL_GetVersion(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    OSAL_STATIC const st_osal_version_t s_version =
    {
        .api.major = OSAL_VERSION_MAJOR,
        .api.minor = OSAL_VERSION_MINOR,
        .api.patch = OSAL_VERSION_PATCH,
        .wrapper.major = OSAL_WRAPPER_VERSION_MAJOR,
        .wrapper.minor = OSAL_WRAPPER_VERSION_MINOR,
        .wrapper.patch = OSAL_WRAPPER_VERSION_PATCH
    };

    return &s_version;
}
/***********************************************************************************************************************
* End of function R_OSAL_GetVersion()
***********************************************************************************************************************/

