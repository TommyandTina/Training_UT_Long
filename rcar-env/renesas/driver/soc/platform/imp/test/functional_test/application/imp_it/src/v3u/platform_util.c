#include "soc_util.h"

const st_impdrv_core_info_t platform_soc[SOC_CORE_NUM_MAX] = {
	{ IMPDRV_CORE_TYPE_IMP,		0 },
	{ IMPDRV_CORE_TYPE_IMP,		1 },
	{ IMPDRV_CORE_TYPE_IMP,		2 },
	{ IMPDRV_CORE_TYPE_IMP,		3 },
	{ IMPDRV_CORE_TYPE_OCV,		0 },
	{ IMPDRV_CORE_TYPE_OCV,		1 },
	{ IMPDRV_CORE_TYPE_OCV,		2 },
	{ IMPDRV_CORE_TYPE_OCV,		3 },
	{ IMPDRV_CORE_TYPE_OCV,		4 },
	{ IMPDRV_CORE_TYPE_OCV,		5 },
    { IMPDRV_CORE_TYPE_OCV,		6 },
    { IMPDRV_CORE_TYPE_OCV,		7 },
	{ IMPDRV_CORE_TYPE_DMAC,	0 },
	{ IMPDRV_CORE_TYPE_DMAC,	1 },
	{ IMPDRV_CORE_TYPE_DMAC,	2 },
    { IMPDRV_CORE_TYPE_DMAC,	3 },
	{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
	{ IMPDRV_CORE_TYPE_PSCEXE,	1 },
	{ IMPDRV_CORE_TYPE_CNN,		0 },
	{ IMPDRV_CORE_TYPE_CNN,		1 },
	{ IMPDRV_CORE_TYPE_CNN,		2 }
};

const st_impdrv_core_info_t invalid_core[INVALID_CORE_NUM] = {
	{ IMPDRV_CORE_TYPE_INVALID,		0				},
	{ IMPDRV_CORE_TYPE_IMP,			IMP_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_OCV,			OCV_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_DMAC,		DMA_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_PSCEXE,		PSCEXE_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_CNN,			CNN_CORE_MAX	}
};

const st_impdrv_core_info_t soc_coremap[SOC_COREMAP_MAX] = {
	{ IMPDRV_CORE_TYPE_IMP,		0 },
	{ IMPDRV_CORE_TYPE_IMP,		1 },
	{ IMPDRV_CORE_TYPE_IMP,		2 },
	{ IMPDRV_CORE_TYPE_IMP,		3 },
	{ IMPDRV_CORE_TYPE_OCV,		0 },
	{ IMPDRV_CORE_TYPE_OCV,		1 },
	{ IMPDRV_CORE_TYPE_OCV,		2 },
	{ IMPDRV_CORE_TYPE_OCV,		3 },
	{ IMPDRV_CORE_TYPE_OCV,		4 },
	{ IMPDRV_CORE_TYPE_OCV,		5 },
	{ IMPDRV_CORE_TYPE_DMAC,	0 },
	{ IMPDRV_CORE_TYPE_DMAC,	1 },
	{ IMPDRV_CORE_TYPE_CNN,		0 },
	{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
	{ IMPDRV_CORE_TYPE_DTA,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,	0 }
};

const e_impdrv_core_type_t not_supported_exec_core[NOT_SUPPORTED_EXEC_CORE] = {
	IMPDRV_CORE_TYPE_PSCOUT,
	IMPDRV_CORE_TYPE_LDMAC,
	IMPDRV_CORE_TYPE_DTA,
	IMPDRV_CORE_TYPE_IMR
};

const uint32_t read_value_for_init[READ_NUM_FOR_INIT] = {	/* TODO: Need to check again with the right SoC */
	0,
	0,
	0,
	0x2U,
	0x2U,
	0x2U,
	0,
	0,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_PSC_VCR,
	IMPDRV_PSC_VCR,
	IMPDRV_CNN_VCR,
	IMPDRV_CNN_VCR,
	IMPDRV_CNN_VCR
};

const uint32_t read_value_for_start[READ_NUM_FOR_START] = {
	[20] = IMPDRV_OCV_RESET_STATUS_VAL,
	// [23] = IMPDRV_OCV_RESET_LOCAL_STATUS_VAL,
	[26] = IMPDRV_OCV_RESET_STATUS_VAL,
	// [29] = IMPDRV_OCV_RESET_LOCAL_STATUS_VAL,
	[32] = IMPDRV_OCV_RESET_STATUS_VAL,
	// [35] = IMPDRV_OCV_RESET_LOCAL_STATUS_VAL,
	[38] = IMPDRV_OCV_RESET_STATUS_VAL,
	// [41] = IMPDRV_OCV_RESET_LOCAL_STATUS_VAL,
	[44] = IMPDRV_OCV_RESET_STATUS_VAL,
	// [47] = IMPDRV_OCV_RESET_LOCAL_STATUS_VAL
};

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

const irq_mask_t irq_mask_by_attr_init_validator[TOTAL_IRQ_MASK_CORES][5] = {
	[IMPDRV_CORE_TYPE_INVALID]		= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false }
									  },
	[IMPDRV_CORE_TYPE_IMP]			= {
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_IRQMASK_HPINT,		.value = true	},
										{ .type = IMPDRV_IRQMASK_APIPINT,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_IMP_SLIM]		= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false }
									  },
	[IMPDRV_CORE_TYPE_OCV]			= {
										{ .type = IMPDRV_IRQMASK_USIER,		.value = true },
										{ .type = IMPDRV_IRQMASK_USINT,		.value = true },
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true },
										{ .type = IMPDRV_IRQMASK_PBCOVF,	.value = true },
										{ .type = IMPDRV_IRQMASK_SBO0ME,	.value = true }
									  },
	[IMPDRV_CORE_TYPE_DMAC]			= {
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_DMAC_SLIM]	= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false }
									  },
	[IMPDRV_CORE_TYPE_PSCEXE]		= {
										{ .type = IMPDRV_IRQMASK_END,		.value = true	},
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_PSCOUT]		= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false },
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false }
									  },
	[IMPDRV_CORE_TYPE_CNN]			= {
										{ .type = IMPDRV_IRQMASK_MSCO,		.value = true	},
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  }
};

const st_impdrv_irq_group_t g_p_irq_param[5] = {
	[IMPDRV_IRQ_GROUP_INVALID]	= {
									.irq_group = IMPDRV_IRQ_GROUP_INVALID
								  },
	[IMPDRV_IRQ_GROUP_NONE]		= {
									.irq_group = IMPDRV_IRQ_GROUP_NONE,
									.group_core_num = SOC_CORE_NUM_MAX,
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_OCV,		2 },
										{ IMPDRV_CORE_TYPE_OCV,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		4 },
										{ IMPDRV_CORE_TYPE_OCV,		5 },
										{ IMPDRV_CORE_TYPE_OCV,		6 },
										{ IMPDRV_CORE_TYPE_OCV,		7 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_DMAC,	2 },
										{ IMPDRV_CORE_TYPE_DMAC,	3 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	1 },
										{ IMPDRV_CORE_TYPE_CNN,		0 },
										{ IMPDRV_CORE_TYPE_CNN,		1 },
										{ IMPDRV_CORE_TYPE_CNN,		2 }
									}
								  },
	[IMPDRV_IRQ_GROUP_0]		= {
									.irq_group = IMPDRV_IRQ_GROUP_0,
									.group_core_num = 11,
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_OCV,		4 },
										{ IMPDRV_CORE_TYPE_OCV,		6 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_CNN,		0 },
										{ IMPDRV_CORE_TYPE_CNN,		2 }
									}
								  },
	[IMPDRV_IRQ_GROUP_1]		= {
									.irq_group = IMPDRV_IRQ_GROUP_1,
									.group_core_num = 10,
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		2 },
										{ IMPDRV_CORE_TYPE_OCV,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		5 },
										{ IMPDRV_CORE_TYPE_OCV,		7 },
										{ IMPDRV_CORE_TYPE_DMAC,	2 },
										{ IMPDRV_CORE_TYPE_DMAC,	3 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	1 },
										{ IMPDRV_CORE_TYPE_CNN,		1 }
									}
								  },
	[IMPDRV_IRQ_GROUP_2]		= {
									.irq_group = IMPDRV_IRQ_GROUP_2,
									.group_core_num = SOC_CORE_NUM_MAX,
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_OCV,		2 },
										{ IMPDRV_CORE_TYPE_OCV,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		4 },
										{ IMPDRV_CORE_TYPE_OCV,		5 },
										{ IMPDRV_CORE_TYPE_OCV,		6 },
										{ IMPDRV_CORE_TYPE_OCV,		7 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_DMAC,	2 },
										{ IMPDRV_CORE_TYPE_DMAC,	3 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	1 },
										{ IMPDRV_CORE_TYPE_CNN,		0 },
										{ IMPDRV_CORE_TYPE_CNN,		1 },
										{ IMPDRV_CORE_TYPE_CNN,		2 }
									}
								  }
};

static const st_impdrv_group_tbl_t s_impdrv_inten0_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   0x00000800U,            IMPDRV_REG_G00INTSEL    },  /** Group00. */
    {   0x00001000U,            IMPDRV_REG_G01INTSEL    },  /** Group01. */
    {   0x00002000U,            IMPDRV_REG_G02INTSEL    }   /** Group02. */
};

static const st_impdrv_group_tbl_t s_impdrv_inten1_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   0x00000800U,            IMPDRV_REG_G10INTSEL    },  /** Group10. */
    {   0x00001000U,            IMPDRV_REG_G11INTSEL    },  /** Group11. */
    {   0x00002000U,            IMPDRV_REG_G12INTSEL    }   /** Group12. */
};

static const st_impdrv_group_tbl_t s_impdrv_minten_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32     },  /**< Dummy for Index */
    {   0x00400000U,            IMPDRV_REG_MG0INTSEL    },  /** Group20. */
    {   0x00800000U,            IMPDRV_REG_MG1INTSEL    },  /** Group21. */
    {   0x01000000U,            IMPDRV_REG_MG2INTSEL    }   /** Group22. */
};

static const st_impdrv_inten_tbl_t s_impdrv_inten0_cnv_table[IMPDRV_INTEN0_CORE_MAX] =
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

static const st_impdrv_inten_tbl_t s_impdrv_inten1_cnv_table[IMPDRV_INTEN1_CORE_MAX] =
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

static const st_impdrv_inten_tbl_t s_impdrv_minten_cnv_table[IMPDRV_MINTEN_CORE_MAX] =
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

e_impdrv_errorcode_t util_get_group(
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

    /** Set group nunber */
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

int8_t get_correct_hw_info(
	const st_impdrv_hwrsc_def_t	*const p_hwrsc_def_table,
	const char					*const p_device_id,
	st_impdrv_reg_info_t		*const p_reg_info
)
{
	int8_t ret = -1;

	for (uint32_t i = 0; i < IMPDRV_HWRSC_TABLE_NUM; i++)
	{
		if (NULL != p_hwrsc_def_table[i].name)
		{
			if (!strcmp(p_device_id, p_hwrsc_def_table[i].name))
			{
				p_reg_info->addr_phys	= p_hwrsc_def_table[i].addr_phys;
				p_reg_info->size		= p_hwrsc_def_table[i].size;
				ret = 0;
				break;
			}
		}
	}

	return ret;
}