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
* File Name    : r_impdrv_version.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver public header for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_VERSION_H_
#define R_IMPDRV_VERSION_H_

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/
#include <stdint.h>

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_API
 * @defgroup IMPDRV_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
#define IMPDRV_VERSION_MAJOR        (3U)        /**< Major version number of IMP driver */
#define IMPDRV_VERSION_MINOR        (19U)       /**< Minor version number of IMP driver */
#define IMPDRV_VERSION_PATCH        (0U)        /**< Patch version number of IMP driver */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_API
 * @defgroup IMPDRV_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @struct st_impdrv_version_t
 * IMPDRV Structure for Interrupt group setting.
***********************************************************************************************************************/
typedef struct st_impdrv_version
{
    uint32_t    major;      /**< Major version of IMP driver    */
    uint32_t    minor;      /**< Minor version of IMP driver    */
    uint32_t    patch;      /**< Patch version of IMP driver    */
} st_impdrv_version_t;

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_API IMPDRV API
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************************************************//**
* @brief            This function returns IMP driver version information.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @return           IMP driver version information.
***********************************************************************************************************************/
const st_impdrv_version_t* R_IMPDRV_GetVersion (
    void
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* R_IMPDRV_VERSION_H_ */

/*======================================================================================================================
End of File
======================================================================================================================*/

