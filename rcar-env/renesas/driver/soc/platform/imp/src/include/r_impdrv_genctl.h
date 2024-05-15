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
* File Name    : r_impdrv_genctl.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver private header of impdrv_cmnctl for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_GENCTL_H
#define R_IMPDRV_GENCTL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_GENCTL
 * @defgroup IMPDRV_GENCTL_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_GENCTL
 * @defgroup IMPDRV_GENCTL_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_GENCTL
 * @defgroup IMPDRV_GENCTL_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @struct st_impdrv_ctl_t
 * IMPDRV structure of control table.
***********************************************************************************************************************/
typedef struct st_impdrv_ctl
{
    st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX]; /**< Core control structure data for each core type */
    st_impdrv_commonctl_t   common_ctl;                         /**< Common control structure data to all core      */
} st_impdrv_ctl_t;

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_GENCTL IMPDRV GENCTL
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
* @brief            This function initializes of the General control.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in, out]   p_data              Initialization data
* @param[in]        osal_cb_args        OSAL callback argument
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
* @retval           IMPDRV_EC_NG_INSTANCE           Instance parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_init(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    st_impdrv_initdata_t        *const p_data,
    const impdrv_ctrl_handle_t  osal_cb_args
);

/*******************************************************************************************************************//**
* @brief            This function initializes the Core attribute data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Core information
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_attr_init(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function uninitializes of the General control.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_quit(
    st_impdrv_ctl_t             *const p_impdrv_ctl
);

/*******************************************************************************************************************//**
* @brief            This function starts of the General control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant.
* @param[in]        p_core_info
* @param[in-out]    p_handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_start(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function stops of the General control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant.
* @param[in]        p_core_info
* @param[in-out]    p_handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_stop(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function starts executing the command list of the General control.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        p_core_info
* @param[in]        callback_func
* @param[in]        p_callback_args
* @param[in-out]    p_impdrv_ctl
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_execute(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
);

/*******************************************************************************************************************//**
* @brief            This function resumes the command list of the General control.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        p_core_info         Core information
* @param[in, out]   p_impdrv_ctl        General control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_resume_exe(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function changes the Power management policy.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Core information
* @param[in]        policy              Power management policy
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_pm_set_policy(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const e_impdrv_pm_policy_t  policy
);

/*******************************************************************************************************************//**
* @brief            This function returns the Power management policy.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Core information
* @param[out]       p_policy            Power management policy
* @return           Return value
* @retval           R_IMPDRV_EC_OK                   Success completion.
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_pm_get_policy(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    e_impdrv_pm_policy_t        *const p_policy
);

/*******************************************************************************************************************//**
* @brief            This function returns the Power management state.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in]        p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Core information
* @param[out]       p_pmstate           Power management state.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_pm_get_state(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    e_impdrv_pm_state_t         *const p_pmstate
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for Core memory initialize.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        enable              Memory initialize setting.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_core_mem_init(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const e_impdrv_param_t      enable
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for Sync core map.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        core_map            Sync core map array
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_core_map(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID]
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for CL address data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        claddr_phys         Physical address of CL data
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_cl(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const uint32_t              claddr_phys
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for IRQ mask data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        irq_mask            Array for IRQ mask setting
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_irq_mask(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const bool                  irq_mask[IMPDRV_IRQMASK_MAX]
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for IRQ group data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_irq_param         IRQ group setting
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_irq_group(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_irq_group_t *const p_irq_param
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for CL break address data.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        cl_brk_addr         CL break address
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_cl_brk_addr(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const uint32_t              cl_brk_addr
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for conditional GOSUB instruction.
* @syncasync        Synchronous.
* @reentrant        Reentrant.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        condition           Conditional GOSUB instruction attribute.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_gosub_cond(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const e_impdrv_gosub_cond_t condition
);

/*******************************************************************************************************************//**
* @brief            This function is handle interrupts.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant.
* @param[in]        irq_channel     Interrupt IRQ Channel.
* @param[in]        ercd            The IMP Driver error code.
* @param[in, out]   p_impdrv_ctl    General control handle
* @return           None
***********************************************************************************************************************/
void impdrv_genctl_int_handler(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const uint64_t              irq_channel,
    const e_impdrv_errorcode_t  ercd
);

/*******************************************************************************************************************//**
* @brief            This function is check and wait clear interrupt request.
* @syncasync        Synchronous.
* @reentrant        No
* @param[in]        p_core_info     Specified core information
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_check_intclear(
    const st_impdrv_core_info_t *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            This function is sets the Memory protection mode.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl        General control handle
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
e_impdrv_errorcode_t impdrv_genctl_reg_set_mem_protect(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const e_impdrv_protect_mode_t   protect_mode
);

/*******************************************************************************************************************//**
* @brief            This function returns the Hardware register area specification information in the argument.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl    General control handle
* @param[in]        p_device_id     Pointer to Target OSAL device ID.
* @param[out]       p_reg_info      Pointer to Hardware register area specification information.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_reg_get_hw_info(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
);

/*******************************************************************************************************************//**
* @brief            This function returns the result read from hardware register.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl    General control handle
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
e_impdrv_errorcode_t impdrv_genctl_reg_read32(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
);

/*******************************************************************************************************************//**
* @brief            This function writes data to hardware register.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl    General control handle
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
e_impdrv_errorcode_t impdrv_genctl_reg_write32(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
);

/*******************************************************************************************************************//**
* @brief            This function is Open/Close the device associated with the target OSAL ID.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl    General control handle
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
e_impdrv_errorcode_t impdrv_genctl_reg_required(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
);

/*******************************************************************************************************************//**
* @brief            This function is attribute setting for DSP information.
* @syncasync        Synchronous.
* @reentrant        Permitted.
* @param[in, out]   p_impdrv_ctl        General control handle
* @param[in]        p_core_info         Pointer to the Core information data.
* @param[in]        p_dsp_app           The application which run on the DSP
* @param[in]        p_dsp_fw            The DSP FW which run on the DSP
* @param[in]        p_dsp_data          The data which is used by application on the DSP
* @param[in]        p_dsp_dtcm          The internal data which is used by application on the DSP
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_dsp(
    st_impdrv_ctl_t             *p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const st_impdrv_dsp_data_t  *const p_dsp_app,
    const st_impdrv_dsp_data_t  *const p_dsp_fw,
    const st_impdrv_dsp_data_t  *const p_dsp_data,
    const st_impdrv_dsp_data_t  *const p_dsp_dtcm
);

/*******************************************************************************************************************//**
* @brief            This function is handle dsp interrupts.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant.
* @param[in, out]   p_impdrv_ctl    General control handle
* @param[in]        irq_channel     Interrupt IRQ Channel.
* @param[in]        ercd            The IMP Driver error code.
* @param[in]        core_info       Core information
* @return           None
***********************************************************************************************************************/
void impdrv_genctl_dsp_int_handler(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const uint64_t              irq_channel,
    const e_impdrv_errorcode_t  ercd,
    const st_impdrv_core_info_t core_info
);

/*******************************************************************************************************************//**
* @brief            This function is check the interface of internal bus. However, does not execute fault control.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl    General control handle
* @param[in]        p_core_info     Pointer to the Core information data.
* @param[in]        p_chk_resource  Pointer to the OSAL resource for bus interface check.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_bus_if_check(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_chk_resource_t  *const p_chk_resource
);

/*******************************************************************************************************************//**
* @brief            This function is check the configuration register. However, does not execute fault control.
* @syncasync        Synchronous
* @reentrant        Permitted.
* @param[in]        p_impdrv_ctl    General control handle
* @param[in]        p_chk_param     Pointer to Array of check parameters.
* @param[in]        param_num       Number of check parameters.
* @param[in]        p_core_info     Pointer to the Core information data.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Successful completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_conf_reg_check(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
 * @brief           Return size of minimum work size.
 * @syncasync       Synchronous.
 * @reentrant       Permitted.
 * @param[in]       work_type           type of work size
 * @param[out]      p_work_size         size of work area
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_get_worksize(
    uint32_t      work_type,
    uint32_t      *p_work_size
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_GENCTL_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

