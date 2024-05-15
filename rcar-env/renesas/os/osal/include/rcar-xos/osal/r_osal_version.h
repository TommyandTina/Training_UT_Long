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
* File Name :    r_osal_version.h
* Version :      3.7.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  OSAL public header file
***********************************************************************************************************************/

#ifndef OSAL_VERSION_H
#define OSAL_VERSION_H

/* ################################################################################### */
/* Proposal: Enforce C99 ANSI C standard compatibility in header */

#if !defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/* Compiler is compatible to C99 standard */
#pragma error "Compiler must be compatible with at least C99 standard"
#endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

/* ################################################################################### */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************************************************//**
 * @ingroup macro_definitions
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @def OSAL_VERSION_MAJOR
 *
 **********************************************************************************************************************/
#define OSAL_VERSION_MAJOR   (3U) /*!< major version */

/*******************************************************************************************************************//**
 * @def OSAL_VERSION_MINOR
 *
***********************************************************************************************************************/
#define OSAL_VERSION_MINOR   (7U) /*!< minor version */

/*******************************************************************************************************************//**
 * @def OSAL_VERSION_PATCH
 *
 **********************************************************************************************************************/
#define OSAL_VERSION_PATCH   (0U) /*!< patch version */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_GENERIC_TYPES
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @typedef st_osal_sub_version_t
 * @brief   This type is the typedef definition of struct #st_osal_sub_version.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct  st_osal_sub_version
 * @brief   sub-version structure.
 **********************************************************************************************************************/
typedef struct st_osal_sub_version {
   uint32_t major;      /*!< major version */
   uint32_t minor;      /*!< minor version */
   uint32_t patch;      /*!< patch version */
} st_osal_sub_version_t;

/*******************************************************************************************************************//**
 * @typedef st_osal_version_t
 * @brief   This type is the typedef definition of struct #st_osal_version.
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct  st_osal_version
 * @brief   version structure.
 **********************************************************************************************************************/
/* PRQA S 3630 1 # This definition must be published because the user references the member after calling */
typedef struct st_osal_version {
    st_osal_sub_version_t api;      /*!< api version strcture */
    st_osal_sub_version_t wrapper;  /*!< wrapper version strcture */
} st_osal_version_t;

/** @} */

/*******************************************************************************************************************//**
 * @ingroup OSAL_Common_Function
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * XOS3_OSAL_NOS_AD_HED_COM_004
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief   Return OSAL API/Wrapper version.
 * @return  #st_osal_version_t *
 * @note    None.
 * @see
 *      None.
 **********************************************************************************************************************/
const st_osal_version_t* R_OSAL_GetVersion(void);
/***********************************************************************************************************************
 * [Covers: XOS2_OSAL_NOS_AD_FD_COM_014]
***********************************************************************************************************************/

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* OSAL_VERSION_H */
/*======================================================================================================================
End of File
======================================================================================================================*/

