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
* File Name    : r_impdrv_api.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_api for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_api.c
 */

// #include <stdio.h>
/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
#include "rcar-xos/imp/r_impdrv_version.h"
#include "r_impdrv_private.h"

/*======================================================================================================================
Private macro definitions
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Defines Private macro definitions
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @def IMPDRV_CHK_WORK_ADDR_ALIGN
 * Maximum number of SYNCC table.
***********************************************************************************************************************/
#define IMPDRV_CHK_WORK_ADDR_ALIGN    (7U)      /**< The work area address alignment check definition       */

/** @} */

/*======================================================================================================================
Private function prototypes
======================================================================================================================*/
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
e_impdrv_errorcode_t impdrv_api_chk_core_info(
    const st_impdrv_core_info_t     core_info
);

/*******************************************************************************************************************//**
 * @brief           Check the Instance number value.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num        Specified instance number
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_instance_num(
    const e_impdrv_instance_t       instance_num
);

/*******************************************************************************************************************//**
 * @brief           Check the initialization data for IMP Driver.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_initdata          Initialization data
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_init_data(
    const st_impdrv_initdata_t      *const p_initdata
);

/*******************************************************************************************************************//**
 * @brief           Check the internal bus check resource data for IMP Driver.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_core_info     Pointer to the Specified core information
 * @param[in]       p_chk_resource  Pointer to the OSAL resource for bus interface check.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_bus_if_check(
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_chk_resource_t  *const p_chk_resource
);

/*******************************************************************************************************************//**
 * @brief           Check the test parameter of configuration register  for IMP Driver.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_chk_param     Pointer to Array of check parameters.
 * @param[in]       param_num       Number of check parameters.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_conf_reg_check(
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
);

/*******************************************************************************************************************//**
 * @brief           Convert the Input handle to Control structure pointer.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       handle          IMP Driver control handle.
 * @param[in, out]  p_impdrv_ctl    General control handle.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_cnv_ctrl_handle(
    impdrv_ctrl_handle_t        handle,
    st_impdrv_ctl_t             **const p_impdrv_ctl
);

/*******************************************************************************************************************//**
 * @brief           Convert the Error code of private function error.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       ercd                Error code
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_***                Converted error code
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_cnv_error_code(
    const e_impdrv_errorcode_t      ercd
);

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function R_IMPDRV_Init()
* CD_PD_02_01_0001
* [Covers: UD_PD_UD02_01_0001]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Init(
    st_impdrv_initdata_t    *const p_initdata,
    impdrv_ctrl_handle_t    *const p_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;
    impdrv_ctrl_handle_t    osal_cb_args;

    /** Null check of input parameters */
    if ((NULL == p_initdata) || (NULL == p_handle))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Check component initialization data */
        ercd = impdrv_api_chk_init_data(p_initdata);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Set handle data in output parameter */
            *p_handle = (impdrv_ctrl_handle_t)p_initdata->p_work_addr;
            osal_cb_args = *p_handle;

            /** Conversion handle data and Broken work memory detection. */
            ercd = impdrv_api_cnv_ctrl_handle(*p_handle, &p_impdrv_ctl);
        }
    }

    /** Initialize of subcomponents */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the initialization process */
        ercd = impdrv_genctl_init(
            p_impdrv_ctl,
            p_initdata,
            osal_cb_args
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Recovery process when initialization fails */
    if ((IMPDRV_EC_OK != ercd) && (NULL != p_handle))
    {
        *p_handle = NULL;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_Init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_SetMemProtect()
* CD_PD_02_01_0030
* [Covers: UD_PD_UD02_01_0030]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_SetMemProtect(
    impdrv_ctrl_handle_t            handle,
    const e_impdrv_protect_mode_t   protect_mode
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if (IMPDRV_HANDLE_INVALID == handle)
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

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Sets the Memory protection mode. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the Memory protection process */
        ercd = impdrv_genctl_reg_set_mem_protect(
            p_impdrv_ctl,
            protect_mode
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_SetMemProtect()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrInit()
* CD_PD_02_01_0003
* [Covers: UD_PD_UD02_01_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrInit(
    impdrv_ctrl_handle_t        handle,
    const st_impdrv_core_info_t *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Initialize the core attributes of subcomponent */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the attribute initialization process */
        ercd = impdrv_genctl_attr_init(
            p_impdrv_ctl,
            p_core_info
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrInit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_Quit()
* CD_PD_02_01_0002
* [Covers: UD_PD_UD02_01_0002]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Quit(
    impdrv_ctrl_handle_t  handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if (IMPDRV_HANDLE_INVALID == handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Quit of subcomponents */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the quit process */
        ercd = impdrv_genctl_quit(
            p_impdrv_ctl
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_Quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetCoreMemInit()
* CD_PD_02_01_0004
* [Covers: UD_PD_UD02_01_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetCoreMemInit(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_param_t          enable
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PARAM_OFF != enable) && (IMPDRV_PARAM_ON != enable))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Core memory initialization attribute settings */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the set attribute process */
        ercd = impdrv_genctl_set_core_mem_init(
            p_impdrv_ctl,
            p_core_info,
            enable
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetCoreMemInit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_Start()
* CD_PD_02_01_1005
* [Covers: UD_PD_UD02_01_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Start(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Start the specified core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the start process */
        ercd = impdrv_genctl_start(
            p_impdrv_ctl,
            p_core_info
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_Start()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_Stop()
* CD_PD_02_01_1006
* [Covers: UD_PD_UD02_01_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Stop(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Stop the specified core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the stop process */
        ercd = impdrv_genctl_stop(
            p_impdrv_ctl,
            p_core_info
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_Stop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetCoreMap()
* CD_PD_02_01_0007
* [Covers: UD_PD_UD02_01_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetCoreMap(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_core_info_t     core_map[IMPDRV_COREMAP_MAXID]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;
    uint32_t                i;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info) || (NULL == core_map))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Check the Input value of core map information */
            i = 0U;
            while (IMPDRV_COREMAP_MAXID > i)
            {
                if ((IMPDRV_CORE_TYPE_INVALID != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_IMP != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_IMP_SLIM != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_OCV != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_DMAC != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_DMAC_SLIM != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_PSCEXE != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_PSCOUT != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_CNN != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_DTA != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_DSP != core_map[i].core_type)
                    && (IMPDRV_CORE_TYPE_IMR != core_map[i].core_type))
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                }
                else if (IMPDRV_CORE_NUM_MAX <= core_map[i].core_num)
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                }
                else
                {
                    /** Nothing to do */
                }
                if (IMPDRV_EC_OK != ercd)
                {
                    break;
                }
                i++;
            }
        }
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set core map attributes */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the set attribute process */
        ercd = impdrv_genctl_set_core_map(
            p_impdrv_ctl,
            p_core_info,
            core_map
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetCoreMap()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetCl()
* CD_PD_02_01_0008
* [Covers: UD_PD_UD02_01_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetCl(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const uintptr_t                 claddr_phys
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U != (claddr_phys & IMPDRV_CHK_CL_MEM_ALIGN))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set Command list physical address attributes */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the set attribute process */
        ercd = impdrv_genctl_set_cl(
            p_impdrv_ctl,
            p_core_info,
            (const uint32_t)claddr_phys
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetCl()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetDsp()
* CD_PD_02_01_0037
* [Covers: UD_PD_UD02_01_0037]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetDsp (
    const impdrv_ctrl_handle_t      handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_dsp_data_t      *const p_dsp_app,
    const st_impdrv_dsp_data_t      *const p_dsp_fw,
    const st_impdrv_dsp_data_t      *const p_dsp_data,
    const st_impdrv_dsp_data_t      *const p_dsp_dtcm
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((NULL == handle) || (NULL == p_core_info)
        || (NULL == p_dsp_app) || (NULL == p_dsp_fw)
        || (NULL == p_dsp_data) || (NULL == p_dsp_dtcm))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    /** Check DSP Application address Alignment */
    else if ((0U != (p_dsp_app->addr_phys & IMPDRV_CHK_MEM_ALIGN))
        || (0U != (p_dsp_fw->addr_phys & IMPDRV_CHK_MEM_ALIGN))
        || (0U != (p_dsp_data->addr_phys & IMPDRV_CHK_MEM_ALIGN))
        || (0U != (p_dsp_dtcm->addr_phys & IMPDRV_CHK_MEM_ALIGN)))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    /** Check DSP Application size */
    else if ((IMPDRV_VDSP_INT_PROG_MEM_SIZE < p_dsp_fw->size) || 
                    (IMPDRV_VDSP_DTCM_MEM_SIZE < p_dsp_dtcm->size))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set Command list physical address attributes */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_set_dsp(
            p_impdrv_ctl,
            p_core_info,
            p_dsp_app,
            p_dsp_fw,
            p_dsp_data,
            p_dsp_dtcm
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetDsp()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetIrqMask()
* CD_PD_02_01_0009
* [Covers: UD_PD_UD02_01_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetIrqMask(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const bool                      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;
    uint32_t                i;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info) || (NULL == irq_mask))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
        if (IMPDRV_EC_OK == ercd)
        {
            i = 0U;
            while (IMPDRV_IRQMASK_MAX > i)
            {
                if ((false != irq_mask[i]) && (true != irq_mask[i]))
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                    break;
                }
                i++;
            }
        }
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set Command list physical address attributes */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the set attribute process */
        ercd = impdrv_genctl_set_irq_mask(
            p_impdrv_ctl,
            p_core_info,
            irq_mask
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetIrqMask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_SetIrqGroup()
* CD_PD_02_01_0011
* [Covers: UD_PD_UD02_01_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_SetIrqGroup(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_irq_group_t     *const p_irq_param
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;
    uint32_t                i;
    uint32_t                j;
    st_impdrv_core_info_t   checker;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_irq_param))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_IRQ_GROUP_NONE != p_irq_param->irq_group)
        && (IMPDRV_IRQ_GROUP_0 != p_irq_param->irq_group)
        && (IMPDRV_IRQ_GROUP_1 != p_irq_param->irq_group)
        && (IMPDRV_IRQ_GROUP_2 != p_irq_param->irq_group))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((0U == p_irq_param->group_core_num) || (IMPDRV_IRQGROUP_MAXID < p_irq_param->group_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        i = 0U;
        while ((p_irq_param->group_core_num) > i)
        {
            ercd = impdrv_api_chk_core_info(p_irq_param->group_core_info[i]);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            i++;
        }
    }

    /** Check the duplicate settings for Core information. */
    if (IMPDRV_EC_OK == ercd)
    {
        i = 0U;
        while ((p_irq_param->group_core_num - 1U) > i)
        {
            j = i + 1U;
            checker = p_irq_param->group_core_info[i];
            while (p_irq_param->group_core_num > j)
            {
                if ((checker.core_type ==  p_irq_param->group_core_info[j].core_type)
                    && (checker.core_num ==  p_irq_param->group_core_info[j].core_num))
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                    break;
                }
                j++;
            }
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            i++;
        }
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set IRQ group attributes */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the IRQ group setting */
        ercd = impdrv_genctl_set_irq_group(
            p_impdrv_ctl,
            p_irq_param
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_SetIrqGroup()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetClBrkAddr()
* CD_PD_02_01_0036
* [Covers: UD_PD_UD02_01_0036]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetClBrkAddr(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const uintptr_t                 cl_brk_addr
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_CLBRK_ADDR_INVALID != cl_brk_addr)
                && (0U != (cl_brk_addr & IMPDRV_CHK_CL_MEM_ALIGN)))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set CL break address attribute */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the CL break address setting */
        ercd = impdrv_genctl_set_cl_brk_addr(
            p_impdrv_ctl,
            p_core_info,
            (const uint32_t)cl_brk_addr
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetClBrkAddr()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_AttrSetGosubCond()
* CD_PD_02_01_0036
* [Covers: UD_PD_UD02_01_0036]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_AttrSetGosubCond(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_gosub_cond_t     condition
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_GOSUB_COND_NOTEXEC != condition) && (IMPDRV_GOSUB_COND_EXEC != condition))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set CL break address attribute */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the conditional GOSUB instruction setting */
        ercd = impdrv_genctl_set_gosub_cond(
            p_impdrv_ctl,
            p_core_info,
            condition
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_AttrSetGosubCond()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_Execute()
* CD_PD_02_01_1012
* [Covers: UD_PD_UD02_01_0012]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_Execute(
    impdrv_ctrl_handle_t        handle,
    const st_impdrv_core_info_t *const p_core_info,
    const p_impdrv_cbfunc_t     callback_func,
    void                        *const p_callback_args
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info) || (NULL == callback_func))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Execute command list on the specified core */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Request genctl to perform the execute process */
        ercd = impdrv_genctl_execute(
            p_impdrv_ctl,
            p_core_info,
            callback_func,
            p_callback_args
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_Execute()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_ResumeExecution()
* CD_PD_02_01_1013
* [Covers: UD_PD_UD02_01_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_ResumeExecution(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Resume command list on the specified core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_resume_exe(
            p_impdrv_ctl,
            p_core_info
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_ResumeExecution()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_SetPMPolicy()
* CD_PD_02_01_0018
* [Covers: UD_PD_UD02_01_0018]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_SetPMPolicy (
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_pm_policy_t      policy
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PM_POLICY_CG != policy) && (IMPDRV_PM_POLICY_PG != policy) && (IMPDRV_PM_POLICY_HP != policy))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Set the Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_pm_set_policy(
            p_impdrv_ctl,
            p_core_info,
            policy
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_SetPMPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_GetPMPolicy()
* CD_PD_02_01_0019
* [Covers: UD_PD_UD02_01_0019]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_GetPMPolicy (
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    e_impdrv_pm_policy_t            *const p_policy
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;
    e_impdrv_pm_policy_t    policy;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info) || (NULL == p_policy))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Get the Power management policy */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_pm_get_policy(
            p_impdrv_ctl,
            p_core_info,
            &policy
        );

        /** Set the policy data of the output parameter */
        if (IMPDRV_EC_OK == ercd)
        {
            *p_policy = policy;
        }
        else
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_GetPMPolicy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_GetPMState()
* CD_PD_02_01_0038
* [Covers: UD_PD_UD02_01_0038]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_GetPMState(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    e_impdrv_pm_state_t             *const p_pmstate
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;
    e_impdrv_pm_state_t     state;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info) || (NULL == p_pmstate))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Get the Power management state */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_pm_get_state(
            p_impdrv_ctl,
            p_core_info,
            &state
        );

        /** Set the state data of the output parameter */
        if (IMPDRV_EC_OK == ercd)
        {
            *p_pmstate = state;
        }
        else
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_GetPMState()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_RegGetHwInfo()
* CD_PD_02_01_0026
* [Covers: UD_PD_UD02_01_0026]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegGetHwInfo(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_device_id) || (NULL == p_reg_info))
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

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Get information the Hardware register area specification. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Entrust processing to general control component. */
        ercd = impdrv_genctl_reg_get_hw_info(
            p_impdrv_ctl,
            p_device_id,
            p_reg_info
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_RegGetHwInfo()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_RegRead32()
* CD_PD_02_01_0027
* [Covers: UD_PD_UD02_01_0027]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegRead32(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_device_id) || (NULL == p_data))
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

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /**  Read from hardware register. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Entrust processing to general control component. */
        ercd = impdrv_genctl_reg_read32(
            p_impdrv_ctl,
            p_device_id,
            offset,
            p_data
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_RegRead32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_RegWrite32()
* CD_PD_02_01_0028
* [Covers: UD_PD_UD02_01_0028]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegWrite32(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_device_id))
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

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Writes data to hardware register. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Entrust processing to general control component. */
        ercd = impdrv_genctl_reg_write32(
            p_impdrv_ctl,
            p_device_id,
            offset,
            data
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_RegWrite32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_RegRequired()
* CD_PD_02_01_0029
* [Covers: UD_PD_UD02_01_0029]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_RegRequired(
    impdrv_ctrl_handle_t            handle,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_device_id))
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

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Open or Close the device associated with the target OSAL ID. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Entrust processing to general control component. */
        ercd = impdrv_genctl_reg_required(
            p_impdrv_ctl,
            p_device_id,
            new_state
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_RegRequired()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_BusIfCheck()
* CD_PD_02_01_0031
* [Covers: UD_PD_UD02_01_0031]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_BusIfCheck(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_core_info) || (NULL == p_chk_resource))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_api_chk_bus_if_check(
                p_core_info,
                p_chk_resource
            );
        }
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Perform DMAC transfer on IMP-Xn core and check the internal bus. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Entrust processing to general control component. */
        ercd = impdrv_genctl_bus_if_check(
            p_impdrv_ctl,
            p_core_info,
            p_chk_resource
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_BusIfCheck()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_ConfRegCheck()
* CD_PD_02_01_0032
* [Covers: UD_PD_UD02_01_0032]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_ConfRegCheck(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    st_impdrv_ctl_t         *p_impdrv_ctl;

    /** Null check of input parameters */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_chk_param) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Input parameter confirmation process */
        ercd = impdrv_api_chk_core_info(*p_core_info);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_api_chk_conf_reg_check(p_chk_param, param_num);
        }
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_cnv_ctrl_handle(handle, &p_impdrv_ctl);
    }

    /** Check the configuration register is expected value. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Entrust processing to general control component. */
        ercd = impdrv_genctl_conf_reg_check(
            p_impdrv_ctl,
            p_chk_param,
            param_num,
            p_core_info
        );
        if (IMPDRV_EC_OK != ercd)
        {
            /** Convert error code from private error */
            ercd = impdrv_api_cnv_error_code(ercd);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_ConfRegCheck()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_ModuleStopCheck()
* CD_PD_02_01_0033
* [Covers: UD_PD_UD02_01_0033]
***********************************************************************************************************************/
e_impdrv_errorcode_t R_IMPDRV_ModuleStopCheck(
    impdrv_ctrl_handle_t            handle,
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the configuration register is expected value. */
    ercd = R_IMPDRV_ConfRegCheck(
        handle,
        p_chk_param,
        param_num,
        p_core_info
    );

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_ModuleStopCheck()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function R_IMPDRV_GetVersion()
* CD_PD_02_01_0020
* [Covers: UD_PD_UD02_01_0020]
***********************************************************************************************************************/
const st_impdrv_version_t* R_IMPDRV_GetVersion (
    void
)
{
    /** Initialize static version variables */
    static const st_impdrv_version_t s_version =
    {
        IMPDRV_VERSION_MAJOR,
        IMPDRV_VERSION_MINOR,
        IMPDRV_VERSION_PATCH
    };

    /** Returns version information */
    return &s_version;
}
/***********************************************************************************************************************
* End of function R_IMPDRV_GetVersion()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/

/***********************************************************************************************************************
* Start of function impdrv_api_chk_core_info()
* CD_PD_02_01_0022
* [Covers: UD_PD_UD02_01_0022]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_core_info(
    const st_impdrv_core_info_t     core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the Input value of core information */
    if ((IMPDRV_CORE_TYPE_PSCOUT == core_info.core_type)
        || (IMPDRV_CORE_TYPE_DTA == core_info.core_type)
        || (IMPDRV_CORE_TYPE_LDMAC == core_info.core_type)
        || (IMPDRV_CORE_TYPE_IMR == core_info.core_type))
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }
    else if ((IMPDRV_CORE_TYPE_IMP != core_info.core_type)
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
* End of function impdrv_api_chk_core_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_api_chk_instance_num()
* CD_PD_02_01_0023
* [Covers: UD_PD_UD02_01_0023]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_instance_num(
    const e_impdrv_instance_t       instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the Instance number value */
    if ((IMPDRV_INSTANCE_0 != instance_num)
        && (IMPDRV_INSTANCE_1 != instance_num)
        && (IMPDRV_INSTANCE_2 != instance_num)
        && (IMPDRV_INSTANCE_3 != instance_num)
        && (IMPDRV_INSTANCE_4 != instance_num)
        && (IMPDRV_INSTANCE_5 != instance_num)
        && (IMPDRV_INSTANCE_6 != instance_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** Set error code in return value */
    return ercd;
}

/***********************************************************************************************************************
* End of function impdrv_api_chk_instance_num()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_api_chk_init_data()
* CD_PD_02_01_0021
* [Covers: UD_PD_UD02_01_0021]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_init_data(
    const st_impdrv_initdata_t      *const p_initdata
)
{
    uint32_t                i;
    uint32_t                j;
    st_impdrv_core_info_t   checker;
    uint32_t                l_worksize;

    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the p_initdata pointer value */
    if (NULL == p_initdata)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    /** Check the work area address to value and alignment */
    else if ((NULL == p_initdata->p_work_addr)
        || (0U != (((uintptr_t)(p_initdata->p_work_addr)) & (IMPDRV_CHK_WORK_ADDR_ALIGN))))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    /** Check the Fatal callback function pointer */
    else if (NULL == p_initdata->callback_func_fatal)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    /** Check the Number of core list value */
    else if ((0U == p_initdata->use_core_num)
        || (IMPDRV_INNER_FIXED_VALUE < p_initdata->use_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Check the work area size valid */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_genctl_get_worksize(0u, &l_worksize);
        if (l_worksize > p_initdata->work_size)
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
    }

    /** Check the OSAL parameter value */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check the time period value to Mutex lock and Message queue */
        if ((0 > p_initdata->osal_resource.mutex_time_period)
            || (0 > p_initdata->osal_resource.mq_time_period))
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
        /** Check the Interrupt priority value */
        else if ((OSAL_INTERRUPT_PRIORITY_TYPE0 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE1 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE2 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE3 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE4 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE5 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE6 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE7 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE8 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE9 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE10 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE11 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE12 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE13 != p_initdata->osal_resource.irq_priority)
                    && (OSAL_INTERRUPT_PRIORITY_TYPE14 != p_initdata->osal_resource.irq_priority))
        {
            ercd = IMPDRV_EC_NG_PARAM;
        }
        else
        {
            /** Nothing to do. */
        }
    }

    /** Check the Instance number value */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_api_chk_instance_num(p_initdata->instance_num);
    }

    /** Check the Input value of core type */
    if (IMPDRV_EC_OK == ercd)
    {
        i = 0U;
        while (i < p_initdata->use_core_num)
        {
            checker = p_initdata->core_info[i];
            ercd = impdrv_api_chk_core_info(checker);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            i++;
        }
    }

    /** Check the duplicate settings for Core information. */
    if (IMPDRV_EC_OK == ercd)
    {
        i = 0U;
        while (i < (p_initdata->use_core_num - 1U))
        {
            j = i + 1U;
            checker = p_initdata->core_info[i];
            while (j < p_initdata->use_core_num)
            {
                if ((checker.core_type == p_initdata->core_info[j].core_type)
                    && (checker.core_num == p_initdata->core_info[j].core_num))
                {
                    ercd = IMPDRV_EC_NG_PARAM;
                    break;
                }
                j++;
            }
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            i++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_api_chk_init_data()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_api_chk_bus_if_check()
* CD_PD_02_01_0034
* [Covers: UD_PD_UD02_01_0034]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_bus_if_check(
    const st_impdrv_core_info_t     *const p_core_info,
    const st_impdrv_chk_resource_t  *const p_chk_resource
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Check the p_initdata pointer value */
    if ((NULL == p_core_info) || (NULL == p_chk_resource)
        || (OSAL_MEMORY_BUFFER_HANDLE_INVALID == p_chk_resource->memory_handle))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((OSAL_AXI_BUS_ID_INVALID == p_chk_resource->axi_id)
        || ((osal_mq_id_t)0UL == p_chk_resource->mq_id)
        || (0 > p_chk_resource->mq_time_period))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((IMPDRV_CORE_TYPE_IMP != p_core_info->core_type)
        && (IMPDRV_CORE_TYPE_IMP_SLIM != p_core_info->core_type)
        && (IMPDRV_CORE_TYPE_DMAC != p_core_info->core_type))
    {
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_api_chk_bus_if_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_api_chk_conf_reg_check()
* CD_PD_02_01_0035
* [Covers: UD_PD_UD02_01_0035]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_chk_conf_reg_check(
    const st_impdrv_chk_param_t     *const p_chk_param,
    const uint32_t                  param_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                i;

    /** Check the p_initdata pointer value */
    if (NULL == p_chk_param)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U == param_num)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Check the contents of test parameters. */
        i = 0U;
        while (param_num > i)
        {
            if (0U != (p_chk_param[i].offset & IMPDRV_CHK_REG_ALIGN))
            {
                ercd = IMPDRV_EC_NG_PARAM;
                break;
            }
            i++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_api_chk_conf_reg_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_api_cnv_ctrl_handle()
* CD_PD_02_01_0025
* [Covers: UD_PD_UD02_01_0025]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_cnv_ctrl_handle(
    impdrv_ctrl_handle_t        handle,
    st_impdrv_ctl_t             **const p_impdrv_ctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if ((IMPDRV_HANDLE_INVALID == handle) || (NULL == p_impdrv_ctl))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Conversion handle data and Broken work memory detection. */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Convert to Control structure pointer from Abstract handle */
        *p_impdrv_ctl = (st_impdrv_ctl_t*)handle;

        /** Under development.... */
        /** Call the Broken work memory detect function here */
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_api_cnv_ctrl_handle()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_api_cnv_error_code()
* CD_PD_02_01_0024
* [Covers: UD_PD_UD02_01_0024]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_api_cnv_error_code(
    const e_impdrv_errorcode_t      ercd
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    retcd = ercd;

    /** Convert error code */
    if ((IMPDRV_EC_NG_ARGNULL == retcd) || (IMPDRV_EC_NG_PARAM == retcd))
    {
        retcd = IMPDRV_EC_NG_SYSTEMERROR;
    }

    /** Set error code in return value */
    return retcd;
}
/***********************************************************************************************************************
* End of function impdrv_api_cnv_error_code()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
