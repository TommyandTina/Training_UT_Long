/***********************************************************************************************************************
* Copyright [2020-2024] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name    : r_impdrv_udefctl.h
* Version      : 1.0.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver private header of impdrv_udefctl for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_UDEFCTL_H
#define R_IMPDRV_UDEFCTL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_UDEFCTL
 * @defgroup IMPDRV_UDEFCTL_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_UDEFCTL
 * @defgroup IMPDRV_UDEFCTL_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_UDEFCTL
 * @defgroup IMPDRV_UDEFCTL_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_UDEFCTL IMPDRV UDEFCTL
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
* @brief            Calculate CRC defined By User.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        sz              sz
* @param[in]        st              st
* @return           Return value
* @retval           
***********************************************************************************************************************/
uint32_t impdrv_udefctl_udef_crc(
    uint32_t              st,
    uint32_t              sz
);


/*******************************************************************************************************************//**
* @brief            ECM Set by user.
* @syncasync        Asynchronous.
* @reentrant        Non-Reentrant
* @param[in]        flag
* @return           None
* @retval           
***********************************************************************************************************************/
void impdrv_udefctl_set_ecm(
    uint32_t          core_num, 
    bool              flag
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_UDEFCTL_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

