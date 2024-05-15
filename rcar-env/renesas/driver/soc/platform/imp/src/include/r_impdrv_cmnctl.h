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
* File Name    : r_impdrv_cmnctl.h
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver private header of impdrv_cmnctl for xOS3
***********************************************************************************************************************/

#ifndef R_IMPDRV_CMNCTL_H
#define R_IMPDRV_CMNCTL_H

/*******************************************************************************************************************//**
 * includes
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CMNCTL
 * @defgroup IMPDRV_CMNCTL_Defines macro definitions
 *
 * @{
 **********************************************************************************************************************/
#define IMPDRV_INTSTS_BITNUM        (32U)           /**< Bit width of Interrupt status register                 */

/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CMNCTL
 * @defgroup IMPDRV_CMNCTL_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/** @} */

/*******************************************************************************************************************//**
 * @ingroup IMPDRV_CMNCTL
 * @defgroup IMPDRV_CMNCTL_GENERIC_TYPES IMPDRV Generic Type definitions
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @struct st_impdrv_commonctl_t
 * IMPDRV structure of common control handle.
***********************************************************************************************************************/
typedef struct
{
    e_impdrv_instance_t         instance_num;           /**< Instance number                            */
    uint32_t                    use_core_ins;           /**< The current instance's use core            */
    uint32_t                    use_dsp_core_ins;       /**< The current instance's DSP use core        */
    p_impdrv_cbfunc_fatal_t     cbfunc_fatal;           /**< Fatal callback function                    */
    void                        *p_cb_args_fatal;       /**< Fatal callback argument                    */
    uint32_t                    irq_mask_value;         /**< Save data of IRQ mask register             */
    e_impdrv_protect_mode_t     protect_mode;           /**< Save data of memory protection mode        */
    void                        *p_hwrsc_def_table;     /**< Extended information for debug interface   */
    void                        *p_hwrsc_mng_table;     /**< Extended information for debug interface   */
    e_osal_interrupt_priority_t irq_priority;           /**< Interrupt priority                         */
    impdrv_ctrl_handle_t        osal_cb_args;           /**< OSAL callback argument                     */
} st_impdrv_commonctl_t;

/*******************************************************************************************************************//**
 * @struct  st_impdrv_cmn_init_data_t
 * IMPDRV structure of common control init data.
***********************************************************************************************************************/
typedef struct
{
    e_impdrv_instance_t             instance_num;           /**< Instance number                            */
    e_osal_interrupt_priority_t     irq_priority;           /**< Interrupt priority                         */
    st_impdrv_core_info_t           *p_core_info;           /**< Related core number                        */
    uint32_t                        use_core_num;           /**< Related core information list              */
    impdrv_ctrl_handle_t            osal_cb_args;           /**< OSAL callback argument                     */
    p_impdrv_cbfunc_fatal_t         cbfunc_fatal;           /**< Fatal callback function                    */
    void                            *p_cb_args_fatal;       /**< Fatal callback argument                    */
    osal_mutex_id_t                 mutex_id;               /**< OSAL mutex id                              */
    osal_milli_sec_t                time_period;            /**< Mutex timeout value by [msec] order        */
} st_impdrv_cmn_init_data_t;

/*******************************************************************************************************************//**
 * @typedef st_impdrv_corectl_func_t
 * 
***********************************************************************************************************************/
typedef struct
{
    bool (*p_impdrvCorectlIsValidCore)(
        const uint32_t  core_num
    );  /**< Function table for distinguishing each core. Valid core.   */

    e_impdrv_errorcode_t (*p_impdrvCorectlCheckState)(
        const st_impdrv_corectl_t   *const p_corectl,
        const uint32_t              core_num,
        const e_impdrv_state_t      state
    );  /**< Function table for distinguishing each core. Check state.  */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetState)(
        st_impdrv_corectl_t         *const p_corectl,
        const e_impdrv_state_t      state
    );  /**< Function table for distinguishing each core. Set state.    */

    e_impdrv_errorcode_t (*p_impdrvCorectlInitStart)(
        st_impdrv_coretypectl_t     *const p_coretypectl,
        const uint32_t              core_num
    );  /**< Function table for distinguishing each core. Init start.   */

    e_impdrv_errorcode_t (*p_impdrvCorectlInitEnd)(
        st_impdrv_coretypectl_t     *const p_coretypectl,
        const uint32_t              core_num
    );  /**< Function table for distinguishing each core. Init end.     */

    e_impdrv_errorcode_t (*p_impdrvCorectlStart)(
        st_impdrv_coretypectl_t     *const p_coretypectl,
        const uint32_t              core_num
    );  /**< Function table for distinguishing each core. Start core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlStop)(
        st_impdrv_coretypectl_t     *const p_coretypectl,
        const uint32_t              core_num
    );  /**< Function table for distinguishing each core. Stop core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlAttInit)(
        st_impdrv_corectl_t         *const p_corectl,
        const uint32_t              core_num
    );  /**< Function table for distinguishing each core. Attribute initialize. */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetMemInit)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num,
        const e_impdrv_param_t  enable
    );  /**< Function table for distinguishing each core. Set core memory initialize.   */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetCoreMap)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num,
        const uint8_t           syncc_val[IMPDRV_COREMAP_MAXID]
    );  /**< Function table for distinguishing each core. Set sync core map.    */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetCl)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num,
        const uint32_t          claddr_phys
    );  /**< Function table for distinguishing each core. Set CL address.   */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetIrqMask)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num,
        const bool              irq_mask[IMPDRV_IRQMASK_MAX]
    );  /**< Function table for distinguishing each core. Set IRQ mask setting. */

    e_impdrv_errorcode_t (*p_impdrvCorectlExecute)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num,
        const p_impdrv_cbfunc_t callback_func,
        void                    *const p_callback_args
    );  /**< Function table for distinguishing each core. Execute core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlIntHandler)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num
    );  /**< Function table for distinguishing each core. int handler imp control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlResumeExe)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num
    );  /**< Function table for distinguishing each core. Resume core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlPmSetPolicy)(
        st_impdrv_coretypectl_t        *const p_coretypectl,
        const uint32_t                  core_num,
        const e_impdrv_pm_policy_t      policy
    );  /**< Function table for distinguishing each core. Pm set policy core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlPmGetPolicy)(
        st_impdrv_coretypectl_t         *const p_coretypectl,
        const uint32_t                  core_num,
        e_impdrv_pm_policy_t            *const p_policy
    );  /**< Function table for distinguishing each core. Pm get policy core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetDsp)(
        st_impdrv_corectl_t         *p_corectl,
        const uint32_t              core_num,
        const st_impdrv_dsp_data_t  *const p_dsp_app,
        const st_impdrv_dsp_data_t  *const p_dsp_fw,
        const st_impdrv_dsp_data_t  *const p_dsp_data,
        const st_impdrv_dsp_data_t  *const p_dsp_dtcm
    );  /**< Function table for distinguishing each core. Pm get policy core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlBusIfCheck)(
        st_impdrv_coretypectl_t         *const p_coretypectl,
        const uint32_t                  core_num,
        const st_impdrv_chk_resource_t  *const p_chk_resource
    );  /**< Function table for distinguishing each core. Interface bus check of core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlConfRegCheck)(
        const st_impdrv_coretypectl_t   *const p_coretypectl,
        const uint32_t                  core_num,
        const st_impdrv_chk_param_t     *const p_chk_param,
        const uint32_t                  param_num
    );  /**< Function table for distinguishing each core. Configuration register check of core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetClBrkAddr)(
        st_impdrv_corectl_t     *const p_corectl,
        const uint32_t          core_num,
        const uint32_t          cl_brk_addr
    );  /**< Function table for distinguishing each core. Set CL break address.   */

    e_impdrv_errorcode_t (*p_impdrvCorectlSetGosubCond)(
        st_impdrv_corectl_t         *const p_corectl,
        const uint32_t              core_num,
        const e_impdrv_gosub_cond_t condition
    );  /**< Function table for distinguishing each core. Set Conditional GOSUB instruction attribute.  */

    e_impdrv_errorcode_t (*p_impdrvCorectlPmGetState)(
        st_impdrv_coretypectl_t     *const p_coretypectl,
        const uint32_t              core_num,
        e_impdrv_pm_state_t         *const p_pmstate
    );  /**< Function table for distinguishing each core. Pm get state core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlQuit)(
        const uint32_t              core_num
    );  /**< Function table for distinguishing each core. Quit core control. */

    e_impdrv_errorcode_t (*p_impdrvCorectlSyncStart)(
        st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX],
        const uint32_t          core_num
    );  /**< Function table for distinguishing each core. Starts controlling the core in sync. */
    e_impdrv_errorcode_t (*p_impdrvCorectlSyncStop)(
        st_impdrv_coretypectl_t coretype_ctl[IMPDRV_CORE_TYPE_MAX],
        const uint32_t          core_num
    );  /**< Function table for distinguishing each core. Stop controlling the core in sync. */

} st_impdrv_corectl_func_t;

/*******************************************************************************************************************//**
 * @typedef p_impdrv_dspctl_dsp_start_t
 * IMPDRV function of control start of DSP core pre-preparation.
***********************************************************************************************************************/
typedef e_impdrv_errorcode_t (*p_impdrv_dspctl_dsp_start_t)(
    st_impdrv_corectl_t                 *p_corectl,
    const uint32_t                      core_num,
    const e_osal_interrupt_priority_t   irq_priority,
    const impdrv_ctrl_handle_t          osal_cb_args
);

/** @} */

/******************************************************************************************************************//**
 * @defgroup IMPDRV_CMNCTL IMPDRV CMNCTL
 *
 * @{
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /** __cplusplus */

/*******************************************************************************************************************//**
* @brief            This function initializes of the Common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_commonctl         Common Control handle
* @param[in]        p_cmn_init_data     Common control init data.
* @param[out]       p_is_dmac_mb_init   DMAC multi-bank init
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
* @retval           IMPDRV_EC_NG_INSTANCE           Instance parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_init(
    st_impdrv_commonctl_t           *const p_commonctl,
    const st_impdrv_cmn_init_data_t *const p_cmn_init_data,
    bool                            *const p_is_dmac_mb_init
);

/*******************************************************************************************************************//**
* @brief            This function uninitializes of the Common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_quit(
    st_impdrv_commonctl_t           *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Judgement which core caused the interrupt.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle.
* @param[in]        irq_channel         IRQ channel associated with OSAL interrupt manager.
* @param[in]        is_fb_callback      Whether to callback.
* @param[out]       core_info           Core lists information.
* @param[out]       p_use_core_num      Number of using core.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_judge_int(
    const st_impdrv_commonctl_t *const p_commonctl,
    const uint64_t              irq_channel,
    st_impdrv_core_info_t       core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                    *const p_use_core_num,
    bool                        is_fb_callback
);

/*******************************************************************************************************************//**
* @brief            Creating mutex object.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle
* @param[in]        mutex_id            To set value of mutex id.
* @param[in]        time_period         Time period.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_create(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const osal_mutex_id_t           mutex_id,
    const osal_milli_sec_t          time_period
);

/*******************************************************************************************************************//**
* @brief            Discard mutex object.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_destroy(
    const st_impdrv_commonctl_t           *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Mutex lock.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_lock(
    st_impdrv_commonctl_t           *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Mutex unlock.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_unlock(
    st_impdrv_commonctl_t           *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Setting IRQ group.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle
* @param[in]        p_irq_param         Setting IRQ group.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_set_irq_group(
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_irq_group_t *const p_irq_param
);

/*******************************************************************************************************************//**
* @brief            Save the current IRQ mask settings.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_save_irq_mask(
    st_impdrv_commonctl_t       *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Force setting the IRQ mask with no group.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle
* @param[in]        p_core_info         Core information
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_force_irq_mask(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_core_info_t     *const p_core_info
);

/*******************************************************************************************************************//**
* @brief            Restore the IRQ mask settings form save data.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_SEQSTATE           Sequence Error
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_restore_irq_mask(
    st_impdrv_commonctl_t       *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Get a function pointer of the Core control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        core_type           Specified core type.
* @return           Return value
* @retval           p_impdrv_corectl_func_t         Struct data of function pointer.
***********************************************************************************************************************/
const st_impdrv_corectl_func_t* impdrv_cmnctl_get_corectl_func(
    const e_impdrv_core_type_t      core_type
);

/*******************************************************************************************************************//**
* @brief            Get a function pointer of the Control start of DSP core pre-preparation.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param            None.
* @return           Return value
* @retval           p_impdrv_dspctl_dsp_start_t     function pointer.
***********************************************************************************************************************/
p_impdrv_dspctl_dsp_start_t impdrv_cmnctl_get_dsp_func(
    void
);

/*******************************************************************************************************************//**
* @brief            Get core sync value.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       syncc_val           Core sync value
* @param[in]        p_commonctl         Common Control handle
* @param[in]        core_map            Setting Core map.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_syncc_val(
    uint8_t                     syncc_val[IMPDRV_COREMAP_MAXID],
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID]
);

/*******************************************************************************************************************//**
* @brief            Get core sync value.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl          Common Control handle
* @param[in]        p_core_info          Core information
* @param[in]        fatal_code           IMP driver Fatal code.
* @param[in]        error_code           IMP driver Error code.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_fatal_cb(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_fatalcode_t      fatal_code,
    const e_impdrv_errorcode_t      error_code
);

/*******************************************************************************************************************//**
* @brief            Check the Instance number value.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        instance_num        Specified instance number
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_chk_instance_num(
    const e_impdrv_instance_t       instance_num
);

/*******************************************************************************************************************//**
* @brief            Get the Control handle of 'imp_top_00'.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_handle            Device handle of 'imp_top' resource
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_io_imptop(
    st_impdrv_device_handle_t       *const p_handle
);

/*******************************************************************************************************************//**
* @brief            Get the Control handle of 'imp_dta_00'.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_handle            Device handle of 'imp_dta' resource
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_io_dta(
    st_impdrv_device_handle_t       *const p_handle
);

/*******************************************************************************************************************//**
* @brief            Get the Control handle of 'vdsp_xx'.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[out]       p_handle            Device handle of 'vdsp_00' resource
* @param[in]        core_num
* @param[in]        sub_deviceid
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_io_dsp(
    st_impdrv_device_handle_t       ** const p_handle,
    const uint32_t                  core_num,
    const uint32_t                  subdevice_id
);

/*******************************************************************************************************************//**
* @brief            Get the initialize parameter of DSP.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl                Common Control handle
* @param[out]       p_irq_priority             the value of interrupt priority
* @param[out]       p_osal_cb_args             interrupt handler function's argument.
* @return           Return value
* @retval           IMPDRV_EC_OK               Success completion
* @retval           IMPDRV_EC_NG_ARGNULL       Arguments is NULL
* @retval           IMPDRV_EC_NG_NOTSUPPORT    Not supported parameter error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_init_param(
    const st_impdrv_commonctl_t *const p_commonctl,
    e_osal_interrupt_priority_t *const p_irq_priority,
    impdrv_ctrl_handle_t        *const p_osal_cb_args
);

/*******************************************************************************************************************//**
* @brief            Configuration register check in Common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in, out]   p_commonctl         Common Control handle
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_conf_reg_check(
    const st_impdrv_commonctl_t     *const p_commonctl
);

/*******************************************************************************************************************//**
* @brief            Calculate checksum in Common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_data              Start address
* @param[in]        size                Data size
* @param[out]       p_checksum          Checksum value
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_calc_checksum(
    const void*         *const p_data,
    const size_t        size,
    uint32_t            *const p_checksum
);

#ifdef __cplusplus
}
#endif /** __cplusplus */

/** @} */

#endif /** R_IMPDRV_CMNCTL_H */

/*======================================================================================================================
End of File
======================================================================================================================*/

