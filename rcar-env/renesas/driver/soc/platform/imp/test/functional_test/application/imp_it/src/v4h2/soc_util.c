#include "soc_common.h"

bool close_device_io_flg = false;

#define IMPDRV_IMP_INTSTS_CLBRK     (0x00000400U)
#define IMPDRV_OCV_SR1_CLBRK        (0x00000400U)
#define IMPDRV_PSC_SR_CLBRK         (0x00000400U)
#define IMPDRV_DMAS_SR_CLBRK        (0x00000400U)
#define IMPDRV_DMA_SR_CLBRK         (0x00000400U)
#define IMPDRV_CNN_SR_CLBRK         (0x00000020U)

static const st_impdrv_core_info_t platform_soc[] = {
	{ IMPDRV_CORE_TYPE_IMP,			0 },
	{ IMPDRV_CORE_TYPE_IMP,			1 },
	{ IMPDRV_CORE_TYPE_IMP,			2 },
	{ IMPDRV_CORE_TYPE_IMP,			3 },
	{ IMPDRV_CORE_TYPE_OCV,			0 },
	{ IMPDRV_CORE_TYPE_OCV,			1 },
	{ IMPDRV_CORE_TYPE_OCV,			2 },
	{ IMPDRV_CORE_TYPE_OCV,			3 },
	{ IMPDRV_CORE_TYPE_DMAC,		0 },
	{ IMPDRV_CORE_TYPE_DMAC,		1 },
	{ IMPDRV_CORE_TYPE_DMAC,		2 },
	{ IMPDRV_CORE_TYPE_DMAC,		3 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },
	{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
	{ IMPDRV_CORE_TYPE_CNN,			0 },
	{ IMPDRV_CORE_TYPE_DSP,         0 }
};

static const st_impdrv_core_info_t platform_soc_no_dma[] = {
	{ IMPDRV_CORE_TYPE_IMP,			0 },
	{ IMPDRV_CORE_TYPE_IMP,			1 },
	{ IMPDRV_CORE_TYPE_IMP,			2 },
	{ IMPDRV_CORE_TYPE_IMP,			3 },
	{ IMPDRV_CORE_TYPE_OCV,			0 },
	{ IMPDRV_CORE_TYPE_OCV,			1 },
	{ IMPDRV_CORE_TYPE_OCV,			2 },
	{ IMPDRV_CORE_TYPE_OCV,			3 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },
	{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
	{ IMPDRV_CORE_TYPE_CNN,			0 },
	{ IMPDRV_CORE_TYPE_DSP,         0 }
};

static const st_impdrv_core_info_t invalid_core[] = {
	{ IMPDRV_CORE_TYPE_INVALID,		0					},
	{ IMPDRV_CORE_TYPE_IMP,			IMP_CORE_MAX		},
	{ IMPDRV_CORE_TYPE_OCV,			OCV_CORE_MAX		},
	{ IMPDRV_CORE_TYPE_DMAC,		DMA_CORE_MAX		},
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	DMA_SLIM_CORE_MAX	},
	{ IMPDRV_CORE_TYPE_PSCEXE,		PSCEXE_CORE_MAX		},
	{ IMPDRV_CORE_TYPE_CNN,			CNN_CORE_MAX		},
	{ IMPDRV_CORE_TYPE_DSP,			DSP_CORE_MAX		}
};

static const st_impdrv_core_info_t soc_coremap[] = {
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 },
	{ IMPDRV_CORE_TYPE_INVALID,		0 }
};

static const st_impdrv_core_info_t not_supported_exec_core[] = {
	{ IMPDRV_CORE_TYPE_PSCOUT,		0 },
	{ IMPDRV_CORE_TYPE_LDMAC,		0 },
	{ IMPDRV_CORE_TYPE_DTA,			0 },
	{ IMPDRV_CORE_TYPE_IMR,			0 }
};

static const st_impdrv_core_info_t busif_exec_core[] = {
	{ IMPDRV_CORE_TYPE_IMP,			0 },
	{ IMPDRV_CORE_TYPE_IMP,			1 },
	{ IMPDRV_CORE_TYPE_IMP,			2 },
	{ IMPDRV_CORE_TYPE_IMP,			3 },
	{ IMPDRV_CORE_TYPE_IMP,			4 },
	{ IMPDRV_CORE_TYPE_IMP_SLIM,	0 },
	{ IMPDRV_CORE_TYPE_DMAC,		0 },
	{ IMPDRV_CORE_TYPE_DMAC,		1 },
	{ IMPDRV_CORE_TYPE_DMAC,		2 },
	{ IMPDRV_CORE_TYPE_DMAC,		3 }
};

static const st_impdrv_core_info_t not_supported_busif_exec_core[] = {
	{ IMPDRV_CORE_TYPE_OCV,			0 },
	{ IMPDRV_CORE_TYPE_OCV,			1 },
	{ IMPDRV_CORE_TYPE_OCV,			2 },
	{ IMPDRV_CORE_TYPE_OCV,			3 },
	{ IMPDRV_CORE_TYPE_OCV,			4 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
	{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
	{ IMPDRV_CORE_TYPE_PSCEXE,		1 },
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

static const st_impdrv_chk_param_t config_reg_check_DSP[] = {
	{ 0xC0094, 0x20111119, 0xFFFFFFFF }
};

/* parameter for Unexpected Module Stop Check */
/* IMP core/Slim-IMP core */
const st_impdrv_chk_param_t module_stop_check_IMP[] = {
	{ 0x0080, 0x18080510, 0xFFFFFFFF },
	{ 0x0030, 0x0000FFFF, 0xFFFFFFFF }
};
/* OCV core */
const st_impdrv_chk_param_t module_stop_check_OCV[] = {
	{ 0x0000, 0x00000006, 0xFFFFFFFF },
	{ 0x0008, 0x00000000, 0xFFFFFFFE }
};
/* IMP DMAC */
const st_impdrv_chk_param_t module_stop_check_DMAC[] = {
	{ 0x0000, 0x20021013, 0xFFFFFFFF }
};
/* IMP Slim-DMAC */
const st_impdrv_chk_param_t module_stop_check_SlimDMAC[] = {
	{ 0x0000, 0x20021013, 0xFFFFFFFF }
};
/* IMP PSC */
const st_impdrv_chk_param_t module_stop_check_PSC[] = {
	{ 0x0000, 0x20040718, 0xFFFFFFFF }
};
/* IMP CNN */
const st_impdrv_chk_param_t module_stop_check_CNN[] = {
	{ 0x0000, 0x00030200, 0xFFFFFFFF },
	{ 0x0008, 0x00000000, 0xFFFFFFFE }
};

const uint32_t read_value_for_init[READ_NUM_FOR_INIT] = {
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_IMP_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_OCV_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMA_VCR,
	IMPDRV_DMAS_VCR,
	IMPDRV_DMAS_VCR,
	IMPDRV_DMAS_VCR,
	IMPDRV_DMAS_VCR,
	IMPDRV_PSC_VCR,
	IMPDRV_CNN_VCR,
	IMPDRV_VDSP_VERSION1_VAL,
};

/* @todo : Unknown the configuration data for V4H */
const uint32_t read_value_for_start[READ_NUM_FOR_START] = {
	[20] = IMPDRV_OCV_RESET_STATUS_VAL,
	[26] = IMPDRV_OCV_RESET_STATUS_VAL,
	[32] = IMPDRV_OCV_RESET_STATUS_VAL,
	[38] = IMPDRV_OCV_RESET_STATUS_VAL,
	[44] = IMPDRV_OCV_RESET_STATUS_VAL,
	[50] = IMPDRV_OCV_RESET_STATUS_VAL,
	[56] = IMPDRV_OCV_RESET_STATUS_VAL,
};

const uint32_t s_impdrv_inten_addr[IMPDRV_INSTANCETABLE_NUM] =
{
	IMPDRV_DUMMY_UINT32,		/**< Dummy for Index adjustment */
	IMPDRV_REG_INTEN00,			/**< IMPDRV_INSTANCE_0 */
	IMPDRV_REG_INTEN01,			/**< IMPDRV_INSTANCE_1 */
	IMPDRV_REG_INTEN02			/**< IMPDRV_INSTANCE_2 */
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
	[IMPDRV_CORE_TYPE_OCV]			= {
										{ .type = IMPDRV_IRQMASK_USIER,		.value = true	},
										{ .type = IMPDRV_IRQMASK_USINT,		.value = true	},
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_IRQMASK_PBCOVF,	.value = true	},
										{ .type = IMPDRV_IRQMASK_SBO0ME,	.value = true	}
									  },
	[IMPDRV_CORE_TYPE_DMAC]			= {
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_DMAC_SLIM]	= {
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_PSCEXE]		= {
										{ .type = IMPDRV_IRQMASK_END,		.value = true	},
										{ .type = IMPDRV_IRQMASK_WUPCOVF,	.value = true	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  },
	[IMPDRV_CORE_TYPE_PSCOUT]		= {
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,	.value = false	}
									  },
	[IMPDRV_CORE_TYPE_CNN]			= {
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	},
										{ .type = IMPDRV_DUMMY_UINT32,		.value = false	}
									  }
}; // Need to fix too

const uint32_t param_by_attr_init_validator[10][IMPDRV_EXEPARAM_MAX] = {
	[IMPDRV_CORE_TYPE_INVALID]		= {
										0u,
										0u,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_IMP]			= {
										IMPDRV_CLBRK_ADDR_INVALID,
										IMPDRV_EXEPARAM_INVALID,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_OCV]			= {
										IMPDRV_CLBRK_ADDR_INVALID,
										IMPDRV_EXEPARAM_INVALID,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_DMAC]			= {
										IMPDRV_CLBRK_ADDR_INVALID,
										IMPDRV_EXEPARAM_INVALID,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_DMAC_SLIM]	= {
										IMPDRV_CLBRK_ADDR_INVALID,
										IMPDRV_EXEPARAM_INVALID,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_PSCEXE]		= {
										IMPDRV_CLBRK_ADDR_INVALID,
										IMPDRV_EXEPARAM_INVALID,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_CNN]			= {
										IMPDRV_CLBRK_ADDR_INVALID,
										IMPDRV_EXEPARAM_INVALID,
										0u,
										0u
									  },
	[IMPDRV_CORE_TYPE_DSP]			= {
										0u,
										0u,
										0u,
										0u
									  }
};

const st_impdrv_irq_group_t g_p_irq_param[5] = {
	[IMPDRV_IRQ_GROUP_INVALID]	= {
									.irq_group = IMPDRV_IRQ_GROUP_INVALID
								  },
	[IMPDRV_IRQ_GROUP_NONE]		= {
									.irq_group = IMPDRV_IRQ_GROUP_NONE,
#if !defined (V4H2)
									.group_core_num = ARRAY_SIZE(platform_soc),
#else
									.group_core_num = 18, /* DSP not support IRQ group */
#endif
									.group_core_info = {
											{ IMPDRV_CORE_TYPE_IMP,			0 },
											{ IMPDRV_CORE_TYPE_IMP,			1 },
											{ IMPDRV_CORE_TYPE_IMP,			2 },
											{ IMPDRV_CORE_TYPE_IMP,			3 },
											{ IMPDRV_CORE_TYPE_OCV,			0 },
											{ IMPDRV_CORE_TYPE_OCV,			1 },
											{ IMPDRV_CORE_TYPE_OCV,			2 },
											{ IMPDRV_CORE_TYPE_OCV,			3 },
											{ IMPDRV_CORE_TYPE_DMAC,		0 },
											{ IMPDRV_CORE_TYPE_DMAC,		1 },
											{ IMPDRV_CORE_TYPE_DMAC,		2 },
											{ IMPDRV_CORE_TYPE_DMAC,		3 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },
											{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
											{ IMPDRV_CORE_TYPE_CNN,			0 },
									}
								  },
	[IMPDRV_IRQ_GROUP_0]		= {
									.irq_group = IMPDRV_IRQ_GROUP_0,
#if !defined (V4H2)
									.group_core_num = ARRAY_SIZE(platform_soc),
#else
									.group_core_num = 18, /* DSP not support IRQ group */
#endif
									.group_core_info = {
											{ IMPDRV_CORE_TYPE_IMP,			0 },
											{ IMPDRV_CORE_TYPE_IMP,			1 },
											{ IMPDRV_CORE_TYPE_IMP,			2 },
											{ IMPDRV_CORE_TYPE_IMP,			3 },
											{ IMPDRV_CORE_TYPE_OCV,			0 },
											{ IMPDRV_CORE_TYPE_OCV,			1 },
											{ IMPDRV_CORE_TYPE_OCV,			2 },
											{ IMPDRV_CORE_TYPE_OCV,			3 },
											{ IMPDRV_CORE_TYPE_DMAC,		0 },
											{ IMPDRV_CORE_TYPE_DMAC,		1 },
											{ IMPDRV_CORE_TYPE_DMAC,		2 },
											{ IMPDRV_CORE_TYPE_DMAC,		3 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },
											{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
											{ IMPDRV_CORE_TYPE_CNN,			0 },
									}
								  },
	[IMPDRV_IRQ_GROUP_1]		= {
									.irq_group = IMPDRV_IRQ_GROUP_1,
#if !defined (V4H2)
									.group_core_num = ARRAY_SIZE(platform_soc),
#else
									.group_core_num = 18, /* DSP not support IRQ group */
#endif
									.group_core_info = {
											{ IMPDRV_CORE_TYPE_IMP,			0 },
											{ IMPDRV_CORE_TYPE_IMP,			1 },
											{ IMPDRV_CORE_TYPE_IMP,			2 },
											{ IMPDRV_CORE_TYPE_IMP,			3 },
											{ IMPDRV_CORE_TYPE_OCV,			0 },
											{ IMPDRV_CORE_TYPE_OCV,			1 },
											{ IMPDRV_CORE_TYPE_OCV,			2 },
											{ IMPDRV_CORE_TYPE_OCV,			3 },
											{ IMPDRV_CORE_TYPE_DMAC,		0 },
											{ IMPDRV_CORE_TYPE_DMAC,		1 },
											{ IMPDRV_CORE_TYPE_DMAC,		2 },
											{ IMPDRV_CORE_TYPE_DMAC,		3 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },
											{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
											{ IMPDRV_CORE_TYPE_CNN,			0 },
									}
								  },
	[IMPDRV_IRQ_GROUP_2]		= {
									.irq_group = IMPDRV_IRQ_GROUP_2,
#if !defined (V4H2)
									.group_core_num = ARRAY_SIZE(platform_soc),
#else
									.group_core_num = 18, /* DSP not support IRQ group */
#endif
									.group_core_info = {
											{ IMPDRV_CORE_TYPE_IMP,			0 },
											{ IMPDRV_CORE_TYPE_IMP,			1 },
											{ IMPDRV_CORE_TYPE_IMP,			2 },
											{ IMPDRV_CORE_TYPE_IMP,			3 },
											{ IMPDRV_CORE_TYPE_OCV,			0 },
											{ IMPDRV_CORE_TYPE_OCV,			1 },
											{ IMPDRV_CORE_TYPE_OCV,			2 },
											{ IMPDRV_CORE_TYPE_OCV,			3 },
											{ IMPDRV_CORE_TYPE_DMAC,		0 },
											{ IMPDRV_CORE_TYPE_DMAC,		1 },
											{ IMPDRV_CORE_TYPE_DMAC,		2 },
											{ IMPDRV_CORE_TYPE_DMAC,		3 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
											{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },
											{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
											{ IMPDRV_CORE_TYPE_CNN,			0 },
									}
								  }
};

static const st_impdrv_group_tbl_t s_impdrv_inten_group_table[IMPDRV_GROUP_MAX] =
{
	/** group_bit				group_reg	*/
	{	IMPDRV_DUMMY_UINT32,	IMPDRV_DUMMY_UINT32	   },  /**< Dummy for Index */
	{	IMPDRV_DUMMY_UINT32,	IMPDRV_DUMMY_UINT32	   },  /**< Dummy for Index */
	{	0x00040000U,			IMPDRV_REG_G00INTSEL   },  /** Group00. */
	{	0x00080000U,			IMPDRV_REG_G01INTSEL   },  /** Group01. */
	{	0x00100000U,			IMPDRV_REG_G02INTSEL   }   /** Group02. */
};

const st_impdrv_inten_tbl_t s_impdrv_inten_cnv_table[IMPDRV_INTEN_CORE_MAX] =
{
	/** coreType,				CoreNum,	bit		*/
	{	IMPDRV_CORE_TYPE_IMP,		0,	0x00000001U },
	{	IMPDRV_CORE_TYPE_IMP,		1,	0x00000002U },
	{	IMPDRV_CORE_TYPE_IMP,		2,	0x00000004U },
	{	IMPDRV_CORE_TYPE_IMP,		3,	0x00000008U },
	{	IMPDRV_CORE_TYPE_OCV,		0,	0x00000200U },
	{	IMPDRV_CORE_TYPE_OCV,		1,	0x00000400U },
	{	IMPDRV_CORE_TYPE_OCV,		2,	0x00000800U },
	{	IMPDRV_CORE_TYPE_OCV,		3,	0x00001000U },
	{	IMPDRV_CORE_TYPE_DMAC,		0,	0x00000020U },
	{	IMPDRV_CORE_TYPE_DMAC,		1,	0x00000040U },
	{	IMPDRV_CORE_TYPE_DMAC,		2,	0x00000080U },
	{	IMPDRV_CORE_TYPE_DMAC,		3,	0x00000100U },
	{	IMPDRV_CORE_TYPE_DMAC_SLIM, 0,	0x00004000U },
	{	IMPDRV_CORE_TYPE_DMAC_SLIM, 1,	0x00008000U },
	{	IMPDRV_CORE_TYPE_DMAC_SLIM, 2,	0x00010000U },
	{	IMPDRV_CORE_TYPE_DMAC_SLIM, 3,	0x00020000U },
	{	IMPDRV_CORE_TYPE_PSCEXE,	0,	0x00000010U },
	{	IMPDRV_CORE_TYPE_CNN,		0,	0x00002000U },

};

const st_impdrv_inten_reg_tbl_t s_impdrv_inten_reg_tbl[IMPDRV_INSTANCETABLE_NUM] =
{
	/** p_inten_group_table			p_inten_cnv_table			cnv_table_num */
	{ NULL,							NULL,					  IMPDRV_DUMMY_UINT32	 },	  /**< Dummy for Index */
	{ s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX	 },	  /**< IMPDRV_INSTANCE_0 */
	{ s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX	 },	  /**< IMPDRV_INSTANCE_1 */
	{ s_impdrv_inten_group_table,  s_impdrv_inten_cnv_table,  IMPDRV_INTEN_CORE_MAX	 }	  /**< IMPDRV_INSTANCE_2 */
};

/* core infos for instances */
static const st_impdrv_core_info_t core_info_for_ins0[] = {
	{ IMPDRV_CORE_TYPE_IMP,			0 },
	{ IMPDRV_CORE_TYPE_IMP,			3 },
	{ IMPDRV_CORE_TYPE_OCV,			0 },
	{ IMPDRV_CORE_TYPE_OCV,			3 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 }
};

static const st_impdrv_core_info_t core_info_for_ins1[] = {
	{ IMPDRV_CORE_TYPE_IMP,			1 },
	{ IMPDRV_CORE_TYPE_PSCEXE,		0 },
	{ IMPDRV_CORE_TYPE_OCV,			2 },
	{ IMPDRV_CORE_TYPE_DMAC,		0 },
	{ IMPDRV_CORE_TYPE_DMAC,		1 }
};

static const st_impdrv_core_info_t core_info_for_ins2[] = {
	{ IMPDRV_CORE_TYPE_IMP,			2 },
	{ IMPDRV_CORE_TYPE_OCV,			1 },
	{ IMPDRV_CORE_TYPE_CNN,			0 },
	{ IMPDRV_CORE_TYPE_DMAC,		2 },
	{ IMPDRV_CORE_TYPE_DMAC,		3 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },
	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 }
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
			/* do nothing */
			break;
		}
		case IT_IMPDRV_CMD_BUSIF_TEST_PATTERN:
		{
			/* do nothing */
			break;
		}
		case IT_IMPDRV_CMD_CONF_REG_CHECK:
		{
			if (IMPDRV_CORE_TYPE_IMP == core_type)
			{
				data_info.range	= ARRAY_SIZE(config_reg_check_IMP);
				data_info.info	= (void *)config_reg_check_IMP;
			}
			else if (IMPDRV_CORE_TYPE_DSP == core_type)
			{
				data_info.range	= ARRAY_SIZE(config_reg_check_DSP);
				data_info.info	= (void *)config_reg_check_DSP;
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
			else if (IMPDRV_CORE_TYPE_DMAC_SLIM == core_type)
			{
				data_info.range	= ARRAY_SIZE(module_stop_check_SlimDMAC);
				data_info.info	= (void *)module_stop_check_SlimDMAC;
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
			else if (IMPDRV_CORE_TYPE_DSP == core_type)
			{
				data_info.range	= ARRAY_SIZE(config_reg_check_DSP);
				data_info.info	= (void *)config_reg_check_DSP;
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
		case IT_IMPDRV_CMD_PLATFORM_SOC_NO_DMA:
		{
			core_list.size		= sizeof(platform_soc_no_dma);
			core_list.num		= ARRAY_SIZE(platform_soc_no_dma);
			core_list.core_info	= platform_soc_no_dma;
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
	list_2element(IMP, 0,	OCV,		0),
	list_2element(IMP, 1,	DMAC,		0),
	list_3element(IMP, 2,	PSCEXE,		0,	DMAC,		1),
	list_3element(IMP, 3,	OCV,		1,	CNN,		0),
	list_3element(IMP, 0,	IMP,		2,	OCV,		2),
	list_3element(OCV, 3,	DMAC,		2,	DMAC_SLIM,	0),
	list_3element(OCV, 2,	DMAC,		3,	DMAC_SLIM,	1),
	list_2element(OCV, 0,	DMAC,		3),
	list_endelement(0)
};

static sync_core_t sync_6_cores[] = {
	list_6element(OCV, 0,	OCV, 1,		OCV, 2,		OCV, 3,			IMP, 0,			IMP, 1),
	list_6element(IMP, 0,	IMP, 1,		IMP, 2,		IMP, 3,			PSCEXE, 0,		DMAC, 0),
	list_6element(DMAC, 0,	DMAC, 1,	DMAC, 3,	DMAC_SLIM, 0,	DMAC_SLIM, 1,	CNN, 0),
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
	e_impdrv_irq_group_t			*const p_irq_group,
	const e_impdrv_instance_t		instance_num
)
{
	e_impdrv_errorcode_t	ercd = IMPDRV_EC_OK;

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

uint32_t imp_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	/** initialize local variables */
	uint32_t	inten_val = 0U;
	uint32_t    clbrk_val;
	uint32_t	i;

	/** Input parameter confirmation process */
	if (NULL != irq_mask)
	{
		/** Set interrupt for IMP core by default */
		inten_val = IMPDRV_IMP_INTSTS_MASKCPU;
		inten_val |= (IMPDRV_IMP_INTSTS_TRAP | IMPDRV_IMP_INTSTS_IER | IMPDRV_IMP_INTSTS_INT);

		inten_val |= IMPDRV_IMP_INTSTS_CLBRK;

		/** Set interrupt for IMP core */
		for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
		{
			if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_IMP_INTSTS_WUP;
			}
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
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	/** initialize local variables */
	uint32_t	inten_val = 0U;
	uint32_t	i;

	/** Input parameter confirmation process */
	if (NULL != irq_mask)
	{
		/** Set interrupt for OCV core by default */
		inten_val = (IMPDRV_OCV_SR1_TRAP | IMPDRV_OCV_SR1_IER | IMPDRV_OCV_SR1_INT);

		inten_val |= IMPDRV_OCV_SR1_CLBRK;

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
			if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_OCV_SR1_WUPCOVF;
			}
		}
	}

	/** Set data in return value */
	return inten_val;
}

uint32_t dmac_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	/** initialize local variables */
	uint32_t	inten_val = 0U;
	uint32_t	i;

	/** Input parameter confirmation process */
	if (NULL != irq_mask)
	{
		/** Set interrupt for DMAC core by default */
		inten_val = (IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT);

		inten_val |= IMPDRV_DMA_SR_CLBRK;

		/** Set interrupt for DMAC core */
		for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
		{
			if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_DMA_SR_WUP;
			}
		}
	}

	/** Set data in return value */
	return inten_val;
}

uint32_t dmacs_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	/** initialize local variables */
	uint32_t	inten_val = 0U;
	uint32_t	i;

	/** Input parameter confirmation process */
	if (NULL != irq_mask)
	{
		/** Set interrupt for Slim-DMAC core by default */
		inten_val = (IMPDRV_DMAS_SR_TRAP | IMPDRV_DMAS_SR_IER | IMPDRV_DMAS_SR_INT);

		inten_val |= IMPDRV_DMAS_SR_CLBRK;

		/** Set interrupt for Slim-DMAC core */
		for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
		{
			if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_DMAS_SR_WUP;
			}
		}
	}

	/** Set data in return value */
	return inten_val;
}

uint32_t psc_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	/** initialize local variables */
	uint32_t	inten_val = 0U;
	uint32_t	i;

	/** Input parameter confirmation process */
	if (NULL != irq_mask)
	{
		/** Set interrupt for PSC core by default */
		inten_val = (IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT);

		inten_val |= IMPDRV_PSC_SR_CLBRK;

		/** Set interrupt for PSC core */
		for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
		{
			if (((uint32_t)IMPDRV_IRQMASK_END == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_PSC_SR_TEND;
			}
			if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_PSC_SR_WUP;
			}
		}
	}

	/** Set data in return value */
	return inten_val;
}

uint32_t cnn_get_inten_val(
	const bool		irq_mask[IMPDRV_IRQMASK_MAX]
)
{
	/** initialize local variables */
	uint32_t	inten_val = 0U;
	uint32_t	i;

	/** Input parameter confirmation process */
	if (NULL != irq_mask)
	{
		/** Set interrupt for imp core by default */
		inten_val = (IMPDRV_CNN_SR_TRAP | IMPDRV_CNN_SR_IER | IMPDRV_CNN_SR_INT);

		inten_val |= IMPDRV_CNN_SR_CLBRK;
		
		/** Set interrupt for imp core */
		for (i = 0U; IMPDRV_IRQMASK_MAX > i; i++)
		{
			if (((uint32_t)IMPDRV_IRQMASK_MSCO == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_CNN_SR_MSCO;
			}
			if (((uint32_t)IMPDRV_IRQMASK_WUPCOVF == i) && (false == irq_mask[i]))
			{
				inten_val |= IMPDRV_CNN_SR_WUPCOVF;
			}
		}
	}

	/** Set data in return value */
	return inten_val;
}

void cnv_int_core_bit(
	const st_impdrv_core_info_t		*const p_core_info,
	const uint32_t					use_core_num,
	const st_impdrv_inten_tbl_t		*const p_inten_cnv_table,
	const uint32_t					cnv_table_num,
	uint32_t						*const p_val
)
{
	/** initialize local variables */
	bool					hit;
	uint32_t				i;
	uint32_t				j;

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
	const st_impdrv_corectl_t	*const p_corectl,
	uint32_t					syncc_val[IMPDRV_SYNCC_REGNUM]
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
                    | IMPDRV_OCV_SR1_USIER | IMPDRV_OCV_SR1_USINT | IMPDRV_OCV_SR1_WUPCOVF
                    | IMPDRV_OCV_SR1_CLBRK;
			break;
		}
		case IMPDRV_CORE_TYPE_DMAC:
		{
			mask_val = IMPDRV_DMA_SR_TRAP | IMPDRV_DMA_SR_IER | IMPDRV_DMA_SR_INT |
                    IMPDRV_DMA_SR_WUP | IMPDRV_DMA_SR_TEND | IMPDRV_DMA_SR_CLBRK;
			break;
		}
		case IMPDRV_CORE_TYPE_DMAC_SLIM:
		{
			mask_val = IMPDRV_DMAS_SR_TRAP | IMPDRV_DMAS_SR_IER | IMPDRV_DMAS_SR_INT |
                    IMPDRV_DMAS_SR_WUP | IMPDRV_DMAS_SR_TEND | IMPDRV_DMAS_SR_CLBRK;
			break;
		}
		case IMPDRV_CORE_TYPE_PSCEXE:
		{
			mask_val = IMPDRV_PSC_SR_TRAP | IMPDRV_PSC_SR_IER | IMPDRV_PSC_SR_INT
                    | IMPDRV_PSC_SR_TEND | IMPDRV_PSC_SR_WUP | IMPDRV_PSC_SR_CLBRK;
			break;
		}
		case IMPDRV_CORE_TYPE_CNN:
		{
			mask_val = IMPDRV_CNN_SR_TRAP | IMPDRV_CNN_SR_IER | IMPDRV_CNN_SR_INT
                    | IMPDRV_CNN_SR_CLBRK;
			break;
		}
		default:
		{
			break;
		}
	}

	return mask_val;
}
