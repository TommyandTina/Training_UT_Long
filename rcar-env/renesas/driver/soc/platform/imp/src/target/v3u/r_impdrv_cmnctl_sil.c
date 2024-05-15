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
* File Name    : r_impdrv_cmnctl.c
* Version      : 3.19.0
* Product Name : IMP Driver
* Device(s)    : R-Car
* Description  : IMP Driver function of impdrv_cmnctl for xOS3
***********************************************************************************************************************/

/**
 * @file r_impdrv_cmnctl.c
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
 * @def IMPDRV_REG_INTEN00
 * Offset address of hardware register for INTEN00.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN00          (0x0020U)           /**< Offset address of hardware register for INTEN00. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN01
 * Offset address of hardware register for INTEN01.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN01          (0x0024U)           /**< Offset address of hardware register for INTEN01. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN02
 * Offset address of hardware register for INTEN02.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN02          (0x0028U)           /**< Offset address of hardware register for INTEN02. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G00INTSEL
 * Offset address of hardware register for G00INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G00INTSEL        (0x0030U)           /**< Offset address of hardware register for G00INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G01INTSEL
 * Offset address of hardware register for G01INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G01INTSEL        (0x0034U)           /**< Offset address of hardware register for G01INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G02INTSEL
 * Offset address of hardware register for G02INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G02INTSEL        (0x0038U)           /**< Offset address of hardware register for G02INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN10
 * Offset address of hardware register for INTEN10.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN10          (0x0040U)           /**< Offset address of hardware register for INTEN10. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN11
 * Offset address of hardware register for INTEN11.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN11          (0x0044U)           /**< Offset address of hardware register for INTEN11. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN12
 * Offset address of hardware register for INTEN12.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN12          (0x0048U)           /**< Offset address of hardware register for INTEN12. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G10INTSEL
 * Offset address of hardware register for G10INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G10INTSEL        (0x0050U)           /**< Offset address of hardware register for G10INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G11INTSEL
 * Offset address of hardware register for G11INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G11INTSEL        (0x0054U)           /**< Offset address of hardware register for G11INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_G12INTSEL
 * Offset address of hardware register for G12INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_G12INTSEL        (0x0058U)           /**< Offset address of hardware register for G12INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_MINTEN
 * Offset address of hardware register for MINTEN.
***********************************************************************************************************************/
#define IMPDRV_REG_MINTEN           (0x00B0U)           /**< Offset address of hardware register for MINTEN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_MG0INTSEL
 * Offset address of hardware register for MG0INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_MG0INTSEL        (0x00B4U)           /**< Offset address of hardware register for MG0INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_MG1INTSEL
 * Offset address of hardware register for MG1INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_MG1INTSEL        (0x00B8U)           /**< Offset address of hardware register for MG1INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_MG2INTSEL
 * Offset address of hardware register for MG2INTSEL.
***********************************************************************************************************************/
#define IMPDRV_REG_MG2INTSEL        (0x00BCU)           /**< Offset address of hardware register for MG2INTSEL. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTSTS0
 * Offset address of hardware register for INTSTS0.
***********************************************************************************************************************/
#define IMPDRV_REG_INTSTS0          (0x0010U)       /**< Offset address of hardware register for IMPDRV_REG_INTSTS0. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTSTS1
 * Offset address of hardware register for INTSTS1.
***********************************************************************************************************************/
#define IMPDRV_REG_INTSTS1          (0x0014U)       /**< Offset address of hardware register for IMPDRV_REG_INTSTS1. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTSTS2
 * Offset address of hardware register for INTSTS2.
***********************************************************************************************************************/
#define IMPDRV_REG_INTSTS2          (0x0018U)       /**< Offset address of hardware register for IMPDRV_REG_INTSTS2. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_INTEN_INIT_VAL
 * Initial value of interrupt enable register.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN_INIT_VAL   (0x00000000U)       /**< Initial value of interrupt enable register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_INTEN0_CORE_MAX
 * Maximum number of cores for the Interrupt group 1.
***********************************************************************************************************************/
#define IMPDRV_INTEN0_CORE_MAX      (11U)               /**< Maximum number of cores for the Interrupt group 1. */
/*******************************************************************************************************************//**
 * @def IMPDRV_INTEN1_CORE_MAX
 * Maximum number of cores for the Interrupt group 2.
***********************************************************************************************************************/
#define IMPDRV_INTEN1_CORE_MAX      (10U)               /**< Maximum number of cores for the Interrupt group 2. */
/*******************************************************************************************************************//**
 * @def IMPDRV_MINTEN_CORE_MAX
 * Maximum number of cores for the Merge interrupt group.
***********************************************************************************************************************/
#define IMPDRV_MINTEN_CORE_MAX      (21U)               /**< Maximum number of cores for the Merge interrupt group. */
/*******************************************************************************************************************//**
 * @def IMPDRV_GROUP_MAX
 * Maximum number of interrupt groups.
***********************************************************************************************************************/
#define IMPDRV_GROUP_MAX            (5U)                /**< Maximum number of interrupt groups. */
/*******************************************************************************************************************//**
 * @def IMPDRV_INSTANCETABLE_NUM
 * Maximum number of Instance number table.
***********************************************************************************************************************/
#define IMPDRV_INSTANCETABLE_NUM    (8U)                /**< Maximum number of Instance number table. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IRQCHTABLE_NUM
 * Maximum number of IRQ Channel table.
***********************************************************************************************************************/
#define IMPDRV_IRQCHTABLE_NUM       (7U)                /**< Maximum number of IRQ Channel table. */
/*******************************************************************************************************************//**
 * @def IMPDRV_SYNCCTABLE_NUM
 * Maximum number of SYNCC table.
***********************************************************************************************************************/
#define IMPDRV_SYNCCTABLE_NUM       (29U)               /**< Maximum number of SYNCC table. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DUMMY_UINT32
 * Uint32_t dummy value.
***********************************************************************************************************************/
#define IMPDRV_DUMMY_UINT32         (0xFFFFFFFFU)       /**< Uint32_t dummy value. */

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
 * @struct st_impdrv_inten_tbl_t
 * Definition for bit allocation conversion of the Interrupt enable register
***********************************************************************************************************************/
typedef struct
{
    e_impdrv_core_type_t        core_type;              /**< IMP Core type */
    uint32_t                    core_num;               /**< Core number in the core type */
    uint32_t                    bit;                    /**< Bit value to convert */
} st_impdrv_inten_tbl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_group_tbl_t
 * Definition for bit allocation conversion of the Interrupt group register
***********************************************************************************************************************/
typedef struct
{
    uint32_t                    group_bit;              /**< Bit value to convert */
    uint32_t                    group_reg;              /**< Offset address value to convert */
} st_impdrv_group_tbl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_syncc_table_t
 * Definition for conversion of the syncc value
***********************************************************************************************************************/
typedef struct
{
    e_impdrv_core_type_t        core_type;              /**< IMP Core type */
    uint32_t                    core_num;               /**< Core number in the core type */
    uint8_t                     syncc_val;              /**< Syncc value to convert */
} st_impdrv_syncc_table_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_inten_reg_table_t
 * Definition for conversion of register table
***********************************************************************************************************************/
typedef struct
{
    const st_impdrv_group_tbl_t     *p_inten_group_table;   /**< Convert Int group register table */
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;     /**< Convert Int enable register table */
    uint32_t                        cnv_table_num;          /**< Number of convert Int enable register table */
} st_impdrv_inten_reg_tbl_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_commonctl_ins_t
 * Common control global area.
***********************************************************************************************************************/
typedef struct
{
    st_impdrv_device_handle_t   device_io_imptop;                       /**< OSAL device handle (IMP TOP).          */
    st_impdrv_mutex_handle_t    mutex_handle;                           /**< OSAL mutex handle                      */
    osal_mutex_id_t             mutex_id;                               /**< OSAL mutex id                          */
    osal_milli_sec_t            mutex_time_period;                      /**< Millisecond Order Mutex Timeout value  */
    st_impdrv_device_handle_t   device_io_dta;                          /**< OSAL device handle (DTA Core).         */
    st_impdrv_commonctl_t       *p_commonctl[IMPDRV_INSTANCETABLE_NUM]; /**< Address of the commonctl.              */
} st_impdrv_commonctl_ins_t;

/*******************************************************************************************************************//**
 * @struct st_impdrv_commonctl_chk_t
 * Common control global check area.
***********************************************************************************************************************/
typedef struct
{
    uint32_t                    checksum;                               /**< Checksum value of Global variable.     */
    st_impdrv_device_handle_t   device_io_imptop;                       /**< Backup of OSAL device handle(IMP TOP). */
} st_impdrv_commonctl_chk_t;

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
const uint32_t s_impdrv_inten_addr[IMPDRV_INSTANCETABLE_NUM] =
{
    IMPDRV_DUMMY_UINT32,        /**< Dummy for Index adjustment */
    IMPDRV_REG_INTEN00,         /**< IMPDRV_INSTANCE_0 */
    IMPDRV_REG_INTEN01,         /**< IMPDRV_INSTANCE_1 */
    IMPDRV_REG_INTEN02,         /**< IMPDRV_INSTANCE_2 */
    IMPDRV_REG_INTEN10,         /**< IMPDRV_INSTANCE_3 */
    IMPDRV_REG_INTEN11,         /**< IMPDRV_INSTANCE_4 */
    IMPDRV_REG_INTEN12,         /**< IMPDRV_INSTANCE_5 */
    IMPDRV_REG_MINTEN           /**< IMPDRV_INSTANCE_6 */
};

IMPDRV_STATIC
const st_impdrv_inten_tbl_t s_impdrv_inten0_cnv_table[IMPDRV_INTEN0_CORE_MAX] =
{
    /** coreType,               CoreNum,    bit     */
    {   IMPDRV_CORE_TYPE_IMP,       0,  0x00000001U },
    {   IMPDRV_CORE_TYPE_IMP,       1,  0x00000002U },
    {   IMPDRV_CORE_TYPE_PSCEXE,    0,  0x00000004U },
    {   IMPDRV_CORE_TYPE_DMAC,      0,  0x00000008U },
    {   IMPDRV_CORE_TYPE_DMAC,      1,  0x00000010U },
    {   IMPDRV_CORE_TYPE_OCV,       0,  0x00000020U },
    {   IMPDRV_CORE_TYPE_OCV,       1,  0x00000040U },
    {   IMPDRV_CORE_TYPE_OCV,       4,  0x00000080U },
    {   IMPDRV_CORE_TYPE_OCV,       6,  0x00000100U },
    {   IMPDRV_CORE_TYPE_CNN,       0,  0x00000200U },
    {   IMPDRV_CORE_TYPE_CNN,       2,  0x00000400U }
};

IMPDRV_STATIC
const st_impdrv_group_tbl_t s_impdrv_inten0_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   0x00000800U,            IMPDRV_REG_G00INTSEL    },  /** Group00. */
    {   0x00001000U,            IMPDRV_REG_G01INTSEL    },  /** Group01. */
    {   0x00002000U,            IMPDRV_REG_G02INTSEL    }   /** Group02. */
};

IMPDRV_STATIC
const st_impdrv_inten_tbl_t s_impdrv_inten1_cnv_table[IMPDRV_INTEN1_CORE_MAX] =
{
    /** coreType,               CoreNum     bit     */
    {   IMPDRV_CORE_TYPE_IMP,       2,  0x00000001U },
    {   IMPDRV_CORE_TYPE_IMP,       3,  0x00000002U },
    {   IMPDRV_CORE_TYPE_PSCEXE,    1,  0x00000004U },
    {   IMPDRV_CORE_TYPE_DMAC,      2,  0x00000008U },
    {   IMPDRV_CORE_TYPE_DMAC,      3,  0x00000010U },
    {   IMPDRV_CORE_TYPE_OCV,       2,  0x00000020U },
    {   IMPDRV_CORE_TYPE_OCV,       3,  0x00000040U },
    {   IMPDRV_CORE_TYPE_OCV,       5,  0x00000080U },
    {   IMPDRV_CORE_TYPE_OCV,       7,  0x00000100U },
    {   IMPDRV_CORE_TYPE_CNN,       1,  0x00000400U }
};

IMPDRV_STATIC
const st_impdrv_group_tbl_t s_impdrv_inten1_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   0x00000800U,            IMPDRV_REG_G10INTSEL    },  /** Group10. */
    {   0x00001000U,            IMPDRV_REG_G11INTSEL    },  /** Group11. */
    {   0x00002000U,            IMPDRV_REG_G12INTSEL    }   /** Group12. */
};

IMPDRV_STATIC
const st_impdrv_inten_tbl_t s_impdrv_minten_cnv_table[IMPDRV_MINTEN_CORE_MAX] =
{
    /** coreType,               CoreNum     bit     */
    {   IMPDRV_CORE_TYPE_IMP,       0,  0x00000001U },
    {   IMPDRV_CORE_TYPE_IMP,       1,  0x00000002U },
    {   IMPDRV_CORE_TYPE_PSCEXE,    0,  0x00000004U },
    {   IMPDRV_CORE_TYPE_DMAC,      0,  0x00000008U },
    {   IMPDRV_CORE_TYPE_DMAC,      1,  0x00000010U },
    {   IMPDRV_CORE_TYPE_OCV,       0,  0x00000020U },
    {   IMPDRV_CORE_TYPE_OCV,       1,  0x00000040U },
    {   IMPDRV_CORE_TYPE_OCV,       4,  0x00000080U },
    {   IMPDRV_CORE_TYPE_OCV,       6,  0x00000100U },
    {   IMPDRV_CORE_TYPE_CNN,       0,  0x00000200U },
    {   IMPDRV_CORE_TYPE_CNN,       2,  0x00000400U },
    {   IMPDRV_CORE_TYPE_IMP,       2,  0x00000800U },
    {   IMPDRV_CORE_TYPE_IMP,       3,  0x00001000U },
    {   IMPDRV_CORE_TYPE_PSCEXE,    1,  0x00002000U },
    {   IMPDRV_CORE_TYPE_DMAC,      2,  0x00004000U },
    {   IMPDRV_CORE_TYPE_DMAC,      3,  0x00008000U },
    {   IMPDRV_CORE_TYPE_OCV,       2,  0x00010000U },
    {   IMPDRV_CORE_TYPE_OCV,       3,  0x00020000U },
    {   IMPDRV_CORE_TYPE_OCV,       5,  0x00040000U },
    {   IMPDRV_CORE_TYPE_OCV,       7,  0x00080000U },
    {   IMPDRV_CORE_TYPE_CNN,       1,  0x00200000U }
};

IMPDRV_STATIC
const st_impdrv_group_tbl_t s_impdrv_minten_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   0x00400000U,            IMPDRV_REG_MG0INTSEL    },  /** Group20. */
    {   0x00800000U,            IMPDRV_REG_MG1INTSEL    },  /** Group21. */
    {   0x01000000U,            IMPDRV_REG_MG2INTSEL    }   /** Group22. */
};

IMPDRV_STATIC
const st_impdrv_inten_reg_tbl_t s_impdrv_inten_reg_tbl[IMPDRV_INSTANCETABLE_NUM] =
{
    /** p_inten_group_table         p_inten_cnv_table           cnv_table_num */
    { NULL,                         NULL,                       IMPDRV_DUMMY_UINT32     },   /**< Dummy for Index */
    { s_impdrv_inten0_group_table,  s_impdrv_inten0_cnv_table,  IMPDRV_INTEN0_CORE_MAX  },   /**< IMPDRV_INSTANCE_0 */
    { s_impdrv_inten0_group_table,  s_impdrv_inten0_cnv_table,  IMPDRV_INTEN0_CORE_MAX  },   /**< IMPDRV_INSTANCE_1 */
    { s_impdrv_inten0_group_table,  s_impdrv_inten0_cnv_table,  IMPDRV_INTEN0_CORE_MAX  },   /**< IMPDRV_INSTANCE_2 */
    { s_impdrv_inten1_group_table,  s_impdrv_inten1_cnv_table,  IMPDRV_INTEN1_CORE_MAX  },   /**< IMPDRV_INSTANCE_3 */
    { s_impdrv_inten1_group_table,  s_impdrv_inten1_cnv_table,  IMPDRV_INTEN1_CORE_MAX  },   /**< IMPDRV_INSTANCE_4 */
    { s_impdrv_inten1_group_table,  s_impdrv_inten1_cnv_table,  IMPDRV_INTEN1_CORE_MAX  },   /**< IMPDRV_INSTANCE_5 */
    { s_impdrv_minten_group_table,  s_impdrv_minten_cnv_table,  IMPDRV_MINTEN_CORE_MAX  },   /**< IMPDRV_INSTANCE_6 */
};

IMPDRV_STATIC
st_impdrv_commonctl_ins_t g_impdrv_cmn_ctl_ins =
{
    { NULL },                                           /**< device_io_imptop                       */
    { NULL },                                           /**< mutex_handle                           */
    0U,                                                 /**< mutex_id                               */
    0,                                                  /**< mutex_time_period                      */
    { NULL },                                           /**< device_io_dta                          */
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }  /**< *p_commonctl[IMPDRV_INSTANCETABLE_NUM] */
};

IMPDRV_STATIC
st_impdrv_commonctl_chk_t g_impdrv_cmn_ctl_chk =
{
    0U,                                                 /**< Checksum data for g_impdrv_cmn_ctl_ins */
    { NULL }                                            /**< Backup data of device_io_imptop        */
};

IMPDRV_STATIC
const e_impdrv_instance_t s_impdrv_cnv_ins_table[IMPDRV_INSTANCETABLE_NUM - 1U] =
{
    /** core_type  */
    IMPDRV_INSTANCE_0               ,           /**< Identification value for assigning to the IRQ number (0th) */
    IMPDRV_INSTANCE_1               ,           /**< Identification value for assigning to the IRQ number (1st) */
    IMPDRV_INSTANCE_2               ,           /**< Identification value for assigning to the IRQ number (2nd) */
    IMPDRV_INSTANCE_3               ,           /**< Identification value for assigning to the IRQ number (3rd) */
    IMPDRV_INSTANCE_4               ,           /**< Identification value for assigning to the IRQ number (4th) */
    IMPDRV_INSTANCE_5               ,           /**< Identification value for assigning to the IRQ number (5th) */
    IMPDRV_INSTANCE_6                           /**< Identification value for assigning to the IRQ number (6th) */
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
 * @brief           Check the Input value of core information.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       core_info           Specified core information
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_chk_core_info(
    const st_impdrv_core_info_t     core_info
);

/*******************************************************************************************************************//**
* @brief            Check initialization data of the Common control.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle
* @param[in]        p_cmn_init_data     Common control init data.
* @param[in]        p_is_dmac_mb_init   DMAC multi-bank init
* @param[out]       p_use_core_info     Use core information.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_INSTANCE           Instance parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_chk_init_data(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_cmn_init_data_t *const p_cmn_init_data,
    const bool                      *const p_is_dmac_mb_init,
    uint32_t                        *const p_use_core_info
);

/*******************************************************************************************************************//**
* @brief            Check IRQ Group Arguments.
* @syncasync        Synchronous.
* @reentrant        Non-Reentrant
* @param[in]        p_commonctl         Common Control handle
* @param[in]        p_irq_param         Setting IRQ group.
* @return           Return value
* @retval           IMPDRV_EC_OK                    Success completion
* @retval           IMPDRV_EC_NG_ARGNULL            Arguments is NULL
* @retval           IMPDRV_EC_NG_PARAM              Parameter error
* @retval           IMPDRV_EC_NG_SYSTEMERROR        System error
* @retval           IMPDRV_EC_NG_ATTRIBUTE          Attribute setting error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_chk_group_data(
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_irq_group_t *const p_irq_param
);

/*******************************************************************************************************************//**
 * @brief           Get the Interrupt register bit pattern
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       core_info           Array of Core information
 * @param[in]       use_core_num        Number of Core information
 * @param[in]       s_inten_cnv_table   Array of Convert information
 * @param[in]       cnv_table_num       Number of Convert information
 * @param[out]      p_val               Interrupt register bit pattern
 * @return          Return value
 * @retval          IMPDRV_EC_OK                    Success completion
 * @retval          IMPDRV_EC_NG_PARAM              Parameter error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_core_bit(
    const st_impdrv_core_info_t     *const p_core_info,
    const uint32_t                  use_core_num,
    const st_impdrv_inten_tbl_t     *const p_inten_cnv_table,
    const uint32_t                  cnv_table_num,
    uint32_t                        *const p_val
);

/*******************************************************************************************************************//**
 * @brief           Set the core information of the bit
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_s_inten_cnv_table          Private table static variables
 * @param[in]       cnv_table_num                Number of Core information
 * @param[in]       stat                         register value
 * @param[out]      core_info                    Core information
 * @param[out]      p_use_core_num               Core number
 * @return          Return value
 * @retval          IMPDRV_EC_OK                 Success completion
 * @retval          IMPDRV_EC_NG_PARAM           Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL         Arguments is NULL
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_bit_core(
    const st_impdrv_inten_tbl_t     *const p_s_inten_cnv_table,
    const uint32_t                  cnv_table_num,
    const uint32_t                  stat,
    st_impdrv_core_info_t           core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                        *const p_use_core_num
);

/*******************************************************************************************************************//**
 * @brief           Set the core information of the group_core_info
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_handle              Device handle
 * @param[in]       p_inten_group_table          Interrupt group register
 * @param[in]       instance_num                 Instance number
 * @param[in]       group_reg_write_val          Interrupt register value
 * @return          Return value
 * @retval          IMPDRV_EC_OK                 Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL       Hardware check failure
 * @retval          IMPDRV_EC_NG_PARAM           Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL         Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE        Sequence Error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR     System error
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_execute_no_group(
    const st_impdrv_device_handle_t *const p_device_handle,
    const st_impdrv_group_tbl_t     *const p_inten_group_table,
    const e_impdrv_instance_t       instance_num,
    const uint32_t                  write_val
);

/*******************************************************************************************************************//**
 * @brief           Set the core information of the group_core_info
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       p_device_handle              Device handle
 * @param[in]       p_inten_group_table          Interrupt group register
 * @param[in]       instance_num                 Instance number
 * @param[in]       group_reg_write_val          Interrupt register value
 * @param[in]       irq_group                    Interrupt register value
 * @return          Return value
 * @retval          IMPDRV_EC_OK                 Success completion
 * @retval          IMPDRV_EC_NG_CHECKFAIL       Hardware check failure
 * @retval          IMPDRV_EC_NG_PARAM           Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL         Arguments is NULL
 * @retval          IMPDRV_EC_NG_SEQSTATE        Sequence Error
 * @retval          IMPDRV_EC_NG_SYSTEMERROR     Interrupt group number
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_execute_group(
    const st_impdrv_device_handle_t *const p_device_handle,
    const st_impdrv_group_tbl_t     *const p_inten_group_table,
    const e_impdrv_instance_t       instance_num,
    const uint32_t                  group_reg_write_val,
    const e_impdrv_irq_group_t      irq_group
);

/*******************************************************************************************************************//**
 * @brief           Get group number.
 * @syncasync       Synchronous.
 * @reentrant       Non-Reentrant
 * @param[in]       instance_num                 Instance number
 * @param[out]      p_irq_group                  Interrupt register value
 * @return          Return value
 * @retval          IMPDRV_EC_OK                 Success completion
 * @retval          IMPDRV_EC_NG_PARAM           Parameter error
 * @retval          IMPDRV_EC_NG_ARGNULL         Arguments is NULL
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_get_group(
    e_impdrv_irq_group_t            *const p_irq_group,
    const e_impdrv_instance_t       instance_num
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
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_set_checksum(
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
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_test_checksum(
    void
);

/** @} */

/*======================================================================================================================
Public function definitions
======================================================================================================================*/
/***********************************************************************************************************************
* Start of function impdrv_cmnctl_init()
* CD_PD_02_03_0001
* [Covers: UD_PD_UD02_03_0001]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_init(
    st_impdrv_commonctl_t           *const p_commonctl,
    const st_impdrv_cmn_init_data_t *const p_cmn_init_data,
    bool                            *const p_is_dmac_mb_init
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd;
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;
    uint32_t                        cnv_table_num;
    uint32_t                        use_core_info = 0U;
    uint32_t                        write_val = 0U;
    uint32_t                        read_val = 0U;

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_init_data(p_commonctl, p_cmn_init_data, p_is_dmac_mb_init, &use_core_info);

    /** Get interrupt bit pattern */
    if (IMPDRV_EC_OK == ercd)
    {
        p_inten_cnv_table = s_impdrv_inten_reg_tbl[p_cmn_init_data->instance_num].p_inten_cnv_table;
        cnv_table_num = s_impdrv_inten_reg_tbl[p_cmn_init_data->instance_num].cnv_table_num;
        
        ercd = impdrv_cmnctl_cnv_int_core_bit(
            p_cmn_init_data->p_core_info,
            p_cmn_init_data->use_core_num,
            p_inten_cnv_table,
            cnv_table_num,
            &write_val
        );
    }

    /** Request osdep to power on IMP TOP */
    if ((IMPDRV_EC_OK == ercd) && (NULL == g_impdrv_cmn_ctl_ins.device_io_imptop.handle))
    {
        ercd = impdrv_osdep_pow_on_imp_top(&g_impdrv_cmn_ctl_ins.device_io_imptop);
        if (IMPDRV_EC_OK == ercd)
        {
            g_impdrv_cmn_ctl_chk.device_io_imptop = g_impdrv_cmn_ctl_ins.device_io_imptop;
        }
    }

    /** Request osdep to register interrupt handler */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_register_irq(&g_impdrv_cmn_ctl_ins.device_io_imptop, p_cmn_init_data->instance_num,
                                                    p_cmn_init_data->irq_priority, p_cmn_init_data->osal_cb_args);
    }

    /** Request osdep to enable interrupt */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_enable_irq(&g_impdrv_cmn_ctl_ins.device_io_imptop, p_cmn_init_data->instance_num);
    }

    /** Read INTerrupt Enable register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&g_impdrv_cmn_ctl_ins.device_io_imptop,
                        s_impdrv_inten_addr[(uint32_t)p_cmn_init_data->instance_num], &read_val);
    }

    /** Write INTerrupt Enable register */
    if (IMPDRV_EC_OK == ercd)
    {
        write_val = write_val | read_val;
        ercd = impdrv_osdep_write_reg(&g_impdrv_cmn_ctl_ins.device_io_imptop,
                        s_impdrv_inten_addr[(uint32_t)p_cmn_init_data->instance_num], write_val, true, write_val);
    }

    /** Set data */
    if (NULL != p_commonctl)
    {
        p_commonctl->instance_num = p_cmn_init_data->instance_num;
        g_impdrv_cmn_ctl_ins.p_commonctl[(uint32_t)p_cmn_init_data->instance_num] = p_commonctl;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        p_commonctl->use_core_ins = use_core_info;
        p_commonctl->cbfunc_fatal = p_cmn_init_data->cbfunc_fatal;
        p_commonctl->p_cb_args_fatal = p_cmn_init_data->p_cb_args_fatal;
        p_commonctl->irq_mask_value = 0U;
    }

    *p_is_dmac_mb_init = false;

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_init()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_quit()
* CD_PD_02_03_0003
* [Covers: UD_PD_UD02_03_0003]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_quit(
    st_impdrv_commonctl_t   *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_group = IMPDRV_EC_OK;
    e_impdrv_errorcode_t        ercd_sub;
    const st_impdrv_group_tbl_t *p_inten_group_table;
    e_impdrv_irq_group_t        irq_group;
    uint32_t                    i;
    uint32_t                    instance_num = 0U;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_cmnctl_chk_instance_num(p_commonctl->instance_num);
    }

    /** Check last instance and mask Group** INTerrupt SELect */
    if (IMPDRV_EC_OK == ercd)
    {
        for (i = 0U; (IMPDRV_INSTANCETABLE_NUM - 1U) > i; i++)
        {
            if (NULL != g_impdrv_cmn_ctl_ins.p_commonctl[s_impdrv_cnv_ins_table[i]])
            {
               instance_num++;
            }
        }
        instance_num = instance_num - 1U;

        p_inten_group_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_group_table;
        
        ercd_group = impdrv_cmnctl_get_group(&irq_group, p_commonctl->instance_num);
        if (IMPDRV_EC_NG_NOTSUPPORT != ercd_group)
        {
            ercd = ercd_group;
        }
    }

    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_EC_OK == ercd_group))
    {
        ercd = impdrv_osdep_write_reg(&g_impdrv_cmn_ctl_ins.device_io_imptop,
                    p_inten_group_table[(uint32_t)irq_group].group_reg, 
                    IMPDRV_REG_INTEN_INIT_VAL, true, IMPDRV_REG_INTEN_INIT_VAL);
    }

    /** Mask INTerrupt Enable Register */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_write_reg(&g_impdrv_cmn_ctl_ins.device_io_imptop,
                s_impdrv_inten_addr[(uint32_t)p_commonctl->instance_num], IMPDRV_REG_INTEN_INIT_VAL,
                true, IMPDRV_REG_INTEN_INIT_VAL);
    }

    /** Disable IRQ */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_disable_irq(&g_impdrv_cmn_ctl_ins.device_io_imptop, p_commonctl->instance_num);
    }

    /** Unregister IRQ */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_unregister_irq(&g_impdrv_cmn_ctl_ins.device_io_imptop, p_commonctl->instance_num);
    }

    /** Power off IMPTOP */
    if (((IMPDRV_EC_OK == ercd) || (IMPDRV_EC_NG_SYSTEMERROR == ercd)) && (0U == instance_num))
    {
        ercd_sub = impdrv_osdep_pow_off_imp_top(&g_impdrv_cmn_ctl_ins.device_io_imptop);
        if ((IMPDRV_EC_OK == ercd) && (IMPDRV_EC_OK != ercd_sub))
        {
            ercd = ercd_sub;
        }
        if (IMPDRV_EC_OK == ercd_sub)
        {
            g_impdrv_cmn_ctl_chk.device_io_imptop = g_impdrv_cmn_ctl_ins.device_io_imptop;
        }
    }

    /** Clear data */
    if (NULL != p_commonctl)
    {
        g_impdrv_cmn_ctl_ins.p_commonctl[(uint32_t)p_commonctl->instance_num] = NULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        p_commonctl->use_core_ins = 0U;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_quit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_judge_int()
* CD_PD_02_03_0004
* [Covers: UD_PD_UD02_03_0004]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_judge_int(
    const st_impdrv_commonctl_t *const p_commonctl,
    const uint64_t              irq_channel,
    st_impdrv_core_info_t       core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                    *const p_use_core_num,
    bool                        is_fb_callback
)
{
    /** initialize static constance table */
    IMPDRV_STATIC
    const uint32_t s_impdrv_intsts_addr[IMPDRV_IRQCHTABLE_NUM] =
    {
        IMPDRV_REG_INTSTS0,         /**< IMPDRV_INSTANCE_0 */
        IMPDRV_REG_INTSTS0,         /**< IMPDRV_INSTANCE_1 */
        IMPDRV_REG_INTSTS0,         /**< IMPDRV_INSTANCE_2 */
        IMPDRV_REG_INTSTS1,         /**< IMPDRV_INSTANCE_3 */
        IMPDRV_REG_INTSTS1,         /**< IMPDRV_INSTANCE_4 */
        IMPDRV_REG_INTSTS1,         /**< IMPDRV_INSTANCE_5 */
        IMPDRV_REG_INTSTS2          /**< IMPDRV_INSTANCE_6 */
    };

    /** initialize local variables */
    e_impdrv_errorcode_t        ercd = IMPDRV_EC_OK;
    uint32_t                    cnt;
    e_impdrv_errorcode_t        ercd_group = IMPDRV_EC_OK;
    uint32_t                    stat = 0U;
    const st_impdrv_inten_tbl_t *p_inten_cnv_table = NULL;
    const st_impdrv_group_tbl_t *p_inten_group_table = NULL;
    uint32_t                    cnv_table_num = 0U;
    uint32_t                    intsel = 0U;
    uint32_t                    core_bit = 0U;
    e_impdrv_irq_group_t        irq_group;
    st_impdrv_core_info_t       core_info_ins[IMPDRV_INTSTS_BITNUM];
    uint32_t                    use_core_num_ins;
    uint32_t                    core_ins_bit = 0U;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == core_info) || (NULL == p_use_core_num))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((uint64_t)(p_commonctl->instance_num) != (irq_channel + 1U))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        ercd = impdrv_cmnctl_chk_instance_num(p_commonctl->instance_num);
    }

    /** Memory broken check for imp_top handle. */
    if (IMPDRV_EC_OK == ercd)
    {
        if (g_impdrv_cmn_ctl_chk.device_io_imptop.handle != g_impdrv_cmn_ctl_ins.device_io_imptop.handle)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Initialize array */
    if (IMPDRV_EC_OK == ercd)
    {
        for (cnt = 0U; IMPDRV_INTSTS_BITNUM > cnt; cnt++)
        {
            core_info_ins[cnt].core_type = IMPDRV_CORE_TYPE_INVALID;
            core_info_ins[cnt].core_num = 0U;
        }
    }

    /** Read interrupt status */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(&g_impdrv_cmn_ctl_ins.device_io_imptop,
                                    (s_impdrv_intsts_addr[irq_channel]), &stat);
    }

    /** Get own group */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd_group = impdrv_cmnctl_get_group(&irq_group, p_commonctl->instance_num);
        if (IMPDRV_EC_NG_NOTSUPPORT != ercd_group)
        {
            ercd = ercd_group;
        }
    }

    /** Judgement which core caused the interrupt */
    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_EC_OK == ercd_group))
    {
        p_inten_group_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_group_table;
        
        /** W/A : Check for Segmentation fault **/
        if (NULL != p_inten_group_table)
        {
            if (0U != (stat & p_inten_group_table[(uint32_t)irq_group].group_bit))
            {
                ercd = impdrv_osdep_read_reg(&g_impdrv_cmn_ctl_ins.device_io_imptop,
                                                p_inten_group_table[(uint32_t)irq_group].group_reg, &intsel);
            }
        }
        else
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }

        if (IMPDRV_EC_OK == ercd)
        {
            stat &= ~(p_inten_group_table[(uint32_t)irq_group].group_bit);
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        p_inten_cnv_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_cnv_table;
        cnv_table_num = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].cnv_table_num;

        ercd = impdrv_cmnctl_cnv_int_bit_core(s_impdrv_minten_cnv_table, IMPDRV_MINTEN_CORE_MAX,
                                                    p_commonctl->use_core_ins, core_info_ins, &use_core_num_ins);

        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_cnv_int_core_bit(core_info_ins, use_core_num_ins,
                                    p_inten_cnv_table, cnv_table_num, &core_ins_bit);
        }

        if (IMPDRV_EC_OK == ercd)
        {
            core_bit = stat & core_ins_bit;
            core_bit |= intsel;

            ercd = impdrv_cmnctl_cnv_int_bit_core(p_inten_cnv_table, cnv_table_num,
                                                        core_bit, core_info, p_use_core_num);
        }

        if ((IMPDRV_EC_OK == ercd) && (0U == *p_use_core_num) && (true == is_fb_callback))
        {
            /*  stop notice fatal error for simulator, because IMP driver need to wait interrupt status cleared */
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_judge_int()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_mutex_create()
* CD_PD_02_03_0005
* [Covers: UD_PD_UD02_03_0005]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_create(
    const st_impdrv_commonctl_t *const p_commonctl,
    const osal_mutex_id_t       mutex_id,
    const osal_milli_sec_t      time_period
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0 > time_period)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Check the first instance */
    if ((IMPDRV_EC_OK == ercd) && (NULL == g_impdrv_cmn_ctl_ins.mutex_handle.handle))
    {
        /** Request osdep to create mutex resource */
        ercd = impdrv_osdep_mutex_create(&g_impdrv_cmn_ctl_ins.mutex_handle, mutex_id);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Store value */
            g_impdrv_cmn_ctl_ins.mutex_id = mutex_id;
            g_impdrv_cmn_ctl_ins.mutex_time_period = time_period;

            /** Initial setting for Memory broken check */
            ercd = impdrv_cmnctl_set_checksum();
            if (IMPDRV_EC_OK == ercd)
            {
                ercd = impdrv_cmnctl_reg_set_checksum();
            }
            if (IMPDRV_EC_OK != ercd)
            {
                (void)impdrv_osdep_mutex_destroy(&g_impdrv_cmn_ctl_ins.mutex_handle);
                g_impdrv_cmn_ctl_ins.mutex_id = 0U;
                g_impdrv_cmn_ctl_ins.mutex_time_period = 0;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_mutex_create()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_mutex_destroy()
* CD_PD_02_03_0006
* [Covers: UD_PD_UD02_03_0006]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_destroy(
    const st_impdrv_commonctl_t   *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                    i;
    uint32_t                    instance_num = 0U;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Request osdep to destroy mutex resource */
    if (IMPDRV_EC_OK == ercd)
    {
        for (i = 0U; (IMPDRV_INSTANCETABLE_NUM - 1U) > i; i++)
        {
            if (NULL != g_impdrv_cmn_ctl_ins.p_commonctl[s_impdrv_cnv_ins_table[i]])
            {
               instance_num++;
            }
        }

        if (0U == instance_num)
        {
            ercd = impdrv_osdep_mutex_destroy(&g_impdrv_cmn_ctl_ins.mutex_handle);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_mutex_destroy()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_mutex_lock()
* CD_PD_02_03_0007
* [Covers: UD_PD_UD02_03_0007]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_lock(
    st_impdrv_commonctl_t   *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Request osdep to lock mutex */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_mutex_lock(&g_impdrv_cmn_ctl_ins.mutex_handle, g_impdrv_cmn_ctl_ins.mutex_time_period);
    }

    /**  Test the Memory broken check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_test_checksum();
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_test_checksum();
        }
        if (IMPDRV_EC_OK != ercd)
        {
            (void)impdrv_osdep_mutex_unlock(&g_impdrv_cmn_ctl_ins.mutex_handle);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_mutex_lock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_mutex_unlock()
* CD_PD_02_03_0008
* [Covers: UD_PD_UD02_03_0008]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_mutex_unlock(
    st_impdrv_commonctl_t   *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_func;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Update setting for Memory broken check */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_set_checksum();
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = impdrv_cmnctl_reg_set_checksum();
        }
    }

    /** Request osdep to release mutex */
    if (NULL != p_commonctl)
    {
        ercd_func = impdrv_osdep_mutex_unlock(&g_impdrv_cmn_ctl_ins.mutex_handle);
        if (IMPDRV_EC_OK == ercd)
        {
            ercd = ercd_func;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_mutex_unlock()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_set_irq_group()
* CD_PD_02_03_0009
* [Covers: UD_PD_UD02_03_0009]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_set_irq_group(
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_irq_group_t *const p_irq_param
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    const st_impdrv_group_tbl_t     *p_inten_group_table;
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;
    uint32_t                        cnv_table_num;
    uint32_t                        write_val;

    /** Input parameter confirmation process */
    ercd = impdrv_cmnctl_chk_group_data(p_commonctl, p_irq_param);

    /** Get interrupt bit pattern  */
    if (IMPDRV_EC_OK == ercd)
    {
        p_inten_cnv_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_cnv_table;
        cnv_table_num = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].cnv_table_num;

        ercd = impdrv_cmnctl_cnv_int_core_bit(
            p_irq_param->group_core_info,
            p_irq_param->group_core_num,
            p_inten_cnv_table,
            cnv_table_num,
            &write_val
        );
    }

    /** Mask INTerrupt Enable Register */
    if (IMPDRV_EC_OK == ercd)
    {
        p_inten_group_table = s_impdrv_inten_reg_tbl[p_commonctl->instance_num].p_inten_group_table;

        if (IMPDRV_IRQ_GROUP_NONE == p_irq_param->irq_group)
        {
            ercd = impdrv_cmnctl_execute_no_group(
                &g_impdrv_cmn_ctl_ins.device_io_imptop,
                p_inten_group_table,
                p_commonctl->instance_num, 
                write_val);
        }
        else
        {
            ercd = impdrv_cmnctl_execute_group(
                &g_impdrv_cmn_ctl_ins.device_io_imptop,
                p_inten_group_table,
                p_commonctl->instance_num, 
                write_val,
                p_irq_param->irq_group);
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_set_irq_group()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_save_irq_mask()
* CD_PD_02_03_0040
* [Covers: UD_PD_UD02_03_0040]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_save_irq_mask(
    st_impdrv_commonctl_t       *const p_commonctl
)
{
    /**
     * This function is used in bus interface check.
     * However, V3U is does not support of the bus interface checking.
     */

    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_NG_NOTSUPPORT;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        p_commonctl->irq_mask_value = 0U;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_save_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_force_irq_mask()
* CD_PD_02_03_0041
* [Covers: UD_PD_UD02_03_0041]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_force_irq_mask(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_core_info_t     *const p_core_info
)
{
    /**
     * This function is used in bus interface check.
     * However, V3U is does not support of the bus interface checking.
     */

    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (0U == p_commonctl->irq_mask_value)
    {
        ercd = IMPDRV_EC_NG_SEQSTATE;
    }
    else
    {
        ercd = impdrv_cmnctl_chk_core_info(*p_core_info);
    }

    if (IMPDRV_EC_OK == ercd)
    {
         ercd = IMPDRV_EC_NG_NOTSUPPORT;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_force_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_restore_irq_mask()
* CD_PD_02_03_0049
* [Covers: UD_PD_UD02_03_0049]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_restore_irq_mask(
    st_impdrv_commonctl_t       *const p_commonctl
)
{
    /**
     * This function is used in bus interface check.
     * However, V3U is does not support of the bus interface checking.
     */

    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_NG_NOTSUPPORT;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        p_commonctl->irq_mask_value = 0U;
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_restore_irq_mask()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_get_corectl_func()
* CD_PD_02_03_0010
* [Covers: UD_PD_UD02_03_0010]
***********************************************************************************************************************/
const st_impdrv_corectl_func_t* impdrv_cmnctl_get_corectl_func(
    const e_impdrv_core_type_t  core_type
)
{
    /** Private static variables */
    IMPDRV_STATIC
    const st_impdrv_corectl_func_t s_corectl_func_tbl[IMPDRV_CORE_TYPE_MAX] =
    {
        /** Not Assigned */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_IMP */
        {   impdrv_impctl_is_valid_core,    impdrv_impctl_check_state,      impdrv_impctl_set_state,
            impdrv_impctl_init_start,       impdrv_impctl_init_end,         impdrv_impctl_start,
            impdrv_impctl_stop,             impdrv_impctl_attr_init,        impdrv_impctl_set_mem_init,
            impdrv_impctl_set_core_map,     impdrv_impctl_set_cl,           impdrv_impctl_set_irq_mask,
            impdrv_impctl_execute,          impdrv_impctl_int_handler,      impdrv_impctl_resume_exe,
            impdrv_impctl_pm_set_policy,    impdrv_impctl_pm_get_policy,    NULL,
            NULL,                           impdrv_impctl_conf_reg_check,   NULL,
            NULL,                           impdrv_impctl_pm_get_state,     impdrv_impctl_quit,
            NULL,                           NULL
        },
        /* IMPDRV_CORE_TYPE_IMP_SLIM */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_OCV */
        {   impdrv_ocvctl_is_valid_core,    impdrv_ocvctl_check_state,      impdrv_ocvctl_set_state,
            impdrv_ocvctl_init_start,       impdrv_ocvctl_init_end,         impdrv_ocvctl_start,
            impdrv_ocvctl_stop,             impdrv_ocvctl_attr_init,        impdrv_ocvctl_set_mem_init,
            impdrv_ocvctl_set_core_map,     impdrv_ocvctl_set_cl,           impdrv_ocvctl_set_irq_mask,
            impdrv_ocvctl_execute,          impdrv_ocvctl_int_handler,      impdrv_ocvctl_resume_exe,
            impdrv_ocvctl_pm_set_policy,    impdrv_ocvctl_pm_get_policy,    NULL,
            NULL,                           impdrv_ocvctl_conf_reg_check,   NULL,
            NULL,                           impdrv_ocvctl_pm_get_state,     impdrv_ocvctl_quit,
            NULL,                           NULL
        },
        /** IMPDRV_CORE_TYPE_DMAC */
        {   impdrv_dmactl_is_valid_core,    impdrv_dmactl_check_state,      impdrv_dmactl_set_state,
            impdrv_dmactl_init_start,       impdrv_dmactl_init_end,         impdrv_dmactl_start,
            impdrv_dmactl_stop,             impdrv_dmactl_attr_init,        impdrv_dmactl_set_mem_init,
            impdrv_dmactl_set_core_map,     impdrv_dmactl_set_cl,           impdrv_dmactl_set_irq_mask,
            impdrv_dmactl_execute,          impdrv_dmactl_int_handler,      impdrv_dmactl_resume_exe,
            impdrv_dmactl_pm_set_policy,    impdrv_dmactl_pm_get_policy,    NULL,
            NULL,                           impdrv_dmactl_conf_reg_check,   NULL,
            NULL,                           impdrv_dmactl_pm_get_state,     impdrv_dmactl_quit,
            NULL,                           NULL
        },
        /* IMPDRV_CORE_TYPE_DMAC_SLIM */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_PSCEXE */
        {   impdrv_pscctl_is_valid_core,    impdrv_pscctl_check_state,      impdrv_pscctl_set_state,
            impdrv_pscctl_init_start,       impdrv_pscctl_init_end,         impdrv_pscctl_start,
            impdrv_pscctl_stop,             impdrv_pscctl_attr_init,        impdrv_pscctl_set_mem_init,
            impdrv_pscctl_set_core_map,     impdrv_pscctl_set_cl,           impdrv_pscctl_set_irq_mask,
            impdrv_pscctl_execute,          impdrv_pscctl_int_handler,      impdrv_pscctl_resume_exe,
            impdrv_pscctl_pm_set_policy,    impdrv_pscctl_pm_get_policy,    NULL,
            NULL,                           impdrv_pscctl_conf_reg_check,   NULL,
            NULL,                           impdrv_pscctl_pm_get_state,     impdrv_pscctl_quit,
            NULL,                           NULL
        },
        /** IMPDRV_CORE_TYPE_PSCOUT */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_CNN */
        {   impdrv_cnnctl_is_valid_core,    impdrv_cnnctl_check_state,      impdrv_cnnctl_set_state,
            impdrv_cnnctl_init_start,       impdrv_cnnctl_init_end,         impdrv_cnnctl_start,
            impdrv_cnnctl_stop,             impdrv_cnnctl_attr_init,        impdrv_cnnctl_set_mem_init,
            impdrv_cnnctl_set_core_map,     impdrv_cnnctl_set_cl,           impdrv_cnnctl_set_irq_mask,
            impdrv_cnnctl_execute,          impdrv_cnnctl_int_handler,      impdrv_cnnctl_resume_exe,
            impdrv_cnnctl_pm_set_policy,    impdrv_cnnctl_pm_get_policy,    NULL,
            NULL,                           impdrv_cnnctl_conf_reg_check,   NULL,
            NULL,                           impdrv_cnnctl_pm_get_state,     impdrv_cnnctl_quit,
            NULL,                           NULL
        },
        /** IMPDRV_CORE_TYPE_DSP */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        },
        /** IMPDRV_CORE_TYPE_DTA */
        {   NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL,   NULL,
            NULL,   NULL
        }
    };

    /** initialize local variables */
    const st_impdrv_corectl_func_t  *ret_func = NULL;

    /** Input parameter confirmation process */
    if ((IMPDRV_CORE_TYPE_IMP == core_type)
        || (IMPDRV_CORE_TYPE_IMP_SLIM == core_type)
        || (IMPDRV_CORE_TYPE_OCV == core_type)
        || (IMPDRV_CORE_TYPE_DMAC == core_type)
        || (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
        || (IMPDRV_CORE_TYPE_PSCEXE == core_type)
        || (IMPDRV_CORE_TYPE_PSCOUT == core_type)
        || (IMPDRV_CORE_TYPE_CNN == core_type)
        || (IMPDRV_CORE_TYPE_DTA == core_type)
        || (IMPDRV_CORE_TYPE_DSP == core_type))
    {
        ret_func = &s_corectl_func_tbl[core_type];
    }

    /** Return address of the target function table */
    return ret_func;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_get_corectl_func()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_get_syncc_val()
* CD_PD_02_03_0011
* [Covers: UD_PD_UD02_03_0011]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_syncc_val(
    uint8_t                     syncc_val[IMPDRV_COREMAP_MAXID],
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_core_info_t core_map[IMPDRV_COREMAP_MAXID]
)
{
    /** Private static variables */
    IMPDRV_STATIC
    const st_impdrv_syncc_table_t s_impdrv_core_to_syncc_table[IMPDRV_SYNCCTABLE_NUM] =
    {
        /*  core_type,                core_num,   syncc_val */
        {   IMPDRV_CORE_TYPE_INVALID, 0U,         0x00U   },
        {   IMPDRV_CORE_TYPE_IMP,     0U,         0x00U   },
        {   IMPDRV_CORE_TYPE_IMP,     1U,         0x01U   },
        {   IMPDRV_CORE_TYPE_IMP,     2U,         0x02U   },
        {   IMPDRV_CORE_TYPE_IMP,     3U,         0x03U   },
        {   IMPDRV_CORE_TYPE_OCV,     0U,         0x10U   },
        {   IMPDRV_CORE_TYPE_OCV,     1U,         0x11U   },
        {   IMPDRV_CORE_TYPE_OCV,     2U,         0x12U   },
        {   IMPDRV_CORE_TYPE_OCV,     3U,         0x13U   },
        {   IMPDRV_CORE_TYPE_OCV,     4U,         0x14U   },
        {   IMPDRV_CORE_TYPE_OCV,     5U,         0x15U   },
        {   IMPDRV_CORE_TYPE_OCV,     6U,         0x16U   },
        {   IMPDRV_CORE_TYPE_OCV,     7U,         0x17U   },
        {   IMPDRV_CORE_TYPE_DMAC,    0U,         0x20U   },
        {   IMPDRV_CORE_TYPE_DMAC,    1U,         0x21U   },
        {   IMPDRV_CORE_TYPE_DMAC,    2U,         0x22U   },
        {   IMPDRV_CORE_TYPE_DMAC,    3U,         0x23U   },
        {   IMPDRV_CORE_TYPE_PSCEXE,  0U,         0x30U   },
        {   IMPDRV_CORE_TYPE_PSCEXE,  1U,         0x31U   },
        {   IMPDRV_CORE_TYPE_CNN,     0U,         0x40U   },
        {   IMPDRV_CORE_TYPE_CNN,     1U,         0x41U   },
        {   IMPDRV_CORE_TYPE_CNN,     2U,         0x42U   },
        {   IMPDRV_CORE_TYPE_IMR,     0U,         0x60U   },
        {   IMPDRV_CORE_TYPE_IMR,     1U,         0x61U   },
        {   IMPDRV_CORE_TYPE_IMR,     2U,         0x62U   },
        {   IMPDRV_CORE_TYPE_IMR,     3U,         0x63U   },
        {   IMPDRV_CORE_TYPE_IMR,     4U,         0x64U   },
        {   IMPDRV_CORE_TYPE_IMR,     5U,         0x65U   },
        {   IMPDRV_CORE_TYPE_DTA,     0U,         0x70U   }
    };

    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    hit;
    uint32_t                i;
    uint32_t                j;
    st_impdrv_core_info_t   core_info[IMPDRV_INTSTS_BITNUM];
    uint32_t                use_core_num;

    for (i = 0U; i < IMPDRV_INTSTS_BITNUM; i++)
    {
        core_info[i].core_type = IMPDRV_CORE_TYPE_INVALID;
        core_info[i].core_num  = 0U;
    }

    /** Input parameter confirmation process */
    if ((NULL == syncc_val) || (NULL == p_commonctl) || (NULL == core_map))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
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
                && (IMPDRV_CORE_TYPE_IMR != core_map[i].core_type))
            {
                ercd = IMPDRV_EC_NG_PARAM;
            }
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            i++;
        }
    }

    /** Get core sync value */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_cnv_int_bit_core(s_impdrv_minten_cnv_table, IMPDRV_MINTEN_CORE_MAX,
                                                        p_commonctl->use_core_ins, core_info, &use_core_num);

        i = 0U;
        while (IMPDRV_COREMAP_MAXID > i)
        {
            hit = false;
            j = 0U;
            while (j < IMPDRV_SYNCCTABLE_NUM)
            {
                if ((s_impdrv_core_to_syncc_table[j].core_type == core_map[i].core_type) &&
                    (s_impdrv_core_to_syncc_table[j].core_num == core_map[i].core_num))
                {
                    hit = true;
                    syncc_val[i] = s_impdrv_core_to_syncc_table[j].syncc_val;
                    break;
                }
                j++;
            }
            if (false == hit)
            {
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
                break;
            }
            i++;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        i = 0U;
        while (IMPDRV_COREMAP_MAXID > i)
        {
            if ((IMPDRV_CORE_TYPE_INVALID != core_map[i].core_type)
                && (IMPDRV_CORE_TYPE_PSCOUT != core_map[i].core_type)
                && (IMPDRV_CORE_TYPE_DTA != core_map[i].core_type)
                && (IMPDRV_CORE_TYPE_IMR != core_map[i].core_type))
            {
                hit = false;
                j = 0U;
                while (j < IMPDRV_INTSTS_BITNUM)
                {
                    if ((core_info[j].core_type == core_map[i].core_type)
                        && (core_info[j].core_num == core_map[i].core_num))
                    {
                        hit = true;
                        break;
                    }
                    j++;
                }

                if (false == hit)
                {
                    ercd = IMPDRV_EC_NG_ATTRIBUTE;
                    break;
                }
            }

            i++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_get_syncc_val()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_fatal_cb()
* CD_PD_02_03_0016
* [Covers: UD_PD_UD02_03_0016]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_fatal_cb(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_core_info_t     *const p_core_info,
    const e_impdrv_fatalcode_t      fatal_code,
    const e_impdrv_errorcode_t      error_code
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_core_info))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if (NULL == p_commonctl->cbfunc_fatal)
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    if (IMPDRV_EC_OK == ercd)
    {
        p_commonctl->cbfunc_fatal(p_core_info, fatal_code, error_code, p_commonctl->p_cb_args_fatal);
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_fatal_cb()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_chk_instance_num()
* CD_PD_02_03_0013
* [Covers: UD_PD_UD02_03_0013]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_chk_instance_num(
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
* End of function impdrv_cmnctl_chk_instance_num()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_get_io_imptop()
* CD_PD_02_03_0020
* [Covers: UD_PD_UD02_03_0020]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_io_imptop(
    st_impdrv_device_handle_t       *const p_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Store the handle data */
        *p_handle = g_impdrv_cmn_ctl_ins.device_io_imptop;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_get_io_imptop()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_get_io_dta()
* CD_PD_02_03_0021
* [Covers: UD_PD_UD02_03_0021]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_get_io_dta(
    st_impdrv_device_handle_t       *const p_handle
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_handle)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Store the handle data */
        *p_handle = g_impdrv_cmn_ctl_ins.device_io_dta;
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_get_io_dta()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_conf_reg_check()
* CD_PD_02_03_0042
* [Covers: UD_PD_UD02_03_0042]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_conf_reg_check(
    const st_impdrv_commonctl_t     *const p_commonctl
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_commonctl)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /**
     * TODO: Under development...
     * For V3U, the Check specifications of distributor register is have not been decided.
     */

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_conf_reg_check()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_calc_checksum()
* CD_PD_02_03_0044
* [Covers: UD_PD_UD02_03_0044]
***********************************************************************************************************************/
e_impdrv_errorcode_t impdrv_cmnctl_calc_checksum(
    const void*         *const p_data,
    const size_t        size,
    uint32_t            *const p_checksum
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    const uint32_t                  *p_pointer;
    size_t                          count;

    /** Input parameter confirmation process */
    if ((NULL == p_data) || (NULL == p_checksum))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        /** Calculate checksum */
        p_pointer = (uint32_t*)p_data;
        *p_checksum = 0U;
        for (count = size / sizeof(uint32_t); 0U != count; count--)
        {
            *p_checksum ^= *p_pointer;
            p_pointer++;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_calc_checksum()
***********************************************************************************************************************/

/*======================================================================================================================
Private function definitions
======================================================================================================================*/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_chk_core_info()
* CD_PD_02_03_0012
* [Covers: UD_PD_UD02_03_0012]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_chk_core_info(
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
        && (IMPDRV_CORE_TYPE_CNN != core_info.core_type))
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
* End of function impdrv_cmnctl_chk_core_info()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_chk_init_data()
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_chk_init_data(
    const st_impdrv_commonctl_t     *const p_commonctl,
    const st_impdrv_cmn_init_data_t *const p_cmn_init_data,
    const bool                      *const p_is_dmac_mb_init,
    uint32_t                        *const p_use_core_info
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd;
    uint32_t                        i;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_cmn_init_data) || (NULL == p_is_dmac_mb_init)
        || (NULL == p_cmn_init_data->osal_cb_args) || (NULL == p_cmn_init_data->cbfunc_fatal))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else
    {
        ercd = impdrv_cmnctl_chk_instance_num(p_cmn_init_data->instance_num);
    }

    /** Checking for errors between instances */
    if (IMPDRV_EC_OK == ercd)
    {
        if ((NULL != g_impdrv_cmn_ctl_ins.p_commonctl[p_cmn_init_data->instance_num]) ||
                (p_cmn_init_data->mutex_id != g_impdrv_cmn_ctl_ins.mutex_id) ||
                (p_cmn_init_data->time_period != g_impdrv_cmn_ctl_ins.mutex_time_period))
        {
            ercd = IMPDRV_EC_NG_INSTANCE;
        }
    }

    /** Checking for errors between instances core */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_cnv_int_core_bit(
            p_cmn_init_data->p_core_info,
            p_cmn_init_data->use_core_num,
            s_impdrv_minten_cnv_table,
            IMPDRV_MINTEN_CORE_MAX,
            p_use_core_info
        );
    }

    if (IMPDRV_EC_OK == ercd)
    {
        for (i = 0U; (IMPDRV_INSTANCETABLE_NUM - 1U) > i; i++)
        {
            if ((NULL != g_impdrv_cmn_ctl_ins.p_commonctl[s_impdrv_cnv_ins_table[i]]) && 
                (0U != (*p_use_core_info & g_impdrv_cmn_ctl_ins.p_commonctl[s_impdrv_cnv_ins_table[i]]->use_core_ins)))
            {
                ercd = IMPDRV_EC_NG_INSTANCE;
            }
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_chk_init_data()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_chk_group_data()
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_chk_group_data(
    const st_impdrv_commonctl_t *const p_commonctl,
    const st_impdrv_irq_group_t *const p_irq_param
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    uint32_t                        i;
    e_impdrv_irq_group_t            irq_group;

    /** Input parameter confirmation process */
    if ((NULL == p_commonctl) || (NULL == p_irq_param))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_IRQ_GROUP_NONE != p_irq_param->irq_group) && (IMPDRV_IRQ_GROUP_0 != p_irq_param->irq_group)
        && (IMPDRV_IRQ_GROUP_1 != p_irq_param->irq_group) && (IMPDRV_IRQ_GROUP_2 != p_irq_param->irq_group))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((0U == p_irq_param->group_core_num) || (IMPDRV_IRQGROUP_MAXID < p_irq_param->group_core_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        i = 0U;
        while (i < p_irq_param->group_core_num)
        {
            ercd = impdrv_cmnctl_chk_core_info(p_irq_param->group_core_info[i]);
            if (IMPDRV_EC_OK != ercd)
            {
                break;
            }
            i++;
        }
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_chk_instance_num(p_commonctl->instance_num);
    }

    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_IRQ_GROUP_NONE != p_irq_param->irq_group))
    {
        ercd = impdrv_cmnctl_get_group(&irq_group, p_commonctl->instance_num);
        if ((IMPDRV_EC_OK == ercd) && (irq_group != p_irq_param->irq_group))
        {
            ercd = IMPDRV_EC_NG_NOTSUPPORT;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_chk_group_data()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_cnv_int_core_bit()
* CD_PD_02_03_0014
* [Covers: UD_PD_UD02_03_0014]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_core_bit(
    const st_impdrv_core_info_t     *const p_core_info,
    const uint32_t                  use_core_num,
    const st_impdrv_inten_tbl_t     *const p_inten_cnv_table,
    const uint32_t                  cnv_table_num,
    uint32_t                        *const p_val
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    bool                    hit;
    uint32_t                i;
    uint32_t                j;

    /** Input parameter confirmation process */
    if ((NULL == p_core_info) || (NULL == p_inten_cnv_table) || (NULL == p_val))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((0U == use_core_num) || (0U == cnv_table_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else if ((IMPDRV_INNER_FIXED_VALUE < use_core_num) || (IMPDRV_INNER_FIXED_VALUE < cnv_table_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Nothing to do. */
    }

    /** Identify the bits of the corresponding registers from the core type and core number */
    if (IMPDRV_EC_OK == ercd)
    {
        *p_val = 0U;
        i = 0U;
        while (i < use_core_num)
        {
            hit = false;
            ercd = impdrv_cmnctl_chk_core_info(p_core_info[i]);
            if (IMPDRV_EC_OK == ercd)
            {
                j = 0U;
                while (j < cnv_table_num)
                {
                    if ((p_core_info[i].core_type == p_inten_cnv_table[j].core_type) &&
                            (p_core_info[i].core_num == p_inten_cnv_table[j].core_num))
                    {
                        *p_val |= p_inten_cnv_table[j].bit;
                        hit = true;
                        break;
                    }
                    j++;
                }
            }
            if (false == hit)
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
* End of function impdrv_cmnctl_cnv_int_core_bit()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_cnv_int_bit_core()
* CD_PD_02_03_0017
* [Covers: UD_PD_UD02_03_0017]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_cnv_int_bit_core(
    const st_impdrv_inten_tbl_t     *const p_s_inten_cnv_table,
    const uint32_t                  cnv_table_num,
    const uint32_t                  stat,
    st_impdrv_core_info_t           core_info[IMPDRV_INTSTS_BITNUM],
    uint32_t                        *const p_use_core_num
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                i;

    /** Input parameter confirmation process */
    if ((NULL == p_s_inten_cnv_table) || (NULL == core_info) || (NULL == p_use_core_num))
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }
    else if ((IMPDRV_INTEN0_CORE_MAX != cnv_table_num)
        && (IMPDRV_INTEN1_CORE_MAX != cnv_table_num)
        && (IMPDRV_MINTEN_CORE_MAX != cnv_table_num))
    {
        ercd = IMPDRV_EC_NG_PARAM;
    }
    else
    {
        /** Convert core info */
        *p_use_core_num = 0U;
        for (i = 0U; i < cnv_table_num; i++)
        {
            if (0U != (stat & p_s_inten_cnv_table[i].bit))
            {
                core_info[*p_use_core_num].core_type = p_s_inten_cnv_table[i].core_type;
                core_info[*p_use_core_num].core_num = p_s_inten_cnv_table[i].core_num;
                *p_use_core_num += 1U;
            }
        }
    }
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_cnv_int_bit_core()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_execute_no_group()
* CD_PD_02_03_0018
* [Covers: UD_PD_UD02_03_0018]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_execute_no_group(
    const st_impdrv_device_handle_t *const p_device_handle,
    const st_impdrv_group_tbl_t     *const p_inten_group_table,
    const e_impdrv_instance_t       instance_num,
    const uint32_t                  write_val
)
{
    e_impdrv_irq_group_t    irq_group;
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    e_impdrv_errorcode_t    ercd_group = IMPDRV_EC_OK;
    uint32_t                read_inten = 0U;
    uint32_t                inten = 0U;
    uint32_t                read_intsel = 0U;
    uint32_t                intsel = 0U;

    if ((NULL == p_device_handle) || (NULL == p_inten_group_table))
    {
        /** Error Handle */
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd_group = impdrv_cmnctl_get_group(&irq_group, instance_num);
        if (IMPDRV_EC_NG_NOTSUPPORT != ercd_group)
        {
            ercd = ercd_group;
        }
    }

    /** Switching to No interrupt group */
    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_EC_OK == ercd_group))
    {
        ercd = impdrv_osdep_read_reg(p_device_handle, s_impdrv_inten_addr[(uint32_t)instance_num], &read_inten);
        inten = read_inten;
    }

    if ((IMPDRV_EC_OK == ercd) && (IMPDRV_EC_OK == ercd_group))
    {
        /** Care for group interrupt bit in INTerrupt Enable register */
        ercd = impdrv_osdep_read_reg(p_device_handle, p_inten_group_table[(uint32_t)irq_group].group_reg, &read_intsel);
        if (IMPDRV_EC_OK == ercd)
        {
            intsel = read_intsel & ~(write_val);
            if (IMPDRV_REG_INTEN_INIT_VAL == intsel)
            {
                inten &= ~(p_inten_group_table[(uint32_t)irq_group].group_bit);
            }

            if (intsel != read_intsel)
            {
                ercd = impdrv_osdep_write_reg(p_device_handle, p_inten_group_table[(uint32_t)irq_group].group_reg,
                                                                                                intsel , true, intsel);
            }
        }

        /** Set the INTerrupt Enable register */
        inten = inten | write_val;
        if ((IMPDRV_EC_OK == ercd) && (inten != read_inten))
        {
            ercd = impdrv_osdep_write_reg(p_device_handle, s_impdrv_inten_addr[(uint32_t)instance_num],
                                                                                        inten, true, inten);
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_execute_no_group()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_execute_group()
* CD_PD_02_03_0019
* [Covers: UD_PD_UD02_03_0019]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_execute_group(
    const st_impdrv_device_handle_t *const p_device_handle,
    const st_impdrv_group_tbl_t     *const p_inten_group_table,
    const e_impdrv_instance_t       instance_num,
    const uint32_t                  group_reg_write_val,
    const e_impdrv_irq_group_t      irq_group
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;
    uint32_t                read_inten = 0U;
    uint32_t                inten = 0U;
    uint32_t                read_intsel = 0U;
    uint32_t                intsel = 0U;

    if ((NULL == p_device_handle) || (NULL == p_inten_group_table))
    {
        /** Error Handle */
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_cmnctl_chk_instance_num(instance_num);
    }

    /** Switching to interrupt group */
    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(p_device_handle, s_impdrv_inten_addr[(uint32_t)instance_num], &read_inten);
        inten = read_inten;
    }

    if (IMPDRV_EC_OK == ercd)
    {
        ercd = impdrv_osdep_read_reg(p_device_handle, p_inten_group_table[(uint32_t)irq_group].group_reg, &read_intsel);
        if (IMPDRV_EC_OK == ercd)
        {
            /** Care for group interrupt bit in INTerrupt Enable register */
            intsel = read_intsel | group_reg_write_val;
            inten |= p_inten_group_table[(uint32_t)irq_group].group_bit; 
            if (intsel != read_intsel)
            {
                ercd = impdrv_osdep_write_reg(p_device_handle, p_inten_group_table[(uint32_t)irq_group].group_reg,
                                                                                                intsel , true, intsel);
            }
        }

        /** Set the INTerrupt Enable register */
        inten = inten & ~(group_reg_write_val);
        if ((IMPDRV_EC_OK == ercd) && (inten != read_inten))
        {
            ercd = impdrv_osdep_write_reg(p_device_handle, s_impdrv_inten_addr[(uint32_t)instance_num],
                                                                                        inten, true, inten);
        }
   }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_execute_group()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_get_group()
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_get_group(
    e_impdrv_irq_group_t            *const p_irq_group,
    const e_impdrv_instance_t       instance_num
)
{
    e_impdrv_errorcode_t    ercd = IMPDRV_EC_OK;

    /** Input parameter confirmation process */
    if (NULL == p_irq_group)
    {
        ercd = IMPDRV_EC_NG_ARGNULL;
    }

    /** Set group number */
    if (IMPDRV_EC_OK == ercd)
    {
        switch (instance_num)
        {
            case IMPDRV_INSTANCE_0:
            case IMPDRV_INSTANCE_3:
            case IMPDRV_INSTANCE_6:
                *p_irq_group = IMPDRV_IRQ_GROUP_0;
                break;
            case IMPDRV_INSTANCE_1:
            case IMPDRV_INSTANCE_4:
                *p_irq_group = IMPDRV_IRQ_GROUP_1;
                break;
            case IMPDRV_INSTANCE_2:
            case IMPDRV_INSTANCE_5:
                *p_irq_group = IMPDRV_IRQ_GROUP_2;
                break;
            default:
                /** Error Handle */
                ercd = IMPDRV_EC_NG_PARAM;
                break;
        }
    }

    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_get_group()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_set_checksum()
* CD_PD_02_03_0047
* [Covers: UD_PD_UD02_03_0047]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_set_checksum(
    void
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;

    /** Checksum value setting for Memory broken check */
    ercd = impdrv_cmnctl_calc_checksum(
        (void*)&g_impdrv_cmn_ctl_ins,
        sizeof(g_impdrv_cmn_ctl_ins),
        &g_impdrv_cmn_ctl_chk.checksum
    );

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_set_checksum()
***********************************************************************************************************************/

/***********************************************************************************************************************
* Start of function impdrv_cmnctl_test_checksum()
* CD_PD_02_03_0048
* [Covers: UD_PD_UD02_03_0048]
***********************************************************************************************************************/
IMPDRV_STATIC
e_impdrv_errorcode_t impdrv_cmnctl_test_checksum(
    void
)
{
    /** initialize local variables */
    e_impdrv_errorcode_t            ercd = IMPDRV_EC_OK;
    uint32_t                        current_checksum;

    /** Checksum compare test of Memory broken check. */
    ercd = impdrv_cmnctl_calc_checksum(
        (void*)&g_impdrv_cmn_ctl_ins,
        sizeof(g_impdrv_cmn_ctl_ins),
        &current_checksum
    );
    if (IMPDRV_EC_OK == ercd)
    {
        if (current_checksum != g_impdrv_cmn_ctl_chk.checksum)
        {
            ercd = IMPDRV_EC_NG_SYSTEMERROR;
        }
    }

    /** Set error code in return value */
    return ercd;
}
/***********************************************************************************************************************
* End of function impdrv_cmnctl_test_checksum()
***********************************************************************************************************************/

/*======================================================================================================================
End of File
======================================================================================================================*/

