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
* File Name    : r_impdrv_cmnctl_reg.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver private header of impdrv_cmnctl for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_CMNCTL_REG_H
#define R_IMPDRV_CMNCTL_REG_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CMNCTL_REG
 * @defgroup IMPDRV_CMNCTL_REG_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_TABLE_NUM
 * Maximum number of resource tables.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_TABLE_NUM          (16U)       /**< Maximum number of Resource tables.         */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CMNCTL_REG
 * @defgroup IMPDRV_CMNCTL_REG_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CMNCTL_REG
 * @defgroup IMPDRV_CMNCTL_REG_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_impdrv_hwrsc_def_t
 * Hardware resource definition table.
***********************************************************************************************************************/
typedef struct
{
    const char                  *name;                              /**< Resource Name(Key)                     */
    uintptr_t                   addr_phys;                          /**< Physical address                       */
    uint32_t                    size;                               /**< Area size                              */
    bool                        read_only;                          /**< Read only attribute                    */
    bool                        internal;                           /**< Internal resource attribute            */
} st_impdrv_hwrsc_def_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_hwrsc_mng_t
 * Hardware resource management table.
***********************************************************************************************************************/
typedef struct
{
    uint32_t                    resource_used;                      /**< Resource used state                    */
    uint32_t                    memory_initialized;                 /**< Memory initialized state               */
    uint32_t                    in_use[IMPDRV_HWRSC_TABLE_NUM];     /**< In-use status management               */
    st_impdrv_device_handle_t   handle[IMPDRV_HWRSC_TABLE_NUM];     /**< IMPDRV device handle                   */
} st_impdrv_hwrsc_mng_t;

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_CMNCTL_REG IMPDRV CMNCTL_REG
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
* @brief            This function initializes of the Register control of common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_init(
    st_impdrv_commonctl_t           *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            This function uninitializes of the  Register control of common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_quit(
    st_impdrv_commonctl_t           *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            This function is sets the Memory protection mode.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in, out]   p_commonctl         Common Control handle
* @param[in]        protect_mode        Memory protection setting.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_set_mem_protect(
    st_impdrv_commonctl_t           *const p_commonctl,
    const e_impdrv_protect_mode_t   protect_mode
);

/*******************************************************************************************************************//**
* @brief            This function is check the Memory protection mode.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_chk_mem_protect(
    const st_impdrv_commonctl_t     *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            This function is initializes the Scratchpad memory (Multi-bank memory).
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_mem_init(
    st_impdrv_commonctl_t       *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            This function returns the Hardware register area specification information in the argument.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        p_commonctl     Common Control handle
* @param[in]        p_device_id     Pointer to Target OSAL device ID.
* @param[out]       info            Pointer to Hardware register area specification information.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_get_hw_info(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
);

/*******************************************************************************************************************//**
* @brief            This function returns the result read from hardware register.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_commonctl     Common Control handle
* @param[in]        p_device_id     Pointer to the Target OSAL device ID.
* @param[in]        offset          Sets the offset address from Register top.
* @param[out]       p_data          Pointer to the read data to hardware registers.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_read32(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
);

/*******************************************************************************************************************//**
* @brief            This function writes data to hardware register.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_commonctl     Common Control handle
* @param[in]        p_device_id     Pointer to the Target OSAL device ID.
* @param[in]        offset          Sets the Offset address from Register top.
* @param[in]        data            Data written to hardware registers.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_write32(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
);

/*******************************************************************************************************************//**
* @brief            This function is Open/Close the device associated with the target OSAL ID.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_commonctl     Common Control handle
* @param[in]        p_device_id     Pointer to the Target OSAL device ID.
* @param[in]        new_state       Required state to the Register read/write function.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_required(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
);

/*******************************************************************************************************************//**
* @brief            This function gets the cmnctl_reg device handle.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        p_device_id     Pointer to Target OSAL device ID.
* @param[out]       p_device_handle Device handle.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_get_handle(
    const char                      *const p_device_id,
    st_impdrv_device_handle_t       *p_device_handle
);

/*******************************************************************************************************************//**
* @brief            This function is Checksum value setting for Memory broken check.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param            None.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL.
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_set_checksum(
    void
);

/*******************************************************************************************************************//**
* @brief            This function is Checksum compare test of Memory broken check.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param            None.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL.
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_test_checksum(
    void
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_CMNCTL_REG_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

