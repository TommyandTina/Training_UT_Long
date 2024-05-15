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
* File Name :    r_osal_version.c
* Version :      1.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL wrapper version function
***********************************************************************************************************************/

/***********************************************************************************************************************
 * includes
***********************************************************************************************************************/
#include "target/common/r_osal_manager.h"

/*******************************************************************************************************************//**
 * @ingroup OSAL_WRAPPER OSAL WRAPPER
 *
 * @{
 **********************************************************************************************************************/

/*======================================================================================================================
 Extern function prototypes
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @brief    External Wrapper Version function declaration.
 * @return   const st_osal_sub_version_t *
***********************************************************************************************************************/
extern const st_osal_sub_version_t *R_OSAL_OS_GetWrapperVersion(void);

/*======================================================================================================================
 Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
 * XOS3_OSAL_NOS_CD_CD_COM_006
***********************************************************************************************************************/
/***********************************************************************************************************************
* Start of function R_OSAL_GetVersion()
***********************************************************************************************************************/
const st_osal_version_t* R_OSAL_GetVersion(void)
{
    /*------------------------------------------------------------------------------------------------------------------
    Local variables
    ------------------------------------------------------------------------------------------------------------------*/
    const st_osal_version_t             *p_ret_ver;
    OSAL_STATIC st_osal_version_t       s_version;
    const st_osal_sub_version_t         *p_wrapper_version;

    /*------------------------------------------------------------------------------------------------------------------
    Function body
    ------------------------------------------------------------------------------------------------------------------*/
    p_wrapper_version = R_OSAL_OS_GetWrapperVersion();

    s_version.api.major = OSAL_VERSION_MAJOR;
    s_version.api.minor = OSAL_VERSION_MINOR;
    s_version.api.patch = OSAL_VERSION_PATCH;
    s_version.wrapper.major = p_wrapper_version->major;
    s_version.wrapper.minor = p_wrapper_version->minor;
    s_version.wrapper.patch = p_wrapper_version->patch;

    p_ret_ver = (const st_osal_version_t*)&s_version;

    return p_ret_ver;
}
/***********************************************************************************************************************
* End of function R_OSAL_GetVersion()
***********************************************************************************************************************/
/***********************************************************************************************************************
 * [Covers: XOS3_OSAL_NOS_UD_DD_COM_003]
***********************************************************************************************************************/

/** @} OSAL_WRAPPER */

/*======================================================================================================================
End of File
======================================================================================================================*/
