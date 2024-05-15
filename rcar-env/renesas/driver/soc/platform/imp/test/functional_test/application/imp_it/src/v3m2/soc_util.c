#include "soc_common.h"

bool close_device_io_flg = false;

static const st_impdrv_core_info_t platform_soc[] = {
	{ IMPDRV_CORE_TYPE_IMP,		0 },
	{ IMPDRV_CORE_TYPE_IMP,		1 },
	{ IMPDRV_CORE_TYPE_IMP,		2 },
	{ IMPDRV_CORE_TYPE_IMP,		3 },
	{ IMPDRV_CORE_TYPE_OCV,		0 },
	{ IMPDRV_CORE_TYPE_OCV,		1 },
	{ IMPDRV_CORE_TYPE_DMAC,	0 },
	{ IMPDRV_CORE_TYPE_DMAC,	1 },
	{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
	{ IMPDRV_CORE_TYPE_CNN,		0 }
};

static const st_impdrv_core_info_t invalid_core[] = {
	{ IMPDRV_CORE_TYPE_INVALID,	0				},
	{ IMPDRV_CORE_TYPE_IMP,		IMP_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_OCV,		OCV_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_DMAC,	DMA_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_PSCEXE,	PSCEXE_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_CNN,		CNN_CORE_MAX	}
};

static const st_impdrv_core_info_t soc_coremap[] = {
	{ IMPDRV_CORE_TYPE_IMP,		0 },
	{ IMPDRV_CORE_TYPE_IMP,		1 },
	{ IMPDRV_CORE_TYPE_IMP,		2 },
	{ IMPDRV_CORE_TYPE_IMP,		3 },
	{ IMPDRV_CORE_TYPE_OCV,		0 },
	{ IMPDRV_CORE_TYPE_OCV,		1 },
	{ IMPDRV_CORE_TYPE_DMAC,	0 },
	{ IMPDRV_CORE_TYPE_DMAC,	1 },
	{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
	{ IMPDRV_CORE_TYPE_CNN,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,	0 }
};

static const st_impdrv_core_info_t not_supported_exec_core[] = {
	{ IMPDRV_CORE_TYPE_PSCOUT,	0 },
	{ IMPDRV_CORE_TYPE_LDMAC,	0 },
	{ IMPDRV_CORE_TYPE_DTA,		0 },
	{ IMPDRV_CORE_TYPE_IMR,		0 }
};

static const st_impdrv_core_info_t busif_exec_core[] = {
	{ IMPDRV_CORE_TYPE_IMP,			0 },
	{ IMPDRV_CORE_TYPE_IMP,			1 },
	{ IMPDRV_CORE_TYPE_IMP,			2 },
	{ IMPDRV_CORE_TYPE_IMP,			3 },
	{ IMPDRV_CORE_TYPE_DMAC,		0 },
	{ IMPDRV_CORE_TYPE_DMAC,		1 },
};

static const st_impdrv_core_info_t not_supported_busif_exec_core[] = {
	{ IMPDRV_CORE_TYPE_OCV,			0 },
	{ IMPDRV_CORE_TYPE_OCV,			1 },
	{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
	{ IMPDRV_CORE_TYPE_CNN,			0 },
	{ IMPDRV_CORE_TYPE_PSCOUT,		0 },
	{ IMPDRV_CORE_TYPE_LDMAC,		0 },
	{ IMPDRV_CORE_TYPE_DTA,			0 },
	{ IMPDRV_CORE_TYPE_IMR,			0 }
};

/* parameter for Configuration Register Check(needs only for IMP core) */
static const st_impdrv_chk_param_t config_reg_check_IMP[] = {
	{ 0x0030, 0x0000FFFF, 0xFFFFFFFF }
};

/* parameter for Unexpected Module Stop Check */
/* IMP core */
const st_impdrv_chk_param_t module_stop_check_IMP[] = {
	{ 0x0080, 0x17021420, 0xFFFFFFFF },
	{ 0x0030, 0x0000FFFF, 0xFFFFFFFF }
};
/* OCV core */
const st_impdrv_chk_param_t module_stop_check_OCV[] = {
	{ 0x0000, 0x00000004, 0xFFFFFFFF },
	{ 0x0008, 0x00000000, 0xFFFFFFFE }
};
/* IMP DMAC */
const st_impdrv_chk_param_t module_stop_check_DMAC[] = {
	{ 0x0000, 0x17021420, 0xFFFFFFFF }
};
/* IMP PSC */
const st_impdrv_chk_param_t module_stop_check_PSC[] = {
	{ 0x0000, 0x17020121, 0xFFFFFFFF }
};
/* IMP CNN */
const st_impdrv_chk_param_t module_stop_check_CNN[] = {
	{ 0x0000, 0x00030101, 0xFFFFFFFF },
	{ 0x0008, 0x00000000, 0xFFFFFFFE }
};

const uint32_t read_value_for_init[READ_NUM_FOR_INIT] = {
	0,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_PSC_VCR,
	IMPDRV_CNN_VCR
};

const uint32_t read_value_for_start[READ_NUM_FOR_START] = {
	[20] = IMPDRV_OCV_RESET_STATUS_VAL,
	[26] = IMPDRV_OCV_RESET_STATUS_VAL,
	[32] = IMPDRV_OCV_RESET_STATUS_VAL,
	[38] = IMPDRV_OCV_RESET_STATUS_VAL,
	[44] = IMPDRV_OCV_RESET_STATUS_VAL,
};

const uint32_t s_impdrv_inten_addr[IMPDRV_INSTANCETABLE_NUM] =
{
    IMPDRV_DUMMY_UINT32,        /**< Dummy for Index adjustment */
    IMPDRV_REG_IMR,             /**< IMPDRV_INSTANCE_0 */
    IMPDRV_REG_IMR1,            /**< IMPDRV_INSTANCE_1 */
    IMPDRV_REG_IMR2,            /**< IMPDRV_INSTANCE_2 */
    IMPDRV_REG_IMR3,            /**< IMPDRV_INSTANCE_3 */
    IMPDRV_REG_IMR4             /**< IMPDRV_INSTANCE_4 */
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
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  },
	[IMPDRV_CORE_TYPE_OCV]			= {
										{ .type = IMPDRV_IRQMASK_USIER,		.value = true	},
										{ .type = IMPDRV_IRQMASK_USINT,		.value = true	},
										{ .type = IMPDRV_IRQMASK_PBCOVF,	.value = true	},
										{ .type = IMPDRV_IRQMASK_SBO0ME,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_DMAC]			= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  },
	[IMPDRV_CORE_TYPE_DMAC_SLIM]	= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  },
	[IMPDRV_CORE_TYPE_PSCEXE]		= {
										{ .type = IMPDRV_IRQMASK_END,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  },
	[IMPDRV_CORE_TYPE_PSCOUT]		= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  },
	[IMPDRV_CORE_TYPE_CNN]			= {
										{ .type = IMPDRV_IRQMASK_MSCO,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  }
};

const st_impdrv_irq_group_t g_p_irq_param[5] = {
	[IMPDRV_IRQ_GROUP_INVALID]	= {
									.irq_group = IMPDRV_IRQ_GROUP_INVALID
								  },
	[IMPDRV_IRQ_GROUP_NONE]		= {
									.irq_group = IMPDRV_IRQ_GROUP_NONE,
									.group_core_num = ARRAY_SIZE(platform_soc),
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_CNN,		0 }
									}
								  },
	[IMPDRV_IRQ_GROUP_0]		= {
									.irq_group = IMPDRV_IRQ_GROUP_0,
									.group_core_num = ARRAY_SIZE(platform_soc),
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_CNN,		0 }
									}
								  },
	[IMPDRV_IRQ_GROUP_1]		= {
									.irq_group = IMPDRV_IRQ_GROUP_1,
									.group_core_num = ARRAY_SIZE(platform_soc),
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_CNN,		0 }
									}
								  },
	[IMPDRV_IRQ_GROUP_2]		= {
									.irq_group = IMPDRV_IRQ_GROUP_2,
									.group_core_num = ARRAY_SIZE(platform_soc),
									.group_core_info = {
										{ IMPDRV_CORE_TYPE_IMP,		0 },
										{ IMPDRV_CORE_TYPE_IMP,		1 },
										{ IMPDRV_CORE_TYPE_IMP,		2 },
										{ IMPDRV_CORE_TYPE_IMP,		3 },
										{ IMPDRV_CORE_TYPE_OCV,		0 },
										{ IMPDRV_CORE_TYPE_OCV,		1 },
										{ IMPDRV_CORE_TYPE_DMAC,	0 },
										{ IMPDRV_CORE_TYPE_DMAC,	1 },
										{ IMPDRV_CORE_TYPE_PSCEXE,	0 },
										{ IMPDRV_CORE_TYPE_CNN,		0 }
									}
								  }
};

static const st_impdrv_group_tbl_t s_impdrv_inten_group_table[IMPDRV_GROUP_MAX] =
{
    /** group_bit               group_reg   */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32    },  /**< Dummy for Index */
    {   IMPDRV_DUMMY_UINT32,    IMPDRV_DUMMY_UINT32    },  /**< Dummy for Index */
    {   0x20000000U,            IMPDRV_REG_G0INTSEL    },  /** Group0. */
    {   0x40000000U,            IMPDRV_REG_G1INTSEL    },  /** Group1. */
    {   0x80000000U,            IMPDRV_REG_G2INTSEL    }   /** Group2. */
};

const st_impdrv_inten_tbl_t s_impdrv_inten_cnv_table[IMPDRV_INTEN_CORE_MAX] =
{
    /** coreType,               CoreNum,    bit     */
    {   IMPDRV_CORE_TYPE_IMP,       0,  0x00000001U },
    {   IMPDRV_CORE_TYPE_IMP,       1,  0x00000002U },
    {   IMPDRV_CORE_TYPE_IMP,       2,  0x00000004U },
    {   IMPDRV_CORE_TYPE_IMP,       3,  0x00000008U },
    {   IMPDRV_CORE_TYPE_OCV,       0,  0x00000010U },
    {   IMPDRV_CORE_TYPE_OCV,       1,  0x00000020U },
    {   IMPDRV_CORE_TYPE_DMAC,      0,  0x00000040U },
    {   IMPDRV_CORE_TYPE_DMAC,      1,  0x00000080U },
    {   IMPDRV_CORE_TYPE_PSCEXE,    0,  0x00000100U },
    {   IMPDRV_CORE_TYPE_CNN,       0,  0x00000200U },
};

const st_impdrv_inten_reg_tbl_t s_impdrv_inten_reg_tbl[IMPDRV_INSTANCETABLE_NUM] =
{
    /** p_inten_group_table         p_inten_cnv_table           cnv_table_num */
    { NULL,                         NULL,                     IMPDRV_DUMMY_UINT32    },   /**< Dummy for Index */
    { s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX  },   /**< IMPDRV_INSTANCE_0 */
    { s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX  },   /**< IMPDRV_INSTANCE_1 */
    { s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX  },   /**< IMPDRV_INSTANCE_2 */
    { s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX  },   /**< IMPDRV_INSTANCE_3 */
    { s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX  }    /**< IMPDRV_INSTANCE_4 */
};

/* core infos for instances */
static const st_impdrv_core_info_t core_info_for_ins0[] = {
	{ IMPDRV_CORE_TYPE_IMP,			0 },
	{ IMPDRV_CORE_TYPE_IMP,			1 }
};

static const st_impdrv_core_info_t core_info_for_ins1[] = {
	{ IMPDRV_CORE_TYPE_IMP,			2 },
	{ IMPDRV_CORE_TYPE_OCV,			0 }
};

static const st_impdrv_core_info_t core_info_for_ins2[] = {
	{ IMPDRV_CORE_TYPE_IMP,			3 },
	{ IMPDRV_CORE_TYPE_OCV,			1 }
};

static const st_impdrv_core_info_t core_info_for_ins3[] = {
	{ IMPDRV_CORE_TYPE_DMAC,		0 },
	{ IMPDRV_CORE_TYPE_DMAC,		1 }
};

static const st_impdrv_core_info_t core_info_for_ins4[] = {
	{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
	{ IMPDRV_CORE_TYPE_CNN,			0 }
};

static const st_impdrv_imp_write_info_tbl_t imp_transfer_config[] =
{
	{ IMPDRV_REG_IMP_IPFUN2,     0x00100000U,   true,   0x00100000U },
	{ IMPDRV_REG_IMP_IPFORM,     0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_APMAG,      0x01010101U,   true,   0x01010101U },
	{ IMPDRV_REG_IMP_APCLPX,     0x00001FFFU,   true,   0x00001FFFU },
	{ IMPDRV_REG_IMP_CNST,       0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_BINTHR,     0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_BINTHR2,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_KNLMSK,     0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_KNLMSK2,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_LMCTL,      0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF02,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF35,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF68,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF911,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF1214,  0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF15,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF1820,  0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF2123,  0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF24,    0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF02H,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF35H,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF68H,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF911H,  0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF1214H, 0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF1517H, 0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF1820H, 0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF2123H, 0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_COEFF24H,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_PFFTASEL1,  0x76543210U,   true,   0x76543210U },
	{ IMPDRV_REG_IMP_PFFTAMSK,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_PFFTBMSK,   0x00000000U,   true,   0x00000000U },
	{ IMPDRV_REG_IMP_APSBSP,     0U         ,   true,   0U          },
	{ IMPDRV_REG_IMP_APSIZE_SA,  256U       ,   true,   256U        },
	{ IMPDRV_REG_IMP_APSIZE_SB,  0U         ,   true,   0U          },
	{ IMPDRV_REG_IMP_APSIZE_DST, 256U       ,   true,   256U        },
	{ IMPDRV_REG_IMP_APDLY,      0x00000004U,   true,   0x00000004U },
	{ IMPDRV_REG_IMP_APCFG,      0x01033310U,   true,   0x01033310U },
	{ IMPDRV_REG_IMP_IPFUN,      0xF2000000U,   true,   0xF2000000U },
	{ IMPDRV_REG_IMP_IPFUN2,     0x60104000U,   true,   0x60104000U },
	{ IMPDRV_REG_IMP_PFFTASEL1,  0x00000000U,   true,   0x00000000U }
};

static const st_impdrv_dma_write_info_tbl_t dma_transfer_config[] =
{
	{ IMPDRV_REG_DMA_FCR0,      0x000000CCU,    true,   0x000000CCU },
	{ IMPDRV_REG_DMA_S0STR,     0x00000100U,    true,   0x00000100U },
	{ IMPDRV_REG_DMA_S0DATAR,   0x00000000U,    true,   0x00000000U },
	{ IMPDRV_REG_DMA_S0CR,      0x80000003U,    true,   0x80000003U },
	{ IMPDRV_REG_DMA_S1SAR,     0x00000000U,    true,   0x00000000U },
	{ IMPDRV_REG_DMA_S1STR,     0x00000000U,    true,   0x00000000U },
	{ IMPDRV_REG_DMA_S1DATAR,   0x00000000U,    true,   0x00000000U },
	{ IMPDRV_REG_DMA_S1CR,      0x00000003U,    true,   0x00000003U },
	{ IMPDRV_REG_DMA_D0STR,     0x00000100U,    true,   0x00000100U },
	{ IMPDRV_REG_DMA_D0CR,      0x00000003U,    true,   0x00000003U },
	{ IMPDRV_REG_DMA_DSWPR,     0x00000000U,    true,   0x00000000U }
};

static const tp_test_pattern_t imp_test_pattern_tbl[] = {
	{ 0x55555555U, (sizeof(uint32_t) * 8U) },
	{ 0xAAAAAAAAU, (sizeof(uint32_t) * 8U) },
	{ 0x33333333U, (sizeof(uint32_t) * 8U) },
	{ 0xCCCCCCCCU, (sizeof(uint32_t) * 8U) },
	{ 0x0F0F0F0FU, (sizeof(uint32_t) * 8U) },
	{ 0xF0F0F0F0U, (sizeof(uint32_t) * 8U) },
	{ 0xFFFFFFFFU, (sizeof(uint32_t) * 8U) },
	{ 0x00000000U, (sizeof(uint32_t) * 8U) }
};

static const tp_test_pattern_t dma_test_pattern_tbl[] = {
	{ 0x55555555U, (sizeof(uint32_t) * 8U) },
	{ 0xAAAAAAAAU, (sizeof(uint32_t) * 8U) },
	{ 0x33333333U, (sizeof(uint32_t) * 8U) },
	{ 0xCCCCCCCCU, (sizeof(uint32_t) * 8U) },
	{ 0x0F0F0F0FU, (sizeof(uint32_t) * 8U) },
	{ 0xF0F0F0F0U, (sizeof(uint32_t) * 8U) },
	{ 0xFFFFFFFFU, (sizeof(uint32_t) * 8U) },
	{ 0x00000000U, (sizeof(uint32_t) * 8U) }
};

tp_arr_info_t get_data_info(
	const e_impdrv_core_type_t	core_type,
	const tp_cmd_t				cmd
)
{
	tp_arr_info_t data_info = { .range = 0, .info = NULL };

	switch (cmd)
	{
		case IT_IMPDRV_CMD_DMA_TRANSFER:
		{
			if (IMPDRV_CORE_TYPE_IMP == core_type)
			{
				data_info.range	= ARRAY_SIZE(imp_transfer_config);
				data_info.info	= (void *)imp_transfer_config;
			}
			else if (IMPDRV_CORE_TYPE_DMAC == core_type)
			{
				data_info.range	= ARRAY_SIZE(dma_transfer_config);
				data_info.info	= (void *)dma_transfer_config;
			}
			else
			{
				/* do nothing */
			}

			break;
		}
		case IT_IMPDRV_CMD_BUSIF_TEST_PATTERN:
		{
			if (IMPDRV_CORE_TYPE_IMP == core_type)
			{
				data_info.range	= ARRAY_SIZE(imp_test_pattern_tbl);
				data_info.info	= (void *)imp_test_pattern_tbl;
			}
			else if (IMPDRV_CORE_TYPE_DMAC == core_type)
			{
				data_info.range	= ARRAY_SIZE(dma_test_pattern_tbl);
				data_info.info	= (void *)dma_test_pattern_tbl;
			}
			else
			{
				/* do nothing */
			}

			break;
		}
		case IT_IMPDRV_CMD_CONF_REG_CHECK:
		{
			if (IMPDRV_CORE_TYPE_IMP == core_type)
			{
				data_info.range	= ARRAY_SIZE(config_reg_check_IMP);
				data_info.info	= (void *)config_reg_check_IMP;
			}

			break;
		}
		case IT_IMPDRV_CMD_MODULE_STOP_CHECK:
		{
			if (IMPDRV_CORE_TYPE_IMP == core_type)
			{
				data_info.range	= ARRAY_SIZE(module_stop_check_IMP);
				data_info.info	= (void *)module_stop_check_IMP;
			}
			else if (IMPDRV_CORE_TYPE_OCV == core_type)
			{
				data_info.range	= ARRAY_SIZE(module_stop_check_OCV);
				data_info.info	= (void *)module_stop_check_OCV;
			}
			else if (IMPDRV_CORE_TYPE_DMAC == core_type)
			{
				data_info.range	= ARRAY_SIZE(module_stop_check_DMAC);
				data_info.info	= (void *)module_stop_check_DMAC;
			}
			else if (IMPDRV_CORE_TYPE_PSCEXE == core_type)
			{
				data_info.range	= ARRAY_SIZE(module_stop_check_PSC);
				data_info.info	= (void *)module_stop_check_PSC;
			}
			else if (IMPDRV_CORE_TYPE_CNN == core_type)
			{
				data_info.range	= ARRAY_SIZE(module_stop_check_CNN);
				data_info.info	= (void *)module_stop_check_CNN;
			}
			else
			{
				/* do nothing */
			}

			break;
		}
		default:
		{
			break;
		}
	}

	return data_info;
}

tp_core_list_t get_core_list(tp_cmd_t cmd, int8_t sub_cmd)
{
	tp_core_list_t core_list;

	memset(&core_list, 0, sizeof(core_list));

	switch (cmd)
	{
		case IT_IMPDRV_CMD_PLATFORM_SOC:
		{
			core_list.size		= sizeof(platform_soc);
			core_list.num		= ARRAY_SIZE(platform_soc);
			core_list.core_info	= platform_soc;
			break;
		}
		case IT_IMPDRV_CMD_INVALID_CORES:
		{
			core_list.size		= sizeof(invalid_core);
			core_list.num		= ARRAY_SIZE(invalid_core);
			core_list.core_info	= invalid_core;
			break;
		}
		case IT_IMPDRV_CMD_COREMAP:
		{
			core_list.size		= sizeof(soc_coremap);
			core_list.num		= ARRAY_SIZE(soc_coremap);
			core_list.core_info	= soc_coremap;
			break;
		}
		case IT_IMPDRV_CMD_NO_EXEC:
		{
			core_list.size		= sizeof(not_supported_exec_core);
			core_list.num		= ARRAY_SIZE(not_supported_exec_core);
			core_list.core_info	= not_supported_exec_core;
			break;
		}
		case IT_IMPDRV_CMD_INSTANCES:
		{
			switch (sub_cmd)
			{
				case IMPDRV_INSTANCE_0:
				{
					core_list.size		= sizeof(core_info_for_ins0);
					core_list.num		= ARRAY_SIZE(core_info_for_ins0);
					core_list.core_info	= core_info_for_ins0;
					break;
				}
				case IMPDRV_INSTANCE_1:
				{
					core_list.size		= sizeof(core_info_for_ins1);
					core_list.num		= ARRAY_SIZE(core_info_for_ins1);
					core_list.core_info	= core_info_for_ins1;
					break;
				}
				case IMPDRV_INSTANCE_2:
				{
					core_list.size		= sizeof(core_info_for_ins2);
					core_list.num		= ARRAY_SIZE(core_info_for_ins2);
					core_list.core_info	= core_info_for_ins2;
					break;
				}
				case IMPDRV_INSTANCE_3:
				{
					core_list.size		= sizeof(core_info_for_ins3);
					core_list.num		= ARRAY_SIZE(core_info_for_ins3);
					core_list.core_info	= core_info_for_ins3;
					break;
				}
				case IMPDRV_INSTANCE_4:
				{
					core_list.size		= sizeof(core_info_for_ins4);
					core_list.num		= ARRAY_SIZE(core_info_for_ins4);
					core_list.core_info	= core_info_for_ins4;
					break;
				}
				default:
				{
					break;
				}
			}

			break;
		}
		case IT_IMPDRV_CMD_BUSIF:
		{
			core_list.size		= sizeof(busif_exec_core);
			core_list.num		= ARRAY_SIZE(busif_exec_core);
			core_list.core_info	= busif_exec_core;
			break;
		}
    	case IT_IMPDRV_CMD_NO_EXEC_BUSIF:
		{
			core_list.size		= sizeof(not_supported_busif_exec_core);
			core_list.num		= ARRAY_SIZE(not_supported_busif_exec_core);
			core_list.core_info	= not_supported_busif_exec_core;
			break;
		}
		default:
		{
			break;
		}
	}

	return core_list;
}

static sync_core_t sync_core[] = {
	list_2element(IMP, 0,	OCV,    0),
	list_2element(IMP, 1,	DMAC,   0),
	list_3element(IMP, 2,	PSCEXE, 0,	DMAC,    1),
	list_3element(IMP, 3,	OCV,    1,	CNN,     0),
	list_endelement(0),
};

static sync_core_t sync_6_cores[] = {
	list_6element(IMP, 0,	IMP, 1,		OCV, 0,		OCV, 1,			DMAC, 0,		PSCEXE, 0),
	list_6element(OCV, 0,	OCV, 1,		DMAC, 0,	DMAC, 1,		CNN, 0,			IMP, 0),
	list_6element(DMAC, 0,	DMAC, 1,	PSCEXE, 0,	OCV, 1,			IMP, 1,			CNN, 0),
	list_endelement(0)
};

sync_core_t *get_sync_cores(int32_t cmd)
{
	sync_core_t *core_list;

	switch (cmd)
	{
		case CMD_LESS_THAN_6_CORES:
		{
			core_list = sync_core;
			break;
		}
		case CMD_6_CORES:
		{
			core_list = sync_6_cores;
			break;
		}
		default:
		{
			core_list = NULL;
			break;
		}
	}

	return core_list;
}

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
                *p_irq_group = IMPDRV_IRQ_GROUP_0;
                break;
            case IMPDRV_INSTANCE_1:
                *p_irq_group = IMPDRV_IRQ_GROUP_1;
                break;
            case IMPDRV_INSTANCE_2:
                *p_irq_group = IMPDRV_IRQ_GROUP_2;
                break;
            case IMPDRV_INSTANCE_3:
            case IMPDRV_INSTANCE_4:
            case IMPDRV_INSTANCE_5:
            case IMPDRV_INSTANCE_6:
                ercd = IMPDRV_EC_NG_NOTSUPPORT;
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

/** Private static variables */
const st_impdrv_dma_write_info_tbl_t s_mb_init_info1[IMPDRV_DMA_MB_INFO_SIZE1] =
{
	{IMPDRV_REG_DMA_IMGSIZER,   IMPDRV_DMA_IMGSIZER_VAL,    true,   IMPDRV_DMA_IMGSIZER_VAL   },
	{IMPDRV_REG_DMA_FCR0,       IMPDRV_DMA_FCR0_VAL,        true,   IMPDRV_DMA_FCR0_VAL       },
	{IMPDRV_REG_DMA_FCR1,       IMPDRV_DMA_FCR1_VAL,        true,   IMPDRV_DMA_FCR1_VAL       }
};

const st_impdrv_dma_write_info_tbl_t s_mb_init_info2[IMPDRV_DMA_MB_INFO_SIZE2] =
{
	{IMPDRV_REG_DMA_S0STR,      IMPDRV_DMA_S0STR_VAL,       true,   IMPDRV_DMA_S0STR_VAL      },
	{IMPDRV_REG_DMA_S0DATAR,    IMPDRV_DMA_S0DATAR_VAL,     true,   IMPDRV_DMA_S0DATAR_VAL    },
	{IMPDRV_REG_DMA_S0CR,       IMPDRV_DMA_S0CR_VAL,        true,   IMPDRV_DMA_S0CR_VAL       },
	{IMPDRV_REG_DMA_S1SAR,      IMPDRV_DMA_S1SAR_VAL,       true,   IMPDRV_DMA_S1SAR_VAL      },
	{IMPDRV_REG_DMA_S1STR,      IMPDRV_DMA_S1STR_VAL,       true,   IMPDRV_DMA_S1STR_VAL      },
	{IMPDRV_REG_DMA_S1DATAR,    IMPDRV_DMA_S1DATAR_VAL,     true,   IMPDRV_DMA_S1DATAR_VAL    },
	{IMPDRV_REG_DMA_S1CR,       IMPDRV_DMA_S1CR_VAL,        true,   IMPDRV_DMA_S1CR_VAL       }
};

const st_impdrv_dma_write_info_tbl_t s_mb_init_info3[IMPDRV_DMA_MB_INFO_SIZE3] =
{
	{IMPDRV_REG_DMA_D0STR,      IMPDRV_DMA_D0STR_VAL,       true,   IMPDRV_DMA_D0STR_VAL      },
	{IMPDRV_REG_DMA_D0CR,       IMPDRV_DMA_D0CR_VAL,        true,   IMPDRV_DMA_D0CR_VAL       },
	{IMPDRV_REG_DMA_DSWPR,      IMPDRV_DMA_DSWPR_VAL,       true,   IMPDRV_DMA_DSWPR_VAL      },
	{IMPDRV_REG_DMA_TSCR,       IMPDRV_DMA_TSCR_VAL,        true,   IMPDRV_DMA_TSCR_VAL       },
	{IMPDRV_REG_DMA_SCTLR,      IMPDRV_DMA_SCTLR0_MB_VAL,   false,  0U                        }
};

uint32_t imp_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != irq_mask)
    {
        /** Set interrupt for IMP core by default */
        inten_val = IMPDRV_IMP_INTSTS_MASKCPU;
        inten_val |= (IMPDRV_IMP_INTSTS_TRAP | IMPDRV_IMP_INTSTS_IER | IMPDRV_IMP_INTSTS_INT);

        /** Set interrupt for IMP core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_HPINT == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_IMP_INTSTS_HPINT;
            }
            if (((uint32_t)IMPDRV_IRQMASK_APIPINT == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_IMP_INTSTS_APIPINT;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}

uint32_t ocv_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != irq_mask)
    {
        /** Set interrupt for OCV core by default */
        inten_val = (IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT);

        /** Set interrupt for OCV core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_PBCOVF == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_PBCOVF;
            }
            if (((uint32_t)IMPDRV_IRQMASK_SBO0ME == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_SBO0ME;
            }
            if (((uint32_t)IMPDRV_IRQMASK_USIER == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_USIER;
            }
            if (((uint32_t)IMPDRV_IRQMASK_USINT == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_OCV_SR1_USINT;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}

uint32_t dmac_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;

    /** Input parameter confirmation process */
    if (NULL != irq_mask)
    {
        /** Set interrupt for DMAC core by default */
        inten_val = (IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT);
    }

    /** Set data in return value */
    return inten_val;
}

uint32_t psc_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != irq_mask)
    {
        /** Set interrupt for PSC core by default */
        inten_val = (IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT);

        /** Set interrupt for PSC core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_END == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_PSC_SR_TEND;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}

uint32_t cnn_get_inten_val(
    const bool      irq_mask[IMPDRV_IRQMASK_MAX]
)
{
    /** initialize local variables */
    uint32_t    inten_val = 0U;
    uint32_t    i;

    /** Input parameter confirmation process */
    if (NULL != irq_mask)
    {
        /** Set interrupt for imp core by default */
        inten_val = (IMPDRV_CNN_SR_TRAP | IMPDRV_CNN_SR_IER | IMPDRV_CNN_SR_INT);

        /** Set interrupt for imp core */
        for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
        {
            if (((uint32_t)IMPDRV_IRQMASK_MSCO == i) && (false == irq_mask[i]))
            {
                inten_val |= IMPDRV_CNN_SR_MSCO;
            }
        }
    }

    /** Set data in return value */
    return inten_val;
}

void cnv_int_core_bit(
    const st_impdrv_core_info_t     *const p_core_info,
    const uint32_t                  use_core_num,
    const st_impdrv_inten_tbl_t     *const p_inten_cnv_table,
    const uint32_t                  cnv_table_num,
    uint32_t                        *const p_val
)
{
    /** initialize local variables */
    bool                    hit;
    uint32_t                i;
    uint32_t                j;

    /** Identify the bits of the corresponding registers from the core type and core number */
	*p_val = 0U;
	i = 0U;
	while (i < use_core_num)
	{
		hit = false;
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
		if (false == hit)
		{
			break;
		}
		i++;
	}
}

void get_syncc_val(
    const st_impdrv_corectl_t   *const p_corectl,
    uint32_t                    syncc_val[IMPDRV_SYNCC_REGNUM]
)
{
	uint32_t offset;
    uint32_t reg_val;
    uint32_t syncc_val_tmp;

	for (uint32_t cnt = 0U; IMPDRV_SYNCC_REGNUM > cnt; cnt++)
	{
		offset = cnt * IMPDRV_OFFSET_4BYTE;

		syncc_val_tmp = (uint32_t)(p_corectl->cur_core_map[offset]);

		reg_val = (uint32_t)(p_corectl->cur_core_map[offset + 1U]);
		syncc_val_tmp |= (reg_val << IMPDRV_SHIFT_8BIT);

		reg_val = (uint32_t)(p_corectl->cur_core_map[offset + 2U]);
		syncc_val_tmp |= (reg_val << IMPDRV_SHIFT_16BIT);

		reg_val = (uint32_t)(p_corectl->cur_core_map[offset + 3U]);
		syncc_val_tmp |= (reg_val << IMPDRV_SHIFT_24BIT);

		syncc_val[cnt] = syncc_val_tmp;
	}
}

uint32_t get_irq_mask_val(
    const e_impdrv_core_type_t core_type
)
{
	uint32_t mask_val = 0U;

	switch (core_type)
	{
		case IMPDRV_CORE_TYPE_OCV:
		{
			mask_val = IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT
						| IMPDRV_OCV_SR1_PBCOVF | IMPDRV_OCV_SR1_SBO0ME
						| IMPDRV_OCV_SR1_USIER | IMPDRV_OCV_SR1_USINT;
			break;
		}
		case IMPDRV_CORE_TYPE_DMAC:
		{
			mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    	IMPDRV_DMA_SR_TEND;
			break;
		}
		case IMPDRV_CORE_TYPE_PSCEXE:
		{
			mask_val = IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT
                    	| IMPDRV_PSC_SR_TEND;
			break;
		}
		case IMPDRV_CORE_TYPE_CNN:
		{
			mask_val = IMPDRV_CNN_SR_TRAP | IMPDRV_CNN_SR_IER | IMPDRV_CNN_SR_INT
                    	| IMPDRV_CNN_SR_MSCO;
			break;
		}
		default:
		{
			break;
		}
	}

	return mask_val;
}