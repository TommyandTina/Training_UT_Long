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
#define IMPDRV_INSTANCETABLE_NUM        (6U)        /**< Maximum number of Instance number table.   */
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
 * @def IMPDRV_REG_WPR
 * Offset address of hardware register for WPR.
***********************************************************************************************************************/
#define IMPDRV_REG_WPR                  (0x8000U)   /** Offset address of hardware register for WPR.        */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_RAMTSTR
 * Offset address of hardware register for RAMTSTR.
***********************************************************************************************************************/
#define IMPDRV_REG_RAMTSTR              (0x8120U)   /** Offset address of hardware register for RAMTSTR.    */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_SRAMFMR
 * Offset address of hardware register for SRAMFMR.
***********************************************************************************************************************/
#define IMPDRV_REG_SRAMFMR              (0x0314U)   /** Offset address of hardware register for SRAMFMR.    */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_MB_MEM_INIT_ODD
 * Multi-Bank memory odd initialization value.
***********************************************************************************************************************/
#define IMPDRV_REG_MB_MEM_INIT_ODD       (0x0000AAAAU)   /** Multi-Bank memory odd initialization value.    */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_MB_MEM_INIT_EVEN
 * Multi-Bank memory even initialization value.
***********************************************************************************************************************/
#define IMPDRV_REG_MB_MEM_INIT_EVEN      (0x00005555U)   /** Multi-Bank memory even initialization value.   */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PROTECT_EDC
 * EDC mode register value.
***********************************************************************************************************************/
#define IMPDRV_REG_PROTECT_EDC           (0x00000000U)   /** EDC mode register value.                       */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PROTECT_ECC
 * ECC mode register value.
***********************************************************************************************************************/
#define IMPDRV_REG_PROTECT_ECC           (0x80000000U)   /** ECC mode register value.                       */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PROTECT_MASK
 * Bitmask for Memory protection mode.
***********************************************************************************************************************/
#define IMPDRV_REG_PROTECT_MASK          (0x80000000U)   /** Bitmask for Memory protection mode.            */
/*******************************************************************************************************************//**
 * @def IMPDRV_MASK_L16BIT
 * Bit mask for Lower 16 bits.
***********************************************************************************************************************/
#define IMPDRV_MASK_L16BIT               (0x0000FFFFU)   /**< Bit mask for Lower 16 bits                    */

/** @} */

/*======================================================================================================================
Generic Enumeration
======================================================================================================================*/
/*******************************************************************************************************************//**
 * @ingroup IMPDRV_PRIVATE
 * @defgroup IMPDRV_GENERIC_ENUM  IMPDRV Generic Enumeration
 *
 * @{
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @enum e_impdrv_mb_mem_init_t
 * IMPDRV Multi-Bank memory initialization kind.
***********************************************************************************************************************/
typedef enum
{
    IMPDRV_MB_MBM_INIT_ODD,                        /**< Multi-Bank memory odd initialization       */
    IMPDRV_MB_MBM_INIT_EVEN                        /**< Multi-Bank memory even initialization      */
} e_impdrv_mb_mem_init_t;

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
    { "imp_top_00"      ,   0xFFA00000U ,   0x00010000U ,   true    ,   false       },
    { "imp_spmi_00"     ,   0xED000000U ,   0x00200000U ,   false   ,   false       },
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
    0x00000004U,    /**< IMPDRV_INSTANCE_2 */
    0x00000008U,    /**< IMPDRV_INSTANCE_3 */
    0x00000010U     /**< IMPDRV_INSTANCE_4 */
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

/*******************************************************************************************************************//**
 * @brief           Multi-Bank memory initialization.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       mb_mem_init_kind        Multi-Bank memory initialization kind.
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL          Hardware check failure
 * @retval          IMPDRV_EC_NG_SEQSTATE           Sequence Error
 * @retval          IMPDRV_EC_NG_ARGNULL            Arguments is NULL
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR        System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_mb_initialize(
    const e_impdrv_mb_mem_init_t   mb_mem_init_kind
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
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (IMPDRV_PROTECT_EDC == protect_mode)
    {
        reg_val = IMPDRV_REG_PROTECT_EDC;
    }
    else if (IMPDRV_PROTECT_ECC == protect_mode)
    {
        reg_val = IMPDRV_REG_PROTECT_ECC;
    }
    else
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** Memory protection setting */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&g_hwrsc_mng_table.handle[IMPDRV_HWRSC_INDEX_IMP_TOP],
                                                    IMPDRV_REG_SRAMFMR, reg_val, true, reg_val);
        if (IMPDRV_EC_OK == ercd)
        {
            p_commonctl->protect_mode = protect_mode;
        }
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
    uint32_t                reg_val;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_PROTECT_EDC != p_commonctl->protect_mode)
            && (IMPDRV_PROTECT_ECC != p_commonctl->protect_mode))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do */
    }

    /** Check the Memory protection setting */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&g_hwrsc_mng_table.handle[IMPDRV_HWRSC_INDEX_IMP_TOP],
                                                    IMPDRV_REG_SRAMFMR, &reg_val);
        if ((IMPDRV_EC_OK == ercd) && (IMPDRV_PROTECT_EDC == p_commonctl->protect_mode))
        {
            reg_val &= IMPDRV_REG_PROTECT_MASK;
            if (IMPDRV_REG_PROTECT_EDC != reg_val)
            {
                /** Error Handring */
                ercd = IMPDRV_EC_NG_CHECKFAIL;
            }
        }
        if ((IMPDRV_EC_OK == ercd) && (IMPDRV_PROTECT_ECC == p_commonctl->protect_mode))
        {
            reg_val &= IMPDRV_REG_PROTECT_MASK;
            if (IMPDRV_REG_PROTECT_ECC != reg_val)
            {
                /** Error Handring */
                ercd = IMPDRV_EC_NG_CHECKFAIL;
            }
        }
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
        /** Multi-Bank memory initialize */
        ercd = impdrv_cmnctl_reg_mb_initialize(IMPDRV_MB_MBM_INIT_ODD);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_mb_initialize(IMPDRV_MB_MBM_INIT_EVEN);
        }
    }

    /** Set the Memory initialized state. */
    if (IMPDRV_EC_OK == ercd)
    {
        g_hwrsc_mng_table.memory_initialized |= s_impdrv_used_instance[instance_num];
        p_commonctl->protect_mode = IMPDRV_PROTECT_EDC;
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

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_reg_mb_initialize()
* CD_PD_02_03_0038
* [Covers: UD_PD_UD02_03_0038]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_reg_mb_initialize(
    const e_impdrv_mb_mem_init_t   mb_mem_init_kind
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                reg_write_val;
    uint32_t                reg_read_val;

    /** Input parameter confirmation process */
    if (IMPDRV_MB_MBM_INIT_ODD == mb_mem_init_kind)
    {
        reg_write_val = IMPDRV_REG_MB_MEM_INIT_ODD;
    }
    else if (IMPDRV_MB_MBM_INIT_EVEN == mb_mem_init_kind)
    {
        reg_write_val = IMPDRV_REG_MB_MEM_INIT_EVEN;
    }
    else
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }

    /** Multi-Bank memory initialize */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&g_hwrsc_mng_table.handle[IMPDRV_HWRSC_INDEX_IMP_TOP],
                                                IMPDRV_REG_WPR, ~reg_write_val, true, ~reg_write_val);

        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_osdep_write_reg(&g_hwrsc_mng_table.handle[IMPDRV_HWRSC_INDEX_IMP_TOP],
                                                        IMPDRV_REG_RAMTSTR, reg_write_val, false, 0U);
        }
    }

    /** Check Multi-Bank memory initialize */
   if (IMPDRV_EC_OK == ercd)
    {
        do
        {
            ercd = impdrv_osdep_read_reg(&g_hwrsc_mng_table.handle[IMPDRV_HWRSC_INDEX_IMP_TOP],
                                                                    IMPDRV_REG_RAMTSTR, &reg_read_val);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            reg_read_val &= IMPDRV_MASK_L16BIT;
        }
        while (0U != reg_read_val);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_reg_mb_initialize()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/
