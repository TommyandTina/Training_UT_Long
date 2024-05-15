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
* File Name    : r_impdrv_private.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver  private header of impdrv_private for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_PRIVATE_H
#define R_IMPDRV_PRIVATE_H

/*******************************************************************************************************************//**
 * include of standard headers 
***********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_PRIVATE_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
#ifndef IMPDRV_BUILD_UNIT_TEST
  #define IMPDRV_STATIC     static              /**< Static declaration of IMP driver for Normal build      */
#else
  #define IMPDRV_STATIC                         /**< Static declaration of IMP driver for Unit test         */
#endif

#define IMPDRV_UNUSED_ARGS(a)   ((void)(a))     /**< Workaround for warning fix                             */
#define IMPDRV_CHK_MEM_ALIGN    (3U)            /**< Memory alignment check definition                      */
#define IMPDRV_CHK_REG_ALIGN    (3U)            /**< Register alignment check definition                    */
#define IMPDRV_CHK_CL_MEM_ALIGN (3U)            /**< CL Memory alignment check definition                   */
#define IMPDRV_CHK_CACHE_ALIGN  (63U)           /**< Memory cache alignment check definition                */
#define IMPDRV_CORE_TYPE_MAX    (11U)           /**< Maximum value of Core type definition                  */
#define IMPDRV_DSP_DEV_NUM_MAX  (8U)            /**< num of max sub device for DSP core                     */


/** @} */

/*******************************************************************************************************************//**
 * include of private headers
***********************************************************************************************************************/
#include "r_impdrv_osdep.h"
#include "r_impdrv_corectl.h"
#include "r_impdrv_cmnctl.h"
#include "r_impdrv_cmnctl_reg.h"
#include "r_impdrv_genctl.h"
#include "r_impdrv_impctl.h"
#include "r_impdrv_cnnctl.h"
#include "r_impdrv_ocvctl.h"
#include "r_impdrv_dmactl.h"
#include "r_impdrv_dmasctl.h"
#include "r_impdrv_pscctl.h"
#include "r_impdrv_dspctl.h"
#include "r_impdrv_udefctl.h"

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_PRIVATE_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_PRIVATE_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_PRIVATE IMPDRV PRIVATE
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_PRIVATE_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

