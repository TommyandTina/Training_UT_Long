#include "r_impdrv.h"
#include "r_impdrv_private.h"

#define IMP_CORE_MAX        (4)
#define OCV_CORE_MAX        (8)
#define DMA_CORE_MAX        (4)
#define PSCEXE_CORE_MAX     (2)
#define CNN_CORE_MAX        (3)
#define SOC_CORE_NUM_MAX    (IMP_CORE_MAX + OCV_CORE_MAX + DMA_CORE_MAX + PSCEXE_CORE_MAX + CNN_CORE_MAX)
#define INVALID_CORE_NUM    (6)
#define SOC_COREMAP_MAX     (16)
#define NOT_SUPPORTED_EXEC_CORE  (4)
#define READ_NUM_FOR_INIT        (29)
#define READ_NUM_FOR_START       (60)
#define TOTAL_IRQ_MASK_CORES     (9)

/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_VCR
 * Offset address of VCR register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_VCR          (0x0004U)           /**< Offset address of VCR register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_VCR0
 * Offset address of VCR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_VCR0         (0x0000U)           /**< Offset address of VCR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_VCR
 * Offset address of VCR register for DMA.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_VCR          (0x0000U)           /**< Offset address of VCR register for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_PSC_VCR
 * Offset address of VCR register for PSC.
***********************************************************************************************************************/
#define IMPDRV_REG_PSC_VCR          (0x0000U)           /**< Offset address of VCR register for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_CNN_VCR
 * Offset address of VCR register for CNN.
***********************************************************************************************************************/
#define IMPDRV_REG_CNN_VCR          (0x0000U)           /**< Offset address of VCR register for CNN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_DMA_THREAD_OFFSET
 * H/W Register offset for DMA sub-thread.
***********************************************************************************************************************/
#define IMPDRV_REG_DMA_THREAD_OFFSET    (0x00001000U)       /**< H/W Register offset for DMA sub-thread. */
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
 * @def IMPDRV_REG_MINTEN
 * Offset address of hardware register for MINTEN.
***********************************************************************************************************************/
#define IMPDRV_REG_MINTEN           (0x00B0U)           /**< Offset address of hardware register for MINTEN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DUMMY_UINT32
 * Uint32_t dummy value.
***********************************************************************************************************************/
#define IMPDRV_DUMMY_UINT32         (0xFFFFFFFFU)       /**< Uint32_t dummy value. */
/*******************************************************************************************************************//**
 * @def IMPDRV_INSTANCETABLE_NUM
 * Maximum number of Instance number table.
***********************************************************************************************************************/
#define IMPDRV_INSTANCETABLE_NUM    (8U)                /**< Maximum number of Instance number table. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_VCR
 * Hardware version value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_VCR              (0x00700000U)       /**< Hardware version value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_VCR
 * Hardware version value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_VCR              (0x00000005U)       /**< Hardware version value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_DMA_VCR
 * Hardware version value for DMA.
***********************************************************************************************************************/
#define IMPDRV_DMA_VCR                  (0x18071916U)       /**< Hardware version value for DMA. */
/*******************************************************************************************************************//**
 * @def IMPDRV_PSC_VCR
 * Hardware version value for PSC.
***********************************************************************************************************************/
#define IMPDRV_PSC_VCR              (0x18082420U)       /**< Hardware version value for PSC. */
/*******************************************************************************************************************//**
 * @def IMPDRV_CNN_VCR
 * Hardware version value for CNN.
***********************************************************************************************************************/
#define IMPDRV_CNN_VCR              (0x00030200U)       /**< Hardware version value for CNN. */
/*******************************************************************************************************************//**
 * @def IMPDRV_OCV_RESET_STATUS_VAL
 * RESET_STATUS value for OCV.
***********************************************************************************************************************/
#define IMPDRV_OCV_RESET_STATUS_VAL (0x00F10000U)          /**< RESET_STATUS value for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCF
 * Offset address of CNF register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_CNF          (0x0008U)           /**< Offset address of CNF register for IMP. */
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
 * @def IMPDRV_REG_INTEN_INIT_VAL
 * Initial value of interrupt enable register.
***********************************************************************************************************************/
#define IMPDRV_REG_INTEN_INIT_VAL   (0x00000000U)       /**< Initial value of interrupt enable register. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_INTSTS
 * Offset address of INTSTS register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_INTSTS       (0x0010U)           /**< Offset address of INTSTS register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_INTEN
 * Offset address of INTEN register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_INTEN        (0x0014U)           /**< Offset address of INTEN register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_OCV_MEMINITR
 * Offset address of MEMINITR register for OCV.
***********************************************************************************************************************/
#define IMPDRV_REG_OCV_MEMINITR     (0x00F8U)           /**< Offset address of MEMINITR register for OCV. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_MEM_FUNC_SA
 * Offset address of SA register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_MEM_FUNC_SA  (0x0B78U)           /**< Offset address of SA register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCF
 * Offset address of CNF register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_CNF          (0x0008U)           /**< Offset address of CNF register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCFG
 * Offset address of CNFG register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IFCFG        (0x0018U)           /**< Offset address of CNFG register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_IFCTL
 * Offset address of CIFCTL register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_IFCTL        (0x001CU)           /**< Offset address of IFCTL register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR0
 * Offset address of SYNCCR0 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR0      (0x0040U)           /**< Offset address of SYNCCR0 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR1
 * Offset address of SYNCCR1 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR1      (0x0044U)           /**< Offset address of SYNCCR1 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR2
 * Offset address of SYNCCR2 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR2      (0x0048U)           /**< Offset address of SYNCCR2 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_REG_IMP_SYNCCR03
 * Offset address of SYNCCR3 register for IMP.
***********************************************************************************************************************/
#define IMPDRV_REG_IMP_SYNCCR3      (0x004CU)           /**< Offset address of SYNCCR03 register for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_IMP_IFCTL_EXE
 * EXE register value for IMP.
***********************************************************************************************************************/
#define IMPDRV_IMP_IFCTL_EXE        (0x00000001U)       /**< EXE register value for IMP. */
/*******************************************************************************************************************//**
 * @def IMPDRV_HWRSC_NAME_MAX
 * Maximum length of hardware resource name.
***********************************************************************************************************************/
#define IMPDRV_HWRSC_NAME_MAX           (32U)       /** Maximum length of hardware resource name    */

/* define IMP instance offset */
#define IT_IMP_REG_INSTANCE0    IMPDRV_REG_INTEN00
#define IT_IMP_REG_INSTANCE1    IMPDRV_REG_INTEN01
#define IT_IMP_REG_INSTANCE2    IMPDRV_REG_INTEN02
#define IT_IMP_REG_INSTANCE3    IMPDRV_REG_INTEN10
#define IT_IMP_REG_INSTANCE4    IMPDRV_REG_INTEN11
#define IT_IMP_REG_INSTANCE5    IMPDRV_REG_INTEN12
#define IT_IMP_REG_INSTANCE6    IMPDRV_REG_MINTEN

typedef struct {
    uint32_t    type;
    bool        value;
} irq_mask_t;

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
 * @struct st_impdrv_inten_reg_table_t
 * Definition for conversion of register table
***********************************************************************************************************************/
typedef struct
{
    const st_impdrv_group_tbl_t     *p_inten_group_table;   /**< Convert Int group register table */
    const st_impdrv_inten_tbl_t     *p_inten_cnv_table;     /**< Convert Int enable register table */
    uint32_t                        cnv_table_num;          /**< Number of convert Int enable register table */
} st_impdrv_inten_reg_tbl_t;

extern const st_impdrv_core_info_t platform_soc[SOC_CORE_NUM_MAX];
extern const st_impdrv_core_info_t invalid_core[INVALID_CORE_NUM];
extern const st_impdrv_core_info_t soc_coremap[SOC_COREMAP_MAX];
extern const e_impdrv_core_type_t not_supported_exec_core[NOT_SUPPORTED_EXEC_CORE];
extern const uint32_t read_value_for_init[READ_NUM_FOR_INIT];
extern const uint32_t read_value_for_start[READ_NUM_FOR_START];
extern const uint32_t s_impdrv_inten_addr[IMPDRV_INSTANCETABLE_NUM];
extern const irq_mask_t irq_mask_by_attr_init_validator[TOTAL_IRQ_MASK_CORES][5];
extern const st_impdrv_irq_group_t g_p_irq_param[5];
extern const st_impdrv_inten_reg_tbl_t s_impdrv_inten_reg_tbl[IMPDRV_INSTANCETABLE_NUM];

int8_t get_correct_hw_info(
	const st_impdrv_hwrsc_def_t	*const p_hwrsc_def_table,
	const char					*const p_device_id,
	st_impdrv_reg_info_t		*const p_reg_info
);

e_impdrv_errorcode_t util_get_group(
    e_impdrv_irq_group_t            *const p_irq_group,
    const e_impdrv_instance_t       instance_num
);