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
* File Name    : r_impdrv_cmnctl_reg.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_cmnctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_cmnctl_reg.c
 */

/*======================================================================================================================
Includes <System Includes> , "Project Includes"
======================================================================================================================*/
#include "rcar-xos/imp/r_impdrv.h"
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
 * @def IMPDRV_INSTANCETABLE_NUM
 * Maximum number of Instance number table.
***********************************************************************************************************************/
#define IMPDRV_INSTANCETABLE_NUM        (4U)        /**< Maximum number of Instance number table.   */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_UNUSED
 * Unused resources state.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_UNUSED             (0U)        /** Unused resources state                      */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_NAME_MAX
 * Maximum length of hardware resource name.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_NAME_MAX           (32U)       /** Maximum length of hardware resource name    */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_INDEX_IMP_TOP
 * Index number of IMP top resource.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_INDEX_IMP_TOP      (0U)        /** Index number of IMP top resource            */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_SPM_PSET_TBL_MAX
 * Maximum index number of the preset table.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_SPM_PSET_TBL_MAX   (3U)        /** Maximum index number of the preset table    */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_INIT_SPMI0_IDX
 * SPMI0 index number of the init table.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_INIT_SPMI0_IDX      (0U)        /** SPMI0 index number of the init table.      */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_INIT_SPMC0_IDX
 * SPMC0 index number of the init table.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_INIT_SPMC0_IDX      (1U)        /** SPMC0 index number of the init table.      */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_SPM_INIT_TBL_MAX
 * Maximum index number of the init table.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_SPM_INIT_TBL_MAX   (2U)        /** Maximum index number of the init table      */

/** @} */

/*======================================================================================================================
Private data types
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Types Private type definitions
 *
 * @{
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @struct st_impdrv_hwrsc_chk_t
 * Common control global check area.
***********************************************************************************************************************/
typedef struct
{
    uint32_t                    checksum;                               /**< Checksum value of Global variable.     */
} st_impdrv_hwrsc_chk_t;

/** @} */

/*======================================================================================================================
Private file static variables
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_Private_Static_Variables Private file static variables
 *
 * @{
***********************************************************************************************************************/
IMPDRV_STATIC
const st_impdrv_hwrsc_def_t     s_hwrsc_def_table[IMPDRV_HWRSC_TABLE_NUM] =
{
    /** Resource Name(Key), Physical adrs,  Area size       Read only   internal    */
    { "imp_top_00"      ,   0xFF900000U ,   0x00006000U ,   true    ,   false       },
    { "imp_spmi_00"     ,   0xED300000U ,   0x00100000U ,   false   ,   false       },
    { "imp_spmc_00"     ,   0xED400000U ,   0x00200000U ,   false   ,   false       },
    { "imp_reg_spmi_00" ,   0xFFA8C000U ,   0x00004000U ,   true    ,   false       },
    { "imp_reg_spmc_00" ,   0xFFAB0000U ,   0x00004000U ,   true    ,   false       },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        },
    { NULL              ,   0x00000000U ,   0x00000000U ,   true    ,   true        }
};

IMPDRV_STATIC
const uint32_t s_impdrv_used_instance[IMPDRV_INSTANCETABLE_NUM] =
{
    0xFFFFFFFFU,    /**< Dummy for Index adjustment */
    0x00000001U,    /**< IMPDRV_INSTANCE_0 */
    0x00000002U,    /**< IMPDRV_INSTANCE_1 */
    0x00000004U     /**< IMPDRV_INSTANCE_2 */
};

IMPDRV_STATIC
st_impdrv_hwrsc_mng_t     g_hwrsc_mng_table =
{
    IMPDRV_HWRSC_UNUSED,        /**< Resource used state                    */
    IMPDRV_HWRSC_UNUSED,        /**< Memory initialized state               */
    {
        IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, 
        IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, 
        IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, 
        IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED, IMPDRV_HWRSC_UNUSED
    },                          /**< In-use status management               */
    {
        { NULL }, { NULL }, { NULL }, { NULL }, 
        { NULL }, { NULL }, { NULL }, { NULL }, 
        { NULL }, { NULL }, { NULL }, { NULL }, 
        { NULL }, { NULL }, { NULL }, { NULL }
    }                           /**< IMPDRV device handle                   */
};

IMPDRV_STATIC
st_impdrv_hwrsc_chk_t   g_hwrsc_chk_table = 
{
    0U                          /**< Checksum data for g_hwrsc_mng_table    */
};

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
 * @brief           Open the related hardware resource and powers it ON.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_hwrsc_open(
    const e_impdrv_instance_t   instance_num
);

/*******************************************************************************************************************//**
 * @brief           Close the related hardware resource and powers it OFF.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_hwrsc_close(
    const e_impdrv_instance_t   instance_num
);

/*******************************************************************************************************************//**
 * @brief           Opens the specified resource and powers it ON.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @param[in]       index           Index number to Management table.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_pow_on_hwrsc(
    const e_impdrv_instance_t   instance_num,
    const uint32_t              index
);

/*******************************************************************************************************************//**
 * @brief           Close the specified resource and powers it OFF.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @param[in]       index           Index number to Management table.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_pow_off_hwrsc(
    const e_impdrv_instance_t   instance_num,
    const uint32_t              index
);

/*******************************************************************************************************************//**
 * @brief           Set the PRESET register for Scratchpad memory.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_preset(
    const e_impdrv_instance_t   instance_num
);

/*******************************************************************************************************************//**
 * @brief           Set the Enable register access to Scratchpad memory.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       index           Index number to Management table.
 * @param[in]       enable          Enable flag of SPM register access. (true : enable, false : disable)
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_ena_access(
    const uint32_t              index,
    const bool                  enable
);

/*******************************************************************************************************************//**
 * @brief           Check the completion of Scratchpad memory initialization.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @param[in]       num_devices     Number of Target OSAL device ID.
 * @param[in]       p_device_id     Pointer to the Target OSAL device ID.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_chk_init(
    const e_impdrv_instance_t   instance_num,
    const uint32_t              num_devices,
    const char                  *const*const p_device_id
);

/*******************************************************************************************************************//**
 * @brief           Wait for the Scratchpad memory initialization to complete.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_wait_init(
    const e_impdrv_instance_t   instance_num
);

/*******************************************************************************************************************//**
 * @brief           Initialize the Scratchpad memory.
 * @syncasync       Synchronous
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num    Specified instance number.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Successful completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
 * @retval          IMPDRV_EC_NG_PMSYSTEMERROR      PMSystem error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_initialize(
    const e_impdrv_instance_t   instance_num
);

/*******************************************************************************************************************//**
 * @brief           Find of matching hardware information.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_id             Pointer to the Target OSAL device ID.
 * @param[out]      p_index                 Pointer to Resource index value.
 * @param[out]      p_resource              Pointer to Resource definition data.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_find_resource(
    const char              *const p_device_id,
    uint32_t                *const p_index,
    st_impdrv_hwrsc_def_t   *const p_resource
);

/*******************************************************************************************************************//**
 * @brief           Check of matching hardware resource name.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_rsc_name              Pointer to Resource name string.
 * @param[in]       p_device_id             Pointer to the Target OSAL device ID.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_NOTSUPPORT         Not supported parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
bool impdrv_cmnctl_reg_chk_rsc_name(
    const char              *const p_rsc_name,
    const char              *const p_device_id
);
/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_init()
* CD_PD_02_03_0025
* [Covers: UD_PD_UD02_03_0025]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_init(
    st_impdrv_commonctl_t           *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_instance_t     instance_num;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        instance_num = p_commonctl->instance_num;
        ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** Open the related hardware resource and powers it ON */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_hwrsc_open(instance_num);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Set the Extended information for debug interface. */
        p_commonctl->p_hwrsc_def_table = (void*)&s_hwrsc_def_table;
        p_commonctl->p_hwrsc_mng_table = (void*)&g_hwrsc_mng_table;

        /** Initial value setting of memory protection mode. */
        p_commonctl->protect_mode = IMPDRV_PROTECT_INVALID;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_quit()
* CD_PD_02_03_0026
* [Covers: UD_PD_UD02_03_0026]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_quit(
    st_impdrv_commonctl_t           *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_instance_t     instance_num;
    uint32_t                index;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        instance_num = p_commonctl->instance_num;
        ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** Close the related hardware resource and powers it OFF */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_hwrsc_close(instance_num);
    }

    if (IMPDRV_EC_OK == ercd)
    {
        /** Clear the In-use status management. */
        for (index = 0U; IMPDRV_HWRSC_TABLE_NUM > index; index++)
        {
            g_hwrsc_mng_table.in_use[index] &= ~(s_impdrv_used_instance[instance_num]);
        }

        /** Clear the Memory initialized state. */
        g_hwrsc_mng_table.memory_initialized &= ~(s_impdrv_used_instance[instance_num]);

        /** Clear the Extended information for debug interface. */
        p_commonctl->p_hwrsc_def_table = NULL;
        p_commonctl->p_hwrsc_mng_table = NULL;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_set_mem_protect()
* CD_PD_02_03_0039
* [Covers: UD_PD_UD02_03_0039]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_set_mem_protect(
    st_impdrv_commonctl_t           *const p_commonctl,
    const e_impdrv_protect_mode_t   protect_mode
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PROTECT_EDC != protect_mode) && (IMPDRV_PROTECT_ECC != protect_mode))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Not supported on this SoC */
        p_commonctl->protect_mode = IMPDRV_PROTECT_INVALID;
        ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_set_mem_protect()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_chk_mem_protect()
* CD_PD_02_03_0043
* [Covers: UD_PD_UD02_03_0043]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_chk_mem_protect(
    const st_impdrv_commonctl_t     *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_PROTECT_INVALID != p_commonctl->protect_mode)
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
* End of function impdrv_cmnctl_reg_chk_mem_protect()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_mem_init()
* CD_PD_02_03_0027
* [Covers: UD_PD_UD02_03_0027]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_mem_init(
    st_impdrv_commonctl_t       *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_instance_t     instance_num;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        instance_num = p_commonctl->instance_num;
        ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** When the needs to memory initialized */
    if ((IMPDRV_EC_OK == ercd)
        && (IMPDRV_HWRSC_UNUSED == g_hwrsc_mng_table.memory_initialized))
    {
        /** Initial setting the using PRESET registers */
            ercd = impdrv_cmnctl_reg_spm_preset(
                instance_num
            );

        /** Initialization of Scratchpad memory */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_spm_initialize(
                instance_num
            );
        }
    }

    /** Set the Memory initialized state. */
    if (IMPDRV_EC_OK == ercd)
    {
        g_hwrsc_mng_table.memory_initialized |= s_impdrv_used_instance[instance_num];
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_mem_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_get_hw_info()
* CD_PD_02_03_0028
* [Covers: UD_PD_UD02_03_0028]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_get_hw_info(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    st_impdrv_reg_info_t            *const p_reg_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index;
    st_impdrv_hwrsc_def_t   resource;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_device_id) || (NULL == p_reg_info))
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

    /** Return the matching hardware information */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_find_resource(p_device_id, &index, &resource);
        if (IMPDRV_EC_OK == ercd)
        {
            p_reg_info->addr_phys = resource.addr_phys;
            p_reg_info->size      = resource.size;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_get_hw_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_read32()
* CD_PD_02_03_0029
* [Covers: UD_PD_UD02_03_0029]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_read32(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    uint32_t                        *const p_data
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index = 0U;
    st_impdrv_hwrsc_def_t   resource;
    uint32_t                current_state;
    e_impdrv_instance_t     instance_num;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_device_id) || (NULL == p_data))
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
        instance_num = p_commonctl->instance_num;
        ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** Get the matching resource information */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_find_resource(p_device_id, &index, &resource);
        if ((IMPDRV_EC_OK == ercd)
            && (true == s_hwrsc_def_table[index].internal))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        if ((IMPDRV_EC_OK == ercd)
            && (resource.size <= offset))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** When the Read data using to OS dependent components */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check for API sequence errors */
        current_state = (g_hwrsc_mng_table.in_use[index] & s_impdrv_used_instance[instance_num]);
        if (IMPDRV_HWRSC_UNUSED == current_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            if (NULL == g_hwrsc_mng_table.handle[index].handle)
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
            else
            {
                /** Read data using to OS dependent components */
                ercd = impdrv_osdep_read_reg(
                    &g_hwrsc_mng_table.handle[index],
                    (uint32_t)offset,
                    p_data
                );
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_read32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_write32()
* CD_PD_02_03_0030
* [Covers: UD_PD_UD02_03_0030]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_write32(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const uintptr_t                 offset,
    const uint32_t                  data
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index = 0U;
    st_impdrv_hwrsc_def_t   resource;
    uint32_t                current_state;
    e_impdrv_instance_t     instance_num;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_device_id))
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
        instance_num = p_commonctl->instance_num;
        ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** Get the matching resource information */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_find_resource(p_device_id, &index, &resource);
        if ((IMPDRV_EC_OK == ercd)
            && (true == s_hwrsc_def_table[index].internal))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        if ((IMPDRV_EC_OK == ercd)
            && (resource.size <= offset))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        if ((IMPDRV_EC_OK == ercd)
            && (true == s_hwrsc_def_table[index].read_only))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** When the Write data using to OS dependent components */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Check for API sequence errors */
        current_state = (g_hwrsc_mng_table.in_use[index] & s_impdrv_used_instance[instance_num]);
        if (IMPDRV_HWRSC_UNUSED == current_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            if (NULL == g_hwrsc_mng_table.handle[index].handle)
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
            else
            {
                /** Write data using to OS dependent components */
                ercd = impdrv_osdep_write_reg(
                    &g_hwrsc_mng_table.handle[index],
                    (uint32_t)offset,
                    data,
                    false,      /* read_back */
                    data
                );
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_write32()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_required()
* CD_PD_02_03_0031
* [Covers: UD_PD_UD02_03_0031]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_required(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const char                      *const p_device_id,
    const e_impdrv_reg_req_state_t  new_state
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index = 0U;
    st_impdrv_hwrsc_def_t   resource;
    uint32_t                current_state;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_device_id))
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
        ercd = impdrv_cmnctl_chk_instance_num(p_commonctl->instance_num);
    }

    /** Get the matching resource information */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_find_resource(p_device_id, &index, &resource);
        if ((IMPDRV_EC_OK == ercd) 
            && (true == s_hwrsc_def_table[index].internal))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Check the management status of resource handle */
    if (NULL == g_hwrsc_mng_table.handle[index].handle)
    {
        ercd = IMPDRV_EC_NG_SYSTEMERROR;
    }

    /** When requesting a change to state required. */
    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_REG_REQ_STATE_REQUIRED == new_state))
    {
        /** Confirmation of duplicate requests */
        current_state = (g_hwrsc_mng_table.in_use[index] & s_impdrv_used_instance[p_commonctl->instance_num]);
        if (IMPDRV_HWRSC_UNUSED != current_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            /** Set of in-use state */
            g_hwrsc_mng_table.in_use[index] |= s_impdrv_used_instance[p_commonctl->instance_num];
        }
    }

    /** When requesting a change to state released. */
    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_REG_REQ_STATE_RELEASED == new_state))
    {
        /** Confirmation of duplicate release */
        current_state = (g_hwrsc_mng_table.in_use[index] & s_impdrv_used_instance[p_commonctl->instance_num]);
        if (IMPDRV_HWRSC_UNUSED == current_state)
        {
            ercd = IMPDRV_EC_NG_SEQSTATE;
        }
        else
        {
            /** Clear of in-use state */
            g_hwrsc_mng_table.in_use[index] &= ~(s_impdrv_used_instance[p_commonctl->instance_num]);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_required()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_get_handle()
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_get_handle(
    const char                      *const p_device_id,
    st_impdrv_device_handle_t       *p_device_handle
)
{

    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index = 0U;
    st_impdrv_hwrsc_def_t   resource;

    /** Input parameter confirmation process */
    if ( (NULL == p_device_id) || (NULL == p_device_handle))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Get the matching resource information */
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_find_resource(p_device_id, &index, &resource);
        }

        if (IMPDRV_EC_OK == ercd)
        {
            if (NULL == g_hwrsc_mng_table.handle[index].handle)
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }

        if (IMPDRV_EC_OK == ercd)
        {
            p_device_handle->handle = g_hwrsc_mng_table.handle[index].handle;
        }
        else
        {
            p_device_handle->handle = NULL;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_get_handle()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_set_checksum()
* CD_PD_02_03_0045
* [Covers: UD_PD_UD02_03_0045]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_set_checksum(
    void
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;

    /** Checksum value setting for Memory broken check */
    ercd = impdrv_cmnctl_calc_checksum(
        (void*)&g_hwrsc_mng_table,
        sizeof(g_hwrsc_mng_table),
        &g_hwrsc_chk_table.checksum
    );

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_set_checksum()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_test_checksum()
* CD_PD_02_03_0046
* [Covers: UD_PD_UD02_03_0046]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_reg_test_checksum(
    void
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    uint32_t                        current_checksum;

    /** Checksum compare test of Memory broken check. */
    ercd = impdrv_cmnctl_calc_checksum(
        (void*)&g_hwrsc_mng_table,
        sizeof(g_hwrsc_mng_table),
        &current_checksum
    );
    if (IMPDRV_EC_OK == ercd)
    {
        if (current_checksum != g_hwrsc_chk_table.checksum)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_test_checksum()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_hwrsc_open()
* CD_PD_02_03_0032
* [Covers: UD_PD_UD02_03_0032]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_hwrsc_open(
    const e_impdrv_instance_t   instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index;

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_instance_num(instance_num);

    /** When the needs to Devices open */
    if (IMPDRV_HWRSC_UNUSED == g_hwrsc_mng_table.resource_used)
    {
        index = 0U;
        while ((IMPDRV_EC_OK == ercd) && (IMPDRV_HWRSC_TABLE_NUM > index))
        {
            if (NULL != s_hwrsc_def_table[index].name)
            {
                /** Open the specified device */
                ercd = impdrv_cmnctl_reg_pow_on_hwrsc(instance_num, index);
            }
            index++;
        }
    }

    /** Set the Resource used state. */
    if (IMPDRV_EC_OK == ercd)
    {
        g_hwrsc_mng_table.resource_used |= s_impdrv_used_instance[instance_num];
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_hwrsc_open()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_hwrsc_close()
* CD_PD_02_03_0033
* [Covers: UD_PD_UD02_03_0033]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_hwrsc_close(
    const e_impdrv_instance_t   instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index;

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_instance_num(instance_num);

    /** When the needs to Devices close */
    g_hwrsc_mng_table.resource_used &= ~(s_impdrv_used_instance[instance_num]);
    if (IMPDRV_HWRSC_UNUSED == g_hwrsc_mng_table.resource_used)
    {
        index = 0U;
        while ((IMPDRV_EC_OK == ercd) && (IMPDRV_HWRSC_TABLE_NUM > index))
        {
            if (NULL != s_hwrsc_def_table[index].name)
            {
                /** Close the specified device */
                ercd = impdrv_cmnctl_reg_pow_off_hwrsc(instance_num, index);
            }
            index++;
        }
    }

    /** Set the Resource used state. */
    if (IMPDRV_EC_OK != ercd)
    {
        g_hwrsc_mng_table.resource_used |= s_impdrv_used_instance[instance_num];
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_hwrsc_close()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_pow_on_hwrsc()
* CD_PD_02_03_0034
* [Covers: UD_PD_UD02_03_0034]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_pow_on_hwrsc(
    const e_impdrv_instance_t   instance_num,
    const uint32_t              index
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (IMPDRV_HWRSC_TABLE_NUM <= index)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
       ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** When the needs to Devices open */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Change to state required. */
        if (IMPDRV_HWRSC_INDEX_IMP_TOP != index)
        {
            if (NULL == g_hwrsc_mng_table.handle[index].handle)
            {
                ercd = impdrv_osdep_pow_on_hwrsc(
                    &g_hwrsc_mng_table.handle[index],
                    s_hwrsc_def_table[index].name
                );
            }
            else
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
        else
        {
            ercd = impdrv_cmnctl_get_io_imptop(&g_hwrsc_mng_table.handle[index]);
            if ((IMPDRV_EC_OK == ercd)
                && (NULL == g_hwrsc_mng_table.handle[index].handle))
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_pow_on_hwrsc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_pow_off_hwrsc()
* CD_PD_02_03_0035
* [Covers: UD_PD_UD02_03_0035]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_pow_off_hwrsc(
    const e_impdrv_instance_t   instance_num,
    const uint32_t              index
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (IMPDRV_HWRSC_TABLE_NUM <= index)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
       ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** When the needs to Devices open */
    if (IMPDRV_EC_OK == ercd)
    {
        /** Change to state release. */
        if (IMPDRV_HWRSC_INDEX_IMP_TOP != index)
        {
            if (NULL != g_hwrsc_mng_table.handle[index].handle)
            {
                ercd = impdrv_osdep_pow_off_hwrsc(
                    &g_hwrsc_mng_table.handle[index]
                );
            }
        }
        else
        {
            g_hwrsc_mng_table.handle[index].handle = NULL;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_pow_off_hwrsc()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_spm_preset()
* CD_PD_02_03_0050
* [Covers: UD_PD_UD02_03_0050]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_preset(
    const e_impdrv_instance_t   instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                count;
    uint32_t                index = 0U;
    st_impdrv_hwrsc_def_t   resource;

    struct {
        const char                  *name;
        const uint32_t              offset;
        const uint32_t              value;
        const bool                  readback;
    } const procedure[IMPDRV_HWRSC_SPM_PSET_TBL_MAX] = {
        /** Resource Name(Key)  , Offset adrs,  value       ,   readback */
        {   "imp_top_00"        ,   0x2000U ,   0x0000000AU ,   true    },  /** PRESET Register(IMPSLV)         */
        {   "imp_reg_spmi_00"   ,   0x2000U ,   0x0000000AU ,   true    },  /** PRESET Register(SPMI0)          */
        {   "imp_reg_spmc_00"   ,   0x2000U ,   0x00000009U ,   true    }   /** PRESET Register(SPMC0)          */
    };

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_instance_num(instance_num);

    /** PRESET register settings */
    count = 0U;
    while ((IMPDRV_EC_OK == ercd) && (IMPDRV_HWRSC_SPM_PSET_TBL_MAX > count))
    {
        /** When the Write at using to OS dependent components */
        ercd = impdrv_cmnctl_reg_find_resource(procedure[count].name, &index, &resource);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Check for sequence errors */
            if (NULL == g_hwrsc_mng_table.handle[index].handle)
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
            else
            {
                /** Write data at using to OS dependent components */
                ercd = impdrv_osdep_write_reg(
                    &g_hwrsc_mng_table.handle[index],
                    procedure[count].offset,
                    procedure[count].value,
                    procedure[count].readback,
                    procedure[count].value
                );
            }
        }
        count++;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_spm_preset()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_spm_ena_access()
* CD_PD_02_03_0051
* [Covers: UD_PD_UD02_03_0051]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_ena_access(
    const uint32_t              index,
    const bool                  enable
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                reg_val;
    const bool              read_back = true;
    const uint32_t          offset_adrs = 0x0F18U;  /** SPMCTRL4 register */

    /** Input parameter confirmation process */
    if ((IMPDRV_HWRSC_TABLE_NUM <= index)
        || ((true != enable) && (false != enable)))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** Read the SPMCTRL4 register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(
            &g_hwrsc_mng_table.handle[index],
            offset_adrs,
            &reg_val
        );
    }

    /** Set the SPMCTRL4 register */
    if (IMPDRV_EC_OK == ercd)
    {
        if (true == enable)
        {
            /** Enable to Scratchpad memory register access */
            reg_val |= 0x80000000U;
        }
        else
        {
            /** Disable to Scratchpad memory register access */
            reg_val &= ~(0x80000000U);
        }

        /** Write the SPMCTRL4 register */
        ercd = impdrv_osdep_write_reg(
            &g_hwrsc_mng_table.handle[index],
            offset_adrs,
            reg_val,
            read_back,
            reg_val
        );
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_spm_ena_access()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_spm_chk_init()
* CD_PD_02_03_0052
* [Covers: UD_PD_UD02_03_0052]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_chk_init(
    const e_impdrv_instance_t   instance_num,
    const uint32_t              num_devices,
    const char                  *const*const p_device_id
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                index = 0U;
    st_impdrv_hwrsc_def_t   resource;
    uint32_t                count;
    uint32_t                wait_loop;
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_instance_num(instance_num);

    /** Check the initialization of target resource is completed. */
    count = 0U;
    while ((IMPDRV_EC_OK == ercd) && (num_devices > count))
    {
        /** Check the internal management data */
        if (NULL == p_device_id[count])
        {
            ercd = IMPDRV_EC_NG_ARGNULL;
        }
        else
        {
            ercd = impdrv_cmnctl_reg_find_resource(p_device_id[count], &index, &resource);
            if ((IMPDRV_EC_OK == ercd)
                && (NULL == g_hwrsc_mng_table.handle[index].handle))
            {
                ercd = IMPDRV_EC_NG_SYSTEMERROR;
            }
        }

        /* To wait over 10msec, count of bus access limit to 800M * 0.01 */
        wait_loop = 5U;

        /** Check the initialization */
        while ((IMPDRV_EC_OK == ercd) && (wait_loop > 0U))
        {
            /** Read data using to OS dependent components */
            ercd = impdrv_osdep_read_reg(
                &g_hwrsc_mng_table.handle[index],
                (((256U * 1024U) * 7U) + 4U), /** Memory initializations status register */
                &reg_val
            );

            /** If read value of the Memory initializations status register bit 1 is 1, */
            /** the initialization is finished.*/
            if ((IMPDRV_EC_OK == ercd) && (0U != (reg_val & 0x2U)))
            {
                break;
            }
            wait_loop--;
        }

        /** Check the timeout waiting for initialized */
        if ((IMPDRV_EC_OK == ercd) && (0U == wait_loop))
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
        count++;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_spm_chk_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_spm_wait_init()
* CD_PD_02_03_0053
* [Covers: UD_PD_UD02_03_0053]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_wait_init(
    const e_impdrv_instance_t   instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    const char* p_check_devices_id[1U] =
    {
        "imp_spmc_00"
    };
    const uint32_t num_devices = (uint32_t)(sizeof(p_check_devices_id)/sizeof(p_check_devices_id[0]));

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_instance_num(instance_num);

    /** Confirm that memory initialization is complete */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_reg_spm_chk_init(
            instance_num,
            num_devices,
            p_check_devices_id
        );
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_spm_wait_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_spm_initialize()
* CD_PD_02_03_0054
* [Covers: UD_PD_UD02_03_0054]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_spm_initialize(
    const e_impdrv_instance_t   instance_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                count;
    st_impdrv_hwrsc_def_t   resource;

    struct {
        const char                  *name;
        const uint32_t              offset;
        const uint32_t              value;
        const bool                  readback;
    } const procedure[IMPDRV_HWRSC_SPM_INIT_TBL_MAX] = {
        /** Resource Name(Key)  , Offset adrs,  value       ,   readback */
        { "imp_reg_spmi_00"     ,   0x2300U ,   0x80000000U ,   false   },  /** SPMI0 SPMCTRLX0 */
        { "imp_reg_spmc_00"     ,   0x2300U ,   0x80000000U ,   false   }   /** SPMC0 SPMCTRLX0 */
    };
    uint32_t        index[IMPDRV_HWRSC_SPM_INIT_TBL_MAX];

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_instance_num(instance_num);

    /** Find the index of handle information */
    if (IMPDRV_EC_OK == ercd)
    {
        count = 0;
        while ((IMPDRV_EC_OK == ercd) && (IMPDRV_HWRSC_SPM_INIT_TBL_MAX > count))
        {
            index[count] = 0U;
            ercd = impdrv_cmnctl_reg_find_resource(procedure[count].name, &index[count], &resource);
            count++;
        }
    }

    /** Enable to the register access of Scratchpad Memory */
    if (IMPDRV_EC_OK == ercd)
    {
        /** SPMI0 access enable */
        ercd = impdrv_cmnctl_reg_spm_ena_access(index[IMPDRV_HWRSC_INIT_SPMI0_IDX], true);
        if (IMPDRV_EC_OK == ercd)
        {
            /** SPMC0 access enable */
            ercd = impdrv_cmnctl_reg_spm_ena_access(index[IMPDRV_HWRSC_INIT_SPMC0_IDX], true);
        }
    }

    /* Initialize scratchpad memory */
    if (IMPDRV_EC_OK == ercd)
    {
        count = 0U;
        while ((IMPDRV_EC_OK == ercd) && (IMPDRV_HWRSC_SPM_INIT_TBL_MAX > count))
        {
            ercd = impdrv_osdep_write_reg(
                &g_hwrsc_mng_table.handle[index[count]],
                procedure[count].offset,
                procedure[count].value,
                procedure[count].readback,
                procedure[count].value
            );
            count++;
        }
        if (IMPDRV_EC_OK == ercd)
        {
            /** Waiting for scratchpad memory initialization to complete */
            ercd = impdrv_cmnctl_reg_spm_wait_init(
                instance_num
            );
        }
    }

    /** Disable to the register access of Scratchpad Memory */
    if (IMPDRV_EC_OK == ercd)
    {
        /** SPMI0 access disable */
        ercd = impdrv_cmnctl_reg_spm_ena_access(index[IMPDRV_HWRSC_INIT_SPMI0_IDX], false);
        if (IMPDRV_EC_OK == ercd)
        {
            /** SPMC0 access disable */
            ercd = impdrv_cmnctl_reg_spm_ena_access(index[IMPDRV_HWRSC_INIT_SPMC0_IDX], false); 
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_spm_initialize()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_find_resource()
* CD_PD_02_03_0036
* [Covers: UD_PD_UD02_03_0036]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_find_resource(
    const char              *const p_device_id,
    uint32_t                *const p_index,
    st_impdrv_hwrsc_def_t   *const p_resource
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    matched = false;
    uint32_t                index;

    /** Input parameter confirmation process */
    if ((NULL == p_device_id) || (NULL == p_index) || (NULL == p_resource))
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

    /** Find for matching hardware resource */
    if (IMPDRV_EC_OK == ercd)
    {
        index = 0U;
        while (IMPDRV_HWRSC_TABLE_NUM > index)
        {
            if (NULL != s_hwrsc_def_table[index].name)
            {
                matched = impdrv_cmnctl_reg_chk_rsc_name(
                    s_hwrsc_def_table[index].name,
                    p_device_id
                );
                if (true == matched)
                {
                    break;
                }
            }
            index++;
        }
        if (false == matched)
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
        else if (IMPDRV_HWRSC_TABLE_NUM > index)
        {
            *p_index = index;
            *p_resource = s_hwrsc_def_table[index];
        }
        else
        {
            /** Nothing to do */
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_find_resource()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_chk_rsc_name()
* CD_PD_02_03_0037
* [Covers: UD_PD_UD02_03_0037]
***********************************************************************************************************************/
IMPDRV_STATIC
bool impdrv_cmnctl_reg_chk_rsc_name(
    const char              *const p_rsc_name,
    const char              *const p_device_id
)
{
    /** initialize local variables */
    bool                    matched = true;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == p_rsc_name) || (NULL == p_device_id))
    {
        matched = false;
    }
    else
    {
        /** Check for matching hardware resource name */
        i = 0U;
        while ((IMPDRV_HWRSC_NAME_MAX > i) 
            && (('\0' != p_rsc_name[i]) && ('\0' != p_device_id[i])))
        {
            if (p_rsc_name[i] != p_device_id[i])
            {
                matched = false;
                break;
            }
            i++;
        }

        /* Check the length of the string. */
        if ((matched == true) 
            && (('\0' != p_rsc_name[i]) || ('\0' != p_device_id[i])))
        {
            matched = false;
        }
    }

    /** Set the check result in return value */
    return matched;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_chk_rsc_name()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
