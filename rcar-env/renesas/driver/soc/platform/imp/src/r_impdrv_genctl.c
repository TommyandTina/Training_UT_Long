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
* File Name    : r_impdrv_genctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_genctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_genctl.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
#include "r_impdrv_private.h"

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Static_Variables Private file static variables
 *
 * @{
***********************************************************************************************************************/
IMPDRV_STATIC
const e_impdrv_core_type_t s_impdrv_core_type_table[IMPDRV_CORE_TYPE_MAX - 1U] =
{
    /** core_type  */
    IMPDRV_CORE_TYPE_IMP        ,   /**< IMP core               */
    IMPDRV_CORE_TYPE_IMP_SLIM   ,   /**< Slim-IMP core          */
    IMPDRV_CORE_TYPE_OCV        ,   /**< OCV core               */
    IMPDRV_CORE_TYPE_DMAC       ,   /**< DMAC core              */
    IMPDRV_CORE_TYPE_DMAC_SLIM  ,   /**< Slim-DMAC core         */
    IMPDRV_CORE_TYPE_PSCEXE     ,   /**< PSC(exe) core          */
    IMPDRV_CORE_TYPE_PSCOUT     ,   /**< PSC(out) core          */
    IMPDRV_CORE_TYPE_CNN        ,   /**< CNN core               */
    IMPDRV_CORE_TYPE_DSP        ,   /**< DSP core               */
    IMPDRV_CORE_TYPE_DTA            /**< Debug Trace Agent      */
};

/** @} */

/******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Functions Private function definitions
 *
 * @{
 *********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @brief           Check the Input value of core information.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       core_info           Specified core information
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_chk_core_info(
    const st_impdrv_core_info_t core_info
);

/*******************************************************************************************************************//**
 * @brief           Check the Core status is initialized.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[out]      p_funcs             Array of specified control function
 * @param[out]      init_core_info      Array of the core of the Init state
 * @param[out]      p_init_core_num     Pointer to number of cores in Init state
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_chk_state_init(
    const st_impdrv_ctl_t               *const p_impdrv_ctl,
    const st_impdrv_corectl_func_t      *p_funcs[IMPDRV_CORE_TYPE_MAX],
    st_impdrv_core_info_t               init_core_info[IMPDRV_INNER_FIXED_VALUE],
    uint32_t                            *const p_init_core_num
);

/*******************************************************************************************************************//**
 * @brief           Check the Core status is uninitialized.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_chk_state_uninit(
    const st_impdrv_ctl_t       *const p_impdrv_ctl
);

/*******************************************************************************************************************//**
 * @brief           Set the Core status is uninitialized.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_set_state_uninit(
    st_impdrv_ctl_t     *const p_impdrv_ctl
);

/*******************************************************************************************************************//**
 * @brief           Prologue processing for General control.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[in]       core_info           Specified core information
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_prologue(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
);

/*******************************************************************************************************************//**
 * @brief           Epilogue processing for General control.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[in]       core_info           Specified core information
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_***                Final error code
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_epilogue(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const e_impdrv_errorcode_t  ercd
);

/*******************************************************************************************************************//**
 * @brief           Get control function in the Initialization routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        Pointer to initialization data
 * @param[in]       use_core_num        Number of core information
 * @param[in, out]  p_core_array        Array of specified core information
 * @param[in, out]  p_func_array        Array of specified control function
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_get_func(
    const st_impdrv_initdata_t      *const p_data,
    const uint32_t                  use_core_num,
    st_impdrv_core_info_t           *const p_core_array,
    const st_impdrv_corectl_func_t  **const p_func_array
);

/*******************************************************************************************************************//**
 * @brief           Core status check in the Initialization routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[in]       use_core_num        Number of core information
 * @param[in]       p_core_info         Array of specified core information
 * @param[in]       p_funcs             Array of specified control function
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_chk_state(
    const st_impdrv_ctl_t           *const p_impdrv_ctl,
    const uint32_t                  use_core_num,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_corectl_func_t  **const p_funcs
);

/*******************************************************************************************************************//**
 * @brief           Update core status in the Initialization routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[in]       use_core_num        Number of core information
 * @param[in]       p_core_info         Array of specified core information
 * @param[in]       p_funcs             Array of specified control function
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_set_state(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const uint32_t                  use_core_num,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_corectl_func_t  **const p_funcs
);

/*******************************************************************************************************************//**
 * @brief           Attribute data initialization in the Initialization routine.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[in]       use_core_num        Number of core information
 * @param[in]       p_core_info         Array of specified core information
 * @param[in]       p_funcs             Array of specified control function
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_attr_init(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const uint32_t                  use_core_num,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_corectl_func_t  **const p_funcs
);

/*******************************************************************************************************************//**
 * @brief           Core processing during initialization.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_impdrv_ctl        General control handle
 * @param[in]       core_info           Core information
 * @param[in]       p_funcs             Core function information
 * @param[in]       use_core_num        Use Core num
 * @param[in]       is_dmac_mb_init     DMAC multi-bank init
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_core_init(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const st_impdrv_core_info_t     core_info[IMPDRV_INNER_FIXED_VALUE],
    const st_impdrv_corectl_func_t  *const p_funcs[IMPDRV_INNER_FIXED_VALUE],
    const uint32_t                  use_core_num,
    const bool                      is_dmac_mb_init
);

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_genctl_init()
* CD_PD_02_02_0001
* [Covers: UD_PD_UD02_02_0001]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_init(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    st_impdrv_initdata_t        *const p_data,
    const impdrv_ctrl_handle_t  osal_cb_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    func_ercd;
    bool                    is_mtx_cre = false;
    bool                    is_mtx_lock = false;
    uint32_t                use_core_num = 0U;
    uint32_t                cnt;

    st_impdrv_core_info_t       core_info[IMPDRV_INNER_FIXED_VALUE];
    st_impdrv_cmn_init_data_t   cmn_init_data;
    bool                        is_dmac_mb_init;

    const st_impdrv_corectl_func_t  *p_funcs[IMPDRV_INNER_FIXED_VALUE];

    for (cnt = 0U; IMPDRV_INNER_FIXED_VALUE > cnt; cnt++)
    {
        /** Initialize of function pointers  */
        p_funcs[cnt] = NULL;
    }

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_data) || (NULL == osal_cb_args))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == p_data->use_core_num) || (IMPDRV_INNER_FIXED_VALUE < p_data->use_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Initialize for common control init information */
        cmn_init_data.instance_num = p_data->instance_num;
        cmn_init_data.irq_priority = p_data->osal_resource.irq_priority;
        cmn_init_data.p_core_info  = p_data->core_info;
        cmn_init_data.use_core_num = p_data->use_core_num;
        cmn_init_data.osal_cb_args = osal_cb_args;
        cmn_init_data.cbfunc_fatal = p_data->callback_func_fatal;
        cmn_init_data.p_cb_args_fatal = p_data->p_cb_args_fatal;
        cmn_init_data.mutex_id = p_data->osal_resource.mutex_id;
        cmn_init_data.time_period = p_data->osal_resource.mutex_time_period;

        /** initialize local use_core_num variables */
        use_core_num = p_data->use_core_num;
    }

    /** Initialize the core_info and p_funcs arrays */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_init_get_func(p_data, use_core_num, core_info, p_funcs);
    }

    /** Request cmnctl to create the mutex resource */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_create(
            &p_impdrv_ctl->common_ctl,
            p_data->osal_resource.mutex_id,
            p_data->osal_resource.mutex_time_period
        );
    }

    /** Lock the mutex */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Flag on the mutex create  */
        is_mtx_cre = true;

        /** Request cmnctl to lock the mutex */
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
    }

    /** Check_state IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Flag on the mutex lock  */
        is_mtx_lock = true;

        /** Check IMPDRV_STATE_UNINIT state for each core */
        ercd = impdrv_genctl_init_chk_state(p_impdrv_ctl, use_core_num, core_info, p_funcs);
    }

    /** Request cmnctl to initialize common resource */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_init(&p_impdrv_ctl->common_ctl, &cmn_init_data, &is_dmac_mb_init);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_init(&p_impdrv_ctl->common_ctl);
        }
    }
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_mem_init(&p_impdrv_ctl->common_ctl);
    }

    /** Core Init */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_core_init(p_impdrv_ctl, core_info, p_funcs, use_core_num, is_dmac_mb_init);
    }

    /** Set State IMPDRV_STATE_INIT and Attribute Init*/
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_init_set_state(p_impdrv_ctl, use_core_num, core_info, p_funcs);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_genctl_init_attr_init(p_impdrv_ctl, use_core_num, core_info, p_funcs);
        }
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        /** Request cmnctl to release the mutex */
        func_ercd = impdrv_cmnctl_mutex_unlock(
            &p_impdrv_ctl->common_ctl
        );
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = func_ercd;
        }
    }

    /** Post-processing after function execution */
    if ((IMPDRV_EC_OK != ercd) && (true == is_mtx_cre))
    {
        /** When after creating the mutex, recover with Quit function. */
        /** Do not update because an error code has already been set. */
        (void)impdrv_genctl_quit(p_impdrv_ctl);
    }

    /** Output with Control table size set as input parameter */
    if (NULL != p_data)
    {
        p_data->out_work_size = (uint32_t)sizeof(st_impdrv_ctl_t);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_attr_init()
* CD_PD_02_02_0009
* [Covers: UD_PD_UD02_02_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_attr_init(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlAttInit))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlAttInit(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_quit()
* CD_PD_02_02_0002
* [Covers: UD_PD_UD02_02_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_quit(
    st_impdrv_ctl_t   *const p_impdrv_ctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t            ercd_func;
    bool                            is_mtx_lock = false;
    uint32_t                        cnt;
    const st_impdrv_corectl_func_t  *p_funcs[IMPDRV_CORE_TYPE_MAX];
    st_impdrv_core_info_t           init_core_info[IMPDRV_INNER_FIXED_VALUE];
    uint32_t                        init_core_num = 0U;

    /** Initialize of function pointers  */
    for (cnt = 0U; IMPDRV_CORE_TYPE_MAX > cnt; cnt++)
    {
        p_funcs[cnt] = NULL;
    }

    /** Input parameter confirmation process */
    if (NULL == p_impdrv_ctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
    }

    /** Check if all core states are IMPDRV_STATE_INIT or IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        is_mtx_lock = true;
        ercd = impdrv_genctl_chk_state_init(p_impdrv_ctl, p_funcs, init_core_info, &init_core_num);
    }

    /** Core Quit */
    if (IMPDRV_EC_OK == ercd)
    {
        cnt = 0U;
        while (cnt < init_core_num)
        {
            if ((NULL != p_funcs[init_core_info[cnt].core_type])
                && (NULL != p_funcs[init_core_info[cnt].core_type]->p_impdrvCorectlQuit))
            {
                ercd = p_funcs[init_core_info[cnt].core_type]->p_impdrvCorectlQuit(
                            init_core_info[cnt].core_num
                        );
            }

            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }

            cnt++;
        }
    }

    /** IMP TOP quit process */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_quit(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_quit(&p_impdrv_ctl->common_ctl);
        }
    }

    /** Set State IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_set_state_uninit(p_impdrv_ctl);
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Destroy the mutex */
    ercd_func = impdrv_cmnctl_mutex_destroy(&p_impdrv_ctl->common_ctl);
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = ercd_func;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_start()
* CD_PD_02_02_0003
* [Covers: UD_PD_UD02_02_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_start(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    is_mtx_lock = false;

    p_impdrv_dspctl_dsp_start_t     func_dsp;
    const st_impdrv_corectl_func_t  *p_funcs;

    e_osal_interrupt_priority_t irq_priority;
    impdrv_ctrl_handle_t        osal_cb_args = NULL;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);

    if (IMPDRV_EC_OK == ercd)
    {
        /** Flag on the mutex lock */
        is_mtx_lock = true;
    }

    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_CORE_TYPE_DSP == p_core_info->core_type))
    {
        ercd = impdrv_cmnctl_get_init_param(&p_impdrv_ctl->common_ctl, &irq_priority, &osal_cb_args);

        if (IMPDRV_EC_OK == ercd)
        {
            /** Get DSP core control function pointer */
            func_dsp = impdrv_cmnctl_get_dsp_func();
            if(NULL != func_dsp)
            {
                /** Call core control function of DSP */
                ercd = func_dsp(&(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                                                                    p_core_info->core_num, irq_priority, osal_cb_args);
            }
            else
            {
                /** Error not support*/
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSyncStart))
        {
            /** Call core control function of specified core */
           ercd = p_funcs->p_impdrvCorectlSyncStart(p_impdrv_ctl->coretype_ctl,  p_core_info->core_num);
        }
        else if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlStart))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlStart(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                p_core_info->core_num
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    if (true == is_mtx_lock)
    {
        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_stop()
* CD_PD_02_02_0004
* [Covers: UD_PD_UD02_02_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_stop(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSyncStop))
        {
            /** Call core control function of specified core */
           ercd = p_funcs->p_impdrvCorectlSyncStop(p_impdrv_ctl->coretype_ctl,  p_core_info->core_num);
        }
        else if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlStop))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlStop(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                p_core_info->core_num
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_execute()
* CD_PD_02_02_0005
* [Covers: UD_PD_UD02_02_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_execute(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if (NULL == callback_func)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Mutex lock and prologue */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlExecute))
        {
            ercd = p_funcs->p_impdrvCorectlExecute(
                    &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                    p_core_info->core_num,
                    callback_func,
                    p_callback_args
            );
        }
        else
        {
            /** Error Handle */
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_resume_exe()
* CD_PD_02_02_0006
* [Covers: UD_PD_UD02_02_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_resume_exe(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
)
{
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);

    /* resume_exe */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlResumeExe))
        {
            ercd = p_funcs->p_impdrvCorectlResumeExe(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num
            );
        }
        else
        {
            /** Error Handle */
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_resume_exe()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_pm_set_policy()
* CD_PD_02_02_0007
* [Covers: UD_PD_UD02_02_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_pm_set_policy(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const e_impdrv_pm_policy_t  policy
)
{
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if ((IMPDRV_PM_POLICY_CG != policy) && (IMPDRV_PM_POLICY_PG != policy) && (IMPDRV_PM_POLICY_HP != policy))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** Mutex lock and prologue */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    }

    /* Changes the Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlPmSetPolicy))
        {
            ercd = p_funcs->p_impdrvCorectlPmSetPolicy(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                p_core_info->core_num,
                policy
            );
        }
        else
        {
            /** Error Handle */
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_pm_set_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_pm_get_policy()
* CD_PD_02_02_0008
* [Covers: UD_PD_UD02_02_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_pm_get_policy(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    e_impdrv_pm_policy_t        *const p_policy
)
{
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if (NULL == p_policy)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Mutex lock and prologue */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    }

    /* get the Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlPmGetPolicy))
        {
            ercd = p_funcs->p_impdrvCorectlPmGetPolicy(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                p_core_info->core_num,
                p_policy
            );
        }
        else
        {
            /** Error Handle */
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_pm_get_policy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_pm_get_state()
* CD_PD_02_02_0015
* [Covers: UD_PD_UD02_02_0015]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_pm_get_state(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    e_impdrv_pm_state_t         *const p_pmstate
)
{
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if (NULL == p_pmstate)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Mutex lock and prologue */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    }

    /* get the Power management state */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlPmGetState))
        {
            ercd = p_funcs->p_impdrvCorectlPmGetState(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                p_core_info->core_num,
                p_pmstate
            );
        }
        else
        {
            /** Error Handle */
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_pm_get_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_core_mem_init()
* CD_PD_02_02_0010
* [Covers: UD_PD_UD02_02_0010]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_core_mem_init(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const e_impdrv_param_t      enable
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetMemInit))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlSetMemInit(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num,
                enable
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_core_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_core_map()
* CD_PD_02_02_0011
* [Covers: UD_PD_UD02_02_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_core_map(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint8_t                 syncc_val[IMPDRV_COREMAP_MAXID];
    uint32_t                i;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Convert core map information to SYNCC register values */
        for (i = 0U; IMPDRV_COREMAP_MAXID > i; i++)
        {
            syncc_val[i] = 0U;
        }
        ercd = impdrv_cmnctl_get_syncc_val(syncc_val, &p_impdrv_ctl->common_ctl, core_map);

        /** Get core control function pointer of specified core */
        if (IMPDRV_EC_OK == ercd)
        {
            p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
            if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetCoreMap))
            {
                /** Call core control function of specified core */
                ercd = p_funcs->p_impdrvCorectlSetCoreMap(
                    &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                    p_core_info->core_num,
                    syncc_val
                );
            }
            else
            {
                /** Error Handle*/
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_core_map()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_cl()
* CD_PD_02_02_0012
* [Covers: UD_PD_UD02_02_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_cl(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const uint32_t              claddr_phys
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetCl))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlSetCl(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num,
                claddr_phys
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_cl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_dsp()
* CD_PD_02_02_0043
* [Covers: UD_PD_UD02_02_0043]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_dsp(
    st_impdrv_ctl_t             *p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const st_impdrv_dsp_data_t  *const p_dsp_app,
    const st_impdrv_dsp_data_t  *const p_dsp_fw,
    const st_impdrv_dsp_data_t  *const p_dsp_data,
    const st_impdrv_dsp_data_t  *const p_dsp_dtcm
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetDsp))
        {
            ercd = p_funcs->p_impdrvCorectlSetDsp(
                    &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                    p_core_info->core_num,
                    p_dsp_app,
                    p_dsp_fw,
                    p_dsp_data,
                    p_dsp_dtcm
                    );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_dsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_irq_mask()
* CD_PD_02_02_0013
* [Covers: UD_PD_UD02_02_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_irq_mask(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const bool                  irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetIrqMask))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlSetIrqMask(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num,
                irq_mask
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_irq_group()
* CD_PD_02_02_0014
* [Covers: UD_PD_UD02_02_0014]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_irq_group(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_irq_group_t *const p_irq_param
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_func;
    bool                    is_mtx_lock = false;
    uint32_t                cnt;
    st_impdrv_core_info_t   core_info;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_irq_param))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == p_irq_param->group_core_num) || (IMPDRV_IRQGROUP_MAXID < p_irq_param->group_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        cnt = 0U;
        while (p_irq_param->group_core_num > cnt)
        {
            ercd = impdrv_genctl_chk_core_info(p_irq_param->group_core_info[cnt]);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            cnt++;
        }
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
    }

    /** Check_state IMPDRV_STATE_READY */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Flag on the mutex lock  */
        is_mtx_lock = true;

        /** Check IMPDRV_STATE_READY state for each core */
        cnt = 0U;
        while (cnt < p_irq_param->group_core_num)
        {
            /** Get core control function pointer of each core */
            core_info = p_irq_param->group_core_info[cnt];
            p_funcs = impdrv_cmnctl_get_corectl_func(core_info.core_type);
            if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlCheckState))
            {
                /** Check IMPDRV_STATE_READY state */
                ercd = p_funcs->p_impdrvCorectlCheckState(
                        &(p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num]),
                        core_info.core_num,
                        IMPDRV_STATE_READY
                    );
            }
            else
            {
                /** Error Handle*/
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            /** To the next use core */
            cnt++;
        }
    }

    /** Attribute data setting for IRQ group */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_set_irq_group(
            &p_impdrv_ctl->common_ctl,
            p_irq_param
        );
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_irq_group()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_cl_brk_addr()
* CD_PD_02_02_0041
* [Covers: UD_PD_UD02_02_0041]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_cl_brk_addr(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const uint32_t              cl_brk_addr
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetClBrkAddr))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlSetClBrkAddr(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num,
                cl_brk_addr
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_cl_brk_addr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_gosub_cond()
* CD_PD_02_02_0042
* [Covers: UD_PD_UD02_02_0042]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_set_gosub_cond(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info,
    const e_impdrv_gosub_cond_t condition
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlSetGosubCond))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlSetGosubCond(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num,
                condition
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_gosub_cond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_reg_set_mem_protect()
* CD_PD_02_02_0024
* [Covers: UD_PD_UD02_02_0024]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_reg_set_mem_protect(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const e_impdrv_protect_mode_t   protect_mode
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t            ercd_func;
    bool                            is_mtx_lock = false;
    uint32_t                        cnt;
    const st_impdrv_corectl_func_t  *p_funcs[IMPDRV_CORE_TYPE_MAX];
    st_impdrv_core_info_t           init_core_info[IMPDRV_INNER_FIXED_VALUE];
    uint32_t                        init_core_num = 0U;

    /** Initialize of function pointers  */
    for (cnt = 0U; IMPDRV_CORE_TYPE_MAX > cnt; cnt++)
    {
        p_funcs[cnt] = NULL;
    }

    /** Input parameter confirmation process */
    if (NULL == p_impdrv_ctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PROTECT_EDC != protect_mode) && (IMPDRV_PROTECT_ECC != protect_mode))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
    }

    /** Check if all core states are IMPDRV_STATE_INIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Flag on the mutex lock */
        is_mtx_lock = true;

        /** Check the Core status */
        ercd = impdrv_genctl_chk_state_init(p_impdrv_ctl, p_funcs, init_core_info, &init_core_num);
    }

    /** Entrust processing to common control component. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_set_mem_protect(
            &p_impdrv_ctl->common_ctl,
            protect_mode
        );
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_reg_set_mem_protect()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_reg_get_hw_info()
* CD_PD_02_02_0025
* [Covers: UD_PD_UD02_02_0025]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_reg_get_hw_info(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_func;
    bool                    is_mtx_lock = false;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_device_id) || (NULL == p_reg_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ('\0' == p_device_id[0])
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Flag on the mutex lock */
            is_mtx_lock = true;
        }
    }

    /** Check if all core states are IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check the Core status */
        ercd = impdrv_genctl_chk_state_uninit(p_impdrv_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            ercd = IMPDRV_EC_OK;
        }
    }

    /** Entrust processing to common control component. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_get_hw_info(
            &p_impdrv_ctl->common_ctl,
            p_device_id,
            p_reg_info
        );
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_reg_get_hw_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_reg_read32()
* CD_PD_02_02_0026
* [Covers: UD_PD_UD02_02_0026]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_reg_read32(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_func;
    bool                    is_mtx_lock = false;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_device_id) || (NULL == p_data))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ('\0' == p_device_id[0])
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (0U != (offset & IMPDRV_CHK_REG_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Flag on the mutex lock */
            is_mtx_lock = true;
        }
    }

    /** Check if all core states are IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check the Core status */
        ercd = impdrv_genctl_chk_state_uninit(p_impdrv_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            ercd = IMPDRV_EC_OK;
        }
    }

    /** Entrust processing to common control component. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_read32(
            &p_impdrv_ctl->common_ctl,
            p_device_id,
            offset,
            p_data
        );
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_reg_read32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_reg_write32()
* CD_PD_02_02_0027
* [Covers: UD_PD_UD02_02_0027]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_reg_write32(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_func;
    bool                    is_mtx_lock = false;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_device_id))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ('\0' == p_device_id[0])
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (0U != (offset & IMPDRV_CHK_REG_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Flag on the mutex lock */
            is_mtx_lock = true;
        }
    }

    /** Check if all core states are IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check the Core status */
        ercd = impdrv_genctl_chk_state_uninit(p_impdrv_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            ercd = IMPDRV_EC_OK;
        }
    }

    /** Entrust processing to common control component. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_write32(
            &p_impdrv_ctl->common_ctl,
            p_device_id,
            offset,
            data
        );
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_reg_write32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_reg_required()
* CD_PD_02_02_0028
* [Covers: UD_PD_UD02_02_0028]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_reg_required(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_func;
    bool                    is_mtx_lock = false;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_device_id))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ('\0' == p_device_id[0])
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((IMPDRV_REG_REQ_STATE_REQUIRED != new_state) && (IMPDRV_REG_REQ_STATE_RELEASED != new_state))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Acquired the mutex lock */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Flag on the mutex lock */
            is_mtx_lock = true;
        }
    }

    /** Check if all core states are IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check the Core status */
        ercd = impdrv_genctl_chk_state_uninit(p_impdrv_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            ercd = IMPDRV_EC_OK;
        }
    }

    /** Entrust processing to common control component. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_required(
            &p_impdrv_ctl->common_ctl,
            p_device_id,
            new_state
        );
    }

    /** Release the mutex lock */
    if (true == is_mtx_lock)
    {
        ercd_func = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_reg_required()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_bus_if_check()
* CD_PD_02_02_0034
* [Covers: UD_PD_UD02_02_0034]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_bus_if_check(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) &&
            (NULL != p_funcs->p_impdrvCorectlCheckState) &&
            (NULL != p_funcs->p_impdrvCorectlBusIfCheck))
        {
            /** IMP driver state check */
            ercd = p_funcs->p_impdrvCorectlCheckState(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type].core_ctl[p_core_info->core_num]),
                p_core_info->core_num,
                IMPDRV_STATE_READY
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Save interrupt mask setting */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_save_irq_mask(&p_impdrv_ctl->common_ctl);
        }

        if (IMPDRV_EC_OK == ercd)
        {
            /** Force setting of interrupt mask */
            ercd = impdrv_cmnctl_force_irq_mask(&p_impdrv_ctl->common_ctl, p_core_info);
            if (IMPDRV_EC_OK == ercd)
            {
                    /** Call core control function of specified core */
                    ercd = p_funcs->p_impdrvCorectlBusIfCheck(
                        &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                        p_core_info->core_num,
                        p_chk_resource
                    );
            }

            /** Restore interrupt mask setting */
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_cmnctl_restore_irq_mask(&p_impdrv_ctl->common_ctl);
            }
            else
            {
                (void)impdrv_cmnctl_restore_irq_mask(&p_impdrv_ctl->common_ctl);
            }
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_bus_if_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_conf_reg_check()
* CD_PD_02_02_0035
* [Covers: UD_PD_UD02_02_0035]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_conf_reg_check(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Mutex lock and prologue */
    ercd = impdrv_genctl_prologue(p_impdrv_ctl, p_core_info);
    if (IMPDRV_EC_OK == ercd)
    {
        /** Get core control function pointer of specified core */
        p_funcs = impdrv_cmnctl_get_corectl_func(p_core_info->core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlConfRegCheck))
        {
            /** Call core control function of specified core */
            ercd = p_funcs->p_impdrvCorectlConfRegCheck(
                &(p_impdrv_ctl->coretype_ctl[p_core_info->core_type]),
                p_core_info->core_num,
                p_chk_param,
                param_num
            );
        }
        else
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }

        /** Call common control function of configuration register check */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_conf_reg_check(&p_impdrv_ctl->common_ctl);
        }
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_chk_mem_protect(&p_impdrv_ctl->common_ctl);
        }

        /** Mutex unlock and epilogue */
        ercd = impdrv_genctl_epilogue(p_impdrv_ctl, ercd);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_conf_reg_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_int_handler()
* CD_PD_02_02_0016
* [Covers: UD_PD_UD02_02_0016]
***********************************************************************************************************************/
void impdrv_genctl_int_handler(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const uint64_t              irq_channel,
    const e_impdrv_errorcode_t  ercd
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        fb_ercd = ercd;
    st_impdrv_core_info_t       core_info[IMPDRV_INTSTS_BITNUM];
    uint32_t                    use_core_num;
    uint32_t                    index;
    uint32_t                    fcb_index = 0U;

    const st_impdrv_corectl_func_t  *p_funcs;

    for (index= 0U; index < IMPDRV_INTSTS_BITNUM; index++)
    {
        core_info[index].core_type = IMPDRV_CORE_TYPE_INVALID;
        core_info[index].core_num  = 0U;
    }

    /** Input parameter confirmation process */
    if (IMPDRV_EC_OK == ercd)
    {
        if (NULL == p_impdrv_ctl)
        {
            fb_ercd = IMPDRV_EC_NG_ARGNULL;
        }
    }

    /** Judgement which core caused the interrupt */
    if (IMPDRV_EC_OK == fb_ercd)
    {
        fb_ercd = impdrv_cmnctl_judge_int(&p_impdrv_ctl->common_ctl, irq_channel, core_info, &use_core_num, true);
    }

    /** Core control interrupt handling */
    if (IMPDRV_EC_OK == fb_ercd)
    {
        for (index= 0U; index < use_core_num; index++)
        {
            /** Get core control function pointer of each core */
            p_funcs = impdrv_cmnctl_get_corectl_func(core_info[index].core_type);
            if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlIntHandler) 
                                && (IMPDRV_CORE_TYPE_DSP != core_info[index].core_type))
            {
                fb_ercd = p_funcs->p_impdrvCorectlIntHandler(
                        &(p_impdrv_ctl->coretype_ctl[core_info[index].core_type].core_ctl[core_info[index].core_num]),
                        core_info[index].core_num
                    );
            }
            else
            {
                /** Error Handle*/
                fb_ercd = IMPDRV_EC_NG_NOTSUPPORT;
            }
            if (IMPDRV_EC_OK != fb_ercd)
            {
                /** Error, exit the loop */
                fcb_index = index;
                break;
            }
        }
    }

    /** Fatal callback */
    if (IMPDRV_EC_OK != fb_ercd)
    {
        (void)impdrv_cmnctl_fatal_cb(&p_impdrv_ctl->common_ctl, &core_info[fcb_index], IMPDRV_FC_DRV_ERROR, fb_ercd);
    }
}
/***********************************************************************************************************************
* End of function impdrv_genctl_int_handler()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_check_intclear()
* CD_PD_02_02_0029
* [Covers: UD_PD_UD02_02_0029]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_check_intclear(
    const st_impdrv_core_info_t *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        fb_ercd;

    fb_ercd = IMPDRV_EC_OK;

    if (NULL == p_core_info)
    {
        fb_ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }

    return fb_ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_check_intclear()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_dsp_int_handler()
* CD_PD_02_02_0044
* [Covers: UD_PD_UD02_02_0044]
***********************************************************************************************************************/
void impdrv_genctl_dsp_int_handler(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const uint64_t              irq_channel,
    const e_impdrv_errorcode_t  ercd,
    const st_impdrv_core_info_t core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        fb_ercd = ercd;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if (IMPDRV_EC_OK == ercd)
    {
        if (NULL == p_impdrv_ctl)
        {
            fb_ercd = IMPDRV_EC_NG_ARGNULL;
        }
        else if (0U != irq_channel)
        {
            fb_ercd = IMPDRV_EC_NG_PARAM;
        }
        else
        {
            fb_ercd = impdrv_genctl_chk_core_info(core_info);
        }
    }

    /** Get core control function pointer of each core */
    if (IMPDRV_EC_OK == fb_ercd)
    {
        p_funcs = impdrv_cmnctl_get_corectl_func(core_info.core_type);
        if ((NULL != p_funcs) && (NULL != p_funcs->p_impdrvCorectlIntHandler)
                                && (IMPDRV_CORE_TYPE_DSP == core_info.core_type))
        {
            fb_ercd = p_funcs->p_impdrvCorectlIntHandler(
                    &(p_impdrv_ctl->coretype_ctl[core_info.core_type].core_ctl[core_info.core_num]),
                    core_info.core_num
                );
        }
        else
        {
            /** Error Handle*/
            fb_ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Fatal callback */
    if (IMPDRV_EC_OK != fb_ercd)
    {
        (void)impdrv_cmnctl_fatal_cb(&p_impdrv_ctl->common_ctl, &core_info, IMPDRV_FC_DRV_ERROR, fb_ercd);
    }
}
/***********************************************************************************************************************
* End of function impdrv_genctl_dsp_int_handler()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_genctl_chk_core_info()
* CD_PD_02_02_0021
* [Covers: UD_PD_UD02_02_0021]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_chk_core_info(
    const st_impdrv_core_info_t     core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the Input value of core information */
    if ((IMPDRV_CORE_TYPE_IMP != core_info.core_type)
        && (IMPDRV_CORE_TYPE_IMP_SLIM != core_info.core_type)
        && (IMPDRV_CORE_TYPE_OCV != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DMAC != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DMAC_SLIM != core_info.core_type)
        && (IMPDRV_CORE_TYPE_PSCEXE != core_info.core_type)
        && (IMPDRV_CORE_TYPE_CNN != core_info.core_type)
        && (IMPDRV_CORE_TYPE_DSP != core_info.core_type))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (IMPDRV_CORE_NUM_MAX <= core_info.core_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_chk_core_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_chk_state_init()
* CD_PD_02_02_0030
* [Covers: UD_PD_UD02_02_0030]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_chk_state_init(
    const st_impdrv_ctl_t               *const p_impdrv_ctl,
    const st_impdrv_corectl_func_t      *p_funcs[IMPDRV_CORE_TYPE_MAX],
    st_impdrv_core_info_t               init_core_info[IMPDRV_INNER_FIXED_VALUE],
    uint32_t                            *const p_init_core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_core_type_t        core_type;
    uint32_t                    core_table_num;
    uint32_t                    core_num;
    bool                        is_valid_core = false;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_funcs) || (NULL == init_core_info) || (NULL == p_init_core_num))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Check all core states with INIT. */
    if (IMPDRV_EC_OK == ercd)
    {
        *p_init_core_num = 0U;
        core_table_num = 1U;
        while (core_table_num < IMPDRV_CORE_TYPE_MAX)
        {
            core_type = s_impdrv_core_type_table[core_table_num - 1U];
            p_funcs[core_type] = impdrv_cmnctl_get_corectl_func(core_type);
            if ((NULL != p_funcs[core_type])
                && (NULL != p_funcs[core_type]->p_impdrvCorectlIsValidCore)
                && (NULL != p_funcs[core_type]->p_impdrvCorectlCheckState))
            {
                core_num = 0U;
                while (core_num < IMPDRV_CORE_NUM_MAX)
                {
                    is_valid_core = p_funcs[core_type]->p_impdrvCorectlIsValidCore(core_num);
                    if (true == is_valid_core)
                    {
                        ercd = p_funcs[core_type]->p_impdrvCorectlCheckState(
                            &(p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num]),
                            core_num,
                            IMPDRV_STATE_INIT
                        );
                        if (IMPDRV_EC_OK != ercd)
                        {
                            ercd = p_funcs[core_type]->p_impdrvCorectlCheckState(
                                &(p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num]),
                                core_num,
                                IMPDRV_STATE_UNINIT
                            );
                        }
                        else
                        {
                            init_core_info[*p_init_core_num].core_type = core_type;
                            init_core_info[*p_init_core_num].core_num = core_num;
                            (*p_init_core_num)++;
                        }
                    }
                    else
                    {
                        /** Nothing to do. */
                    }
                    if (IMPDRV_EC_OK != ercd)
                    {
                        /** Error, exit the loop */
                        break;
                    }
                    /** To the next core */
                    core_num++;
                }
            }
            else
            {
                /** Nothing to do. */
            }
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            /** To the next core */
            core_table_num++;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_chk_state_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_chk_state_uninit()
* CD_PD_02_02_0031
* [Covers: UD_PD_UD02_02_0031]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_chk_state_uninit(
    const st_impdrv_ctl_t       *const p_impdrv_ctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_core_type_t        core_type;
    uint32_t                    core_table_num;
    uint32_t                    core_num;
    bool                        is_valid_core = false;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if (NULL == p_impdrv_ctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Check all core states with UNINIT. */
    if (IMPDRV_EC_OK == ercd)
    {
        core_table_num = 1U;
        while (core_table_num < IMPDRV_CORE_TYPE_MAX)
        {
            core_type = s_impdrv_core_type_table[core_table_num - 1U];
            p_funcs = impdrv_cmnctl_get_corectl_func(core_type);
            if ((NULL != p_funcs)
                && (NULL != p_funcs->p_impdrvCorectlIsValidCore)
                && (NULL != p_funcs->p_impdrvCorectlCheckState))
            {
                core_num = 0U;
                while (core_num < IMPDRV_CORE_NUM_MAX)
                {
                    is_valid_core = p_funcs->p_impdrvCorectlIsValidCore(core_num);
                    if (true == is_valid_core)
                    {
                        ercd = p_funcs->p_impdrvCorectlCheckState(
                            &(p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num]),
                            core_num,
                            IMPDRV_STATE_UNINIT
                        );
                    }
                    else
                    {
                        /** Nothing to do. */
                    }
                    if (IMPDRV_EC_OK != ercd)
                    {
                        /** Error, exit the loop */
                        break;
                    }
                    /** To the next core */
                    core_num++;
                }
            }
            else
            {
                /** Nothing to do. */
            }
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            /** To the next core */
            core_table_num++;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_chk_state_uninit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_set_state_uninit()
* CD_PD_02_02_0032
* [Covers: UD_PD_UD02_02_0032]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_set_state_uninit(
    st_impdrv_ctl_t     *const p_impdrv_ctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_core_type_t        core_type;
    uint32_t                    core_table_num;
    uint32_t                    core_num;
    bool                        is_valid_core = false;

    const st_impdrv_corectl_func_t  *p_funcs;

    /** Input parameter confirmation process */
    if (NULL == p_impdrv_ctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Set all core states with UNINIT. */
    if (IMPDRV_EC_OK == ercd)
    {
        core_table_num = 1U;
        while (core_table_num < IMPDRV_CORE_TYPE_MAX)
        {
            core_type = s_impdrv_core_type_table[core_table_num - 1U];
            p_funcs = impdrv_cmnctl_get_corectl_func(core_type);
            if ((NULL != p_funcs)
                && (NULL != p_funcs->p_impdrvCorectlIsValidCore)
                && (NULL != p_funcs->p_impdrvCorectlSetState))
            {
                core_num = 0U;
                while (core_num < IMPDRV_CORE_NUM_MAX)
                {
                    is_valid_core = p_funcs->p_impdrvCorectlIsValidCore(
                        core_num
                    );
                    if (true == is_valid_core)
                    {
                        ercd = p_funcs->p_impdrvCorectlSetState(
                            &(p_impdrv_ctl->coretype_ctl[core_type].core_ctl[core_num]),
                            IMPDRV_STATE_UNINIT
                        );
                    }
                    else
                    {
                        /** Nothing to do. */
                    }
                    /** Error, exit the loop */
                    if (IMPDRV_EC_OK != ercd)
                    {
                        break;
                    }
                    /** To the next core */
                    core_num++;
                }
            }
            else
            {
                /** Nothing to do. */
            }
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            /** To the next core */
            core_table_num++;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_set_state_uninit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_prologue()
* CD_PD_02_02_0022
* [Covers: UD_PD_UD02_02_0022]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_prologue(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const st_impdrv_core_info_t *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_genctl_chk_core_info(*p_core_info);
    }

    /** Request cmnctl to lock the mutex */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_mutex_lock(&p_impdrv_ctl->common_ctl);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_prologue()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_epilogue()
* CD_PD_02_02_0023
* [Covers: UD_PD_UD02_02_0023]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_epilogue(
    st_impdrv_ctl_t             *const p_impdrv_ctl,
    const e_impdrv_errorcode_t  ercd
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    retcd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_impdrv_ctl)
    {
        retcd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Request cmnctl to release the mutex */
    if (IMPDRV_EC_OK == retcd)
    {
        retcd = impdrv_cmnctl_mutex_unlock(&p_impdrv_ctl->common_ctl);
    }

    /** Restore error code */
    if (IMPDRV_EC_OK != ercd)
    {
        retcd = ercd;
    }

    /** Set error code in return value */
    return retcd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_epilogue()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_init_get_func()
* CD_PD_02_02_0036
* [Covers: UD_PD_UD02_02_0036]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_get_func(
    const st_impdrv_initdata_t      *const p_data,
    const uint32_t                  use_core_num,
    st_impdrv_core_info_t           *const p_core_array,
    const st_impdrv_corectl_func_t  **const p_func_array
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    uint32_t                        cnt;
    st_impdrv_core_info_t           *p_core_info;
    const st_impdrv_corectl_func_t  **p_funcs;

    /** Input parameter confirmation process */
    if ((NULL == p_data) || (NULL == p_core_array) || (NULL == p_func_array))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == use_core_num) || (IMPDRV_INNER_FIXED_VALUE < use_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        p_core_info = p_core_array;
        p_funcs = p_func_array;
    }

    /** Initialize array */
    if (IMPDRV_EC_OK == ercd)
    {
        cnt = 0U;
        while (cnt < use_core_num)
        {
            /** Initialize of core informations  */
            p_core_info[cnt] = p_data->core_info[cnt];

            /** Check the Input value of core information */
            ercd = impdrv_genctl_chk_core_info(p_core_info[cnt]);
            if (IMPDRV_EC_OK == ercd)
            {
                /** Get core control function pointer of each core */
                p_funcs[cnt] = impdrv_cmnctl_get_corectl_func(p_core_info[cnt].core_type);
                if ((NULL == p_funcs[cnt])
                    || (NULL == p_funcs[cnt]->p_impdrvCorectlCheckState)
                    || (NULL == p_funcs[cnt]->p_impdrvCorectlInitStart)
                    || (NULL == p_funcs[cnt]->p_impdrvCorectlInitEnd)
                    || (NULL == p_funcs[cnt]->p_impdrvCorectlSetState)
                    || (NULL == p_funcs[cnt]->p_impdrvCorectlAttInit))
                {
                    /** Error Handle*/
                    ercd = IMPDRV_EC_NG_NOTSUPPORT;
                }
            }
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            cnt++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_init_get_func()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_init_chk_state()
* CD_PD_02_02_0037
* [Covers: UD_PD_UD02_02_0037]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_chk_state(
    const st_impdrv_ctl_t           *const p_impdrv_ctl,
    const uint32_t                  use_core_num,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_corectl_func_t  **const p_funcs
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    uint32_t                        cnt;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_core_info) || (NULL == p_funcs))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == use_core_num) || (IMPDRV_INNER_FIXED_VALUE < use_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Check_state IMPDRV_STATE_UNINIT */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check IMPDRV_STATE_UNINIT state for each core */
        cnt = 0U;
        while (cnt < use_core_num)
        {
            /** Check IMPDRV_STATE_UNINIT state */
            ercd = p_funcs[cnt]->p_impdrvCorectlCheckState(
                    &(p_impdrv_ctl->coretype_ctl[p_core_info[cnt].core_type].core_ctl[p_core_info[cnt].core_num]),
                    p_core_info[cnt].core_num,
                    IMPDRV_STATE_UNINIT
                );
            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            /** To the next use core */
            cnt++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_init_chk_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_init_set_state()
* CD_PD_02_02_0038
* [Covers: UD_PD_UD02_02_0038]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_set_state(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const uint32_t                  use_core_num,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_corectl_func_t  **const p_funcs
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t            func_ercd;
    uint32_t                        cnt;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_core_info) || (NULL == p_funcs))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == use_core_num) || (IMPDRV_INNER_FIXED_VALUE < use_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Set State IMPDRV_STATE_INIT and Attribute Init*/
    if (IMPDRV_EC_OK == ercd)
    {
        for (cnt = 0U; cnt < use_core_num; cnt++)
        {
            /** Set State IMPDRV_STATE_INIT */
            func_ercd = p_funcs[cnt]->p_impdrvCorectlSetState(
                &(p_impdrv_ctl->coretype_ctl[p_core_info[cnt].core_type].core_ctl[p_core_info[cnt].core_num]),
                IMPDRV_STATE_INIT
            );
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = func_ercd;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_init_set_state()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_init_attr_init()
* CD_PD_02_02_0039
* [Covers: UD_PD_UD02_02_0039]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_init_attr_init(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const uint32_t                  use_core_num,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_corectl_func_t  **const p_funcs
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t            func_ercd;
    uint32_t                        cnt;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_core_info) || (NULL == p_funcs))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == use_core_num) || (IMPDRV_INNER_FIXED_VALUE < use_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Set State IMPDRV_STATE_INIT and Attribute Init*/
    if (IMPDRV_EC_OK == ercd)
    {
        for (cnt = 0U; cnt < use_core_num; cnt++)
        {
            /** Attribute Init*/
            func_ercd = p_funcs[cnt]->p_impdrvCorectlAttInit(
                &(p_impdrv_ctl->coretype_ctl[p_core_info[cnt].core_type].core_ctl[p_core_info[cnt].core_num]),
                p_core_info[cnt].core_num
            );
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = func_ercd;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_init_attr_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_core_init()
* CD_PD_02_02_0033
* [Covers: UD_PD_UD02_02_0033]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_genctl_core_init(
    st_impdrv_ctl_t                 *const p_impdrv_ctl,
    const st_impdrv_core_info_t     core_info[IMPDRV_INNER_FIXED_VALUE],
    const st_impdrv_corectl_func_t  *const p_funcs[IMPDRV_INNER_FIXED_VALUE],
    const uint32_t                  use_core_num,
    const bool                      is_dmac_mb_init
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t            ercd_param = IMPDRV_EC_OK;
    e_impdrv_errorcode_t            func_ercd;
    uint32_t                        cnt;
    bool                            is_dmac_mb_init_exe = false;
    const st_impdrv_corectl_func_t  *p_dmac_func;

    /** Input parameter confirmation process */
    if ((NULL == p_impdrv_ctl) || (NULL == p_funcs) || (NULL == core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U == use_core_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (IMPDRV_INNER_FIXED_VALUE < use_core_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((false != is_dmac_mb_init) && (true != is_dmac_mb_init))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        cnt = 0U;
        while (cnt < use_core_num)
        {
            ercd = impdrv_genctl_chk_core_info(core_info[cnt]);
            if ((IMPDRV_EC_OK == ercd) &&
                    ((NULL == p_funcs[cnt]) ||
                     (NULL == p_funcs[cnt]->p_impdrvCorectlInitStart) ||
                     (NULL == p_funcs[cnt]->p_impdrvCorectlInitEnd)))
            {
                ercd = IMPDRV_EC_NG_ARGNULL;
                /** Error, exit the loop */
                break;
            }
            cnt++;
        }
    }

    /** Core Init */
    if (IMPDRV_EC_OK == ercd)
    {
        cnt = 0U;
        while (cnt < use_core_num)
        {
            /** Start the initialization process of each core */
            ercd = p_funcs[cnt]->p_impdrvCorectlInitStart(
                &(p_impdrv_ctl->coretype_ctl[core_info[cnt].core_type]),
                core_info[cnt].core_num
            );

            /** DMAC Multi-Bank memory initialization */
            if ((IMPDRV_EC_OK == ercd) && (true == is_dmac_mb_init) &&
                ( IMPDRV_CORE_TYPE_DMAC == core_info[cnt].core_type) && (0U == core_info[cnt].core_num))
            {
                is_dmac_mb_init_exe = true;
                ercd = impdrv_dmactl_mb_init(
                    &(p_impdrv_ctl->coretype_ctl[core_info[cnt].core_type].core_ctl[core_info[cnt].core_num]),
                    core_info[cnt].core_num
                );
            }

            /** Start the termination process of each core */
            func_ercd = p_funcs[cnt]->p_impdrvCorectlInitEnd(
                &(p_impdrv_ctl->coretype_ctl[core_info[cnt].core_type]),
                core_info[cnt].core_num
            );

            if (IMPDRV_EC_OK == ercd)
            {
                ercd = func_ercd;
            }

            if (IMPDRV_EC_OK != ercd)
            {
                /** Error, exit the loop */
                break;
            }
            /** To the next use core */
            cnt++;
        }
    }

    /** DMAC Multi-Bank memory initialization */
    if ((IMPDRV_EC_OK == ercd) && (true == is_dmac_mb_init) && (false == is_dmac_mb_init_exe))
    {
        p_dmac_func = impdrv_cmnctl_get_corectl_func(IMPDRV_CORE_TYPE_DMAC);
        if ((NULL == p_dmac_func)
            || (NULL == p_dmac_func->p_impdrvCorectlInitStart)
            || (NULL == p_dmac_func->p_impdrvCorectlInitEnd))
        {
            /** Error Handle*/
            ercd = IMPDRV_EC_NG_PARAM;
            ercd_param = IMPDRV_EC_NG_PARAM;
        }
        /** Start the initialization process of each core */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = p_dmac_func->p_impdrvCorectlInitStart(&(p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DMAC]), 0U);
        }

        /** DMAC Multi-Bank memory initialization */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_dmactl_mb_init(&(p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DMAC].core_ctl[0U]), 0U);
        }

        /** Start the termination process of each core */
        if (IMPDRV_EC_OK == ercd_param)
        {
            func_ercd =p_dmac_func->p_impdrvCorectlInitEnd(&(p_impdrv_ctl->coretype_ctl[IMPDRV_CORE_TYPE_DMAC]), 0U);
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = func_ercd;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_core_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_genctl_get_worksize()
* CD_PD_02_02_0040
* [Covers: UD_PD_UD02_02_0040]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_genctl_get_worksize(
    uint32_t      work_type,
    uint32_t      *p_work_size
)
{
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    if (0u != work_type)
    {
        /* this is dead-code. after resolve coding error. */
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if (NULL == p_work_size)
    {
        /* this is dead-code. after resolve coding error. */
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        *p_work_size = sizeof(st_impdrv_ctl_t);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_genctl_get_worksize()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/

