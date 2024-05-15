#include <stdio.h>
#include <string.h>
#include "it_common.h"
#include "it_stub.h"

#define READ_REG_TIME       (1000U)
#define MEMORY_NUM          (20U)

extern e_impdrv_errorcode_t impdrv_cmnctl_reg_set_checksum(void);

void (*g_int_handler[4])(
    osal_device_handle_t    device_handle,
    const uint64_t          irq_channel,
    void*                   irq_arg
);

// common macro for stub function definition
#define STUB_VAR_DEFINE(type, func)   static volatile type g_ret_##func;
#define STUB_SET_FUNC_DEFINE(type, func)  \
    void it_stub_set_##func(type ret)  \
    {   \
        g_ret_##func = ret;\
    }

#define STUB_COMMON_DEFINE(type, func)    \
    STUB_VAR_DEFINE(type, func)       \
    STUB_SET_FUNC_DEFINE(type, func)

STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_InterruptRegisterIsr)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_InterruptEnableIsr)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_InterruptDisableIsr)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_InterruptUnregisterIsr)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_PmSetPolicy)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_PmGetState)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_PmGetPolicy)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_PmSetResetState)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_PmGetResetState)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_PmWaitForState)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_ThsyncMutexCreate)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_ThsyncMutexDestroy)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_ThsyncMutexLockForTimePeriod)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_ThsyncMutexUnlock)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_ThsyncMemoryBarrier)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_MmngrFlush)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_MmngrInvalidate)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_MqCreate)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_MqReset)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_MqDelete)
STUB_COMMON_DEFINE(e_osal_return_t, R_OSAL_MqSendForTimePeriod)
STUB_COMMON_DEFINE(e_impdrv_errorcode_t, impdrv_cmnctl_reg_set_checksum)

static volatile e_osal_return_t g_ret_R_OSAL_IoRead32[READ_REG_TIME];
static volatile e_osal_return_t g_ret_R_OSAL_IoWrite32;
static volatile e_osal_return_t g_ret_R_OSAL_IoDeviceOpen;
static volatile e_osal_return_t g_ret_R_OSAL_IoDeviceClose;
static volatile e_osal_return_t g_ret_R_OSAL_PmSetRequiredState;
static volatile e_osal_return_t g_ret_R_OSAL_PmGetRequiredState;
static volatile e_osal_return_t g_ret_R_OSAL_MmngrGetSize[MEMORY_NUM];
static volatile e_osal_return_t g_ret_R_OSAL_MmngrGetCpuPtr[MEMORY_NUM];
static volatile e_osal_return_t g_ret_R_OSAL_MmngrGetHwAddr[MEMORY_NUM];
static volatile e_osal_return_t g_ret_R_OSAL_MqReceiveForTimePeriod;

static volatile uint32_t g_chk_core_idx[READ_REG_TIME];
static volatile uint64_t g_chk_irq_ch[READ_REG_TIME];
static volatile uint32_t g_read_chk_offset;
static volatile bool g_chk_offset_flag;
static volatile uint32_t g_read_value[READ_REG_TIME];
static volatile uint32_t g_read_back_value;
volatile uint32_t g_write_value[WRITE_REG_TIME];
volatile bool update_read_back_flag;
static volatile uint32_t g_write_chk_offset;
static volatile uint32_t g_read_reg_cnt;
static volatile char *g_R_OSAL_IoDeviceOpen_devid_list;
static volatile char *g_R_OSAL_IoDeviceClose_devid_list;
static volatile char *g_R_OSAL_PmSetRequiredState_devid_list;
static volatile e_osal_pm_required_state_t g_set_pm_required_state;
static volatile e_osal_pm_required_state_t g_get_pm_required_state;
static volatile size_t g_mem_size[MEMORY_NUM];
volatile uint32_t g_mem_size_cnt;
static volatile void *g_cpu_addr[MEMORY_NUM];
volatile uint32_t g_logical_addr_cnt;
static volatile uintptr_t g_hw_addr[MEMORY_NUM];
volatile uint32_t g_hw_addr_cnt;
static uint32_t g_buffer;
volatile bool init_test_flag = false;
volatile bool start_test_flag = false;
volatile bool stop_test_flag = false;
volatile bool quit_test_flag = false;
volatile bool dsp_register_cbfunc_flag[4] = { false, false, false, false };
volatile bool dsp_unregister_cbfunc_flag[4] = { false, false, false, false };
static uint32_t write_cnt_vdsp_icu_lock = 0u;
static uint32_t write_cnt_d_sft_scpd_0 = 0u;

static uint32_t imp_top_handle;
static uint32_t hwrsc_handle[5];
static uint32_t dta_0_handle;
static uint32_t imp_0_handle;
static uint32_t imp_1_handle;
static uint32_t imp_2_handle;
static uint32_t imp_3_handle;
static uint32_t imp_4_handle;
static uint32_t imp_slim_handle;
static uint32_t ocv_0_handle;
static uint32_t ocv_1_handle;
static uint32_t ocv_2_handle;
static uint32_t ocv_3_handle;
static uint32_t ocv_4_handle;
static uint32_t ocv_5_handle;
static uint32_t ocv_6_handle;
static uint32_t ocv_7_handle;
static uint32_t dmac_0_handle;
static uint32_t dmac_1_handle;
static uint32_t dmac_2_handle;
static uint32_t dmac_3_handle;
static uint32_t dmac_slim_0_handle;
static uint32_t dmac_slim_1_handle;
static uint32_t dmac_slim_2_handle;
static uint32_t dmac_slim_3_handle;
static uint32_t pscexe_0_handle;
static uint32_t pscexe_1_handle;
static uint32_t cnn_0_handle;
static uint32_t cnn_1_handle;
static uint32_t cnn_2_handle;
static uint32_t vdsp_0_handle[8];
static uint32_t vdsp_1_handle[8];
static uint32_t vdsp_2_handle[8];
static uint32_t vdsp_3_handle[8];
osal_device_handle_t g_osal_device_handle[4];
osal_mq_handle_t mq_handle;

typedef struct version_register {
	uint32_t	offset;
	uint32_t	reg_val
} version_register_t;

typedef struct private_properties {
	char		*name;
	st_impdrv_core_info_t	core_info;
	version_register_t		version_reg;
	osal_device_handle_t	handle
} private_properties_t;

static const private_properties_t core_properties[] =
{
	{	"imp_top_00",		{ 0,							0 },	{ 0,													0				},	(osal_device_handle_t)&imp_top_handle		},

	{	"imp_spmi_00",		{ 0,							0 },	{ 0,													0				},	(osal_device_handle_t)&hwrsc_handle[0]		},
	{	"imp_spmc_00",		{ 0,							0 },	{ 0,													0				},	(osal_device_handle_t)&hwrsc_handle[1]		},
	{	"imp_reg_spmi_00",	{ 0,							0 },	{ 0,													0				},	(osal_device_handle_t)&hwrsc_handle[2]		},
	{	"imp_reg_spmc_00",	{ 0,							0 },	{ 0,													0				},	(osal_device_handle_t)&hwrsc_handle[3]		},
#if defined (V4H2)
	{	"imp_spmc_01",		{ 0,							0 },	{ 0,													0				},	(osal_device_handle_t)&hwrsc_handle[4]		},
#endif

	{	"imp_dta_00",		{ IMPDRV_CORE_TYPE_DTA,			0 },	{ 0,													0				},	(osal_device_handle_t)&dta_0_handle			},

	{	"imp_00",			{ IMPDRV_CORE_TYPE_IMP,			0 },	{ IMPDRV_REG_IMP_VCR,									IMPDRV_IMP_VCR	},	(osal_device_handle_t)&imp_0_handle			},
	{	"imp_01",			{ IMPDRV_CORE_TYPE_IMP,			1 },	{ IMPDRV_REG_IMP_VCR,									IMPDRV_IMP_VCR	},	(osal_device_handle_t)&imp_1_handle			},
	{	"imp_02",			{ IMPDRV_CORE_TYPE_IMP,			2 },	{ IMPDRV_REG_IMP_VCR,									IMPDRV_IMP_VCR	},	(osal_device_handle_t)&imp_2_handle			},
	{	"imp_03",			{ IMPDRV_CORE_TYPE_IMP,			3 },	{ IMPDRV_REG_IMP_VCR,									IMPDRV_IMP_VCR	},	(osal_device_handle_t)&imp_3_handle			},
	{	"imp_04",			{ IMPDRV_CORE_TYPE_IMP,			4 },	{ IMPDRV_REG_IMP_VCR,									IMPDRV_IMP_VCR	},	(osal_device_handle_t)&imp_4_handle			},
#if defined (V3H1) || defined (V3H2)
	{	"imp_slim_00",		{ IMPDRV_CORE_TYPE_IMP_SLIM,	0 },	{ IMPDRV_REG_IMPS_VCR,									IMPDRV_IMPS_VCR },	(osal_device_handle_t)&imp_slim_handle		},
#endif
	{	"imp_ocv_00",		{ IMPDRV_CORE_TYPE_OCV,			0 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_0_handle			},
	{	"imp_ocv_01",		{ IMPDRV_CORE_TYPE_OCV,			1 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_1_handle			},
	{	"imp_ocv_02",		{ IMPDRV_CORE_TYPE_OCV,			2 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_2_handle			},
	{	"imp_ocv_03",		{ IMPDRV_CORE_TYPE_OCV,			3 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_3_handle			},
	{	"imp_ocv_04",		{ IMPDRV_CORE_TYPE_OCV,			4 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_4_handle			},
	{	"imp_ocv_05",		{ IMPDRV_CORE_TYPE_OCV,			5 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_5_handle			},
	{	"imp_ocv_06",		{ IMPDRV_CORE_TYPE_OCV,			6 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_6_handle			},
	{	"imp_ocv_07",		{ IMPDRV_CORE_TYPE_OCV,			7 },	{ IMPDRV_REG_OCV_VCR0,									IMPDRV_OCV_VCR	},	(osal_device_handle_t)&ocv_7_handle			},

	{	"imp_dma_00",		{ IMPDRV_CORE_TYPE_DMAC,		0 },	{ IMPDRV_REG_DMA_VCR,									IMPDRV_DMA_VCR	},	(osal_device_handle_t)&dmac_0_handle		},
	{	"imp_dma_01",		{ IMPDRV_CORE_TYPE_DMAC,		1 },	{ IMPDRV_REG_DMA_VCR,									IMPDRV_DMA_VCR	},	(osal_device_handle_t)&dmac_1_handle		},
#if defined (V3H2) || defined (V4H) || defined (V4H2)
	{	"imp_dma_slim_00",	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	0 },	{ IMPDRV_REG_DMAS_VCR,									IMPDRV_DMAS_VCR	},	(osal_device_handle_t)&dmac_slim_0_handle	},
	{	"imp_dma_slim_01",	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	1 },	{ IMPDRV_REG_DMAS_VCR + IMPDRV_REG_DMA_THREAD_OFFSET,	IMPDRV_DMAS_VCR	},	(osal_device_handle_t)&dmac_slim_1_handle	},
	{	"imp_dma_slim_02",	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	2 },	{ IMPDRV_REG_DMAS_VCR,									IMPDRV_DMAS_VCR	},	(osal_device_handle_t)&dmac_slim_2_handle	},
	{	"imp_dma_slim_03",	{ IMPDRV_CORE_TYPE_DMAC_SLIM,	3 },	{ IMPDRV_REG_DMAS_VCR + IMPDRV_REG_DMA_THREAD_OFFSET,	IMPDRV_DMAS_VCR	},	(osal_device_handle_t)&dmac_slim_3_handle	},
#endif
	{	"imp_psc_00",		{ IMPDRV_CORE_TYPE_PSCEXE,		0 },	{ IMPDRV_REG_PSC_VCR,									IMPDRV_PSC_VCR	},	(osal_device_handle_t)&pscexe_0_handle		},
	{	"imp_psc_01",		{ IMPDRV_CORE_TYPE_PSCEXE,		1 },	{ IMPDRV_REG_PSC_VCR,									IMPDRV_PSC_VCR	},	(osal_device_handle_t)&pscexe_1_handle		},

	{	"imp_cnn_00",		{ IMPDRV_CORE_TYPE_CNN,			0 },	{ IMPDRV_REG_CNN_VCR,									IMPDRV_CNN_VCR	},	(osal_device_handle_t)&cnn_0_handle			},
	{	"imp_cnn_01",		{ IMPDRV_CORE_TYPE_CNN,			1 },	{ IMPDRV_REG_CNN_VCR,									IMPDRV_CNN_VCR	},	(osal_device_handle_t)&cnn_1_handle			},
	{	"imp_cnn_02",		{ IMPDRV_CORE_TYPE_CNN,			2 },	{ IMPDRV_REG_CNN_VCR,									IMPDRV_CNN_VCR	},	(osal_device_handle_t)&cnn_2_handle			},

	{	"vdsp00_00",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[0]		},
	{	"vdsp00_01",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[1]		},
	{	"vdsp00_02",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[2]		},
	{	"vdsp00_03",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[3]		},
	{	"vdsp00_04",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[4]		},
	{	"vdsp00_05",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[5]		},
	{	"vdsp00_06",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[6]		},
	{	"vdsp00_07",		{ IMPDRV_CORE_TYPE_DSP,			0 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_0_handle[7]		},
	{	"vdsp01_00",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[0]		},
	{	"vdsp01_01",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[1]		},
	{	"vdsp01_02",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[2]		},
	{	"vdsp01_03",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[3]		},
	{	"vdsp01_04",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[4]		},
	{	"vdsp01_05",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[5]		},
	{	"vdsp01_06",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[6]		},
	{	"vdsp01_07",		{ IMPDRV_CORE_TYPE_DSP,			1 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_1_handle[7]		},
	{	"vdsp02_00",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[0]		},
	{	"vdsp02_01",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[1]		},
	{	"vdsp02_02",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[2]		},
	{	"vdsp02_03",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[3]		},
	{	"vdsp02_04",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[4]		},
	{	"vdsp02_05",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[5]		},
	{	"vdsp02_06",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[6]		},
	{	"vdsp02_07",		{ IMPDRV_CORE_TYPE_DSP,			2 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_2_handle[7]		},
	{	"vdsp03_00",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[0]		},
	{	"vdsp03_01",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[1]		},
	{	"vdsp03_02",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[2]		},
	{	"vdsp03_03",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[3]		},
	{	"vdsp03_04",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[4]		},
	{	"vdsp03_05",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[5]		},
	{	"vdsp03_06",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[6]		},
	{	"vdsp03_07",		{ IMPDRV_CORE_TYPE_DSP,			3 },	{ 0,													0				},	(osal_device_handle_t)&vdsp_3_handle[7]		}
};

static volatile uint32_t mutex_handle;
volatile bool it_stub_check_version_reg_val_flag;

void it_stub_reset(void)
{
	memset(g_chk_core_idx, 0, sizeof(g_chk_core_idx));
	g_read_chk_offset = 0;
    memset(g_chk_irq_ch, 0, sizeof(g_chk_irq_ch));
    memset(g_read_value, 0, sizeof(g_read_value));
    memset(g_write_value, 0, sizeof(g_write_value));
    g_read_reg_cnt = 0;
    g_write_chk_offset = RCVDRV_IMP_OFFSET_ALL;
	g_read_back_value = 0;
	g_chk_offset_flag = false;
	g_mem_size_cnt = 0;
    g_logical_addr_cnt = 0;
    g_hw_addr_cnt = 0;
	write_cnt_vdsp_icu_lock = 0U;
	write_cnt_d_sft_scpd_0 = 0U;

	g_ret_R_OSAL_ThsyncMutexCreate = OSAL_RETURN_OK;
	g_ret_R_OSAL_ThsyncMutexDestroy = OSAL_RETURN_OK;
	g_ret_R_OSAL_ThsyncMutexLockForTimePeriod = OSAL_RETURN_OK;
	g_ret_R_OSAL_ThsyncMutexUnlock = OSAL_RETURN_OK;
	g_ret_R_OSAL_InterruptRegisterIsr = OSAL_RETURN_OK;
	g_ret_R_OSAL_InterruptUnregisterIsr = OSAL_RETURN_OK;
	g_ret_R_OSAL_InterruptEnableIsr = OSAL_RETURN_OK;
	g_ret_R_OSAL_InterruptDisableIsr = OSAL_RETURN_OK;
	g_R_OSAL_IoDeviceOpen_devid_list = NULL;
	g_ret_R_OSAL_IoDeviceOpen = OSAL_RETURN_OK;
	g_R_OSAL_IoDeviceClose_devid_list = NULL;
	g_ret_R_OSAL_IoDeviceClose = OSAL_RETURN_OK;
	memset(g_ret_R_OSAL_IoRead32, OSAL_RETURN_OK, sizeof(g_ret_R_OSAL_IoRead32));
	it_stub_check_version_reg_val_flag = false;
	g_ret_R_OSAL_IoWrite32 = OSAL_RETURN_OK;
	update_read_back_flag = false;
	g_ret_R_OSAL_PmSetRequiredState = OSAL_RETURN_OK;
	g_R_OSAL_PmSetRequiredState_devid_list = NULL;
	g_set_pm_required_state = OSAL_PM_REQUIRED_STATE_REQUIRED;
	g_ret_R_OSAL_PmGetRequiredState = OSAL_RETURN_OK;
	g_get_pm_required_state = OSAL_PM_REQUIRED_STATE_RELEASED;
	g_ret_R_OSAL_PmGetState = OSAL_RETURN_OK;
	g_ret_R_OSAL_PmSetPolicy = OSAL_RETURN_OK;
	g_ret_R_OSAL_PmGetPolicy = OSAL_RETURN_OK;
	g_ret_R_OSAL_PmSetResetState = OSAL_RETURN_OK;
	g_ret_R_OSAL_PmGetResetState = OSAL_RETURN_OK;
	g_ret_R_OSAL_PmWaitForState = OSAL_RETURN_OK;
	g_ret_R_OSAL_ThsyncMemoryBarrier = OSAL_RETURN_OK;
	memset(g_ret_R_OSAL_MmngrGetSize, OSAL_RETURN_OK, sizeof(g_ret_R_OSAL_MmngrGetSize));
    memset(g_mem_size, 0, sizeof(g_mem_size));
    memset(g_ret_R_OSAL_MmngrGetCpuPtr, OSAL_RETURN_OK, sizeof(g_ret_R_OSAL_MmngrGetCpuPtr));
    for (int8_t i = 0; i < MEMORY_NUM; i++)
    {
        g_cpu_addr[i] = NULL;
    }
    memset(g_ret_R_OSAL_MmngrGetHwAddr, OSAL_RETURN_OK, sizeof(g_ret_R_OSAL_MmngrGetHwAddr));
    memset(g_hw_addr, 0, sizeof(g_hw_addr));
	g_ret_R_OSAL_MmngrFlush = OSAL_RETURN_OK;
	g_ret_R_OSAL_MmngrInvalidate = OSAL_RETURN_OK;
	g_ret_R_OSAL_MqCreate = OSAL_RETURN_OK;
	g_ret_R_OSAL_MqReset = OSAL_RETURN_OK;
	g_ret_R_OSAL_MqDelete = OSAL_RETURN_OK;
	g_ret_R_OSAL_MqReceiveForTimePeriod = OSAL_RETURN_OK;
	g_buffer = 0;
	g_ret_R_OSAL_MqSendForTimePeriod = OSAL_RETURN_OK;
	g_ret_impdrv_cmnctl_reg_set_checksum = IMPDRV_EC_OK;
}

/*************************************************
 * IMP Driver Stub function setting
 * **********************************************/
void it_stub_set_R_OSAL_IoRead32(e_osal_return_t ret, uint32_t index, uint32_t read_value)
{
	g_ret_R_OSAL_IoRead32[index] = ret;
	g_read_value[index] = read_value;
}

void it_stub_set_R_OSAL_IoWrite32(e_osal_return_t ret, uint32_t offset, uint32_t read_back)
{
	g_write_chk_offset = offset;
	g_ret_R_OSAL_IoWrite32 = ret;
	g_read_back_value = read_back;
}

void it_stub_set_R_OSAL_IoDeviceOpen(e_osal_return_t ret, const char *const type)
{
	g_R_OSAL_IoDeviceOpen_devid_list = type;
	g_ret_R_OSAL_IoDeviceOpen = ret;
}

void it_stub_set_R_OSAL_IoDeviceClose(e_osal_return_t ret, const char *const type)
{
	g_R_OSAL_IoDeviceClose_devid_list = type;
	g_ret_R_OSAL_IoDeviceClose = ret;
}

void it_stub_set_R_OSAL_PmSetRequiredState(e_osal_return_t ret, const char *const type, e_osal_pm_required_state_t pm_required_state)
{
	g_R_OSAL_PmSetRequiredState_devid_list = type;
	g_set_pm_required_state = pm_required_state;
	g_ret_R_OSAL_PmSetRequiredState = ret;
}

void it_stub_set_R_OSAL_PmGetRequiredState(e_osal_return_t ret, e_osal_pm_required_state_t pm_required_state)
{
	g_get_pm_required_state = pm_required_state;
	g_ret_R_OSAL_PmGetRequiredState = ret;
}

void it_stub_set_R_OSAL_MmngrGetSize(e_osal_return_t ret, uint32_t index, size_t mem_size)
{
    g_ret_R_OSAL_MmngrGetSize[index] = ret;
    g_mem_size[index] = mem_size;
}

void it_stub_set_R_OSAL_MmngrGetCpuPtr(e_osal_return_t ret, uint32_t index, void *cpu_addr)
{
    g_ret_R_OSAL_MmngrGetCpuPtr[index] = ret;
    g_cpu_addr[index] = cpu_addr;
}

void it_stub_set_R_OSAL_MmngrGetHwAddr(e_osal_return_t ret, uint32_t index, uint32_t phys_addr)
{
    g_ret_R_OSAL_MmngrGetHwAddr[index] = ret;
    g_hw_addr[index] = (uintptr_t)phys_addr;
}

void it_stub_set_R_OSAL_MqReceiveForTimePeriod(e_osal_return_t ret, uint32_t core_num)
{
    g_ret_R_OSAL_MqReceiveForTimePeriod = ret;
    g_buffer = core_num;
}

/***********************************************************************************
 * Stub functions
 **********************************************************************************/
e_osal_return_t stub_R_OSAL_ThsyncMutexCreate(osal_mutex_id_t mutex_id, osal_mutex_handle_t *const p_handle)
{
	if ((OSAL_RETURN_OK == g_ret_R_OSAL_ThsyncMutexCreate) && (p_handle != NULL))
	{
		*p_handle = (osal_mutex_handle_t)&mutex_handle;
	}

	return g_ret_R_OSAL_ThsyncMutexCreate;
}

e_osal_return_t stub_R_OSAL_ThsyncMutexDestroy(osal_mutex_handle_t handle)
{
	return g_ret_R_OSAL_ThsyncMutexDestroy;
}

e_osal_return_t stub_R_OSAL_ThsyncMutexLockForTimePeriod(osal_mutex_handle_t handle, osal_milli_sec_t time_period)
{
	return g_ret_R_OSAL_ThsyncMutexLockForTimePeriod;
}

e_osal_return_t stub_R_OSAL_ThsyncMutexUnlock(osal_mutex_handle_t handle)
{
	return g_ret_R_OSAL_ThsyncMutexUnlock;
}

e_osal_return_t stub_R_OSAL_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value,
													p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg)
{
	if (init_test_flag == true)
	{
		g_osal_device_handle[0] = handle;
		g_int_handler[0] = (void *)irqHandlerFunction;
	}

	if (start_test_flag == true)
	{
		g_osal_device_handle[1] = handle;
		g_int_handler[1] = (void *)irqHandlerFunction;
	}

	for (int32_t i = 0; i < ARRAY_SIZE(dsp_register_cbfunc_flag); i++)
	{
		if (true == dsp_register_cbfunc_flag[i])
		{
			g_osal_device_handle[i] = handle;
			g_int_handler[i] = (void *)irqHandlerFunction;
		}
	}

	return g_ret_R_OSAL_InterruptRegisterIsr;
}

e_osal_return_t stub_R_OSAL_InterruptUnregisterIsr(osal_device_handle_t handle, uint64_t irq_channel, p_osal_isr_func_t irqHandlerFunction)
{
	if (stop_test_flag == true)
	{
		g_osal_device_handle[1] = NULL;
		g_int_handler[1] = NULL;
	}

	if (quit_test_flag == true)
	{
		g_osal_device_handle[0] = NULL;
		g_int_handler[0] = NULL;
	}

	for (int32_t i = 0; i < ARRAY_SIZE(dsp_unregister_cbfunc_flag); i++)
	{
		if (true == dsp_unregister_cbfunc_flag[i])
		{
			g_osal_device_handle[i] = NULL;
			g_int_handler[i] = NULL;
		}
	}

	return g_ret_R_OSAL_InterruptUnregisterIsr;
}

e_osal_return_t stub_R_OSAL_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
	return g_ret_R_OSAL_InterruptEnableIsr;
}

e_osal_return_t stub_R_OSAL_InterruptDisableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
	return g_ret_R_OSAL_InterruptDisableIsr;
}

e_osal_return_t stub_R_OSAL_IoDeviceOpen(const char* p_id, osal_device_handle_t* const p_handle)
{
	e_osal_return_t ret = OSAL_RETURN_OK;

	if ((NULL != g_R_OSAL_IoDeviceOpen_devid_list) && (OSAL_RETURN_OK != g_ret_R_OSAL_IoDeviceOpen))
	{
		if (strstr(p_id, g_R_OSAL_IoDeviceOpen_devid_list))
		{
			ret = g_ret_R_OSAL_IoDeviceOpen;
		}
	}

	if (OSAL_RETURN_OK == ret)
	{
		for (uint32_t i = 0; i < ARRAY_SIZE(core_properties); i++)
		{
			if (strstr(p_id, core_properties[i].name))
			{
				*p_handle = core_properties[i].handle;
				break;
			}
		}
	}

	return ret;
}

e_osal_return_t stub_R_OSAL_IoDeviceClose(osal_device_handle_t handle)
{
	e_osal_return_t ret = OSAL_RETURN_OK;

	if (OSAL_RETURN_OK != g_ret_R_OSAL_IoDeviceClose)
	{
		for (uint32_t i = 0; i < ARRAY_SIZE(core_properties); i++)
		{
			if (strstr(g_R_OSAL_IoDeviceClose_devid_list, core_properties[i].name) && (handle == core_properties[i].handle))
			{
				ret = g_ret_R_OSAL_IoDeviceClose;
				break;
			}
		}
	}

	return ret;
}

e_osal_return_t stub_R_OSAL_IoRead32(osal_device_handle_t handle, uintptr_t offset, uint32_t* const p_data)
{
	e_osal_return_t ret = g_ret_R_OSAL_IoRead32[g_read_reg_cnt];

	// printf("\t\t\t\t\t%s: cnt = %d\n", __func__, g_read_reg_cnt);
    if (OSAL_RETURN_OK == g_ret_R_OSAL_IoRead32[g_read_reg_cnt])
    {
        if (NULL != p_data)
		{
            *p_data = g_read_value[g_read_reg_cnt];
        }
    }

    g_read_reg_cnt++;

    return ret;
}

e_osal_return_t stub_R_OSAL_IoWrite32(osal_device_handle_t handle, uintptr_t offset, uint32_t data)
{
	e_osal_return_t ret = OSAL_RETURN_OK;

	if (g_ret_R_OSAL_IoWrite32 != OSAL_RETURN_OK)
    {
		if (RCVDRV_IMP_OFFSET_ALL == g_write_chk_offset)
		{
			ret = g_ret_R_OSAL_IoWrite32;
		}
		else
		{
			for (uint32_t i = 0; i < ARRAY_SIZE(core_properties); i++)
			{
				if ((handle == core_properties[i].handle) && (offset == g_write_chk_offset))
				{
					ret = g_ret_R_OSAL_IoWrite32;
					break;
				}
			}
		}
	}
	else
	{
		/* Set DSP AoU related regsiter */
		/* MSS_DMBA */
		if (offset == 0x80600U)
		{
			g_write_value[WRITE_REG_IDX_MSS_DMBA] = data;
		}
		/* P_ADD0_ATT0 */
		else if (offset == 0x80424U)
		{
			g_write_value[WRITE_REG_IDX_P_ADD0_ATT0] = data;
		}
		/* P_ADD2_ATT0 */
		else if (offset == 0x80434U)
		{
			g_write_value[WRITE_REG_IDX_P_ADD2_ATT0] = data;
		}
		/* P_ADD3_START */
		else if (offset == 0x80438U)
		{
			g_write_value[WRITE_REG_IDX_P_ADD3_START] = data;
		}
		/* P_ADD3_ATT0 */
		else if (offset == 0x8043CU)
		{
			g_write_value[WRITE_REG_IDX_P_ADD3_ATT0] = data;
		}
		/* P_ADD4_START */
		else if (offset == 0x80440U)
		{
			g_write_value[WRITE_REG_IDX_P_ADD4_START] = data;
		}
		/* D_DDCL */
		else if (offset == 0x8068CU)
		{
			g_write_value[WRITE_REG_IDX_D_DDCL] = data;
		}
		/* VDSP_ICU_LOCK */
		else if (offset == 0xC0004U)
		{
			if (write_cnt_vdsp_icu_lock == 0U)
			{
				g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK] = data;
			}
			else if (write_cnt_vdsp_icu_lock == 1U)
			{
				g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_2ND] = data;
			}
			else if (write_cnt_vdsp_icu_lock == 2U)
			{
				g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_3RD] = data;
			}
			else if (write_cnt_vdsp_icu_lock == 3U)
			{
				g_write_value[WRITE_REG_IDX_VDSP_ICU_ACU_LOCK_4TH] = data;
			}
			else
			{
				/* do nothing */	
			}
			write_cnt_vdsp_icu_lock++;
		}
		/* D_SFT_SCPD_0 */
		else if (offset == 0x807E0U)
		{
			if (write_cnt_d_sft_scpd_0 == 0U)
			{
				g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0] = data;
			}
			else if (write_cnt_d_sft_scpd_0 == 1U)
			{
				g_write_value[WRITE_REG_IDX_D_SFT_SCPD_0_2ND] = data;
			}
			else
			{
				/* do nothing */	
			}
			write_cnt_d_sft_scpd_0++;
		}
		/* D_SFT_SCPD_1 */
		else if (offset == 0x807E4U)
		{
			g_write_value[WRITE_REG_IDX_D_SFT_SCPD_1] = data;
		}
		/* D_SFT_SCPD_2 */
		else if (offset == 0x807E8U)
		{
			g_write_value[WRITE_REG_IDX_D_SFT_SCPD_2] = data;
		}
		/* D_SFT_SCPD_3 */
		else if (offset == 0x807ECU)
		{
			g_write_value[WRITE_REG_IDX_D_SFT_SCPD_3] = data;
		}
		/* VDSP_OUT_INT_CLR */
		else if (offset == 0xC0044U)
		{
			g_write_value[WRITE_REG_IDX_VDSP_OUT_INT_CLR] = data;
		}
		else
		{
			/* do nothing */
		}
	}

	return ret;
}

e_osal_return_t stub_R_OSAL_PmGetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t* const p_state)
{
	if ((OSAL_RETURN_OK == g_ret_R_OSAL_PmGetRequiredState) && (NULL != handle) && (NULL != p_state))
	{
		*p_state = g_get_pm_required_state;
	}

	return g_ret_R_OSAL_PmGetRequiredState;
}

e_osal_return_t stub_R_OSAL_PmSetRequiredState(osal_device_handle_t handle, e_osal_pm_required_state_t state, bool applyImmediate)
{
	e_osal_return_t ret = OSAL_RETURN_OK;

	if ((OSAL_RETURN_OK != g_ret_R_OSAL_PmSetRequiredState) && (NULL != g_R_OSAL_PmSetRequiredState_devid_list))
	{
		for (uint32_t i = 0; i < ARRAY_SIZE(core_properties); i++)
		{
			if (strstr(g_R_OSAL_PmSetRequiredState_devid_list, core_properties[i].name))
			{
				if ((handle == core_properties[i].handle) && (g_set_pm_required_state == state))
				{
					ret = g_ret_R_OSAL_PmSetRequiredState;
					break;
				}
			}
		}
	}

	return ret;
}

e_osal_return_t stub_R_OSAL_PmGetState(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
	if ((OSAL_RETURN_OK == g_ret_R_OSAL_PmGetState) && (NULL != handle) && (NULL != p_state))
	{
		*p_state = OSAL_PM_STATE_CG;
	}

	return g_ret_R_OSAL_PmGetState;
}

e_osal_return_t stub_R_OSAL_PmSetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t policy, bool applyImmediate)
{
	return g_ret_R_OSAL_PmSetPolicy;
}

e_osal_return_t stub_R_OSAL_PmGetPolicy(osal_device_handle_t handle, e_osal_pm_policy_t* const p_policy)
{
	if ((OSAL_RETURN_OK == g_ret_R_OSAL_PmGetPolicy) && (NULL != handle) && (NULL != p_policy))
	{
		*p_policy = OSAL_PM_POLICY_CG;
	}

	return g_ret_R_OSAL_PmGetPolicy;
}

e_osal_return_t stub_R_OSAL_PmSetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t state)
{
	return g_ret_R_OSAL_PmSetResetState;
}

e_osal_return_t stub_R_OSAL_PmGetResetState(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_state)
{
	if ((OSAL_RETURN_OK == g_ret_R_OSAL_PmGetResetState) && (NULL != handle) && (NULL != p_state))
	{
		*p_state = OSAL_PM_RESET_STATE_RELEASED;
	}

	return g_ret_R_OSAL_PmGetResetState;
}

e_osal_return_t stub_R_OSAL_PmWaitForState(osal_device_handle_t handle, e_osal_pm_state_t state, osal_milli_sec_t time_period)
{
	return g_ret_R_OSAL_PmWaitForState;
}

e_osal_return_t stub_R_OSAL_ThsyncMemoryBarrier(void)
{
	return g_ret_R_OSAL_ThsyncMemoryBarrier;
}

e_osal_return_t stub_R_OSAL_MmngrGetSize(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t* const p_size)
{
	e_osal_return_t ret = g_ret_R_OSAL_MmngrGetSize[g_mem_size_cnt];

    if (OSAL_RETURN_OK == g_ret_R_OSAL_MmngrGetSize[g_mem_size_cnt])
    {
        if (p_size) {
            *p_size = g_mem_size[g_mem_size_cnt];
        }
    }

    g_mem_size_cnt++;

    return ret;
}

e_osal_return_t stub_R_OSAL_MmngrGetCpuPtr(osal_memory_buffer_handle_t memory_buffer_obj_hndl, void** const pp_cpu_ptr)
{
	e_osal_return_t ret = g_ret_R_OSAL_MmngrGetCpuPtr[g_logical_addr_cnt];

    if (OSAL_RETURN_OK == g_ret_R_OSAL_MmngrGetCpuPtr[g_logical_addr_cnt])
    {
        if (pp_cpu_ptr) {
            *pp_cpu_ptr = g_cpu_addr[g_logical_addr_cnt];
        }
    }

    g_logical_addr_cnt++;

    return ret;
}

e_osal_return_t stub_R_OSAL_MmngrGetHwAddr(osal_memory_buffer_handle_t memory_buffer_obj_hndl,
                                      osal_axi_bus_id_t axi_id, uintptr_t * const p_hw_addr)
{
	e_osal_return_t ret = g_ret_R_OSAL_MmngrGetHwAddr[g_hw_addr_cnt];

    if (OSAL_RETURN_OK == g_ret_R_OSAL_MmngrGetHwAddr[g_hw_addr_cnt])
    {
        if (p_hw_addr) {
            *p_hw_addr = g_hw_addr[g_hw_addr_cnt];
        }
    }

    g_hw_addr_cnt++;

    return ret;
}

e_osal_return_t stub_R_OSAL_MmngrFlush(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset, size_t size)
{
	return g_ret_R_OSAL_MmngrFlush;
}

e_osal_return_t stub_R_OSAL_MmngrInvalidate(osal_memory_buffer_handle_t memory_buffer_obj_hndl, size_t offset, size_t size)
{
	return g_ret_R_OSAL_MmngrInvalidate;
}

e_osal_return_t stub_R_OSAL_MqCreate(const st_osal_mq_config_t * const p_config, osal_mq_id_t mq_Id,
                                                             osal_mq_handle_t * const p_handle)
{
	if (OSAL_RETURN_OK == g_ret_R_OSAL_MqCreate)
	{
		if (p_handle) {
			*p_handle = &mq_handle;
		}
	}
	return g_ret_R_OSAL_MqCreate;
}

e_osal_return_t stub_R_OSAL_MqReset(osal_mq_handle_t handle)
{
	return g_ret_R_OSAL_MqReset;
}

e_osal_return_t stub_R_OSAL_MqDelete(osal_mq_handle_t handle)
{
	return g_ret_R_OSAL_MqDelete;
}

e_osal_return_t stub_R_OSAL_MqReceiveForTimePeriod(osal_mq_handle_t handle,
                                              osal_milli_sec_t time_period,
                                              void* p_buffer,
                                              size_t buffer_size)
{
	if (OSAL_RETURN_OK == g_ret_R_OSAL_MqReceiveForTimePeriod)
    {
        if (p_buffer) {
			(void)memcpy(p_buffer, &g_buffer, buffer_size);
        }
    }

	return g_ret_R_OSAL_MqReceiveForTimePeriod;
}

e_osal_return_t stub_R_OSAL_MqSendForTimePeriod(osal_mq_handle_t handle,
                                           osal_milli_sec_t time_period,
                                           const void* p_buffer,
                                           size_t buffer_size)
{
	return g_ret_R_OSAL_MqSendForTimePeriod;
}

e_impdrv_errorcode_t stub_impdrv_cmnctl_reg_set_checksum(
    void
)
{
	if (IMPDRV_EC_OK == g_ret_impdrv_cmnctl_reg_set_checksum)
	{
		return impdrv_cmnctl_reg_set_checksum();
	}

	return g_ret_impdrv_cmnctl_reg_set_checksum;
}