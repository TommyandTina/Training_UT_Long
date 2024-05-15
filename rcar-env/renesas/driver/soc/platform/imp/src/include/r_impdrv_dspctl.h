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
* File Name    : r_impdrv_dspctl.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver private header of impdrv_dspctl for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_DSPCTL_H
#define R_IMPDRV_DSPCTL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_IMPCTL
 * @defgroup IMPDRV_IMPCTL_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_IMPCTL
 * @defgroup IMPDRV_IMPCTL_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_IMPCTL
 * @defgroup IMPDRV_IMPCTL_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_IMPCTL IMPDRV IMPCTL
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
 * @brief           Validate the specified core number
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       core_num        Specified core number
 * @return          Function result
 * @retval          true        Valid core
 * @retval          false       Invalid core
***********************************************************************************************************************/
bool impdrv_dspctl_is_valid_core(
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Check the specified core state
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @param[in]       state               IMP Driver state
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_check_state(
    const st_impdrv_corectl_t   *const p_corectl,
    const uint32_t              core_num,
    const e_impdrv_state_t      state
);

/*******************************************************************************************************************//**
 * @brief           Set the specified core state
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       state               IMP Driver state
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_state(
    st_impdrv_corectl_t         *const p_corectl,
    const e_impdrv_state_t      state
);

/*******************************************************************************************************************//**
 * @brief           Starts control of the specified core
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_coretypectl           Control data structure for each core type
 * @param[in]       core_num                Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Ends control of the specified core
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_coretypectl           Control data structure for each core type
 * @param[in]       core_num                Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_init_end(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Control start of DSP core pre-preparation
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl               Control data structure for specific core
 * @param[in]       core_num                Specified core number
 * @param[in]       irq_priority            the value of interrupt priority
 * @param[in]       osal_cb_args            interrupt handler function's argument
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_start_pre(
    st_impdrv_corectl_t                 *p_corectl,
    const uint32_t                      core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
);

/*******************************************************************************************************************//**
 * @brief           Starts control of the DSP core
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_coretypectl           Control data structure for each core type
 * @param[in]       core_num                Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
 * @retval          IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_start(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Ends control of the specified core
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_coretypectl           Control data structure for each core type
 * @param[in]       core_num                Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_stop(
    st_impdrv_coretypectl_t     *const p_coretypectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Quit control of the specified core
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       core_num                Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PM System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_quit(
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Initialize the attribute settings of specified core
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_attr_init(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief           Attribute settings of Core memory initialize.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @param[in]       enable              Memory initialize setting.
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_mem_init(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const e_impdrv_param_t  enable
);

/*******************************************************************************************************************//**
 * @brief           Attribute settings of Sync core map.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @param[in]       syncc_val           Sync core map array
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_core_map(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
);

/*******************************************************************************************************************//**
 * @brief           Attribute settings of CL address.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @param[in]       claddr_phys         Physical address of CL data
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_cl(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const uint32_t          claddr_phys
);

/*******************************************************************************************************************//**
 * @brief            Attribute settings of DSP information
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in,out]    p_corectl           Control data structure for specific core
 * @param[in]        core_num            Specified core number
 * @param[in]        dsp_info            Information of DSP data
 * @return           Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_dsp(
    st_impdrv_corectl_t         *p_corectl,
    const uint32_t              core_num,
    const st_impdrv_dsp_data_t  *const p_dsp_app,
    const st_impdrv_dsp_data_t  *const p_dsp_fw,
    const st_impdrv_dsp_data_t  *const p_dsp_data,
    const st_impdrv_dsp_data_t  *const p_dsp_dtcm
);

/*******************************************************************************************************************//**
 * @brief           Attribute settings of IRQ mask setting.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @param[in]       irq_mask            Array for IRQ mask setting
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_set_irq_mask(
    st_impdrv_corectl_t     *const p_corectl,
    const uint32_t          core_num,
    const bool              irq_mask[IMPDRV_IRQMASK_MAX]
);

/*******************************************************************************************************************//**
 * @brief            Execute control of the DSP core
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in-out]    p_corectl           Control data structure for specific core
 * @param[in]        core_num            Core number
 * @param[in]        callback_func       Callback function of execute request
 * @param[in]        p_callback_args     Callback arguments
 * @return           Return value
 * @retval           IMPDRV_EC_OK                    Successful completion
 * @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
 * @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_dsp_execute(
    st_impdrv_corectl_t         *p_corectl,
    const uint32_t              core_num,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
);

/*******************************************************************************************************************//**
 * @brief           Execute control of the specified core.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in,out]   p_corectl           Control data structure for specific core
 * @param[in]       core_num            Specified core number
 * @return          Function result
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_int_handler(
    st_impdrv_corectl_t         *const p_corectl,
    const uint32_t              core_num
);

/*******************************************************************************************************************//**
 * @brief            Resume control of the specified core.
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in,out]    p_corectl           Control data structure for specific core
 * @param[in]        core_num            Core number
 * @return           Return value
 * @retval           IMPDRV_EC_OK                    Successful completion
 * @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_resume_exe(
    st_impdrv_corectl_t *const p_corectl,
    const uint32_t      core_num
);

/*******************************************************************************************************************//**
 * @brief            Change the Power management policy.
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in,out]    p_coretypectl       Control data structure for each core type
 * @param[in]        core_num            Core number
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
e_impdrv_errorcode_t impdrv_dspctl_pm_set_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    const e_impdrv_pm_policy_t      policy
);

/*******************************************************************************************************************//**
 * @brief            Get Power Management policy.
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in,out]    p_coretypectl       Control data structure for each core type
 * @param[in]        core_num            Core number
 * @param[out]       p_policy            Power management policy
 * @return           Return value
 * @retval           IMPDRV_EC_OK                    Successful completion
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
 * @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_pm_get_policy(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_policy_t            *const p_policy
);

/*******************************************************************************************************************//**
 * @brief            Get Power Management state.
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in,out]    p_coretypectl       Control data structure for each core type
 * @param[in]        core_num            Core number
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
e_impdrv_errorcode_t impdrv_dspctl_pm_get_state(
    st_impdrv_coretypectl_t         *const p_coretypectl,
    const uint32_t                  core_num,
    e_impdrv_pm_state_t             *const p_pmstate
);

/*******************************************************************************************************************//**
 * @brief            Configuration register check in Core control.
 * @syncasync        Synchronous.
 * @reentrant        Non-Reentrant
 * @param[in]        p_coretypectl       Control data structure for each core type
 * @param[in]        core_num            Core number
 * @param[in]        p_chk_param         Pointer to Array of check parameters.
 * @param[in]        param_num           Number of check parameters.
 * @return           Return value
 * @retval           IMPDRV_EC_OK                    Successful completion
 * @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval           IMPDRV_EC_NG_PARAM              Parameter error
 * @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_dspctl_conf_reg_check(
    const st_impdrv_coretypectl_t   *const p_coretypectl,
    const uint32_t                  core_num,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_DSPCTL_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

