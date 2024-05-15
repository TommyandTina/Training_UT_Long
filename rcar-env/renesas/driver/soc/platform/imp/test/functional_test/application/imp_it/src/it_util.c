#include <stdio.h>
#include <stdlib.h>

#define TEST_MSG_NAME "util"

#include "it_common.h"
#include "msco.h"
/* Atomic Library */
#include "r_atmlib_def.h"
#include "r_atmlib_types.h"
#include "r_atmlib_prot.h"

#include <unistd.h>
#include <time.h>

#ifdef TARGET_OS_QNX
#define MQ_MSG_SIZE (8u)
#define MQ_MSG_MAX_NUM (63u)
#define INT_NOP_NUM (600)
#else
#define MQ_MSG_SIZE (4u)
#define MQ_MSG_MAX_NUM (256u)
#define INT_NOP_NUM (10)
#endif


struct imp_it_memory_pool
{
	osal_memory_buffer_handle_t mem_handle;
	void *vir_addr;
	uintptr_t phys_addr;
	size_t size;
	uint32_t enabled;
};

#define SECTONSEC				(1000000000L)

/* memory pool for IT */
#define OSAL_MEM_ALIGN_MIN		(64u)
#define IMP_MEM_POOL_SIZE		(1000u)

struct imp_it_memory_pool mem_pool[IMP_MEM_POOL_SIZE];

/* test function */
osal_axi_bus_id_t					imp_dev_axi_bus_id = OSAL_AXI_BUS_ID_INVALID;
static st_osal_mmngr_config_t		osal_mmngr_cfg;
volatile bool						msco_int_flg;

int32_t OsDepInit(osal_memory_manager_handle_t *osal_mmngr)
{
	e_osal_return_t osal_ret;
	int32_t ret = 0;

	memset(&mem_pool, 0, sizeof(mem_pool));

	/* initialize OSAL Library */
	osal_ret = R_OSAL_Initialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		LOGPRINT(("[ERROR] R_OSAL_Initialize : %d\n", (int32_t)osal_ret));
		ret = -1;
	}
	else
	{
		/* init OSAL memory manager */
		osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&osal_mmngr_cfg);
		if (osal_ret != OSAL_RETURN_OK)
		{
			LOGPRINT(("[ERROR] R_OSAL_MmngrGetOsalMaxConfig failed %d\n", (int32_t)osal_ret));
			ret = -1;
		}
		else
		{
			/* configure memory manager capacity */
			// osal_mmngr_cfg.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
			// osal_mmngr_cfg.memory_limit						= 0x7E00000;
			// osal_mmngr_cfg.max_allowed_allocations			= (256 * 1024);
			// osal_mmngr_cfg.max_registered_monitors_cbs		= 0u;

			/* open OSAL memory manager */
			osal_ret = R_OSAL_MmngrOpen(&osal_mmngr_cfg, osal_mmngr);
			if (osal_ret != OSAL_RETURN_OK)
			{
				LOGPRINT(("[ERROR] R_OSAL_MmngrOpen failed %d\n", (int32_t)osal_ret));
				ret = -1;
			}
			else
			{
				/* get info for converting to and from hardware address */
				osal_ret = R_OSAL_IoGetAxiBusIdFromDeviceName("imp_top_00", &imp_dev_axi_bus_id);
				if (osal_ret != OSAL_RETURN_OK)
				{
					LOGPRINT(("[ERROR] R_OSAL_IoGetAxiBusIdFromDeviceName failed %d\n", (int32_t)osal_ret));
					ret = -1;
				}
			}
		}
	}

	return ret;
}

int32_t OsDepDeinit(osal_memory_manager_handle_t *osal_mmngr)
{
	e_osal_return_t osal_ret;
	int32_t ret = 0;

	osal_ret = R_OSAL_MmngrClose(*osal_mmngr);
	if (osal_ret != OSAL_RETURN_OK)
	{
		LOGPRINT(("[ERROR] R_OSAL_MmngrClose failed %d\n", (int32_t)osal_ret));
		ret = -1;
	}

	osal_ret = R_OSAL_Deinitialize();
	if (osal_ret != OSAL_RETURN_OK)
	{
		LOGPRINT(("[ERROR] R_OSAL_Deinitialize failed %d\n", (int32_t)osal_ret));
		ret = -1;
	}

	return ret;
}

static struct imp_it_memory_pool *imp_it_get_memory_info(void *vir_addr)
{
	struct imp_it_memory_pool *mem_info = NULL;
	uint32_t i;

	if (!!vir_addr)
	{
		for (i = 0; i < IMP_MEM_POOL_SIZE; i++)
		{
			if ((vir_addr == mem_pool[i].vir_addr) && (mem_pool[i].enabled == true))
			{
				mem_info = &mem_pool[i];
				break;
			}
		}
	}

	return mem_info;
}

static void imp_it_insert_memory_handle(osal_memory_buffer_handle_t mem_handle, void *vir_addr, uintptr_t phys_addr, size_t size)
{
	uint32_t i;

	for (i = 0; i < IMP_MEM_POOL_SIZE; i++)
	{
		if (mem_pool[i].enabled == false)
		{
			mem_pool[i].mem_handle = mem_handle;
			mem_pool[i].vir_addr = vir_addr;
			mem_pool[i].phys_addr = phys_addr;
			mem_pool[i].size = size;
			mem_pool[i].enabled = true;
			break;
		}
	}
}

static void imp_it_remove_memory_handle(osal_memory_buffer_handle_t mem_handle)
{
 	uint32_t i;

	for (i = 0; i < IMP_MEM_POOL_SIZE; i++)
	{
		if ((mem_pool[i].enabled == true) && (mem_pool[i].mem_handle == mem_handle))
		{
			mem_pool[i].mem_handle = NULL;
			mem_pool[i].vir_addr = 0;
			mem_pool[i].phys_addr = 0;
			mem_pool[i].size = 0;
			mem_pool[i].enabled = false;
			break;
		}
	}
}

int32_t create_msg(void **handle, uint32_t id)
{
	st_osal_mq_config_t msgcnf;
	st_osal_mq_config_t msgcnf_verify;
	osal_mq_handle_t p_handle;
	e_osal_return_t osal_ret;
	int32_t ret = 0;

	msgcnf.msg_size = MQ_MSG_SIZE;
	msgcnf.max_num_msg = MQ_MSG_MAX_NUM;

	osal_ret = R_OSAL_MqCreate(&msgcnf, (osal_mq_id_t)id, &p_handle);

	if (OSAL_RETURN_OK != osal_ret) {
		LOGPRINT(("[Error] R_OSAL_MqCreate returns %s\n", osal_err_to_name(osal_ret)));
		ret = -1;
	}

	if (0 == ret)
	{
		osal_ret = R_OSAL_MqGetConfig(p_handle, &msgcnf_verify);
		if (OSAL_RETURN_OK != osal_ret)
		{
			ret = -1;
			LOGPRINT(("<< [ERROR] R_OSAL_MqGetConfig  : code %d>>\n",
				(int32_t)osal_ret)); /* cast to int32_t */
		}
		else
		{
			if ((msgcnf.max_num_msg >  msgcnf_verify.max_num_msg) ||
				(msgcnf.msg_size    != msgcnf_verify.msg_size))
			{
				ret = -1;
				LOGPRINT(("<< [ERROR] Requested Message queue invalid config  : %d, %d [ %d, %d]>>\n",
					(int32_t)msgcnf_verify.max_num_msg,  /* cast to int32_t */
					(int32_t)msgcnf_verify.msg_size,     /* cast to int32_t */
					(int32_t)msgcnf.max_num_msg,         /* cast to int32_t */
					(int32_t)msgcnf.msg_size));          /* cast to int32_t */
			}
		}
	}

	if (0 == ret)
	{
		*handle = p_handle;
	}

	return ret;
}

void destroy_msg(void *handle)
{
	(void)R_OSAL_MqReset((osal_mq_handle_t)handle);
	(void)R_OSAL_MqDelete((osal_mq_handle_t)handle);
}

int32_t send_msg(void *handle, int32_t send_data)
{
	e_osal_return_t osal_ret;
#if !defined(TARGET_OS_QNX)
	int32_t msg = send_data;
#else
	int64_t msg = (int64_t)send_data;
#endif

	while (1)
	{
		osal_ret = R_OSAL_MqSendForTimePeriod((osal_mq_handle_t)handle, 1000, &msg, MQ_MSG_SIZE);
		if (osal_ret == OSAL_RETURN_TIME)
		{
			continue;
		}
		else if (osal_ret != OSAL_RETURN_OK)
		{
			LOGPRINT(("[Error] R_OSAL_MqSendForTimePeriod %d\n", osal_ret));
			return -1;
		}
		else
		{
			break;
		}
	}

	return 0;
}

int32_t receive_msg(void *handle, int32_t *msg)
{
	return receive_msg_time(handle, msg, 500);
}

int32_t receive_msg_time(void *handle, int32_t *msg, int32_t time)
{
	e_osal_return_t osal_ret;
#if defined(TARGET_OS_QNX)
	int64_t recieve_msg;
#endif

	while (1)
	{
#if !defined(TARGET_OS_QNX)
		osal_ret = R_OSAL_MqReceiveForTimePeriod((osal_mq_handle_t)handle, time, msg, MQ_MSG_SIZE);
#else
		osal_ret = R_OSAL_MqReceiveForTimePeriod((osal_mq_handle_t)handle, time, &recieve_msg, MQ_MSG_SIZE);
		*msg = (int32_t)recieve_msg;
#endif
		if (osal_ret == OSAL_RETURN_TIME)
		{
			continue;
		}
		else if (osal_ret != OSAL_RETURN_OK)
		{
			LOGPRINT(("[Error] R_OSAL_MqReceiveForTimePeriod %d\n", osal_ret));
			return -1;
		}
		else
		{
			break;
		}
	}

	return 0;
}

int32_t reset_msg(void *handle)
{
	(void)R_OSAL_MqReset((osal_mq_handle_t)handle);

	return 0;
}

/* memory create */
#define MEM_ALIGN(__size, __align) ((__size + __align - 1) & ~(__align))

void *MemAlloc(osal_memory_manager_handle_t osal_mmngr, size_t size, size_t align, uint32_t flags)
{
    osal_memory_buffer_handle_t    mem_handle = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    e_osal_return_t ret_osal = OSAL_RETURN_FAIL;
	void *org_addr, *addr = NULL;
	uintptr_t phys_addr = 0;

	if ((align & (align - 1)) != 0)
	{
		return NULL; /* error: align is not 2^n */
	}
	/* adjust memory align and memory size */
	align = (align + OSAL_MEM_ALIGN_MIN - 1) & ~(OSAL_MEM_ALIGN_MIN - 1);
	size = (size + align - 1) & ~(align - 1);

	if( flags == IMP_MALLOC_HW )
	{
		ret_osal = R_OSAL_MmngrAlloc(osal_mmngr, size, align, &mem_handle);
		if (ret_osal == OSAL_RETURN_OK)
		{
			ret_osal = R_OSAL_MmngrGetCpuPtr(mem_handle, &org_addr);
			if (ret_osal == OSAL_RETURN_OK)
			{
				ret_osal = R_OSAL_MmngrGetHwAddr(mem_handle, imp_dev_axi_bus_id, &phys_addr);
				if (ret_osal != OSAL_RETURN_OK)
				{
					LOGPRINT(("R_OSAL_MmngrGetHwAddr is failed %d\n", (int32_t)ret_osal));
				}
			}
			else
			{
				LOGPRINT(("R_OSAL_MmngrGetCpuPtr is failed %d\n", (int32_t)ret_osal));
			}
		}
		else
		{
			LOGPRINT(("R_OSAL_MmngrAlloc is failed %d\n", (int32_t)ret_osal));
		}
	}
	else
	{
		org_addr = malloc(size);
	}

	addr = org_addr;

	if( flags == IMP_MALLOC_HW && ret_osal == OSAL_RETURN_OK)
	{
		imp_it_insert_memory_handle(mem_handle, addr, phys_addr, size);
	}

	return addr;
}

void MemFree(osal_memory_manager_handle_t osal_mmngr, void* addr)
{
	struct imp_it_memory_pool * mem_info = imp_it_get_memory_info(addr);
	osal_memory_buffer_handle_t mem_handle;
	if (NULL != mem_info)
	{
		mem_handle = mem_info->mem_handle;
		imp_it_remove_memory_handle(mem_info->mem_handle);
		(void)R_OSAL_MmngrDealloc(osal_mmngr, mem_handle);
	}
	else if (addr != NULL)
	{
		(void)free(addr);
	}
	else
	{
		LOGPRINT(("Warning!! MemFree: Invalid addr\n"));
	}
}

void util_mem_flush(void *addr)
{
	struct imp_it_memory_pool *mem_info = imp_it_get_memory_info(addr);

	if (NULL != mem_info)
	{
		(void)R_OSAL_MmngrFlush(mem_info->mem_handle, 0, mem_info->size);
	}
}

void util_mem_invalidate(void *addr)
{
	struct imp_it_memory_pool *mem_info = imp_it_get_memory_info(addr);
 
    if (NULL != mem_info)
    {
        (void)R_OSAL_MmngrInvalidate(mem_info->mem_handle, 0, mem_info->size);
    }
}

osal_memory_buffer_handle_t util_mem_get_handle(void *addr)
{
	struct imp_it_memory_pool *mem_info = imp_it_get_memory_info(addr);
	osal_memory_buffer_handle_t mem_handle = NULL;

	if (NULL != mem_info)
	{
		mem_handle = mem_info->mem_handle;
	}

	return mem_handle;
}

uintptr_t util_mem_get_paddr(void *addr)
{
	struct imp_it_memory_pool *mem_info = imp_it_get_memory_info(addr);
	uintptr_t phys_addr = 0;

	if (NULL != mem_info)
	{
		phys_addr = mem_info->phys_addr;
	}

	return phys_addr;
}

void *util_mem_get_vaddr(void *addr)
{
	struct imp_it_memory_pool *mem_info = imp_it_get_memory_info(addr);
	void *vir_addr = NULL;

	if (NULL != mem_info)
	{
		vir_addr = mem_info->vir_addr;
	}

	return vir_addr;
}

size_t util_mem_get_size(void *addr)
{
	struct imp_it_memory_pool *mem_info = imp_it_get_memory_info(addr);
	size_t size = 0;

	if (NULL != mem_info)
	{
		size = mem_info->size;
	}

	return size;
}

/* command list */

/* command */
#define FCT_IMP_NOP		0x40000010U
#define FCT_IMP_NOP_2   0x40000000U
#define FCT_IMP_MOV     0x41000000U
#define FCT_IMP_SYNCM   0x43000000U
#define FCT_IMP_WUP		0x48000000U
#define FCT_IMP_SLP		0x49000000U
#define FCT_IMP_INT		0x42000000U
#define FCT_IMP_TRAP	0x4F000000U

#define FCT_OCV_NOP		0x80000010U
#define FCT_OCV_WUP		0xA0000000U
#define FCT_OCV_SLP		0xA1000000U
#define FCT_OCV_INT		0x81000000U
#define FCT_OCV_TRAP	0x88000000U
#define FCT_OCV_WPR		0x55000000U

#define FCT_DMAC_NOP	0x80000010U
#define FCT_DMAC_WUP	0xA0000000U
#define FCT_DMAC_SLP	0xA1000000U
#define FCT_DMAC_INT	0x81000000U
#define FCT_DMAC_TRAP	0x88000000U

#define FCT_SDMAC_NOP	0x80000010U
#define FCT_SDMAC_WUP	0xA0000000U
#define FCT_SDMAC_SLP	0xA1000000U
#define FCT_SDMAC_INT	0x81000000U
#define FCT_SDMAC_TRAP	0x88000000U

#define FCT_PSC_NOP		0x80000010U
#define FCT_PSC_WUP		0xA0000000U
#define FCT_PSC_SLP		0xA1000000U
#define FCT_PSC_INT		0x81000000U
#define FCT_PSC_TRAP	0x88000000U

#define FCT_CNN_NOP		0x80000020U
#define FCT_CNN_WUP		0xA0000000U
#define FCT_CNN_SLP		0xA1000000U
#define FCT_CNN_INT		0x81000000U
#define FCT_CNN_TRAP	0x88000000U

static inline void _set_simplecl(uint32_t *pcl, uint32_t count, uint32_t nop, uint32_t trap)
{
	uint32_t *pcl_end = &pcl[count - 1];

	do {
		*pcl = nop;
		pcl++;
	} while (pcl != pcl_end);
	*pcl = trap;
}

static inline void _set_trapcl(uint32_t *pcl, uint32_t count, uint32_t trap)
{
	for (int i = 0; i < count; i++)
	{
		pcl[i] = trap;
	}
}

#define set_simplecl(__pcl, __core, __count, __tcode) \
	_set_simplecl(__pcl, __count, FCT_##__core##_NOP, FCT_##__core##_TRAP | __tcode)

#define set_trapcl(__pcl, __core, __count, __tcode) \
	_set_trapcl(__pcl, __count, FCT_##__core##_TRAP | __tcode)

#define _set_commandlist(__type, __addr, __func, ...) \
do { \
	uint32_t *pcl = (uint32_t *)util_mem_get_vaddr(__addr); \
	size_t size = util_mem_get_size(__addr); \
	uint32_t count = (uint32_t)(size / sizeof(uint32_t)); \
	if (pcl == NULL) { \
		return -1; \
	} \
	if (count < 4) { \
		return -1; \
	} \
	switch (__type) { \
	case IMPDRV_CORE_TYPE_IMP: \
	case IMPDRV_CORE_TYPE_IMP_SLIM: \
		__func(pcl, IMP, count, __VA_ARGS__); \
		break; \
	case IMPDRV_CORE_TYPE_OCV: \
		__func(pcl, OCV, count, __VA_ARGS__); \
		break; \
	case IMPDRV_CORE_TYPE_DMAC: \
		__func(pcl, DMAC, count, __VA_ARGS__); \
		break; \
	case IMPDRV_CORE_TYPE_DMAC_SLIM: \
		__func(pcl, SDMAC, count, __VA_ARGS__); \
		break; \
	case IMPDRV_CORE_TYPE_PSCEXE: \
		__func(pcl, PSC, count, __VA_ARGS__); \
		break; \
	case IMPDRV_CORE_TYPE_CNN: \
		__func(pcl, CNN, count, __VA_ARGS__); \
		break; \
	case IMPDRV_CORE_TYPE_DSP: \
		break; \
	default: \
		testmsg("type err\n"); \
		return -1; \
	} \
} while (0)

int32_t set_commandlist(uint32_t type, void *addr, uint32_t code)
{
	_set_commandlist(type, addr, set_simplecl, code);
	util_mem_flush(addr);
	return 0;
}

int32_t set_intcommandlist(uint32_t type, void *addr, uint32_t icode, uint32_t tcode)
{
#define set_intcl(__pcl, __core, __count, __tcode, __icode) \
do { \
	set_simplecl(__pcl, __core, __count, __tcode); \
	__pcl[__count / 2] = FCT_##__core##_INT | __icode; \
} while (0)

	_set_commandlist(type, addr, set_intcl, tcode, icode);
	util_mem_flush(addr);
	return 0;
}

int32_t set_conint_commandlist(uint32_t type, void *addr, uint32_t icode, uint32_t tcode)
{
	int32_t ret_count = -1;
#define set_conint_cl(__pcl, __core, __count, __tcode, __icode) \
do { \
	_set_simplecl(__pcl, __count, FCT_##__core##_INT | __icode, FCT_##__core##_TRAP | __tcode); \
	__pcl[0] = FCT_##__core##_NOP; \
	__pcl[1] = FCT_##__core##_NOP; \
	ret_count = __count - 1 - 2; \
} while (0)

	_set_commandlist(type, addr, set_conint_cl, tcode, icode);
	util_mem_flush(addr);
	return ret_count;
}

int32_t set_interror_commandlist(uint32_t type, void *addr, uint32_t icode, uint32_t tcode)
{
#define set_interr_cl(__pcl, __core, __count, __tcode, __icode) \
do { \
	set_simplecl(__pcl, __core, __count, __tcode); \
	__pcl[2] = FCT_##__core##_INT | __icode; \
	__pcl[3] = FCT_##__core##_INT | __icode; \
	__pcl[4] = 0xfdabfdab; \
} while (0)

	_set_commandlist(type, addr, set_interr_cl, tcode, icode);
	util_mem_flush(addr);
	return 0;
}

int32_t set_intpbcovf_commandlist(uint32_t type, void *addr, uint32_t tcode)
{
	uint32_t *pcl = (uint32_t *)util_mem_get_vaddr(addr);
	uint32_t count = (uint32_t)(util_mem_get_size(addr) / sizeof(uint32_t));

	if (type != IMPDRV_CORE_TYPE_OCV) {
		return - 1;
	}

	if (pcl == NULL) {
		return -1;
	}

	if (count < 7) {
		return -1;
	}

	set_simplecl(pcl, OCV, count, tcode);

	pcl[0] = FCT_OCV_WPR | 0x00028C01U;
	pcl[1] = 0x00000002U;
	pcl[2] = FCT_OCV_WPR | 0x00028001U;
	pcl[3] = 0x00000001U;
	pcl[count / 2] = FCT_OCV_INT | 0x00008000U | tcode;

	util_mem_flush(addr);

	return 0;
}

int32_t set_pbcovf_commandlist(uint32_t type, void *addr, uint32_t tcode)
{
	uint32_t *pcl = (uint32_t *)util_mem_get_vaddr(addr);
	uint32_t count = (uint32_t)(util_mem_get_size(addr) / sizeof(uint32_t));

	if (type != IMPDRV_CORE_TYPE_OCV) {
		return - 1;
	}

	if (pcl == NULL) {
		return -1;
	}

	if (count < 7) {
		return -1;
	}

	set_simplecl(pcl, OCV, count, tcode);

	pcl[0] = FCT_OCV_WPR | 0x00028C01U;
	pcl[1] = 0x00000002U;
	pcl[2] = FCT_OCV_WPR | 0x00028001U;
	pcl[3] = 0x00000001U;

	util_mem_flush(addr);

	return 0;
}

int32_t set_synccommandlist(uint32_t type, void *addr, uint32_t code, uint32_t sync)
{
#define set_synccl(__pcl, __core, __count, __tcode, __sync) \
do { \
	set_simplecl(__pcl, __core, __count, __tcode); \
	pcl[__count / 2] = FCT_##__core##_WUP | (__sync & 0xFFFF); \
	pcl[__count / 2 + 1] = FCT_##__core##_SLP | (__sync & 0xFFFF); \
} while (0)

	_set_commandlist(type, addr, set_synccl, code, sync);
	util_mem_flush(addr);

	return 0;
}

int32_t set_trapsynccommandlist(uint32_t type, void *addr, uint32_t code)
{
#define set_trapsynccl(__pcl, __core, __count, __tcode) \
do { \
	set_trapcl(__pcl, __core, __count, __tcode); \
} while (0)

	_set_commandlist(type, addr, set_trapsynccl, code);
	util_mem_flush(addr);

	return 0;
}

int32_t set_errcommandlist(uint32_t type, void *addr, uint32_t code)
{
#define set_errorcl(__pcl, __core, __count, __tcode) \
do { \
	set_simplecl(__pcl, __core, __count, __tcode); \
	__pcl[__count / 2] = 0xfefefefe; \
} while (0)

	_set_commandlist(type, addr, set_errorcl, code);
	util_mem_flush(addr);

	return 0;
}

const e_impdrv_core_type_t type_arry[] = {
	IMPDRV_CORE_TYPE_IMP,
	IMPDRV_CORE_TYPE_IMP_SLIM,
	IMPDRV_CORE_TYPE_OCV,
	IMPDRV_CORE_TYPE_DMAC,
	IMPDRV_CORE_TYPE_DMAC_SLIM,
	IMPDRV_CORE_TYPE_PSCEXE,
	IMPDRV_CORE_TYPE_CNN,
	IMPDRV_CORE_TYPE_INVALID,
};

const e_impdrv_core_type_t noexec_type_arry[] = {
	IMPDRV_CORE_TYPE_PSCOUT,
	IMPDRV_CORE_TYPE_DTA,
	IMPDRV_CORE_TYPE_INVALID,
};

char *type_to_name(uint32_t type)
{
	switch (type) {
		case IMPDRV_CORE_TYPE_INVALID:		return "Core INVALID";
		case IMPDRV_CORE_TYPE_IMP:			return "IMP";
		case IMPDRV_CORE_TYPE_OCV:			return "OCV";
		case IMPDRV_CORE_TYPE_DMAC:			return "DMAC";
		case IMPDRV_CORE_TYPE_PSCEXE:		return "PSCEXE";
		case IMPDRV_CORE_TYPE_CNN:			return "CNN";
		case IMPDRV_CORE_TYPE_IMP_SLIM:		return "SlimIMP";
		case IMPDRV_CORE_TYPE_DMAC_SLIM:	return "SlimDMAC";
		case IMPDRV_CORE_TYPE_PSCOUT:		return "PSCOUT";
		case IMPDRV_CORE_TYPE_LDMAC:		return "LDMAC";
		case IMPDRV_CORE_TYPE_DTA:			return "DTA";
		case IMPDRV_CORE_TYPE_IMR:			return "IMR";
		case IMPDRV_CORE_TYPE_DSP:			return "DSP";
	}

	return "Unknown";
}

char *impdrv_err_to_name(e_impdrv_errorcode_t err)
{
	switch (err) {
		case IMPDRV_EC_OK: return "IMPDRV_EC_OK";
		case IMPDRV_EC_NG_CHECKFAIL: return "IMPDRV_EC_NG_CHECKFAIL";
		case IMPDRV_EC_NG_SEQSTATE: return "IMPDRV_EC_NG_SEQSTATE";
		case IMPDRV_EC_NG_SYSTEMERROR: return "IMPDRV_EC_NG_SYSTEMERROR";
		case IMPDRV_EC_NG_ARGNULL: return "IMPDRV_EC_NG_ARGNULL";
		case IMPDRV_EC_NG_PARAM: return "IMPDRV_EC_NG_PARAM";
		case IMPDRV_EC_NG_ATTRIBUTE: return "IMPDRV_EC_NG_ATTRIBUTE";
		case IMPDRV_EC_NG_NOTSUPPORT: return "IMPDRV_EC_NG_NOTSUPPORT";
		case IMPDRV_EC_NG_PMSYSTEMERROR: return "IMPDRV_EC_NG_PMSYSTEMERROR";
		case IMPDRV_EC_NG_INSTANCE: return "IMPDRV_EC_NG_INSTANCE";
		case IMPDRV_EC_NG_DSP_HALT: return "IMPDRV_EC_NG_DSP_HALT";
	}

	return "Unknown";
}

char *osal_err_to_name(e_osal_return_t osal_err)
{
	switch (osal_err) {
		case OSAL_RETURN_OK                   : return "OSAL_RETURN_OK";
		case OSAL_RETURN_PAR                  : return "OSAL_RETURN_PAR";
		case OSAL_RETURN_MEM                  : return "OSAL_RETURN_MEM";
		case OSAL_RETURN_DEV                  : return "OSAL_RETURN_DEV";
		case OSAL_RETURN_TIME                 : return "OSAL_RETURN_TIME";
		case OSAL_RETURN_CONF                 : return "OSAL_RETURN_CONF";
		case OSAL_RETURN_FAIL                 : return "OSAL_RETURN_FAIL";
		case OSAL_RETURN_IO                   : return "OSAL_RETURN_IO";
		case OSAL_RETURN_BUSY                 : return "OSAL_RETURN_BUSY";
		case OSAL_RETURN_ID                   : return "OSAL_RETURN_ID";
		case OSAL_RETURN_HANDLE               : return "OSAL_RETURN_HANDLE";
		case OSAL_RETURN_STATE                : return "OSAL_RETURN_STATE";
		case OSAL_RETURN_PROHIBITED           : return "OSAL_RETURN_PROHIBITED";
		case OSAL_RETURN_UNSUPPORTED_OPERATION: return "OSAL_RETURN_UNSUPPORTED_OPERATION";
		case OSAL_RETURN_HANDLE_NOT_SUPPORTED : return "OSAL_RETURN_HANDLE_NOT_SUPPORTED";
		case OSAL_RETURN_UNKNOWN_NAME         : return "OSAL_RETURN_UNKNOWN_NAME";
		case OSAL_RETURN_CONF_UNSUPPORTED     : return "OSAL_RETURN_CONF_UNSUPPORTED";
		case OSAL_RETURN_OK_RQST              : return "OSAL_RETURN_OK_RQST";
	}

	return "Unknown";
}

char *policy_to_name(e_impdrv_pm_policy_t target_policy)
{
	switch (target_policy)
	{
		case IMPDRV_PM_POLICY_PG: return "PG";
		case IMPDRV_PM_POLICY_HP: return "HP";
		case IMPDRV_PM_POLICY_CG: return "CG";
	}
	return "Invalid policy";
}

char *state_to_name(e_impdrv_state_t state)
{
	switch (state) {
		case IMPDRV_STATE_UNINIT: return "IMPDRV_STATE_UNINIT";
		case IMPDRV_STATE_INIT: return "IMPDRV_STATE_INIT";
		case IMPDRV_STATE_READY: return "IMPDRV_STATE_READY";
		case IMPDRV_STATE_EXEC: return "IMPDRV_STATE_EXEC";
		case IMPDRV_STATE_INT: return "IMPDRV_STATE_INT";
		case IMPDRV_STATE_INT + 1: return "IMPDRV_STATE_INT + 1";
	}

	return "Unknown";
}

char *instance_to_name(e_impdrv_instance_t instance_num)
{
	switch (instance_num) {
		case IMPDRV_INSTANCE_0:	return "IMPDRV_INSTANCE_0";
		case IMPDRV_INSTANCE_1:	return "IMPDRV_INSTANCE_1";
		case IMPDRV_INSTANCE_2:	return "IMPDRV_INSTANCE_2";
		case IMPDRV_INSTANCE_3:	return "IMPDRV_INSTANCE_3";
		case IMPDRV_INSTANCE_4:	return "IMPDRV_INSTANCE_4";
		case IMPDRV_INSTANCE_5:	return "IMPDRV_INSTANCE_5";
		case IMPDRV_INSTANCE_6:	return "IMPDRV_INSTANCE_6";
		default:				return "Invalid instance";
	}

	return "Unknown";
}

char *irq_group_to_name(e_impdrv_irq_group_t irq_group)
{
	switch (irq_group) {
		case IMPDRV_IRQ_GROUP_NONE:	return "No groups";
		case IMPDRV_IRQ_GROUP_0   :	return "IRQ Group 0";
		case IMPDRV_IRQ_GROUP_1   :	return "IRQ Group 1";
		case IMPDRV_IRQ_GROUP_2   :	return "IRQ Group 2";
		default					  :	return "Invalid IRQ group";
	}

	return "Unknown";
}

char *irq_to_name(uint32_t irq_name)
{
	switch (irq_name) {
		case IMPDRV_IRQMASK_END    :	return "IMPDRV_IRQMASK_END    ";
    	case IMPDRV_IRQMASK_WUPCOVF:	return "IMPDRV_IRQMASK_WUPCOVF";
    	case IMPDRV_IRQMASK_USIER  :	return "IMPDRV_IRQMASK_USIER  ";
    	case IMPDRV_IRQMASK_USINT  :	return "IMPDRV_IRQMASK_USINT  ";
    	case IMPDRV_IRQMASK_PBCOVF :	return "IMPDRV_IRQMASK_PBCOVF ";
    	case IMPDRV_IRQMASK_SBO0ME :	return "IMPDRV_IRQMASK_SBO0ME ";
    	case IMPDRV_IRQMASK_HPINT  :	return "IMPDRV_IRQMASK_HPINT  ";
    	case IMPDRV_IRQMASK_APIPINT:	return "IMPDRV_IRQMASK_APIPINT";
    	case IMPDRV_IRQMASK_MSCO:		return "IMPDRV_IRQMASK_MSCO";
		default:						return "Unknown";
	}

	return "Unknown";
}

int8_t get_test_noexec_core(uint32_t count, st_impdrv_core_info_t *core_info)
{
	if (ARRAY_SIZE(noexec_type_arry) < (size_t)count) {
		return -1;
	}

	if (noexec_type_arry[count] == IMPDRV_CORE_TYPE_INVALID) {
		return -1;
	}

	core_info->core_type = noexec_type_arry[count];
	core_info->core_num = 0;

	return 0;
}

const st_impdrv_core_info_t *get_core_info(
	const st_impdrv_core_info_t	*core_list,
	const uint32_t				range,
	const e_impdrv_core_type_t	chk_core_type,
	const uint32_t				chk_core_num
)
{
	const st_impdrv_core_info_t *p_core_info = NULL;

	for (uint32_t i = 0; i < range; i++)
	{
		if (((core_list + i)->core_type == chk_core_type) && ((core_list + i)->core_num == chk_core_num))
		{
			p_core_info = (core_list + i);
			break;
		}
	}

	return p_core_info;
}

/* check state */
int32_t check_state(
	const st_impdrv_initdata_t *const p_data,
	const st_impdrv_coretypectl_t coretype_ctl[],
	const e_impdrv_state_t state,
	uint32_t num
)
{
	st_impdrv_core_info_t *core_info;
	e_impdrv_core_type_t core_type;
	uint32_t core_num;
	e_impdrv_state_t core_state;
	uint32_t i;

	for (i = 0; i < num; i++) {
		core_info = &p_data->core_info[i];
		core_type = core_info->core_type;
		core_num = core_info->core_num;

		core_state = coretype_ctl[core_type].core_ctl[core_num].core_state;
		if (core_state != state) {
			LOGPRINT(("%s : coretype_ctl[%s]->core_ctl[%u].core_state %s != state %s\n",
							__func__,
							type_to_name(core_type),
							core_num,
							state_to_name(core_state),
							state_to_name(state)
					));
			return -1;
		}
	}

	return 0;
}

/* check callback */
int32_t compare_callback(st_impdrv_core_info_t *core_info, int32_t ercd, int32_t code, int32_t cercd, int32_t ccode)
{
	int32_t check_callback_code;

	if ((ercd != IMPDRV_CB_RET_BEFORE_EXEC) &&
			((core_info->core_type == IMPDRV_CORE_TYPE_IMP) || (core_info->core_type == IMPDRV_CORE_TYPE_IMP_SLIM))) {
		check_callback_code = -1; /* -1 */
	} else {
		check_callback_code = code;
	}
	if ((cercd != ercd) || (ccode != check_callback_code)) {
		LOGPRINT(("callback check : invalid argment ercd=%d, code=0x%08x\n",
			(int32_t)cercd, ccode));
		return -1;
	}

	return 0;
}

int32_t wait_and_check_callback(void *msgcnf, st_impdrv_core_info_t *core_info, int32_t ercd, int32_t code)
{
	int32_t msg1, msg2;

	if (receive_msg(msgcnf, &msg1)) {
		LOGPRINT(("callback check : TIMEOUT\n"));
		return -1;
	}
	if (receive_msg(msgcnf, &msg2)) {
		LOGPRINT(("callback check : TIMEOUT\n"));
		return -1;
	}

	return compare_callback(core_info, ercd, code, msg1, msg2);
}

uint64_t util_counttousec(uint64_t count)
{
	return (count / 1000);
}

uint64_t util_getcount(void)
{
	st_osal_time_t time_stamp;
    uint64_t nsec = 0;

	if (R_OSAL_ClockTimeGetTimeStamp(OSAL_CLOCK_TYPE_HIGH_RESOLUTION, &time_stamp) != OSAL_RETURN_OK)
	{
		LOGPRINT(("[ERROR] %s: R_OSAL_ClockTimeGetTimeStamp\n", __func__));
	}
	else
	{
		nsec = time_stamp.tv_sec * SECTONSEC + time_stamp.tv_nsec;
	}

	return nsec;
}

void *createCL_USIER(uint32_t type)
{
    R_ATMLIB_CLData clData;
    uint32_t        data[10];
    R_ATMLIB_OCVRectParam rect;
    uint32_t program[5] = {
		0x050a0002U, /* STCI CR10,2 */
		0x19f7c080U, /* MOVU R31,0 */
		0x0c841f00U, /* LDI R16,R31,0 */
		0x0a100000U, /* SBOCRD R16,0,0 */
		0x00c00000U  /* TRAP  */
	};
    uint32_t program2[6] = {
		0x40000000u | 0x2fffffffU, /* broken command */
		0x050a0002U, /* STCI CR10,2 */
		0x19f7c080U, /* MOVU R31,0 */
		0x0c841f00U, /* LDI R16,R31,0 */
		0x0a100000U, /* SBOCRD R16,0,0 */
		0x00c00000U  /* TRAP  */
	};

	const uint32_t sbo_data_length = 1024;
	const uint32_t OCV_VIBAR       = 0x0340U;
	const uint32_t OCV_SBOBAR0     = 0x0360U;
	const uint32_t OCV_VPCSAR      = 0x0320U;
	const uint32_t threthold       = 256+256;
	uint32_t *progMem[2] = { NULL, NULL};
	uint32_t *memories = NULL;
	uint32_t *CLmem   = NULL;
	uint32_t progMem_phys[2];
	uint32_t memory_phys;
	size_t progMem_size[2] = { 0, 0 };

	CLmem = (uint32_t *)MemAlloc(osal_mmngr, 256 * sizeof(uint32_t), 128, IMP_MALLOC_HW);

	if ((type & 1) == 0)
	{
		progMem[0] = (uint32_t *)MemAlloc(osal_mmngr, sizeof(program), 128, IMP_MALLOC_HW);
		progMem_phys[0] = util_mem_get_paddr(progMem[0]);
		memcpy(progMem[0], program, sizeof(program));
		progMem_size[0] = sizeof(program);
	}
	else
	{
		progMem[0] = (uint32_t *)MemAlloc(osal_mmngr, sizeof(program2), 128, IMP_MALLOC_HW);
		progMem_phys[0] = util_mem_get_paddr(progMem[0]);
		memcpy(progMem[0], program2, sizeof(program2));
		progMem_size[0] = sizeof(program2);
	}
	if ((type & 2) == 0)
	{
		progMem[1] = (uint32_t *)MemAlloc(osal_mmngr, sizeof(program), 128, IMP_MALLOC_HW);
		progMem_phys[1] = util_mem_get_paddr(progMem[1]);
		memcpy(progMem[1], program, sizeof(program));
		progMem_size[1] = sizeof(program);
	}
	else
	{
		progMem[1] = (uint32_t *)MemAlloc(osal_mmngr, sizeof(program2), 128, IMP_MALLOC_HW);
		progMem_phys[1] = util_mem_get_paddr(progMem[1]);
		memcpy(progMem[1], program2, sizeof(program2));
		progMem_size[1] = sizeof(program2);
	}

	memories = (uint32_t *)MemAlloc(osal_mmngr, sbo_data_length * sizeof(uint32_t), 128, IMP_MALLOC_HW);
	memory_phys = util_mem_get_paddr(memories);
	memset(memories, 0, sbo_data_length * sizeof(uint32_t));

	/* Prepare CL data structure */
	if (R_ATMLIB_E_OK != r_atmlib_InitializeOCVCL(&clData, CLmem, 256))
	{
		LOGPRINT(("Error: r_atmlib_InitializeOCVCL\n"));
	}

	data[0] = progMem_phys[0];
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_VIBAR, 1, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -1\n"));
	}

	data[0] = memory_phys; /* SBOBAR0 */
	data[1] = threthold;        /* SBOMMSR0 */
	data[2] = 0x00000000;  /* SBOCR0: PCKMODE = 0, XYPCK = 0 */
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_SBOBAR0, 3, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -4\n"));
	}

	data[0] = 0;/* entry */
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_VPCSAR, 4, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -5\n"));
	}

	rect.dx1 = 1;
	rect.dy1 = 1;
	rect.dx2 = 0;
	rect.dy2 = 0;
	rect.xlen = 32;
	rect.ylen = 1;
	rect.xs = 0;
	rect.ys = 0;
	if (R_ATMLIB_E_OK != r_atmlib_OCV_RECT(&clData, 0, 0, 0, &rect))
	{
		LOGPRINT(("Error: r_atmlib_OCV_RECT -1\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_OCV_INT(&clData, 0 , (R_ATMLIB_ENABLECONTROL)0))
	{
		LOGPRINT(("Error: r_atmlib_OCV_INT\n"));
	}

	data[0] = progMem_phys[1];
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_VIBAR, 1, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -1\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_OCV_RECT(&clData, 0, 0, 0, &rect))
	{
		LOGPRINT(("Error: r_atmlib_OCV_RECT -2\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_OCV_TRAP(&clData, 0 ))
	{
		LOGPRINT(("Error: r_atmlib_OCV_TRAP\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_FinalizeOCVCL(&clData))
	{
		LOGPRINT(("Error: r_atmlib_FinalizeOCVCL\n"));
	}

	util_mem_flush(clData.top_addr);
	util_mem_flush(progMem[0]);
	util_mem_flush(progMem[1]);
	util_mem_flush(memories);

	return clData.top_addr;
}

void *createCL_SBOtest(const uint32_t threthold)
{
    R_ATMLIB_CLData clData;
    uint32_t        data[10];
    R_ATMLIB_OCVRectParam rect;
    uint32_t program[5] = {
		0x050a0002U, /* STCI CR10,2 */
		0x19f7c080U, /* MOVU R31,0 */
		0x0c841f00U, /* LDI R16,R31,0 */
		0x0a100000U, /* SBOCRD R16,0,0 */
		0x00c00000U  /* TRAP  */
	};

	const uint32_t sbo_data_length = 1024;
	const uint32_t OCV_VIBAR       = 0x0340U;
	const uint32_t OCV_SBOBAR0     = 0x0360U;
	const uint32_t OCV_VPCSAR      = 0x0320U;
	uint32_t *progMem = NULL;
	uint32_t *memories = {NULL};
	uint32_t *CLmem   = NULL;
	uint32_t progMem_phys;
	uint32_t memory_phys;

	// CLmem = (uint32_t*)MemAlloc( 256 * sizeof(uint32_t), 128, 0 );
	CLmem = (uint32_t *)MemAlloc(osal_mmngr, 256 * sizeof(uint32_t), 128, IMP_MALLOC_HW);

	progMem = (uint32_t *)MemAlloc(osal_mmngr, sizeof(program), 128, IMP_MALLOC_HW);
	progMem_phys = util_mem_get_paddr(progMem);
	memcpy(progMem, program, sizeof(program));

	memories = (uint32_t *)MemAlloc(osal_mmngr, sbo_data_length * sizeof(uint32_t), 128, IMP_MALLOC_HW);
	memory_phys = util_mem_get_paddr(memories);
	memset(memories, 0, sbo_data_length * sizeof(uint32_t));

	/* Prepare CL data structure */
	if (R_ATMLIB_E_OK != r_atmlib_InitializeOCVCL(&clData, CLmem, 256))
	{
		LOGPRINT(("Error: r_atmlib_InitializeOCVCL\n"));
	}

	data[0] = progMem_phys;
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_VIBAR, 1, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -1\n"));
	}

	data[0] = memory_phys; /* SBOBAR0 */
	data[1] = threthold;        /* SBOMMSR0 */
	data[2] = 0x00000000;  /* SBOCR0: PCKMODE = 0, XYPCK = 0 */
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_SBOBAR0, 3, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -4\n"));
	}

	data[0] = 0;/* entry */
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
	if (R_ATMLIB_E_OK != r_atmlib_OCV_WPR(&clData, OCV_VPCSAR, 4, data))
	{
		LOGPRINT(("Error: r_atmlib_OCV_WPR -5\n"));
	}

	rect.dx1 = 1;
	rect.dy1 = 1;
	rect.dx2 = 0;
	rect.dy2 = 0;
	rect.xlen = 32;
	rect.ylen = 1;
	rect.xs = 0;
	rect.ys = 0;
	if (R_ATMLIB_E_OK != r_atmlib_OCV_RECT(&clData, 0, 0, 0, &rect))
	{
		LOGPRINT(("Error: r_atmlib_OCV_RECT -1\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_OCV_INT(&clData, 0 , (R_ATMLIB_ENABLECONTROL)0))
	{
		LOGPRINT(("Error: r_atmlib_OCV_INT\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_OCV_RECT(&clData, 0, 0, 0, &rect))
	{
		LOGPRINT(("Error: r_atmlib_OCV_RECT -2\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_OCV_TRAP(&clData, 0 ))
	{
		LOGPRINT(("Error: r_atmlib_OCV_TRAP\n"));
	}

	if (R_ATMLIB_E_OK != r_atmlib_FinalizeOCVCL(&clData))
	{
		LOGPRINT(("Error: r_atmlib_FinalizeOCVCL\n"));
	}

	util_mem_flush(clData.top_addr);
	util_mem_flush(progMem);
	util_mem_flush(memories);

	return clData.top_addr;
}

void createCL_APIPINTtest(const void *const cpu_addr)
{
	uint32_t cnt;

	/* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;
	R_ATMLIB_CLData          cldata;
    R_ATMLIB_IMPSubsetParam2 param;
    R_ATMLIB_ImageParam      imgparam;
	uint32_t                 xsize = 16u;
    uint32_t                 ysize = 16u;
	uintptr_t                src_img_ofs;
    uintptr_t                dst_img_ofs;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));
    memset(&param, 0, sizeof(R_ATMLIB_IMPSubsetParam2));
    memset(&imgparam, 0, sizeof(R_ATMLIB_ImageParam));

	/* save cpu_addr to top_addr of cldata */
	atomic_ret = r_atmlib_InitializeIMPCL(&cldata,
        (uint32_t *)(cpu_addr), /* cast to uint32_t * */
        INT_CL_SIZE / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] %s: r_atmlib_InitializeIMPCL  : code %d>>\n", __func__, (int32_t)atomic_ret)); /* cast to int32_t */
    }

	/*----- Atomic CL Creation -----*/
    for (cnt = 0; cnt < IMAGE_COPY_COUNT; cnt++)
    {
		src_img_ofs = INT_CL_SIZE + APIPINT_IMAGE_SIZE * (cnt * 2 + 0);
		dst_img_ofs = INT_CL_SIZE + APIPINT_IMAGE_SIZE * (cnt * 2 + 1);

		memset((void *)(cpu_addr + src_img_ofs), 0xAA, APIPINT_IMAGE_SIZE);
		memset((void *)(cpu_addr + dst_img_ofs), 0x00, APIPINT_IMAGE_SIZE);

        /* Set IMP Subset */
        param.leng        = (ysize << 16u) | xsize;
        param.mag         = ((uint32_t)1u << 16u) | (uint32_t)1u; /* cast to uint32_t */
        param.scale       = 0u;
        param.const_a     = 0x00;
        param.const_b     = 0x00;
        param.binthr_min  = 0x00;
        param.binthr_max  = 0x00;
        param.opt         = 0u;
        param.srca_unpack = R_ATMLIB_DISABLE;
        param.srcb_unpack = R_ATMLIB_DISABLE;
        param.dst_pack    = R_ATMLIB_DISABLE;

        atomic_ret = r_atmlib_SetDataIMPCL2(&cldata, R_ATMLIB_IMP_COPY, &param);
        if (R_ATMLIB_E_OK != atomic_ret)
        {
            LOGPRINT(("<< [ERROR] %s: r_atmlib_SetDataIMPCL  : code %d>>\n", __func__, atomic_ret)); /* cast to int32_t */
            break;
        }
        else
        {
	        /* Set Image */
	        imgparam.srca_type   = R_ATMLIB_IMG_8U;
	        imgparam.dst_type    = R_ATMLIB_IMG_8U;
	        imgparam.srca_stride = xsize;
	        imgparam.dst_stride  = xsize;
	        imgparam.srca_addr   = (uint32_t)(util_mem_get_paddr(cpu_addr) + src_img_ofs); /* cast to uint32_t */
	        imgparam.dst_addr    = (uint32_t)(util_mem_get_paddr(cpu_addr) + dst_img_ofs); /* cast to uint32_t */

	        atomic_ret = r_atmlib_SetImageIMPCL(&cldata, &imgparam);
	        if (R_ATMLIB_E_OK != atomic_ret)
	        {
	            LOGPRINT(("<< [ERROR] %s: r_atmlib_SetImageIMPCL  : code %d>>\n", __func__, atomic_ret)); /* cast to int32_t */
            	break;
	        }
	        else
	        {
		    	atomic_ret =  r_atmlib_IMP_NOP(&cldata, INT_NOP_NUM);
		        if (R_ATMLIB_E_OK != atomic_ret)
		        {
		            LOGPRINT(("<< [ERROR] %s: r_atmlib_IMP_NOP  : code %d>>\n", __func__, atomic_ret)); /* cast to int32_t */
            		break;
		        }
			}
		}
    }

	if (R_ATMLIB_E_OK == atomic_ret)
    {
        /*----- Atomic Finalization -----*/
        atomic_ret = r_atmlib_FinalizeIMPCL(&cldata);
        if (R_ATMLIB_E_OK != atomic_ret)
        {
            LOGPRINT(("<< [ERROR] %s: r_atmlib_FinalizeIMPCL  : code %d>>\n", __func__, atomic_ret)); /* cast to int32_t */
        }
    }
}

void createCL_HPINTtest(const void *const cpu_addr)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    	atomic_ret;

    R_ATMLIB_CLData          	cldata;

    uint32_t                 	xsize = 16u;
    uint32_t                 	ysize = 16u;

    uintptr_t                	src_img_ofs;
    uintptr_t                	dst_img_ofs;

    R_ATMLIB_IMPSubsetParam2	param;
    R_ATMLIB_ImageParam      	imgparam;

	R_ATMLIB_IMPInitHMParam 	initparam;
	R_ATMLIB_HISTSEL			histsel;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));
    memset(&param, 0, sizeof(R_ATMLIB_IMPSubsetParam2));
    memset(&imgparam, 0, sizeof(R_ATMLIB_ImageParam));

    atomic_ret = r_atmlib_InitializeIMPCL(&cldata,
        (uint32_t *)cpu_addr, /* cast to uint32_t * */
        INT_CL_SIZE / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_InitializeIMPCL  : code %d>>\n", (int32_t)atomic_ret)); /* cast to int32_t */
    }

    if (R_ATMLIB_E_OK == atomic_ret)
    {
	    histsel = R_ATMLIB_HISTSEL_0;

	    initparam.init_en        = R_ATMLIB_ENABLE;
		initparam.init_u         = 0x0000;
		initparam.init_l         = 0x0000;

		atomic_ret = r_atmlib_SetInitializeHM(&cldata, &initparam, histsel);
	    if (R_ATMLIB_E_OK != atomic_ret)
	    {
	        LOGPRINT(("<< [ERROR] r_atmlib_SetInitializeHM  : code %d>>\n", (int32_t)atomic_ret)); /* cast to int32_t */
	    }
		else
		{
	    	atomic_ret =  r_atmlib_IMP_NOP(&cldata, 5);
	        if (R_ATMLIB_E_OK != atomic_ret)
	        {
	            LOGPRINT(("<< [ERROR] r_atmlib_IMP_NOP  : code %d>>\n", atomic_ret)); /* cast to int32_t */
	        }
	    }
	}

    if (R_ATMLIB_E_OK == atomic_ret)
    {
		src_img_ofs = INT_CL_SIZE + HPINT_IMAGE_SIZE * 0;
		dst_img_ofs = INT_CL_SIZE + HPINT_IMAGE_SIZE * 1;

		memset((void *)(cpu_addr + src_img_ofs), 0xAA, HPINT_IMAGE_SIZE);
		memset((void *)(cpu_addr + dst_img_ofs), 0x00, HPINT_IMAGE_SIZE);

	    /* Set IMP Subset */
	    param.leng        = (ysize << 16u) | xsize;
	    param.mag         = ((uint32_t)1u << 16u) | (uint32_t)1u; /* cast to uint32_t */
	    param.scale       = 0u;
	    param.const_a     = 0x00;
	    param.const_b     = 0x00;
	    param.binthr_min  = 0x00;
	    param.binthr_max  = 0x00;
	    param.opt         = 0u;
	    param.srca_unpack = R_ATMLIB_DISABLE;
	    param.srcb_unpack = R_ATMLIB_DISABLE;
	    param.dst_pack    = R_ATMLIB_DISABLE;

	    atomic_ret = r_atmlib_SetDataIMPCL2(&cldata, R_ATMLIB_IMP_COPY, &param);
	    if (R_ATMLIB_E_OK != atomic_ret)
	    {
	        LOGPRINT(("<< [ERROR] r_atmlib_SetDataIMPCL  : code %d>>\n", atomic_ret)); /* cast to int32_t */
	    }
	    else
	    {
	        /* Set Image */
	        imgparam.srca_type   = R_ATMLIB_IMG_8U;
	        imgparam.dst_type    = R_ATMLIB_IMG_8U;
	        imgparam.srca_stride = xsize;
	        imgparam.dst_stride  = xsize;
	        imgparam.srca_addr   = (uint32_t)(util_mem_get_paddr(cpu_addr) + src_img_ofs); /* cast to uint32_t */
	        imgparam.dst_addr    = (uint32_t)(util_mem_get_paddr(cpu_addr) + dst_img_ofs); /* cast to uint32_t */

	        atomic_ret = r_atmlib_SetImageIMPCL(&cldata, &imgparam);
	        if (R_ATMLIB_E_OK != atomic_ret)
	        {
	            LOGPRINT(("<< [ERROR] r_atmlib_SetImageIMPCL  : code %d>>\n", atomic_ret)); /* cast to int32_t */
	        }
	        else
	        {
		    	atomic_ret =  r_atmlib_IMP_NOP(&cldata, INT_NOP_NUM);
		        if (R_ATMLIB_E_OK != atomic_ret)
		        {
		            LOGPRINT(("<< [ERROR] r_atmlib_IMP_NOP  : code %d>>\n", atomic_ret)); /* cast to int32_t */
		        }
			}
		}
	}

    if (R_ATMLIB_E_OK == atomic_ret)
    {
        /*----- Atomic Finalization -----*/
        atomic_ret = r_atmlib_FinalizeIMPCL(&cldata);
        if (R_ATMLIB_E_OK != atomic_ret)
        {
            LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n", atomic_ret)); /* cast to int32_t */
        }
    }
}

void createCL_USINTtest(void *const cpu_addr)
{
    uint32_t index;
	uint32_t *claddr;

	claddr = (uint32_t *)cpu_addr;

	/* Set CL */
	index = 0;

	claddr[index++] =	0x55028401;	/* SPBSYCNTR*/
	claddr[index++] =	0x00000000;
	claddr[index++] =	0x55028001;	/* SPCR */
	claddr[index++] =	0x00000003;

	claddr[index++] =	0x55034001;	/* VIBAR */
	claddr[index++] =	util_mem_get_paddr(cpu_addr) + 0x1000;	/* thread program */
	claddr[index++] =	0x550c0009;	/* Uniform register */
	claddr[index++] =	0x01234567;
	claddr[index++] =	0x89abcdef;
	claddr[index++] =	0x00112233;
	claddr[index++] =	0x44556677;
	claddr[index++] =	0x8899aabb;
	claddr[index++] =	0xccddeeff;
	claddr[index++] =	0x12343210;
	claddr[index++] =	0x45677654;
	claddr[index++] =	0x00000010;

	claddr[index++] =	0x55070801;	/* TGSEN1 */
	claddr[index++] =	0x000000ff;
	claddr[index++] =	0x55032001;	/* VPCSAR */
	claddr[index++] =	0x00000000;

	claddr[index++] =	0x55041803;
	claddr[index++] =	util_mem_get_paddr(cpu_addr) + 0x2000;	/* output image */
	claddr[index++] =	0x00000004;
	claddr[index++] =	0x00051105;

	claddr[index++] =	0x22000000;
	claddr[index++] =	0x00010001;
	claddr[index++] =	0x00000000;
	claddr[index++] =	0x00000000;
	claddr[index++] =	0x00010020;
	claddr[index++] =	0x8b00c000;
	claddr[index++] =	0x55028001;
	claddr[index++] =	0x00000000;
	claddr[index++] =	0x88000000;

	/* Set thread program */
	index = 0x1000 / 4;

	claddr[index++] =	0x05090002;	/* STCI	CR9, 2 */
	claddr[index++] =	0x18804108;	/* MULU	R1,R1,8 */

	claddr[index++] =	0x01000000;	/* USINT */

	claddr[index++] =	0x19F40000; /* MOVU    R16,0 */
	claddr[index++] =	0x19F44001; /* MOVU    R17,1 */
	claddr[index++] =	0x19F48002; /* MOVU    R18,2 */
	claddr[index++] =	0x19F4C003; /* MOVU    R19,3 */
	claddr[index++] =	0x19F50004; /* MOVU    R20,4 */
	claddr[index++] =	0x19F54005; /* MOVU    R21,5 */
	claddr[index++] =	0x19F58006; /* MOVU    R22,6 */
	claddr[index++] =	0x19F5C007; /* MOVU    R23,7 */
	claddr[index++] =	0x34880814; /* PACKB0  R32,R16,R20 */
	claddr[index++] =	0x34884814; /* PACKB0  R33,R16,R20 */
	claddr[index++] =	0x34888814; /* PACKB0  R34,R16,R20 */
	claddr[index++] =	0x3488C814; /* PACKB0  R35,R16,R20 */
	claddr[index++] =	0x34890814; /* PACKB0  R36,R16,R20 */
	claddr[index++] =	0x34894814; /* PACKB0  R37,R16,R20 */
	claddr[index++] =	0x34898814; /* PACKB0  R38,R16,R20 */
	claddr[index++] =	0x3489C814; /* PACKB0  R39,R16,R20 */

	claddr[index++] =	0x348a0814; /* PACKB0  R40,R16,R20 */
	claddr[index++] =	0x34aa4814; /* PACKB1  R41,R16,R20 */
	claddr[index++] =	0x34ca8814; /* PACKB2  R42,R16,R20 */
	claddr[index++] =	0x34eaC814; /* PACKB3  R43,R16,R20 */
	claddr[index++] =	0x344b0814; /* PACKH0  R44,R16,R20 */
	claddr[index++] =	0x346b4814; /* PACKH1  R45,R16,R20 */
	claddr[index++] =	0x348b8814; /* PACKB0  R46,R16,R20 */
	claddr[index++] =	0x34abC814; /* PACKB1  R47,R16,R20 */

	claddr[index++] =	0x08a00000;
	claddr[index++] =	0x08a10001;
	claddr[index++] =	0x08a20002;
	claddr[index++] =	0x08a30003;
	claddr[index++] =	0x08a40004;
	claddr[index++] =	0x08a50005;
	claddr[index++] =	0x08a60006;
	claddr[index++] =	0x08a70007;
	claddr[index++] =	0x08a80008;
	claddr[index++] =	0x08a90009;
	claddr[index++] =	0x08aa000a;
	claddr[index++] =	0x08ab000b;
	claddr[index++] =	0x08ac000c;
	claddr[index++] =	0x08ad000d;
	claddr[index++] =	0x08ae000e;
	claddr[index++] =	0x08af000f;

	claddr[index++] =	0x00c00000;
}

static int8_t check_size(const void *const cpu_addr, uint32_t size)
{
	
	/* Check paramter */
	if (util_mem_get_size(cpu_addr) < size)
	{
		return -1;
	}

	return 0;
}

static int8_t set_dmai0(void *cpu_addr, uint32_t offset)
{
	uint32_t *claddr;

	/* Check paramter */
	if (check_size(cpu_addr, offset + MSCO_INTERNAL_SUB_CL_SIZE))
	{
		return -1;
	}

	/* Set CL */
	claddr = cpu_addr + offset + MSCO_INTERNAL_SUB_CL_SIZE;
	memcpy(claddr, DMAI0_data, sizeof(DMAI0_data));

	return 0;
}

static int8_t set_dma3dc0(void *cpu_addr, uint32_t offset)
{
	uint32_t *claddr;

	/* Check paramter */
	if (check_size(cpu_addr, offset + MSCO_INTERNAL_SUB_CL_SIZE))
	{
		return -1;
	}

	/* Set CL */
	claddr = cpu_addr + offset + MSCO_INTERNAL_SUB_CL_SIZE;
	memcpy(claddr, DMA3DC0_data, sizeof(DMA3DC0_data));

	return 0;
}

static int8_t set_dmao0(void *cpu_addr, uint32_t offset)
{
	uint32_t *claddr;

	/* Check paramter */
	if (check_size(cpu_addr, offset + MSCO_INTERNAL_SUB_CL_SIZE))
	{
		return -1;
	}

	/* Set CL */
	claddr = cpu_addr + offset + MSCO_INTERNAL_SUB_CL_SIZE;
	memcpy(claddr, DMAO0_31_data, sizeof(DMAO0_31_data));

	return 0;
}

int8_t createCL_MSCOtest(void *const cpu_addr)
{
	uint32_t index;
	uint32_t *claddr = (uint32_t *)cpu_addr;

	/* set sub-cl */
	if (set_dmai0(cpu_addr, DMAI0_OFFSET))
	{
		return -1;
	}

	if (set_dma3dc0(cpu_addr, DMA3DC0_OFFSET))
	{
		return -1;
	}

	if (set_dmao0(cpu_addr, DMAO0_31_OFFSET))
	{
		return -1;
	}

	claddr = (uint32_t *)cpu_addr;

	/* Set CL */
	index = 0;

	claddr[index++] =	0x55040001; /* WPR 0x1000 1 */
	claddr[index++] =	0x00000000; /*   ARICSR set to 0x00000000 */
	claddr[index++] =	0x55040401; /* WPR 0x1010 1 */
	claddr[index++] =	0x00100020; /*   ARI_LEN set to 0x00100020 */
	claddr[index++] =	0x55040101; /* WPR 0x1004 1 */
	claddr[index++] =	0x00000001; /*   ARIE set to 0xffffffff */
//	claddr[index++] =	0xffffffff; /*   ARIE set to 0xffffffff */
	claddr[index++] =	0x55100004; /* WPR 0x4000 4 */
	claddr[index++] =	util_mem_get_paddr(cpu_addr) + DMAI0_OFFSET; /*   DMAISA set to 0x40000000 */
	claddr[index++] =	0x00000010; /*   DMAICO set to 0x00000010 */
	claddr[index++] =	0x00100020; /*   DMAIL set to 0x00100020 */
	claddr[index++] =	0x00000020; /*   DMAIST set to 0x00000020 */
	claddr[index++] =	0x55100501; /* WPR 0x4014 1 */
	claddr[index++] =	0x00000002; /*   DMAIFM set to 0x00000002 */
	claddr[index++] =	0x55040201; /* WPR 0x1008 1 */
	claddr[index++] =	0x00111092; /*   PADD_STRIDE_STARTPOS set to 0x00111092 */
	claddr[index++] =	0x55100702; /* WPR 0x401c 2 */
	claddr[index++] =	0x00000001; /*   DMAIE set to 0x0000ffff */
	claddr[index++] =	0x00000003; /*   DMAIS set to 0x00000003 */
	claddr[index++] =	0x55140004; /* WPR 0x5000 4 */
	claddr[index++] =	util_mem_get_paddr(cpu_addr) + DMA3DC0_OFFSET; /*   DMA3DCSA set to 0x50000000 */
	claddr[index++] =	0x00000100; /*   DMA3DCCO set to 0x00000100 */
	claddr[index++] =	0x00100020; /*   DMA3DCL  set to 0x00100020 */
	claddr[index++] =	0x00000020; /*   DMA3DCST set to 0x00000020 */
	claddr[index++] =	0x55140501; /* WPR 0x5014 1 */
	claddr[index++] =	0x00000002; /*   DMA3DCFM set to 0x00000002 */
	claddr[index++] =	0x5514a804; /* WPR 0x52a0 4 */
	claddr[index++] =	0x44444444; /*   DMA3DCFM00_07 set to 0x44444444 */
	claddr[index++] =	0x44444444; /*   DMA3DCFM08_15 set to 0x44444444 */
	claddr[index++] =	0x44444444; /*   DMA3DCFM16_23 set to 0x44444444 */
	claddr[index++] =	0x44444444; /*   DMA3DCFM24_31 set to 0x44444444 */
	claddr[index++] =	0x55140702; /* WPR 0x501c 2 */
	claddr[index++] =	0x00000001; /*   DMA3DCE set to 0xffffffff */
	claddr[index++] =	0x00000003; /*   DMA3DCS set to 0x00000003 */
	claddr[index++] =	0x55180005; /* WPR 0x6000 5 */
	claddr[index++] =	util_mem_get_paddr(cpu_addr) + DMAO0_31_OFFSET; /*   DMAOSA set to 0x60000000 */
	claddr[index++] =	0x00000100; /*   DMAOCO set to 0x00000100 */
	claddr[index++] =	0x00100020; /*   DMAOL set to 0x00100020 */
	claddr[index++] =	0x00000020; /*   DMAOST set to 0x00000020 */
	claddr[index++] =	0x00000002; /*   DMAOFM set to 0x00000002 */
	claddr[index++] =	0x55180602; /* WPR 0x6018 2 */
	claddr[index++] =	0x00000001; /*   DMAOE set to 0xffffffff */
	claddr[index++] =	0x00000003; /*   DMAOS set to 0x00000003 */
	claddr[index++] =	0x4c000000; /*  */
	claddr[index++] =	0x55200007; /* WPR 0x8000 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008004 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008008 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000800c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008010 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008014 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008018 set to 0x00000000 */
	claddr[index++] =	0x55200807; /* WPR 0x8020 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008024 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008028 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000802c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008030 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008034 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008038 set to 0x00000000 */
	claddr[index++] =	0x55201007; /* WPR 0x8040 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008044 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008048 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000804c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008050 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008054 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008058 set to 0x00000000 */
	claddr[index++] =	0x55201807; /* WPR 0x8060 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008064 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008068 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000806c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008070 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008074 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008078 set to 0x00000000 */
	claddr[index++] =	0x55202007; /* WPR 0x8080 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008084 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008088 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000808c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008090 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008094 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008098 set to 0x00000000 */
	claddr[index++] =	0x55202807; /* WPR 0x80a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000080ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000080b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080b8 set to 0x00000000 */
	claddr[index++] =	0x55203007; /* WPR 0x80c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000080cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000080d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080d8 set to 0x00000000 */
	claddr[index++] =	0x55203807; /* WPR 0x80e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000080ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000080f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000080f8 set to 0x00000000 */
	claddr[index++] =	0x55204007; /* WPR 0x8100 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008104 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008108 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000810c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008110 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008114 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008118 set to 0x00000000 */
	claddr[index++] =	0x55204807; /* WPR 0x8120 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008124 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008128 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000812c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008130 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008134 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008138 set to 0x00000000 */
	claddr[index++] =	0x55205007; /* WPR 0x8140 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008144 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008148 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000814c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008150 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008154 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008158 set to 0x00000000 */
	claddr[index++] =	0x55205807; /* WPR 0x8160 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008164 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008168 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000816c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008170 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008174 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008178 set to 0x00000000 */
	claddr[index++] =	0x55206007; /* WPR 0x8180 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008184 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008188 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000818c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008190 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008194 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008198 set to 0x00000000 */
	claddr[index++] =	0x55206807; /* WPR 0x81a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000081ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000081b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081b8 set to 0x00000000 */
	claddr[index++] =	0x55207007; /* WPR 0x81c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000081cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000081d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081d8 set to 0x00000000 */
	claddr[index++] =	0x55207807; /* WPR 0x81e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000081ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000081f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000081f8 set to 0x00000000 */
	claddr[index++] =	0x55208007; /* WPR 0x8200 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008204 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008208 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000820c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008210 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008214 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008218 set to 0x00000000 */
	claddr[index++] =	0x55208807; /* WPR 0x8220 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008224 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008228 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000822c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008230 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008234 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008238 set to 0x00000000 */
	claddr[index++] =	0x55209007; /* WPR 0x8240 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008244 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008248 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000824c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008250 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008254 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008258 set to 0x00000000 */
	claddr[index++] =	0x55209807; /* WPR 0x8260 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008264 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008268 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000826c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008270 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008274 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008278 set to 0x00000000 */
	claddr[index++] =	0x5520a007; /* WPR 0x8280 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008284 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008288 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000828c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008290 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008294 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008298 set to 0x00000000 */
	claddr[index++] =	0x5520a807; /* WPR 0x82a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000082ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000082b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082b8 set to 0x00000000 */
	claddr[index++] =	0x5520b007; /* WPR 0x82c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000082cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000082d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082d8 set to 0x00000000 */
	claddr[index++] =	0x5520b807; /* WPR 0x82e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000082ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000082f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000082f8 set to 0x00000000 */
	claddr[index++] =	0x5520c007; /* WPR 0x8300 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008304 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008308 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000830c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008310 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008314 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008318 set to 0x00000000 */
	claddr[index++] =	0x5520c807; /* WPR 0x8320 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008324 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008328 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000832c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008330 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008334 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008338 set to 0x00000000 */
	claddr[index++] =	0x5520d007; /* WPR 0x8340 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008344 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008348 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000834c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008350 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008354 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008358 set to 0x00000000 */
	claddr[index++] =	0x5520d807; /* WPR 0x8360 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008364 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008368 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000836c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008370 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008374 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008378 set to 0x00000000 */
	claddr[index++] =	0x5520e007; /* WPR 0x8380 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008384 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008388 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000838c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008390 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008394 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008398 set to 0x00000000 */
	claddr[index++] =	0x5520e807; /* WPR 0x83a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000083ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000083b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083b8 set to 0x00000000 */
	claddr[index++] =	0x5520f007; /* WPR 0x83c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000083cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000083d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083d8 set to 0x00000000 */
	claddr[index++] =	0x5520f807; /* WPR 0x83e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I00O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000083ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000083f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000083f8 set to 0x00000000 */
	claddr[index++] =	0x55210007; /* WPR 0x8400 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008404 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008408 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000840c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008410 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008414 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008418 set to 0x00000000 */
	claddr[index++] =	0x55210807; /* WPR 0x8420 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008424 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008428 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000842c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008430 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008434 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008438 set to 0x00000000 */
	claddr[index++] =	0x55211007; /* WPR 0x8440 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008444 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008448 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000844c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008450 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008454 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008458 set to 0x00000000 */
	claddr[index++] =	0x55211807; /* WPR 0x8460 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008464 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008468 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000846c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008470 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008474 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008478 set to 0x00000000 */
	claddr[index++] =	0x55212007; /* WPR 0x8480 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008484 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008488 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000848c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008490 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008494 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008498 set to 0x00000000 */
	claddr[index++] =	0x55212807; /* WPR 0x84a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000084ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000084b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084b8 set to 0x00000000 */
	claddr[index++] =	0x55213007; /* WPR 0x84c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000084cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000084d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084d8 set to 0x00000000 */
	claddr[index++] =	0x55213807; /* WPR 0x84e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000084ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000084f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000084f8 set to 0x00000000 */
	claddr[index++] =	0x55214007; /* WPR 0x8500 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008504 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008508 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000850c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008510 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008514 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008518 set to 0x00000000 */
	claddr[index++] =	0x55214807; /* WPR 0x8520 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008524 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008528 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000852c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008530 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008534 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008538 set to 0x00000000 */
	claddr[index++] =	0x55215007; /* WPR 0x8540 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008544 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008548 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000854c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008550 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008554 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008558 set to 0x00000000 */
	claddr[index++] =	0x55215807; /* WPR 0x8560 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008564 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008568 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000856c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008570 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008574 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008578 set to 0x00000000 */
	claddr[index++] =	0x55216007; /* WPR 0x8580 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008584 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008588 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000858c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008590 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008594 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008598 set to 0x00000000 */
	claddr[index++] =	0x55216807; /* WPR 0x85a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000085ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000085b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085b8 set to 0x00000000 */
	claddr[index++] =	0x55217007; /* WPR 0x85c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000085cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000085d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085d8 set to 0x00000000 */
	claddr[index++] =	0x55217807; /* WPR 0x85e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000085ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000085f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000085f8 set to 0x00000000 */
	claddr[index++] =	0x55218007; /* WPR 0x8600 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008604 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008608 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000860c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008610 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008614 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008618 set to 0x00000000 */
	claddr[index++] =	0x55218807; /* WPR 0x8620 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008624 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008628 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000862c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008630 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008634 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008638 set to 0x00000000 */
	claddr[index++] =	0x55219007; /* WPR 0x8640 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008644 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008648 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000864c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008650 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008654 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008658 set to 0x00000000 */
	claddr[index++] =	0x55219807; /* WPR 0x8660 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008664 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008668 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000866c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008670 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008674 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008678 set to 0x00000000 */
	claddr[index++] =	0x5521a007; /* WPR 0x8680 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008684 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008688 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000868c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008690 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008694 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008698 set to 0x00000000 */
	claddr[index++] =	0x5521a807; /* WPR 0x86a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000086ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000086b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086b8 set to 0x00000000 */
	claddr[index++] =	0x5521b007; /* WPR 0x86c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000086cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000086d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086d8 set to 0x00000000 */
	claddr[index++] =	0x5521b807; /* WPR 0x86e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000086ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000086f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000086f8 set to 0x00000000 */
	claddr[index++] =	0x5521c007; /* WPR 0x8700 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008704 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008708 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000870c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008710 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008714 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008718 set to 0x00000000 */
	claddr[index++] =	0x5521c807; /* WPR 0x8720 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008724 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008728 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000872c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008730 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008734 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008738 set to 0x00000000 */
	claddr[index++] =	0x5521d007; /* WPR 0x8740 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008744 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008748 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000874c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008750 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008754 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008758 set to 0x00000000 */
	claddr[index++] =	0x5521d807; /* WPR 0x8760 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008764 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008768 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000876c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008770 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008774 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008778 set to 0x00000000 */
	claddr[index++] =	0x5521e007; /* WPR 0x8780 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008784 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008788 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000878c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008790 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008794 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008798 set to 0x00000000 */
	claddr[index++] =	0x5521e807; /* WPR 0x87a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000087ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000087b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087b8 set to 0x00000000 */
	claddr[index++] =	0x5521f007; /* WPR 0x87c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000087cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000087d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087d8 set to 0x00000000 */
	claddr[index++] =	0x5521f807; /* WPR 0x87e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I01O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000087ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000087f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000087f8 set to 0x00000000 */
	claddr[index++] =	0x55220007; /* WPR 0x8800 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008804 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008808 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000880c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008810 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008814 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008818 set to 0x00000000 */
	claddr[index++] =	0x55220807; /* WPR 0x8820 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008824 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008828 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000882c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008830 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008834 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008838 set to 0x00000000 */
	claddr[index++] =	0x55221007; /* WPR 0x8840 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008844 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008848 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000884c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008850 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008854 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008858 set to 0x00000000 */
	claddr[index++] =	0x55221807; /* WPR 0x8860 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008864 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008868 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000886c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008870 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008874 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008878 set to 0x00000000 */
	claddr[index++] =	0x55222007; /* WPR 0x8880 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008884 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008888 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000888c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008890 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008894 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008898 set to 0x00000000 */
	claddr[index++] =	0x55222807; /* WPR 0x88a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000088ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000088b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088b8 set to 0x00000000 */
	claddr[index++] =	0x55223007; /* WPR 0x88c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000088cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000088d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088d8 set to 0x00000000 */
	claddr[index++] =	0x55223807; /* WPR 0x88e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000088ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000088f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000088f8 set to 0x00000000 */
	claddr[index++] =	0x55224007; /* WPR 0x8900 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008904 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008908 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000890c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008910 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008914 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008918 set to 0x00000000 */
	claddr[index++] =	0x55224807; /* WPR 0x8920 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008924 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008928 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000892c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008930 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008934 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008938 set to 0x00000000 */
	claddr[index++] =	0x55225007; /* WPR 0x8940 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008944 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008948 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000894c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008950 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008954 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008958 set to 0x00000000 */
	claddr[index++] =	0x55225807; /* WPR 0x8960 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008964 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008968 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000896c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008970 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008974 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008978 set to 0x00000000 */
	claddr[index++] =	0x55226007; /* WPR 0x8980 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008984 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008988 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000898c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008990 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008994 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008998 set to 0x00000000 */
	claddr[index++] =	0x55226807; /* WPR 0x89a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000089ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000089b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089b8 set to 0x00000000 */
	claddr[index++] =	0x55227007; /* WPR 0x89c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000089cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000089d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089d8 set to 0x00000000 */
	claddr[index++] =	0x55227807; /* WPR 0x89e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000089ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000089f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000089f8 set to 0x00000000 */
	claddr[index++] =	0x55228007; /* WPR 0x8a00 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008a0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a18 set to 0x00000000 */
	claddr[index++] =	0x55228807; /* WPR 0x8a20 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008a2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a38 set to 0x00000000 */
	claddr[index++] =	0x55229007; /* WPR 0x8a40 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008a4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a58 set to 0x00000000 */
	claddr[index++] =	0x55229807; /* WPR 0x8a60 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008a6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a78 set to 0x00000000 */
	claddr[index++] =	0x5522a007; /* WPR 0x8a80 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008a8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008a98 set to 0x00000000 */
	claddr[index++] =	0x5522a807; /* WPR 0x8aa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008aa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008aa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008aac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ab0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ab4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ab8 set to 0x00000000 */
	claddr[index++] =	0x5522b007; /* WPR 0x8ac0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ac4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ac8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008acc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ad0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ad4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ad8 set to 0x00000000 */
	claddr[index++] =	0x5522b807; /* WPR 0x8ae0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ae4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ae8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008aec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008af0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008af4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008af8 set to 0x00000000 */
	claddr[index++] =	0x5522c007; /* WPR 0x8b00 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008b0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b18 set to 0x00000000 */
	claddr[index++] =	0x5522c807; /* WPR 0x8b20 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008b2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b38 set to 0x00000000 */
	claddr[index++] =	0x5522d007; /* WPR 0x8b40 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008b4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b58 set to 0x00000000 */
	claddr[index++] =	0x5522d807; /* WPR 0x8b60 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008b6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b78 set to 0x00000000 */
	claddr[index++] =	0x5522e007; /* WPR 0x8b80 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008b8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008b98 set to 0x00000000 */
	claddr[index++] =	0x5522e807; /* WPR 0x8ba0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ba4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ba8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008bac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bb8 set to 0x00000000 */
	claddr[index++] =	0x5522f007; /* WPR 0x8bc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008bcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bd8 set to 0x00000000 */
	claddr[index++] =	0x5522f807; /* WPR 0x8be0 7 */
	claddr[index++] =	0x00000000; /*   WB0I02O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008be4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008be8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008bec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008bf8 set to 0x00000000 */
	claddr[index++] =	0x55230007; /* WPR 0x8c00 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008c0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c18 set to 0x00000000 */
	claddr[index++] =	0x55230807; /* WPR 0x8c20 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008c2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c38 set to 0x00000000 */
	claddr[index++] =	0x55231007; /* WPR 0x8c40 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008c4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c58 set to 0x00000000 */
	claddr[index++] =	0x55231807; /* WPR 0x8c60 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008c6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c78 set to 0x00000000 */
	claddr[index++] =	0x55232007; /* WPR 0x8c80 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008c8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008c98 set to 0x00000000 */
	claddr[index++] =	0x55232807; /* WPR 0x8ca0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ca4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ca8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008cac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cb8 set to 0x00000000 */
	claddr[index++] =	0x55233007; /* WPR 0x8cc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008ccc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cd8 set to 0x00000000 */
	claddr[index++] =	0x55233807; /* WPR 0x8ce0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ce4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ce8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008cec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008cf8 set to 0x00000000 */
	claddr[index++] =	0x55234007; /* WPR 0x8d00 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008d0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d18 set to 0x00000000 */
	claddr[index++] =	0x55234807; /* WPR 0x8d20 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008d2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d38 set to 0x00000000 */
	claddr[index++] =	0x55235007; /* WPR 0x8d40 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008d4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d58 set to 0x00000000 */
	claddr[index++] =	0x55235807; /* WPR 0x8d60 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008d6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d78 set to 0x00000000 */
	claddr[index++] =	0x55236007; /* WPR 0x8d80 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008d8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008d98 set to 0x00000000 */
	claddr[index++] =	0x55236807; /* WPR 0x8da0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008da4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008da8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008dac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008db0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008db4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008db8 set to 0x00000000 */
	claddr[index++] =	0x55237007; /* WPR 0x8dc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008dc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008dc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008dcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008dd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008dd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008dd8 set to 0x00000000 */
	claddr[index++] =	0x55237807; /* WPR 0x8de0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008de4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008de8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008dec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008df0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008df4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008df8 set to 0x00000000 */
	claddr[index++] =	0x55238007; /* WPR 0x8e00 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008e0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e18 set to 0x00000000 */
	claddr[index++] =	0x55238807; /* WPR 0x8e20 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008e2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e38 set to 0x00000000 */
	claddr[index++] =	0x55239007; /* WPR 0x8e40 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008e4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e58 set to 0x00000000 */
	claddr[index++] =	0x55239807; /* WPR 0x8e60 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008e6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e78 set to 0x00000000 */
	claddr[index++] =	0x5523a007; /* WPR 0x8e80 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008e8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008e98 set to 0x00000000 */
	claddr[index++] =	0x5523a807; /* WPR 0x8ea0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ea4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ea8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008eac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008eb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008eb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008eb8 set to 0x00000000 */
	claddr[index++] =	0x5523b007; /* WPR 0x8ec0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ec4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ec8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008ecc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ed0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ed4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ed8 set to 0x00000000 */
	claddr[index++] =	0x5523b807; /* WPR 0x8ee0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ee4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ee8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008eec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ef0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ef4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ef8 set to 0x00000000 */
	claddr[index++] =	0x5523c007; /* WPR 0x8f00 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008f0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f18 set to 0x00000000 */
	claddr[index++] =	0x5523c807; /* WPR 0x8f20 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008f2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f38 set to 0x00000000 */
	claddr[index++] =	0x5523d007; /* WPR 0x8f40 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008f4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f58 set to 0x00000000 */
	claddr[index++] =	0x5523d807; /* WPR 0x8f60 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008f6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f78 set to 0x00000000 */
	claddr[index++] =	0x5523e007; /* WPR 0x8f80 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008f8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008f98 set to 0x00000000 */
	claddr[index++] =	0x5523e807; /* WPR 0x8fa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008fac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fb8 set to 0x00000000 */
	claddr[index++] =	0x5523f007; /* WPR 0x8fc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008fcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fd8 set to 0x00000000 */
	claddr[index++] =	0x5523f807; /* WPR 0x8fe0 7 */
	claddr[index++] =	0x00000000; /*   WB0I03O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fe4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008fe8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00008fec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ff0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ff4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00008ff8 set to 0x00000000 */
	claddr[index++] =	0x55240007; /* WPR 0x9000 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009004 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009008 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000900c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009010 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009014 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009018 set to 0x00000000 */
	claddr[index++] =	0x55240807; /* WPR 0x9020 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009024 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009028 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000902c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009030 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009034 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009038 set to 0x00000000 */
	claddr[index++] =	0x55241007; /* WPR 0x9040 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009044 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009048 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000904c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009050 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009054 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009058 set to 0x00000000 */
	claddr[index++] =	0x55241807; /* WPR 0x9060 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009064 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009068 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000906c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009070 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009074 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009078 set to 0x00000000 */
	claddr[index++] =	0x55242007; /* WPR 0x9080 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009084 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009088 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000908c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009090 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009094 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009098 set to 0x00000000 */
	claddr[index++] =	0x55242807; /* WPR 0x90a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000090ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000090b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090b8 set to 0x00000000 */
	claddr[index++] =	0x55243007; /* WPR 0x90c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000090cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000090d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090d8 set to 0x00000000 */
	claddr[index++] =	0x55243807; /* WPR 0x90e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000090ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000090f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000090f8 set to 0x00000000 */
	claddr[index++] =	0x55244007; /* WPR 0x9100 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009104 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009108 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000910c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009110 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009114 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009118 set to 0x00000000 */
	claddr[index++] =	0x55244807; /* WPR 0x9120 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009124 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009128 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000912c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009130 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009134 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009138 set to 0x00000000 */
	claddr[index++] =	0x55245007; /* WPR 0x9140 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009144 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009148 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000914c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009150 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009154 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009158 set to 0x00000000 */
	claddr[index++] =	0x55245807; /* WPR 0x9160 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009164 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009168 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000916c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009170 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009174 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009178 set to 0x00000000 */
	claddr[index++] =	0x55246007; /* WPR 0x9180 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009184 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009188 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000918c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009190 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009194 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009198 set to 0x00000000 */
	claddr[index++] =	0x55246807; /* WPR 0x91a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000091ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000091b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091b8 set to 0x00000000 */
	claddr[index++] =	0x55247007; /* WPR 0x91c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000091cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000091d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091d8 set to 0x00000000 */
	claddr[index++] =	0x55247807; /* WPR 0x91e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000091ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000091f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000091f8 set to 0x00000000 */
	claddr[index++] =	0x55248007; /* WPR 0x9200 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009204 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009208 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000920c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009210 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009214 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009218 set to 0x00000000 */
	claddr[index++] =	0x55248807; /* WPR 0x9220 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009224 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009228 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000922c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009230 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009234 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009238 set to 0x00000000 */
	claddr[index++] =	0x55249007; /* WPR 0x9240 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009244 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009248 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000924c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009250 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009254 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009258 set to 0x00000000 */
	claddr[index++] =	0x55249807; /* WPR 0x9260 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009264 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009268 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000926c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009270 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009274 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009278 set to 0x00000000 */
	claddr[index++] =	0x5524a007; /* WPR 0x9280 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009284 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009288 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000928c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009290 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009294 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009298 set to 0x00000000 */
	claddr[index++] =	0x5524a807; /* WPR 0x92a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000092ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000092b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092b8 set to 0x00000000 */
	claddr[index++] =	0x5524b007; /* WPR 0x92c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000092cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000092d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092d8 set to 0x00000000 */
	claddr[index++] =	0x5524b807; /* WPR 0x92e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000092ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000092f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000092f8 set to 0x00000000 */
	claddr[index++] =	0x5524c007; /* WPR 0x9300 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009304 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009308 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000930c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009310 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009314 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009318 set to 0x00000000 */
	claddr[index++] =	0x5524c807; /* WPR 0x9320 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009324 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009328 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000932c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009330 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009334 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009338 set to 0x00000000 */
	claddr[index++] =	0x5524d007; /* WPR 0x9340 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009344 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009348 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000934c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009350 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009354 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009358 set to 0x00000000 */
	claddr[index++] =	0x5524d807; /* WPR 0x9360 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009364 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009368 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000936c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009370 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009374 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009378 set to 0x00000000 */
	claddr[index++] =	0x5524e007; /* WPR 0x9380 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009384 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009388 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000938c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009390 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009394 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009398 set to 0x00000000 */
	claddr[index++] =	0x5524e807; /* WPR 0x93a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000093ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000093b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093b8 set to 0x00000000 */
	claddr[index++] =	0x5524f007; /* WPR 0x93c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000093cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000093d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093d8 set to 0x00000000 */
	claddr[index++] =	0x5524f807; /* WPR 0x93e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I04O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000093ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000093f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000093f8 set to 0x00000000 */
	claddr[index++] =	0x55250007; /* WPR 0x9400 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009404 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009408 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000940c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009410 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009414 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009418 set to 0x00000000 */
	claddr[index++] =	0x55250807; /* WPR 0x9420 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009424 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009428 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000942c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009430 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009434 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009438 set to 0x00000000 */
	claddr[index++] =	0x55251007; /* WPR 0x9440 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009444 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009448 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000944c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009450 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009454 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009458 set to 0x00000000 */
	claddr[index++] =	0x55251807; /* WPR 0x9460 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009464 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009468 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000946c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009470 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009474 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009478 set to 0x00000000 */
	claddr[index++] =	0x55252007; /* WPR 0x9480 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009484 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009488 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000948c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009490 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009494 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009498 set to 0x00000000 */
	claddr[index++] =	0x55252807; /* WPR 0x94a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000094ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000094b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094b8 set to 0x00000000 */
	claddr[index++] =	0x55253007; /* WPR 0x94c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000094cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000094d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094d8 set to 0x00000000 */
	claddr[index++] =	0x55253807; /* WPR 0x94e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000094ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000094f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000094f8 set to 0x00000000 */
	claddr[index++] =	0x55254007; /* WPR 0x9500 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009504 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009508 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000950c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009510 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009514 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009518 set to 0x00000000 */
	claddr[index++] =	0x55254807; /* WPR 0x9520 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009524 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009528 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000952c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009530 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009534 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009538 set to 0x00000000 */
	claddr[index++] =	0x55255007; /* WPR 0x9540 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009544 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009548 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000954c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009550 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009554 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009558 set to 0x00000000 */
	claddr[index++] =	0x55255807; /* WPR 0x9560 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009564 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009568 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000956c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009570 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009574 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009578 set to 0x00000000 */
	claddr[index++] =	0x55256007; /* WPR 0x9580 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009584 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009588 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000958c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009590 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009594 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009598 set to 0x00000000 */
	claddr[index++] =	0x55256807; /* WPR 0x95a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000095ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000095b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095b8 set to 0x00000000 */
	claddr[index++] =	0x55257007; /* WPR 0x95c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000095cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000095d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095d8 set to 0x00000000 */
	claddr[index++] =	0x55257807; /* WPR 0x95e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000095ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000095f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000095f8 set to 0x00000000 */
	claddr[index++] =	0x55258007; /* WPR 0x9600 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009604 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009608 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000960c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009610 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009614 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009618 set to 0x00000000 */
	claddr[index++] =	0x55258807; /* WPR 0x9620 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009624 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009628 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000962c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009630 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009634 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009638 set to 0x00000000 */
	claddr[index++] =	0x55259007; /* WPR 0x9640 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009644 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009648 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000964c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009650 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009654 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009658 set to 0x00000000 */
	claddr[index++] =	0x55259807; /* WPR 0x9660 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009664 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009668 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000966c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009670 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009674 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009678 set to 0x00000000 */
	claddr[index++] =	0x5525a007; /* WPR 0x9680 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009684 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009688 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000968c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009690 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009694 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009698 set to 0x00000000 */
	claddr[index++] =	0x5525a807; /* WPR 0x96a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000096ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000096b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096b8 set to 0x00000000 */
	claddr[index++] =	0x5525b007; /* WPR 0x96c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000096cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000096d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096d8 set to 0x00000000 */
	claddr[index++] =	0x5525b807; /* WPR 0x96e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000096ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000096f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000096f8 set to 0x00000000 */
	claddr[index++] =	0x5525c007; /* WPR 0x9700 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009704 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009708 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000970c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009710 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009714 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009718 set to 0x00000000 */
	claddr[index++] =	0x5525c807; /* WPR 0x9720 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009724 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009728 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000972c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009730 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009734 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009738 set to 0x00000000 */
	claddr[index++] =	0x5525d007; /* WPR 0x9740 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009744 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009748 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000974c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009750 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009754 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009758 set to 0x00000000 */
	claddr[index++] =	0x5525d807; /* WPR 0x9760 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009764 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009768 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000976c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009770 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009774 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009778 set to 0x00000000 */
	claddr[index++] =	0x5525e007; /* WPR 0x9780 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009784 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009788 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000978c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009790 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009794 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009798 set to 0x00000000 */
	claddr[index++] =	0x5525e807; /* WPR 0x97a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000097ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000097b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097b8 set to 0x00000000 */
	claddr[index++] =	0x5525f007; /* WPR 0x97c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000097cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000097d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097d8 set to 0x00000000 */
	claddr[index++] =	0x5525f807; /* WPR 0x97e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I05O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000097ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000097f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000097f8 set to 0x00000000 */
	claddr[index++] =	0x55260007; /* WPR 0x9800 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009804 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009808 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000980c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009810 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009814 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009818 set to 0x00000000 */
	claddr[index++] =	0x55260807; /* WPR 0x9820 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009824 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009828 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000982c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009830 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009834 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009838 set to 0x00000000 */
	claddr[index++] =	0x55261007; /* WPR 0x9840 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009844 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009848 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000984c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009850 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009854 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009858 set to 0x00000000 */
	claddr[index++] =	0x55261807; /* WPR 0x9860 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009864 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009868 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000986c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009870 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009874 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009878 set to 0x00000000 */
	claddr[index++] =	0x55262007; /* WPR 0x9880 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009884 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009888 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000988c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009890 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009894 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009898 set to 0x00000000 */
	claddr[index++] =	0x55262807; /* WPR 0x98a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000098ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000098b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098b8 set to 0x00000000 */
	claddr[index++] =	0x55263007; /* WPR 0x98c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000098cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000098d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098d8 set to 0x00000000 */
	claddr[index++] =	0x55263807; /* WPR 0x98e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000098ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000098f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000098f8 set to 0x00000000 */
	claddr[index++] =	0x55264007; /* WPR 0x9900 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009904 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009908 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000990c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009910 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009914 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009918 set to 0x00000000 */
	claddr[index++] =	0x55264807; /* WPR 0x9920 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009924 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009928 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000992c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009930 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009934 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009938 set to 0x00000000 */
	claddr[index++] =	0x55265007; /* WPR 0x9940 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009944 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009948 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000994c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009950 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009954 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009958 set to 0x00000000 */
	claddr[index++] =	0x55265807; /* WPR 0x9960 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009964 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009968 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000996c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009970 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009974 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009978 set to 0x00000000 */
	claddr[index++] =	0x55266007; /* WPR 0x9980 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009984 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009988 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000998c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009990 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009994 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009998 set to 0x00000000 */
	claddr[index++] =	0x55266807; /* WPR 0x99a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000099ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000099b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099b8 set to 0x00000000 */
	claddr[index++] =	0x55267007; /* WPR 0x99c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000099cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000099d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099d8 set to 0x00000000 */
	claddr[index++] =	0x55267807; /* WPR 0x99e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x000099ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x000099f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x000099f8 set to 0x00000000 */
	claddr[index++] =	0x55268007; /* WPR 0x9a00 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009a0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a18 set to 0x00000000 */
	claddr[index++] =	0x55268807; /* WPR 0x9a20 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009a2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a38 set to 0x00000000 */
	claddr[index++] =	0x55269007; /* WPR 0x9a40 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009a4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a58 set to 0x00000000 */
	claddr[index++] =	0x55269807; /* WPR 0x9a60 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009a6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a78 set to 0x00000000 */
	claddr[index++] =	0x5526a007; /* WPR 0x9a80 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009a8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009a98 set to 0x00000000 */
	claddr[index++] =	0x5526a807; /* WPR 0x9aa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009aa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009aa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009aac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ab0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ab4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ab8 set to 0x00000000 */
	claddr[index++] =	0x5526b007; /* WPR 0x9ac0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ac4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ac8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009acc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ad0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ad4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ad8 set to 0x00000000 */
	claddr[index++] =	0x5526b807; /* WPR 0x9ae0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ae4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ae8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009aec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009af0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009af4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009af8 set to 0x00000000 */
	claddr[index++] =	0x5526c007; /* WPR 0x9b00 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009b0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b18 set to 0x00000000 */
	claddr[index++] =	0x5526c807; /* WPR 0x9b20 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009b2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b38 set to 0x00000000 */
	claddr[index++] =	0x5526d007; /* WPR 0x9b40 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009b4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b58 set to 0x00000000 */
	claddr[index++] =	0x5526d807; /* WPR 0x9b60 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009b6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b78 set to 0x00000000 */
	claddr[index++] =	0x5526e007; /* WPR 0x9b80 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009b8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009b98 set to 0x00000000 */
	claddr[index++] =	0x5526e807; /* WPR 0x9ba0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ba4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ba8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009bac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bb8 set to 0x00000000 */
	claddr[index++] =	0x5526f007; /* WPR 0x9bc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009bcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bd8 set to 0x00000000 */
	claddr[index++] =	0x5526f807; /* WPR 0x9be0 7 */
	claddr[index++] =	0x00000000; /*   WB0I06O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009be4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009be8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009bec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009bf8 set to 0x00000000 */
	claddr[index++] =	0x55270007; /* WPR 0x9c00 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009c0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c18 set to 0x00000000 */
	claddr[index++] =	0x55270807; /* WPR 0x9c20 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009c2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c38 set to 0x00000000 */
	claddr[index++] =	0x55271007; /* WPR 0x9c40 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009c4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c58 set to 0x00000000 */
	claddr[index++] =	0x55271807; /* WPR 0x9c60 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009c6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c78 set to 0x00000000 */
	claddr[index++] =	0x55272007; /* WPR 0x9c80 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009c8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009c98 set to 0x00000000 */
	claddr[index++] =	0x55272807; /* WPR 0x9ca0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ca4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ca8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009cac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cb8 set to 0x00000000 */
	claddr[index++] =	0x55273007; /* WPR 0x9cc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009ccc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cd8 set to 0x00000000 */
	claddr[index++] =	0x55273807; /* WPR 0x9ce0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ce4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ce8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009cec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009cf8 set to 0x00000000 */
	claddr[index++] =	0x55274007; /* WPR 0x9d00 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009d0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d18 set to 0x00000000 */
	claddr[index++] =	0x55274807; /* WPR 0x9d20 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009d2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d38 set to 0x00000000 */
	claddr[index++] =	0x55275007; /* WPR 0x9d40 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009d4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d58 set to 0x00000000 */
	claddr[index++] =	0x55275807; /* WPR 0x9d60 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009d6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d78 set to 0x00000000 */
	claddr[index++] =	0x55276007; /* WPR 0x9d80 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009d8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009d98 set to 0x00000000 */
	claddr[index++] =	0x55276807; /* WPR 0x9da0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009da4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009da8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009dac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009db0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009db4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009db8 set to 0x00000000 */
	claddr[index++] =	0x55277007; /* WPR 0x9dc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009dc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009dc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009dcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009dd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009dd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009dd8 set to 0x00000000 */
	claddr[index++] =	0x55277807; /* WPR 0x9de0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009de4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009de8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009dec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009df0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009df4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009df8 set to 0x00000000 */
	claddr[index++] =	0x55278007; /* WPR 0x9e00 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009e0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e18 set to 0x00000000 */
	claddr[index++] =	0x55278807; /* WPR 0x9e20 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009e2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e38 set to 0x00000000 */
	claddr[index++] =	0x55279007; /* WPR 0x9e40 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009e4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e58 set to 0x00000000 */
	claddr[index++] =	0x55279807; /* WPR 0x9e60 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009e6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e78 set to 0x00000000 */
	claddr[index++] =	0x5527a007; /* WPR 0x9e80 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009e8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009e98 set to 0x00000000 */
	claddr[index++] =	0x5527a807; /* WPR 0x9ea0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ea4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ea8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009eac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009eb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009eb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009eb8 set to 0x00000000 */
	claddr[index++] =	0x5527b007; /* WPR 0x9ec0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ec4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ec8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009ecc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ed0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ed4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ed8 set to 0x00000000 */
	claddr[index++] =	0x5527b807; /* WPR 0x9ee0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ee4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ee8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009eec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ef0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ef4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ef8 set to 0x00000000 */
	claddr[index++] =	0x5527c007; /* WPR 0x9f00 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009f0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f18 set to 0x00000000 */
	claddr[index++] =	0x5527c807; /* WPR 0x9f20 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009f2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f38 set to 0x00000000 */
	claddr[index++] =	0x5527d007; /* WPR 0x9f40 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009f4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f58 set to 0x00000000 */
	claddr[index++] =	0x5527d807; /* WPR 0x9f60 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009f6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f78 set to 0x00000000 */
	claddr[index++] =	0x5527e007; /* WPR 0x9f80 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009f8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009f98 set to 0x00000000 */
	claddr[index++] =	0x5527e807; /* WPR 0x9fa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009fac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fb8 set to 0x00000000 */
	claddr[index++] =	0x5527f007; /* WPR 0x9fc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009fcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fd8 set to 0x00000000 */
	claddr[index++] =	0x5527f807; /* WPR 0x9fe0 7 */
	claddr[index++] =	0x00000000; /*   WB0I07O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fe4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009fe8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x00009fec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ff0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ff4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x00009ff8 set to 0x00000000 */
	claddr[index++] =	0x55280007; /* WPR 0xa000 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a004 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a008 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a00c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a010 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a014 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a018 set to 0x00000000 */
	claddr[index++] =	0x55280807; /* WPR 0xa020 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a024 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a028 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a02c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a030 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a034 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a038 set to 0x00000000 */
	claddr[index++] =	0x55281007; /* WPR 0xa040 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a044 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a048 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a04c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a050 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a054 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a058 set to 0x00000000 */
	claddr[index++] =	0x55281807; /* WPR 0xa060 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a064 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a068 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a06c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a070 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a074 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a078 set to 0x00000000 */
	claddr[index++] =	0x55282007; /* WPR 0xa080 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a084 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a088 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a08c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a090 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a094 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a098 set to 0x00000000 */
	claddr[index++] =	0x55282807; /* WPR 0xa0a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a0ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0b8 set to 0x00000000 */
	claddr[index++] =	0x55283007; /* WPR 0xa0c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a0cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0d8 set to 0x00000000 */
	claddr[index++] =	0x55283807; /* WPR 0xa0e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a0ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a0f8 set to 0x00000000 */
	claddr[index++] =	0x55284007; /* WPR 0xa100 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a104 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a108 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a10c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a110 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a114 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a118 set to 0x00000000 */
	claddr[index++] =	0x55284807; /* WPR 0xa120 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a124 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a128 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a12c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a130 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a134 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a138 set to 0x00000000 */
	claddr[index++] =	0x55285007; /* WPR 0xa140 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a144 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a148 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a14c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a150 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a154 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a158 set to 0x00000000 */
	claddr[index++] =	0x55285807; /* WPR 0xa160 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a164 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a168 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a16c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a170 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a174 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a178 set to 0x00000000 */
	claddr[index++] =	0x55286007; /* WPR 0xa180 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a184 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a188 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a18c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a190 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a194 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a198 set to 0x00000000 */
	claddr[index++] =	0x55286807; /* WPR 0xa1a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a1ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1b8 set to 0x00000000 */
	claddr[index++] =	0x55287007; /* WPR 0xa1c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a1cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1d8 set to 0x00000000 */
	claddr[index++] =	0x55287807; /* WPR 0xa1e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a1ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a1f8 set to 0x00000000 */
	claddr[index++] =	0x55288007; /* WPR 0xa200 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a204 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a208 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a20c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a210 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a214 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a218 set to 0x00000000 */
	claddr[index++] =	0x55288807; /* WPR 0xa220 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a224 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a228 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a22c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a230 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a234 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a238 set to 0x00000000 */
	claddr[index++] =	0x55289007; /* WPR 0xa240 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a244 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a248 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a24c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a250 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a254 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a258 set to 0x00000000 */
	claddr[index++] =	0x55289807; /* WPR 0xa260 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a264 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a268 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a26c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a270 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a274 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a278 set to 0x00000000 */
	claddr[index++] =	0x5528a007; /* WPR 0xa280 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a284 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a288 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a28c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a290 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a294 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a298 set to 0x00000000 */
	claddr[index++] =	0x5528a807; /* WPR 0xa2a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a2ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2b8 set to 0x00000000 */
	claddr[index++] =	0x5528b007; /* WPR 0xa2c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a2cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2d8 set to 0x00000000 */
	claddr[index++] =	0x5528b807; /* WPR 0xa2e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a2ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a2f8 set to 0x00000000 */
	claddr[index++] =	0x5528c007; /* WPR 0xa300 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a304 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a308 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a30c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a310 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a314 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a318 set to 0x00000000 */
	claddr[index++] =	0x5528c807; /* WPR 0xa320 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a324 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a328 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a32c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a330 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a334 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a338 set to 0x00000000 */
	claddr[index++] =	0x5528d007; /* WPR 0xa340 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a344 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a348 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a34c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a350 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a354 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a358 set to 0x00000000 */
	claddr[index++] =	0x5528d807; /* WPR 0xa360 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a364 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a368 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a36c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a370 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a374 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a378 set to 0x00000000 */
	claddr[index++] =	0x5528e007; /* WPR 0xa380 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a384 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a388 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a38c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a390 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a394 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a398 set to 0x00000000 */
	claddr[index++] =	0x5528e807; /* WPR 0xa3a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a3ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3b8 set to 0x00000000 */
	claddr[index++] =	0x5528f007; /* WPR 0xa3c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a3cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3d8 set to 0x00000000 */
	claddr[index++] =	0x5528f807; /* WPR 0xa3e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I08O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a3ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a3f8 set to 0x00000000 */
	claddr[index++] =	0x55290007; /* WPR 0xa400 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a404 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a408 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a40c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a410 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a414 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a418 set to 0x00000000 */
	claddr[index++] =	0x55290807; /* WPR 0xa420 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a424 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a428 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a42c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a430 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a434 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a438 set to 0x00000000 */
	claddr[index++] =	0x55291007; /* WPR 0xa440 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a444 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a448 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a44c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a450 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a454 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a458 set to 0x00000000 */
	claddr[index++] =	0x55291807; /* WPR 0xa460 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a464 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a468 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a46c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a470 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a474 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a478 set to 0x00000000 */
	claddr[index++] =	0x55292007; /* WPR 0xa480 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a484 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a488 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a48c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a490 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a494 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a498 set to 0x00000000 */
	claddr[index++] =	0x55292807; /* WPR 0xa4a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a4ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4b8 set to 0x00000000 */
	claddr[index++] =	0x55293007; /* WPR 0xa4c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a4cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4d8 set to 0x00000000 */
	claddr[index++] =	0x55293807; /* WPR 0xa4e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a4ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a4f8 set to 0x00000000 */
	claddr[index++] =	0x55294007; /* WPR 0xa500 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a504 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a508 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a50c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a510 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a514 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a518 set to 0x00000000 */
	claddr[index++] =	0x55294807; /* WPR 0xa520 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a524 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a528 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a52c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a530 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a534 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a538 set to 0x00000000 */
	claddr[index++] =	0x55295007; /* WPR 0xa540 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a544 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a548 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a54c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a550 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a554 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a558 set to 0x00000000 */
	claddr[index++] =	0x55295807; /* WPR 0xa560 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a564 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a568 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a56c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a570 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a574 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a578 set to 0x00000000 */
	claddr[index++] =	0x55296007; /* WPR 0xa580 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a584 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a588 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a58c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a590 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a594 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a598 set to 0x00000000 */
	claddr[index++] =	0x55296807; /* WPR 0xa5a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a5ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5b8 set to 0x00000000 */
	claddr[index++] =	0x55297007; /* WPR 0xa5c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a5cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5d8 set to 0x00000000 */
	claddr[index++] =	0x55297807; /* WPR 0xa5e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a5ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a5f8 set to 0x00000000 */
	claddr[index++] =	0x55298007; /* WPR 0xa600 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a604 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a608 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a60c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a610 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a614 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a618 set to 0x00000000 */
	claddr[index++] =	0x55298807; /* WPR 0xa620 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a624 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a628 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a62c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a630 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a634 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a638 set to 0x00000000 */
	claddr[index++] =	0x55299007; /* WPR 0xa640 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a644 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a648 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a64c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a650 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a654 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a658 set to 0x00000000 */
	claddr[index++] =	0x55299807; /* WPR 0xa660 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a664 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a668 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a66c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a670 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a674 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a678 set to 0x00000000 */
	claddr[index++] =	0x5529a007; /* WPR 0xa680 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a684 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a688 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a68c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a690 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a694 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a698 set to 0x00000000 */
	claddr[index++] =	0x5529a807; /* WPR 0xa6a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a6ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6b8 set to 0x00000000 */
	claddr[index++] =	0x5529b007; /* WPR 0xa6c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a6cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6d8 set to 0x00000000 */
	claddr[index++] =	0x5529b807; /* WPR 0xa6e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a6ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a6f8 set to 0x00000000 */
	claddr[index++] =	0x5529c007; /* WPR 0xa700 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a704 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a708 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a70c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a710 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a714 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a718 set to 0x00000000 */
	claddr[index++] =	0x5529c807; /* WPR 0xa720 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a724 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a728 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a72c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a730 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a734 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a738 set to 0x00000000 */
	claddr[index++] =	0x5529d007; /* WPR 0xa740 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a744 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a748 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a74c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a750 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a754 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a758 set to 0x00000000 */
	claddr[index++] =	0x5529d807; /* WPR 0xa760 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a764 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a768 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a76c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a770 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a774 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a778 set to 0x00000000 */
	claddr[index++] =	0x5529e007; /* WPR 0xa780 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a784 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a788 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a78c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a790 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a794 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a798 set to 0x00000000 */
	claddr[index++] =	0x5529e807; /* WPR 0xa7a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a7ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7b8 set to 0x00000000 */
	claddr[index++] =	0x5529f007; /* WPR 0xa7c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a7cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7d8 set to 0x00000000 */
	claddr[index++] =	0x5529f807; /* WPR 0xa7e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I09O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a7ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a7f8 set to 0x00000000 */
	claddr[index++] =	0x552a0007; /* WPR 0xa800 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a804 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a808 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a80c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a810 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a814 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a818 set to 0x00000000 */
	claddr[index++] =	0x552a0807; /* WPR 0xa820 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a824 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a828 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a82c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a830 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a834 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a838 set to 0x00000000 */
	claddr[index++] =	0x552a1007; /* WPR 0xa840 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a844 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a848 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a84c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a850 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a854 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a858 set to 0x00000000 */
	claddr[index++] =	0x552a1807; /* WPR 0xa860 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a864 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a868 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a86c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a870 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a874 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a878 set to 0x00000000 */
	claddr[index++] =	0x552a2007; /* WPR 0xa880 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a884 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a888 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a88c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a890 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a894 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a898 set to 0x00000000 */
	claddr[index++] =	0x552a2807; /* WPR 0xa8a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a8ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8b8 set to 0x00000000 */
	claddr[index++] =	0x552a3007; /* WPR 0xa8c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a8cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8d8 set to 0x00000000 */
	claddr[index++] =	0x552a3807; /* WPR 0xa8e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a8ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a8f8 set to 0x00000000 */
	claddr[index++] =	0x552a4007; /* WPR 0xa900 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a904 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a908 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a90c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a910 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a914 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a918 set to 0x00000000 */
	claddr[index++] =	0x552a4807; /* WPR 0xa920 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a924 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a928 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a92c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a930 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a934 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a938 set to 0x00000000 */
	claddr[index++] =	0x552a5007; /* WPR 0xa940 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a944 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a948 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a94c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a950 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a954 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a958 set to 0x00000000 */
	claddr[index++] =	0x552a5807; /* WPR 0xa960 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a964 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a968 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a96c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a970 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a974 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a978 set to 0x00000000 */
	claddr[index++] =	0x552a6007; /* WPR 0xa980 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a984 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a988 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a98c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a990 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a994 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a998 set to 0x00000000 */
	claddr[index++] =	0x552a6807; /* WPR 0xa9a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a9ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9b8 set to 0x00000000 */
	claddr[index++] =	0x552a7007; /* WPR 0xa9c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a9cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9d8 set to 0x00000000 */
	claddr[index++] =	0x552a7807; /* WPR 0xa9e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000a9ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000a9f8 set to 0x00000000 */
	claddr[index++] =	0x552a8007; /* WPR 0xaa00 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aa0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa18 set to 0x00000000 */
	claddr[index++] =	0x552a8807; /* WPR 0xaa20 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aa2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa38 set to 0x00000000 */
	claddr[index++] =	0x552a9007; /* WPR 0xaa40 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aa4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa58 set to 0x00000000 */
	claddr[index++] =	0x552a9807; /* WPR 0xaa60 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aa6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa78 set to 0x00000000 */
	claddr[index++] =	0x552aa007; /* WPR 0xaa80 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aa8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aa98 set to 0x00000000 */
	claddr[index++] =	0x552aa807; /* WPR 0xaaa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aaa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aaa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aaac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aab0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aab4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aab8 set to 0x00000000 */
	claddr[index++] =	0x552ab007; /* WPR 0xaac0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aac4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aac8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aacc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aad0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aad4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aad8 set to 0x00000000 */
	claddr[index++] =	0x552ab807; /* WPR 0xaae0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aae4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aae8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aaec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aaf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aaf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aaf8 set to 0x00000000 */
	claddr[index++] =	0x552ac007; /* WPR 0xab00 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ab0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab18 set to 0x00000000 */
	claddr[index++] =	0x552ac807; /* WPR 0xab20 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ab2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab38 set to 0x00000000 */
	claddr[index++] =	0x552ad007; /* WPR 0xab40 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ab4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab58 set to 0x00000000 */
	claddr[index++] =	0x552ad807; /* WPR 0xab60 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ab6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab78 set to 0x00000000 */
	claddr[index++] =	0x552ae007; /* WPR 0xab80 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ab8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ab98 set to 0x00000000 */
	claddr[index++] =	0x552ae807; /* WPR 0xaba0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aba4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aba8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000abac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abb8 set to 0x00000000 */
	claddr[index++] =	0x552af007; /* WPR 0xabc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000abcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abd8 set to 0x00000000 */
	claddr[index++] =	0x552af807; /* WPR 0xabe0 7 */
	claddr[index++] =	0x00000000; /*   WB0I10O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abe4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abe8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000abec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000abf8 set to 0x00000000 */
	claddr[index++] =	0x552b0007; /* WPR 0xac00 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ac0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac18 set to 0x00000000 */
	claddr[index++] =	0x552b0807; /* WPR 0xac20 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ac2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac38 set to 0x00000000 */
	claddr[index++] =	0x552b1007; /* WPR 0xac40 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ac4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac58 set to 0x00000000 */
	claddr[index++] =	0x552b1807; /* WPR 0xac60 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ac6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac78 set to 0x00000000 */
	claddr[index++] =	0x552b2007; /* WPR 0xac80 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ac8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ac98 set to 0x00000000 */
	claddr[index++] =	0x552b2807; /* WPR 0xaca0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aca4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aca8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000acac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acb8 set to 0x00000000 */
	claddr[index++] =	0x552b3007; /* WPR 0xacc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000accc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acd8 set to 0x00000000 */
	claddr[index++] =	0x552b3807; /* WPR 0xace0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ace4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ace8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000acec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000acf8 set to 0x00000000 */
	claddr[index++] =	0x552b4007; /* WPR 0xad00 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ad0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad18 set to 0x00000000 */
	claddr[index++] =	0x552b4807; /* WPR 0xad20 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ad2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad38 set to 0x00000000 */
	claddr[index++] =	0x552b5007; /* WPR 0xad40 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ad4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad58 set to 0x00000000 */
	claddr[index++] =	0x552b5807; /* WPR 0xad60 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ad6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad78 set to 0x00000000 */
	claddr[index++] =	0x552b6007; /* WPR 0xad80 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ad8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ad98 set to 0x00000000 */
	claddr[index++] =	0x552b6807; /* WPR 0xada0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ada4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ada8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000adac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adb8 set to 0x00000000 */
	claddr[index++] =	0x552b7007; /* WPR 0xadc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000adcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000add0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000add4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000add8 set to 0x00000000 */
	claddr[index++] =	0x552b7807; /* WPR 0xade0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ade4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ade8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000adec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000adf8 set to 0x00000000 */
	claddr[index++] =	0x552b8007; /* WPR 0xae00 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ae0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae18 set to 0x00000000 */
	claddr[index++] =	0x552b8807; /* WPR 0xae20 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ae2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae38 set to 0x00000000 */
	claddr[index++] =	0x552b9007; /* WPR 0xae40 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ae4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae58 set to 0x00000000 */
	claddr[index++] =	0x552b9807; /* WPR 0xae60 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ae6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae78 set to 0x00000000 */
	claddr[index++] =	0x552ba007; /* WPR 0xae80 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ae8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ae98 set to 0x00000000 */
	claddr[index++] =	0x552ba807; /* WPR 0xaea0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aea4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aea8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aeac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aeb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aeb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aeb8 set to 0x00000000 */
	claddr[index++] =	0x552bb007; /* WPR 0xaec0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aec4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aec8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aecc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aed0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aed4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aed8 set to 0x00000000 */
	claddr[index++] =	0x552bb807; /* WPR 0xaee0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aee4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aee8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000aeec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aef0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aef4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aef8 set to 0x00000000 */
	claddr[index++] =	0x552bc007; /* WPR 0xaf00 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000af0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af18 set to 0x00000000 */
	claddr[index++] =	0x552bc807; /* WPR 0xaf20 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000af2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af38 set to 0x00000000 */
	claddr[index++] =	0x552bd007; /* WPR 0xaf40 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000af4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af58 set to 0x00000000 */
	claddr[index++] =	0x552bd807; /* WPR 0xaf60 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000af6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af78 set to 0x00000000 */
	claddr[index++] =	0x552be007; /* WPR 0xaf80 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000af8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000af98 set to 0x00000000 */
	claddr[index++] =	0x552be807; /* WPR 0xafa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000afac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afb8 set to 0x00000000 */
	claddr[index++] =	0x552bf007; /* WPR 0xafc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000afcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afd8 set to 0x00000000 */
	claddr[index++] =	0x552bf807; /* WPR 0xafe0 7 */
	claddr[index++] =	0x00000000; /*   WB0I11O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afe4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000afe8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000afec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aff0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aff4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000aff8 set to 0x00000000 */
	claddr[index++] =	0x552c0007; /* WPR 0xb000 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b004 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b008 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b00c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b010 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b014 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b018 set to 0x00000000 */
	claddr[index++] =	0x552c0807; /* WPR 0xb020 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b024 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b028 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b02c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b030 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b034 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b038 set to 0x00000000 */
	claddr[index++] =	0x552c1007; /* WPR 0xb040 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b044 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b048 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b04c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b050 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b054 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b058 set to 0x00000000 */
	claddr[index++] =	0x552c1807; /* WPR 0xb060 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b064 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b068 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b06c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b070 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b074 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b078 set to 0x00000000 */
	claddr[index++] =	0x552c2007; /* WPR 0xb080 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b084 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b088 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b08c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b090 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b094 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b098 set to 0x00000000 */
	claddr[index++] =	0x552c2807; /* WPR 0xb0a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b0ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0b8 set to 0x00000000 */
	claddr[index++] =	0x552c3007; /* WPR 0xb0c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b0cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0d8 set to 0x00000000 */
	claddr[index++] =	0x552c3807; /* WPR 0xb0e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b0ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b0f8 set to 0x00000000 */
	claddr[index++] =	0x552c4007; /* WPR 0xb100 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b104 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b108 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b10c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b110 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b114 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b118 set to 0x00000000 */
	claddr[index++] =	0x552c4807; /* WPR 0xb120 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b124 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b128 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b12c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b130 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b134 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b138 set to 0x00000000 */
	claddr[index++] =	0x552c5007; /* WPR 0xb140 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b144 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b148 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b14c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b150 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b154 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b158 set to 0x00000000 */
	claddr[index++] =	0x552c5807; /* WPR 0xb160 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b164 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b168 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b16c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b170 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b174 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b178 set to 0x00000000 */
	claddr[index++] =	0x552c6007; /* WPR 0xb180 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b184 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b188 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b18c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b190 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b194 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b198 set to 0x00000000 */
	claddr[index++] =	0x552c6807; /* WPR 0xb1a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b1ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1b8 set to 0x00000000 */
	claddr[index++] =	0x552c7007; /* WPR 0xb1c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b1cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1d8 set to 0x00000000 */
	claddr[index++] =	0x552c7807; /* WPR 0xb1e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b1ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b1f8 set to 0x00000000 */
	claddr[index++] =	0x552c8007; /* WPR 0xb200 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b204 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b208 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b20c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b210 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b214 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b218 set to 0x00000000 */
	claddr[index++] =	0x552c8807; /* WPR 0xb220 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b224 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b228 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b22c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b230 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b234 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b238 set to 0x00000000 */
	claddr[index++] =	0x552c9007; /* WPR 0xb240 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b244 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b248 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b24c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b250 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b254 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b258 set to 0x00000000 */
	claddr[index++] =	0x552c9807; /* WPR 0xb260 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b264 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b268 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b26c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b270 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b274 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b278 set to 0x00000000 */
	claddr[index++] =	0x552ca007; /* WPR 0xb280 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b284 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b288 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b28c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b290 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b294 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b298 set to 0x00000000 */
	claddr[index++] =	0x552ca807; /* WPR 0xb2a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b2ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2b8 set to 0x00000000 */
	claddr[index++] =	0x552cb007; /* WPR 0xb2c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b2cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2d8 set to 0x00000000 */
	claddr[index++] =	0x552cb807; /* WPR 0xb2e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b2ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b2f8 set to 0x00000000 */
	claddr[index++] =	0x552cc007; /* WPR 0xb300 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b304 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b308 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b30c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b310 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b314 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b318 set to 0x00000000 */
	claddr[index++] =	0x552cc807; /* WPR 0xb320 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b324 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b328 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b32c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b330 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b334 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b338 set to 0x00000000 */
	claddr[index++] =	0x552cd007; /* WPR 0xb340 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b344 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b348 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b34c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b350 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b354 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b358 set to 0x00000000 */
	claddr[index++] =	0x552cd807; /* WPR 0xb360 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b364 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b368 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b36c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b370 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b374 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b378 set to 0x00000000 */
	claddr[index++] =	0x552ce007; /* WPR 0xb380 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b384 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b388 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b38c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b390 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b394 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b398 set to 0x00000000 */
	claddr[index++] =	0x552ce807; /* WPR 0xb3a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b3ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3b8 set to 0x00000000 */
	claddr[index++] =	0x552cf007; /* WPR 0xb3c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b3cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3d8 set to 0x00000000 */
	claddr[index++] =	0x552cf807; /* WPR 0xb3e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I12O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b3ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b3f8 set to 0x00000000 */
	claddr[index++] =	0x552d0007; /* WPR 0xb400 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b404 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b408 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b40c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b410 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b414 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b418 set to 0x00000000 */
	claddr[index++] =	0x552d0807; /* WPR 0xb420 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b424 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b428 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b42c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b430 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b434 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b438 set to 0x00000000 */
	claddr[index++] =	0x552d1007; /* WPR 0xb440 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b444 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b448 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b44c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b450 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b454 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b458 set to 0x00000000 */
	claddr[index++] =	0x552d1807; /* WPR 0xb460 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b464 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b468 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b46c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b470 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b474 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b478 set to 0x00000000 */
	claddr[index++] =	0x552d2007; /* WPR 0xb480 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b484 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b488 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b48c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b490 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b494 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b498 set to 0x00000000 */
	claddr[index++] =	0x552d2807; /* WPR 0xb4a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b4ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4b8 set to 0x00000000 */
	claddr[index++] =	0x552d3007; /* WPR 0xb4c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b4cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4d8 set to 0x00000000 */
	claddr[index++] =	0x552d3807; /* WPR 0xb4e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b4ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b4f8 set to 0x00000000 */
	claddr[index++] =	0x552d4007; /* WPR 0xb500 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b504 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b508 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b50c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b510 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b514 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b518 set to 0x00000000 */
	claddr[index++] =	0x552d4807; /* WPR 0xb520 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b524 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b528 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b52c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b530 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b534 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b538 set to 0x00000000 */
	claddr[index++] =	0x552d5007; /* WPR 0xb540 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b544 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b548 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b54c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b550 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b554 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b558 set to 0x00000000 */
	claddr[index++] =	0x552d5807; /* WPR 0xb560 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b564 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b568 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b56c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b570 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b574 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b578 set to 0x00000000 */
	claddr[index++] =	0x552d6007; /* WPR 0xb580 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b584 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b588 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b58c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b590 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b594 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b598 set to 0x00000000 */
	claddr[index++] =	0x552d6807; /* WPR 0xb5a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b5ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5b8 set to 0x00000000 */
	claddr[index++] =	0x552d7007; /* WPR 0xb5c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b5cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5d8 set to 0x00000000 */
	claddr[index++] =	0x552d7807; /* WPR 0xb5e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b5ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b5f8 set to 0x00000000 */
	claddr[index++] =	0x552d8007; /* WPR 0xb600 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b604 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b608 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b60c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b610 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b614 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b618 set to 0x00000000 */
	claddr[index++] =	0x552d8807; /* WPR 0xb620 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b624 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b628 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b62c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b630 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b634 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b638 set to 0x00000000 */
	claddr[index++] =	0x552d9007; /* WPR 0xb640 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b644 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b648 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b64c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b650 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b654 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b658 set to 0x00000000 */
	claddr[index++] =	0x552d9807; /* WPR 0xb660 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b664 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b668 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b66c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b670 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b674 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b678 set to 0x00000000 */
	claddr[index++] =	0x552da007; /* WPR 0xb680 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b684 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b688 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b68c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b690 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b694 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b698 set to 0x00000000 */
	claddr[index++] =	0x552da807; /* WPR 0xb6a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b6ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6b8 set to 0x00000000 */
	claddr[index++] =	0x552db007; /* WPR 0xb6c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b6cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6d8 set to 0x00000000 */
	claddr[index++] =	0x552db807; /* WPR 0xb6e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b6ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b6f8 set to 0x00000000 */
	claddr[index++] =	0x552dc007; /* WPR 0xb700 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b704 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b708 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b70c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b710 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b714 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b718 set to 0x00000000 */
	claddr[index++] =	0x552dc807; /* WPR 0xb720 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b724 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b728 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b72c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b730 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b734 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b738 set to 0x00000000 */
	claddr[index++] =	0x552dd007; /* WPR 0xb740 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b744 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b748 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b74c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b750 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b754 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b758 set to 0x00000000 */
	claddr[index++] =	0x552dd807; /* WPR 0xb760 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b764 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b768 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b76c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b770 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b774 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b778 set to 0x00000000 */
	claddr[index++] =	0x552de007; /* WPR 0xb780 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b784 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b788 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b78c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b790 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b794 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b798 set to 0x00000000 */
	claddr[index++] =	0x552de807; /* WPR 0xb7a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b7ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7b8 set to 0x00000000 */
	claddr[index++] =	0x552df007; /* WPR 0xb7c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b7cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7d8 set to 0x00000000 */
	claddr[index++] =	0x552df807; /* WPR 0xb7e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I13O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b7ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b7f8 set to 0x00000000 */
	claddr[index++] =	0x552e0007; /* WPR 0xb800 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b804 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b808 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b80c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b810 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b814 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b818 set to 0x00000000 */
	claddr[index++] =	0x552e0807; /* WPR 0xb820 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b824 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b828 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b82c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b830 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b834 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b838 set to 0x00000000 */
	claddr[index++] =	0x552e1007; /* WPR 0xb840 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b844 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b848 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b84c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b850 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b854 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b858 set to 0x00000000 */
	claddr[index++] =	0x552e1807; /* WPR 0xb860 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b864 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b868 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b86c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b870 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b874 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b878 set to 0x00000000 */
	claddr[index++] =	0x552e2007; /* WPR 0xb880 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b884 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b888 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b88c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b890 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b894 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b898 set to 0x00000000 */
	claddr[index++] =	0x552e2807; /* WPR 0xb8a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b8ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8b8 set to 0x00000000 */
	claddr[index++] =	0x552e3007; /* WPR 0xb8c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b8cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8d8 set to 0x00000000 */
	claddr[index++] =	0x552e3807; /* WPR 0xb8e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b8ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b8f8 set to 0x00000000 */
	claddr[index++] =	0x552e4007; /* WPR 0xb900 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b904 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b908 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b90c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b910 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b914 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b918 set to 0x00000000 */
	claddr[index++] =	0x552e4807; /* WPR 0xb920 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b924 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b928 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b92c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b930 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b934 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b938 set to 0x00000000 */
	claddr[index++] =	0x552e5007; /* WPR 0xb940 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b944 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b948 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b94c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b950 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b954 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b958 set to 0x00000000 */
	claddr[index++] =	0x552e5807; /* WPR 0xb960 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b964 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b968 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b96c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b970 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b974 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b978 set to 0x00000000 */
	claddr[index++] =	0x552e6007; /* WPR 0xb980 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b984 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b988 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b98c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b990 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b994 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b998 set to 0x00000000 */
	claddr[index++] =	0x552e6807; /* WPR 0xb9a0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9a4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9a8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b9ac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9b0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9b4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9b8 set to 0x00000000 */
	claddr[index++] =	0x552e7007; /* WPR 0xb9c0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9c4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9c8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b9cc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9d0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9d4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9d8 set to 0x00000000 */
	claddr[index++] =	0x552e7807; /* WPR 0xb9e0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9e4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9e8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000b9ec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9f0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9f4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000b9f8 set to 0x00000000 */
	claddr[index++] =	0x552e8007; /* WPR 0xba00 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ba0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba18 set to 0x00000000 */
	claddr[index++] =	0x552e8807; /* WPR 0xba20 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ba2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba38 set to 0x00000000 */
	claddr[index++] =	0x552e9007; /* WPR 0xba40 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ba4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba58 set to 0x00000000 */
	claddr[index++] =	0x552e9807; /* WPR 0xba60 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ba6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba78 set to 0x00000000 */
	claddr[index++] =	0x552ea007; /* WPR 0xba80 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000ba8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000ba98 set to 0x00000000 */
	claddr[index++] =	0x552ea807; /* WPR 0xbaa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000baa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000baa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000baac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bab0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bab4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bab8 set to 0x00000000 */
	claddr[index++] =	0x552eb007; /* WPR 0xbac0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bac4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bac8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bacc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bad0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bad4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bad8 set to 0x00000000 */
	claddr[index++] =	0x552eb807; /* WPR 0xbae0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bae4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bae8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000baec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000baf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000baf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000baf8 set to 0x00000000 */
	claddr[index++] =	0x552ec007; /* WPR 0xbb00 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bb0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb18 set to 0x00000000 */
	claddr[index++] =	0x552ec807; /* WPR 0xbb20 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bb2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb38 set to 0x00000000 */
	claddr[index++] =	0x552ed007; /* WPR 0xbb40 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bb4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb58 set to 0x00000000 */
	claddr[index++] =	0x552ed807; /* WPR 0xbb60 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bb6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb78 set to 0x00000000 */
	claddr[index++] =	0x552ee007; /* WPR 0xbb80 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bb8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bb98 set to 0x00000000 */
	claddr[index++] =	0x552ee807; /* WPR 0xbba0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bba4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bba8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bbac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbb8 set to 0x00000000 */
	claddr[index++] =	0x552ef007; /* WPR 0xbbc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bbcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbd8 set to 0x00000000 */
	claddr[index++] =	0x552ef807; /* WPR 0xbbe0 7 */
	claddr[index++] =	0x00000000; /*   WB0I14O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbe4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbe8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bbec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bbf8 set to 0x00000000 */
	claddr[index++] =	0x552f0007; /* WPR 0xbc00 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O00_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bc0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc18 set to 0x00000000 */
	claddr[index++] =	0x552f0807; /* WPR 0xbc20 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O01_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bc2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc38 set to 0x00000000 */
	claddr[index++] =	0x552f1007; /* WPR 0xbc40 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O02_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bc4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc58 set to 0x00000000 */
	claddr[index++] =	0x552f1807; /* WPR 0xbc60 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O03_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bc6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc78 set to 0x00000000 */
	claddr[index++] =	0x552f2007; /* WPR 0xbc80 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O04_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bc8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bc98 set to 0x00000000 */
	claddr[index++] =	0x552f2807; /* WPR 0xbca0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O05_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bca4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bca8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bcac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcb8 set to 0x00000000 */
	claddr[index++] =	0x552f3007; /* WPR 0xbcc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O06_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bccc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcd8 set to 0x00000000 */
	claddr[index++] =	0x552f3807; /* WPR 0xbce0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O07_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bce4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bce8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bcec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bcf8 set to 0x00000000 */
	claddr[index++] =	0x552f4007; /* WPR 0xbd00 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O08_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bd0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd18 set to 0x00000000 */
	claddr[index++] =	0x552f4807; /* WPR 0xbd20 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O09_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bd2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd38 set to 0x00000000 */
	claddr[index++] =	0x552f5007; /* WPR 0xbd40 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O10_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bd4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd58 set to 0x00000000 */
	claddr[index++] =	0x552f5807; /* WPR 0xbd60 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O11_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bd6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd78 set to 0x00000000 */
	claddr[index++] =	0x552f6007; /* WPR 0xbd80 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O12_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bd8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bd98 set to 0x00000000 */
	claddr[index++] =	0x552f6807; /* WPR 0xbda0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O13_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bda4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bda8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bdac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdb8 set to 0x00000000 */
	claddr[index++] =	0x552f7007; /* WPR 0xbdc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O14_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bdcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdd8 set to 0x00000000 */
	claddr[index++] =	0x552f7807; /* WPR 0xbde0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O15_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bde4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bde8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bdec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdf0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdf4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bdf8 set to 0x00000000 */
	claddr[index++] =	0x552f8007; /* WPR 0xbe00 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O16_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000be0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be18 set to 0x00000000 */
	claddr[index++] =	0x552f8807; /* WPR 0xbe20 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O17_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000be2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be38 set to 0x00000000 */
	claddr[index++] =	0x552f9007; /* WPR 0xbe40 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O18_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000be4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be58 set to 0x00000000 */
	claddr[index++] =	0x552f9807; /* WPR 0xbe60 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O19_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000be6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be78 set to 0x00000000 */
	claddr[index++] =	0x552fa007; /* WPR 0xbe80 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O20_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000be8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000be98 set to 0x00000000 */
	claddr[index++] =	0x552fa807; /* WPR 0xbea0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O21_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bea4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bea8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000beac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000beb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000beb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000beb8 set to 0x00000000 */
	claddr[index++] =	0x552fb007; /* WPR 0xbec0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O22_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bec4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bec8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000becc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bed0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bed4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bed8 set to 0x00000000 */
	claddr[index++] =	0x552fb807; /* WPR 0xbee0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O23_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bee4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bee8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000beec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bef0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bef4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bef8 set to 0x00000000 */
	claddr[index++] =	0x552fc007; /* WPR 0xbf00 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O24_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf04 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf08 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bf0c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf10 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf14 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf18 set to 0x00000000 */
	claddr[index++] =	0x552fc807; /* WPR 0xbf20 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O25_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf24 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf28 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bf2c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf30 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf34 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf38 set to 0x00000000 */
	claddr[index++] =	0x552fd007; /* WPR 0xbf40 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O26_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf44 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf48 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bf4c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf50 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf54 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf58 set to 0x00000000 */
	claddr[index++] =	0x552fd807; /* WPR 0xbf60 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O27_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf64 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf68 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bf6c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf70 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf74 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf78 set to 0x00000000 */
	claddr[index++] =	0x552fe007; /* WPR 0xbf80 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O28_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf84 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf88 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bf8c set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf90 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf94 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bf98 set to 0x00000000 */
	claddr[index++] =	0x552fe807; /* WPR 0xbfa0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O29_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfa4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfa8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bfac set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfb0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfb4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfb8 set to 0x00000000 */
	claddr[index++] =	0x552ff007; /* WPR 0xbfc0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O30_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfc4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfc8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bfcc set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfd0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfd4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfd8 set to 0x00000000 */
	claddr[index++] =	0x552ff807; /* WPR 0xbfe0 7 */
	claddr[index++] =	0x00000000; /*   WB0I15O31_00_03  set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfe4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bfe8 set to 0x00000000 */
	claddr[index++] =	0x00000001; /*   REG 0x0000bfec set to 0x00000001 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bff0 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bff4 set to 0x00000000 */
	claddr[index++] =	0x00000000; /*   REG 0x0000bff8 set to 0x00000000 */
	claddr[index++] =	0x55004401; /* WPR 0x110 1 */
	claddr[index++] =	0x00000001; /*   SLSP set to 0x00000001 */
	claddr[index++] =	0x8b000000; /* SYNCS 0x0 */

	claddr[index++] =	 0x4a000000; /* SCCHK */

	if (true == msco_int_flg)
	{
		claddr[index++] =	0x81000000; /* INT */
	}

	claddr[index++] =	 0x4a000000; /* SCCHK */
    /* claddr[index++] =	 0x81000000 ;INT */
	/* claddr[index++] =	 0x49000003 ;;SCRES */

	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */
	claddr[index++] =	0x8000000f; /* NOP */

	claddr[index++] =	0x8800002a; /* TRAP 0x2a */

	return 0;
}

static R_ATMLIB_RETURN_VALUE setdata_psc(
    R_ATMLIB_CLData *cldata,
    uint32_t imga_addr,
    uint32_t dst0_addr,
    uint32_t dst1_addr,
    uint32_t dst2_addr,
    uint32_t dst3_addr)
{
    R_ATMLIB_RETURN_VALUE    atomic_ret = R_ATMLIB_E_OK;

	R_ATMLIB_PSCSrcParam src_param;
    R_ATMLIB_PSCDstParam dst0param;
    R_ATMLIB_PSCDstParam dst1param;
    R_ATMLIB_PSCDstParam dst2param;
    R_ATMLIB_PSCDstParam dst3param;

    /* Initialize structure to 0 */
    memset(&src_param, 0, sizeof(src_param));
    memset(&dst0param, 0, sizeof(dst0param));
    memset(&dst1param, 0, sizeof(dst1param));
    memset(&dst2param, 0, sizeof(dst2param));
    memset(&dst3param, 0, sizeof(dst3param));

    /* === Set data === */

    src_param.type      = IMAGE_TYPE;
    src_param.addr      = imga_addr;
    src_param.stride    = IMAGE_WIDTH;
    src_param.leng      = (IMAGE_HEIGHT << 16) + IMAGE_WIDTH;

    dst0param.type      = IMAGE_TYPE;
    dst0param.addr      = dst0_addr;
    dst0param.stride    = DST0_WIDTH;
    dst0param.leng      = (DST0_HEIGHT << 16) + DST0_WIDTH;
    dst0param.xmag      = (IMAGE_WIDTH << 20) / DST0_WIDTH;
    dst0param.ymag      = (IMAGE_HEIGHT << 20) / DST0_HEIGHT;
    dst0param.ofs_x     = 0;
    dst0param.ofs_y     = 0;
    dst0param.blend_mode= R_ATMLIB_PSC_BLENDMODE_BL;
    dst0param.round_mode= R_ATMLIB_ROUNDMODE_DOWN;

    dst1param.type      = IMAGE_TYPE;
    dst1param.addr      = dst1_addr;
    dst1param.stride    = DST1_WIDTH;
    dst1param.leng      = (DST1_HEIGHT << 16) + DST1_WIDTH;
    dst1param.xmag      = (IMAGE_WIDTH << 20) / DST1_WIDTH;
    dst1param.ymag      = (IMAGE_HEIGHT << 20) / DST1_HEIGHT;
    dst1param.ofs_x     = 0;
    dst1param.ofs_y     = 0;
    dst1param.blend_mode= R_ATMLIB_PSC_BLENDMODE_BL;
    dst1param.round_mode= R_ATMLIB_ROUNDMODE_DOWN;

    dst2param.type      = IMAGE_TYPE;
    dst2param.addr      = dst2_addr;
    dst2param.stride    = DST2_WIDTH;
    dst2param.leng      = (DST2_HEIGHT << 16) + DST2_WIDTH;
    dst2param.xmag      = (IMAGE_WIDTH << 20) / DST2_WIDTH;
    dst2param.ymag      = (IMAGE_HEIGHT << 20) / DST2_HEIGHT;
    dst2param.ofs_x     = 0;
    dst2param.ofs_y     = 0;
    dst2param.blend_mode= R_ATMLIB_PSC_BLENDMODE_BL;
    dst2param.round_mode= R_ATMLIB_ROUNDMODE_DOWN;

    dst3param.type      = IMAGE_TYPE;
    dst3param.addr      = dst3_addr;
    dst3param.stride    = DST3_WIDTH;
    dst3param.leng      = (DST3_HEIGHT << 16) + DST3_WIDTH;
    dst3param.xmag      = (IMAGE_WIDTH << 20) / DST3_WIDTH;
    dst3param.ymag      = (IMAGE_HEIGHT << 20) / DST3_HEIGHT;
    dst3param.ofs_x     = 0;
    dst3param.ofs_y     = 0;
    dst3param.blend_mode= R_ATMLIB_PSC_BLENDMODE_BL;
    dst3param.round_mode= R_ATMLIB_ROUNDMODE_DOWN;

    atomic_ret = r_atmlib_SetDataPSCCL(
            cldata,
            R_ATMLIB_PSC_BASIC,
            &src_param,
            &dst0param,
            &dst1param,
            &dst2param,
            &dst3param
        );

    return atomic_ret;
}

int8_t createCL_ENDtest(END_cl_data_t *p_data)
{
	/* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret = R_ATMLIB_E_OK;
	R_ATMLIB_CLData          cldata;
	static const uint8_t m_ImgA[(IMAGE_HEIGHT * IMAGE_WIDTH)] =
	{
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
		0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
	};

	memset(&cldata, 0, sizeof(cldata));
	/* set imga data */
	{
		p_data->imga_cpu_addr = MemAlloc(osal_mmngr, ALIGN64(IMAGE_SIZE), END_ALIGNMENT, IMP_MALLOC_HW);
		if (NULL == p_data->imga_cpu_addr)
		{
			LOGPRINT(("%s: Cannot allocate imga\n", __func__));
			return -1;
		}

		memcpy(p_data->imga_cpu_addr, (void *)m_ImgA, sizeof(m_ImgA));
	}

	/* set dst0 data */
	{
		p_data->dst0_cpu_addr = MemAlloc(osal_mmngr, ALIGN64(DST0_SIZE), END_ALIGNMENT, IMP_MALLOC_HW);
		if (NULL == p_data->dst0_cpu_addr)
		{
			LOGPRINT(("%s: Cannot allocate dst0\n", __func__));
			return -1;
		}

		memset(p_data->dst0_cpu_addr, 0, util_mem_get_size(p_data->dst0_cpu_addr));
	}

	/* set dst1 data */
	{
		p_data->dst1_cpu_addr = MemAlloc(osal_mmngr, ALIGN64(DST1_SIZE), END_ALIGNMENT, IMP_MALLOC_HW);
		if (NULL == p_data->dst1_cpu_addr)
		{
			LOGPRINT(("%s: Cannot allocate dst1\n", __func__));
			return -1;
		}

		memset(p_data->dst1_cpu_addr, 0, util_mem_get_size(p_data->dst1_cpu_addr));
	}

	/* set dst2 data */
	{
		p_data->dst2_cpu_addr = MemAlloc(osal_mmngr, ALIGN64(DST2_SIZE), END_ALIGNMENT, IMP_MALLOC_HW);
		if (NULL == p_data->dst2_cpu_addr)
		{
			LOGPRINT(("%s: Cannot allocate dst2\n", __func__));
			return -1;
		}

		memset(p_data->dst2_cpu_addr, 0, util_mem_get_size(p_data->dst2_cpu_addr));
	}

	/* set dst3 data */
	{
		p_data->dst3_cpu_addr = MemAlloc(osal_mmngr, ALIGN64(DST3_SIZE), END_ALIGNMENT, IMP_MALLOC_HW);
		if (NULL == p_data->dst3_cpu_addr)
		{
			LOGPRINT(("%s: Cannot allocate dst3\n", __func__));
			return -1;
		}

		memset(p_data->dst3_cpu_addr, 0, util_mem_get_size(p_data->dst3_cpu_addr));
	}

	/* set cl data */
	{
		p_data->cl_cpu_addr = MemAlloc(osal_mmngr, ALIGN64(END_CL_SIZE), END_ALIGNMENT, IMP_MALLOC_HW);
		if (NULL == p_data->cl_cpu_addr)
		{
			LOGPRINT(("%s: Cannot allocate CL\n", __func__));
			return -1;
		}

		memset(p_data->cl_cpu_addr, 0, util_mem_get_size(p_data->cl_cpu_addr));
	}

	/*----- Atomic Initialization -----*/
	atomic_ret = r_atmlib_InitializePSCCL(&cldata,
		(uint32_t *)p_data->cl_cpu_addr, /* cast to uint32_t * */
		util_mem_get_size(p_data->cl_cpu_addr) / sizeof(uint32_t));
	if (R_ATMLIB_E_OK != atomic_ret)
	{
		LOGPRINT(("<< [ERROR] r_atmlib_InitializePSCCL  : code %d>>\n", (int32_t)atomic_ret)); /* cast to int32_t */
	}

	/*----- Atomic SetData -----*/
	if (R_ATMLIB_E_OK == atomic_ret)
	{
		atomic_ret = setdata_psc(&cldata,
				(uint32_t)util_mem_get_paddr(p_data->imga_cpu_addr),
				(uint32_t)util_mem_get_paddr(p_data->dst0_cpu_addr),
				(uint32_t)util_mem_get_paddr(p_data->dst1_cpu_addr),
				(uint32_t)util_mem_get_paddr(p_data->dst2_cpu_addr),
				(uint32_t)util_mem_get_paddr(p_data->dst3_cpu_addr));
		if (R_ATMLIB_E_OK != atomic_ret)
		{
			LOGPRINT(("<< [ERROR] r_atmlib_SetDataPSCCL  : code %d>>\n", (int32_t)atomic_ret)); /* cast to int32_t */
		}
	}

	/*----- Atomic Finalization -----*/
	if (R_ATMLIB_E_OK == atomic_ret)
	{
		atomic_ret = r_atmlib_FinalizePSCCL(&cldata);
		if (R_ATMLIB_E_OK != atomic_ret)
		{
			LOGPRINT(("<< [ERROR] r_atmlib_FinalizePSCCL  : code %d>>\n", (int32_t)atomic_ret)); /* cast to int32_t */
		}
	}

	return (R_ATMLIB_E_OK == atomic_ret) ? (0) : (-1);
}

static R_ATMLIB_RETURN_VALUE set_cl_wupcovf_imp(void *const cpu_addr, uint32_t sync_id)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;
    R_ATMLIB_CLData          cldata;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));

    atomic_ret = r_atmlib_InitializeIMPCL(&cldata,
        (uint32_t *)cpu_addr, /* cast to uint32_t * */
        util_mem_get_size(cpu_addr) / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_InitializeIMPCL  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /* === Set sync CL === */
	atomic_ret = r_atmlib_IMP_NOP(&cldata, 4);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_SLP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_SLP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_NOP(&cldata, 200);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /*----- Atomic Finalization -----*/
    atomic_ret = r_atmlib_FinalizeIMPCL(&cldata);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n",
            atomic_ret)); /* cast to int32_t */
        goto end;
    }

end:
	return atomic_ret;
}

static R_ATMLIB_RETURN_VALUE set_cl_wupcovf_ocv(void *const cpu_addr, uint32_t sync_id)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;
    R_ATMLIB_CLData          cldata;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));

    atomic_ret = r_atmlib_InitializeOCVCL(&cldata,
        (uint32_t *)cpu_addr, /* cast to uint32_t * */
        util_mem_get_size(cpu_addr) / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_InitializeOCVCL  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /* === Set sync CL === */
	atomic_ret = r_atmlib_OCV_NOP(&cldata, 4);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_OCV_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_OCV_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_OCV_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_OCV_SLP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_OCV_SLP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_OCV_NOP(&cldata, 16);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_OCV_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /*----- Atomic Finalization -----*/
    atomic_ret = r_atmlib_FinalizeOCVCL(&cldata);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n",
            atomic_ret)); /* cast to int32_t */
        goto end;
    }

end:
	return atomic_ret;
}

static R_ATMLIB_RETURN_VALUE set_cl_wupcovf_dmac(void *const cpu_addr, uint32_t sync_id)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;
    R_ATMLIB_CLData          cldata;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));

    atomic_ret = r_atmlib_InitializeDMACL(&cldata,
        (uint32_t *)cpu_addr, /* cast to uint32_t * */
        util_mem_get_size(cpu_addr) / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_InitializeDMACL  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /* === Set sync CL === */
	atomic_ret = r_atmlib_DMA_NOP(&cldata, 4);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_DMA_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_DMA_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_DMA_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_DMA_SLP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_DMA_SLP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_DMA_NOP(&cldata, 16);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_DMA_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /*----- Atomic Finalization -----*/
    atomic_ret = r_atmlib_FinalizeDMACL(&cldata);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n",
            atomic_ret)); /* cast to int32_t */
        goto end;
    }

end:
	return atomic_ret;
}

static R_ATMLIB_RETURN_VALUE set_cl_wupcovf_psc(void *const cpu_addr, uint32_t sync_id)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;
    R_ATMLIB_CLData          cldata;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));

    atomic_ret = r_atmlib_InitializePSCCL(&cldata,
        (uint32_t *)cpu_addr, /* cast to uint32_t * */
        util_mem_get_size(cpu_addr) / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_InitializePSCCL  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /* === Set sync CL === */
	atomic_ret = r_atmlib_PSC_NOP(&cldata, 4);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_PSC_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_PSC_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_PSC_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_PSC_SLP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_PSC_SLP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_PSC_NOP(&cldata, 16);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_PSC_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /*----- Atomic Finalization -----*/
    atomic_ret = r_atmlib_FinalizePSCCL(&cldata);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n",
            atomic_ret)); /* cast to int32_t */
        goto end;
    }

end:
	return atomic_ret;
}

#define FCT_CNN_NOP_1		0x80000000U

static R_ATMLIB_RETURN_VALUE set_cl_wupcovf_cnn(void *const cpu_addr, uint32_t sync_id)
{
	uint32_t *claddr = (uint32_t *)cpu_addr;
	uint32_t cl_size = util_mem_get_size(cpu_addr) / sizeof(uint32_t);
	uint32_t cnt;

	/* AtomicLib is not suppoted CNN(use CNN tool) */

	for (cnt = 0; cnt < cl_size; cnt++)
	{
		claddr[cnt] = FCT_CNN_NOP_1 | 0x3;
	}

	claddr[4]	= FCT_CNN_WUP | sync_id;
	claddr[12]	= FCT_CNN_SLP | sync_id;
	claddr[212]	= FCT_CNN_TRAP | sync_id;

	return R_ATMLIB_E_OK;
}

int32_t createCL_WUPCOVFtest(void *const cpu_addr, const e_impdrv_core_type_t core_type, uint32_t sync_id)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;

	switch (core_type)
	{
		case IMPDRV_CORE_TYPE_IMP:
			atomic_ret = set_cl_wupcovf_imp(cpu_addr, sync_id);
			break;

		case IMPDRV_CORE_TYPE_OCV:
			atomic_ret = set_cl_wupcovf_ocv(cpu_addr, sync_id);
			break;

		case IMPDRV_CORE_TYPE_DMAC:
			atomic_ret = set_cl_wupcovf_dmac(cpu_addr, sync_id);
			break;

		case IMPDRV_CORE_TYPE_PSCEXE:
			atomic_ret = set_cl_wupcovf_psc(cpu_addr, sync_id);
			break;

		case IMPDRV_CORE_TYPE_CNN:
			atomic_ret = set_cl_wupcovf_cnn(cpu_addr, sync_id);
			break;

		default:
			atomic_ret = R_ATMLIB_E_NG;
			break;
	}

	return (atomic_ret == R_ATMLIB_E_OK) ? (0) : (-1);
}

int32_t createCLpair_WUPCOVFtest(void *const cpu_addr, uint32_t sync_id)
{
    /* IMP Atomic */
    R_ATMLIB_RETURN_VALUE    atomic_ret;
    R_ATMLIB_CLData          cldata;

    /* === Initialize memories === */
    memset(&cldata, 0, sizeof(R_ATMLIB_CLData));

    atomic_ret = r_atmlib_InitializeIMPCL(&cldata,
        (uint32_t *)cpu_addr, /* cast to uint32_t * */
        util_mem_get_size(cpu_addr) / sizeof(uint32_t));
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_InitializeIMPCL  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /* === Set sync CL === */
	atomic_ret = r_atmlib_IMP_NOP(&cldata, 4);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_NOP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_SLP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_SLP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

	atomic_ret = r_atmlib_IMP_WUP(&cldata, sync_id);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_IMP_WUP  : code %d>>\n",
            (int32_t)atomic_ret)); /* cast to int32_t */
        goto end;
    }

    /*----- Atomic Finalization -----*/
    atomic_ret = r_atmlib_FinalizeIMPCL(&cldata);
    if (R_ATMLIB_E_OK != atomic_ret)
    {
        LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n",
            atomic_ret)); /* cast to int32_t */
        goto end;
    }

end:
	return (atomic_ret == R_ATMLIB_E_OK) ? (0) : (-1);
}

static uint32_t imp_createCL_nop(e_impdrv_core_type_t core_type, uint32_t *data, uint32_t clsize)
{
	R_ATMLIB_RETURN_VALUE    atomic_ret;
	R_ATMLIB_CLData          cldata;
	uint32_t                 ret = 0U;
	uint32_t                 loop;
	uint32_t                 nop_count;

	nop_count = clsize - 8U;

	/* set cldata for small CL */
	switch(core_type)
	{
		case IMPDRV_CORE_TYPE_IMP:       /* fall through */
		case IMPDRV_CORE_TYPE_IMP_SLIM:
		{
			atomic_ret = r_atmlib_InitializeIMPCL(&cldata, data, clsize);
			if (R_ATMLIB_E_OK != atomic_ret)
			{
				ret = 1U;
				LOGPRINT(("<< [ERROR] r_atmlib_InitializeIMPCL  : code %d>>\n",
					(int32_t)atomic_ret)); /* cast to int32_t */
			}
			else
			{
				loop = 0U;
				while (nop_count > loop)
				{
					loop++;
					atomic_ret = r_atmlib_IMP_NOP( &cldata, 1U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_IMP_NOP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
						break;
					}
				}
				if (0U == ret)
				{
					atomic_ret = r_atmlib_IMP_TRAP( &cldata );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_IMP_TRAP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
					}
					else
					{
						atomic_ret = r_atmlib_FinalizeIMPCL(&cldata);
						if (R_ATMLIB_E_OK != atomic_ret)
						{
							ret = 1U;
							LOGPRINT(("<< [ERROR] r_atmlib_FinalizeIMPCL  : code %d>>\n",
								(int32_t)atomic_ret)); /* cast to int32_t */
						}
					}
				}
			}
			break;
		}
		case IMPDRV_CORE_TYPE_OCV:
		{
			atomic_ret = r_atmlib_InitializeOCVCL(&cldata, data, clsize);
			if (R_ATMLIB_E_OK != atomic_ret)
			{
				ret = 1U;
				LOGPRINT(("<< [ERROR] r_atmlib_InitializeOCVCL  : code %d>>\n",
					(int32_t)atomic_ret)); /* cast to int32_t */
			}
			else
			{
				loop = 0U;
				while (nop_count > loop)
				{
					loop++;
					atomic_ret = r_atmlib_OCV_NOP( &cldata, 1U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_OCV_NOP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
						break;
					}
				}
				if (0U == ret)
				{
					atomic_ret = r_atmlib_OCV_TRAP( &cldata, 0U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_OCV_TRAP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
					}
					else
					{
						atomic_ret = r_atmlib_FinalizeOCVCL(&cldata);
						if (R_ATMLIB_E_OK != atomic_ret)
						{
							ret = 1U;
							LOGPRINT(("<< [ERROR] r_atmlib_FinalizeOCVCL  : code %d>>\n",
								(int32_t)atomic_ret)); /* cast to int32_t */
						}
					}
				}
			}
			break;
		}
		case IMPDRV_CORE_TYPE_DMAC:      /* fall through */
		case IMPDRV_CORE_TYPE_DMAC_SLIM:
		{
			atomic_ret = r_atmlib_InitializeDMACL(&cldata, data, clsize);
			if (R_ATMLIB_E_OK != atomic_ret)
			{
				ret = 1U;
				LOGPRINT(("<< [ERROR] r_atmlib_InitializeDMACL  : code %d>>\n",
					(int32_t)atomic_ret)); /* cast to int32_t */
			}
			else
			{
				loop = 0U;
				while (nop_count > loop)
				{
					loop++;
					atomic_ret = r_atmlib_DMA_NOP( &cldata, 1U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_DMA_NOP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
						break;
					}
				}
				if (0U == ret)
				{
					atomic_ret = r_atmlib_DMA_TRAP( &cldata, 0U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_DMA_TRAP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
					}
					else
					{
						atomic_ret = r_atmlib_FinalizeDMACL(&cldata);
						if (R_ATMLIB_E_OK != atomic_ret)
						{
							ret = 1U;
							LOGPRINT(("<< [ERROR] r_atmlib_FinalizeDMACL  : code %d>>\n",
								(int32_t)atomic_ret)); /* cast to int32_t */
						}
					}
				}
			}
			break;
		}
		case IMPDRV_CORE_TYPE_PSCEXE:
		{
			atomic_ret = r_atmlib_InitializePSCCL(&cldata, data, clsize);
			if (R_ATMLIB_E_OK != atomic_ret)
			{
				ret = 1U;
				LOGPRINT(("<< [ERROR] r_atmlib_InitializePSCCL  : code %d>>\n",
					(int32_t)atomic_ret)); /* cast to int32_t */
			}
			else
			{
				loop = 0U;
				while (nop_count > loop)
				{
					loop++;
					atomic_ret = r_atmlib_PSC_NOP( &cldata, 1U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_PSC_NOP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
						break;
					}
				}
				if (0U == ret)
				{
					atomic_ret = r_atmlib_PSC_TRAP( &cldata, 0U );
					if (R_ATMLIB_E_OK != atomic_ret)
					{
						ret = 1U;
						LOGPRINT(("<< [ERROR] r_atmlib_PSC_NOP  : code %d>>\n",
							(int32_t)atomic_ret)); /* cast to int32_t */
					}
					else
					{
						atomic_ret = r_atmlib_FinalizePSCCL(&cldata);
						if (R_ATMLIB_E_OK != atomic_ret)
						{
							ret = 1U;
							LOGPRINT(("<< [ERROR] r_atmlib_FinalizePSCCL  : code %d>>\n",
								(int32_t)atomic_ret)); /* cast to int32_t */
						}
					}
				}
			}
			break;
		}
		case IMPDRV_CORE_TYPE_CNN:
		{
			loop = 0U;
			while (nop_count > loop)
			{
				data[loop] = FCT_CNN_NOP; /* NOP */
				loop++;
			}
			data[loop] = FCT_CNN_TRAP; /* TRAP */
			ret = 0U;
			break;
		}
		default:
		{
			ret = 1U;
			break;
		}
	}

	return ret;
}

void irq_group_createCLdata(st_buffer_t *buffer)
{
    uint32_t core_num;
    uint32_t ret;
    e_impdrv_core_type_t core_type;
	st_imp_buffer_t *cldata = NULL;

    ret    = 0;

	buffer->cpu_addr = (st_imp_buffer_t *)MemAlloc(osal_mmngr, sizeof(st_imp_buffer_t), 128u, IMP_MALLOC_HW);

    if (NULL != buffer->cpu_addr)
    {
		cldata = (st_imp_buffer_t *)buffer->cpu_addr;
#if IMP_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_IMP;
            core_num = 0U;
            while (IMP_CORE_MAX > core_num)
            {
				ret = imp_createCL_nop(core_type, cldata->imp[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for IMP>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
#if IMP_SLIM_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_IMP_SLIM;
            core_num = 0U;
            while (IMP_SLIM_CORE_MAX > core_num)
            {
				ret = imp_createCL_nop(core_type, cldata->imp_slim[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for IMP>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
#if OCV_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_OCV;
            core_num = 0U;
            while (OCV_CORE_MAX > core_num)
            {

				ret = imp_createCL_nop(core_type, cldata->ocv[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for OCV>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
#if DMA_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_DMAC;
            core_num = 0U;
            while (DMA_CORE_MAX > core_num)
            {
				ret = imp_createCL_nop(core_type, cldata->dmac[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for DMA>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
#if DMA_SLIM_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_DMAC_SLIM;
            core_num = 0U;
            while (DMA_SLIM_CORE_MAX > core_num)
            {
				ret = imp_createCL_nop(core_type, cldata->dmac_slim[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for DMA>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
#if PSCEXE_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_PSCEXE;
            core_num = 0U;
            while (PSCEXE_CORE_MAX > core_num)
            {
				ret = imp_createCL_nop(core_type, cldata->pscexe[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for PSCEXE>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
#if CNN_CORE_MAX > 0
        if (0 == ret)
        {
            core_type = IMPDRV_CORE_TYPE_CNN;
            core_num = 0U;
            while (CNN_CORE_MAX > core_num)
            {
				ret = imp_createCL_nop(core_type, cldata->cnn[core_num].cl_nop, NOPCLSIZE);
                if (0 != ret)
                {
                    LOGPRINT(("<< [ERROR] error create NOP for CNN>>\n"));
                    break;
                }
                core_num++;
            }
        }
#endif
    }

	if (0 != ret)
	{
		MemFree(osal_mmngr, buffer->cpu_addr);
		buffer->cpu_addr = NULL;
	}
	else
	{
		/* update address */
		buffer->buffersize	= util_mem_get_size(buffer->cpu_addr);
		buffer->hw_addr		= util_mem_get_paddr(buffer->cpu_addr);
	}
}

int32_t CLgethw_addr(const st_buffer_t *const cl_buffer, const st_impdrv_core_info_t *core, uint32_t *p_claddr_phys, uint32_t *p_clsize)
{
    uint32_t    claddr_phys;
    uint32_t    clsize;
    uint32_t    *target_buffer;
    int32_t     ret;
    uintptr_t   buffer_top;
    uintptr_t   buffer_end;
    uintptr_t   target_addr;
    uint32_t    core_num;
    st_imp_buffer_t      *cl_data;

    target_buffer = NULL;
    core_num      = core->core_num;
    cl_data       = (st_imp_buffer_t *)cl_buffer->cpu_addr;

	switch (core->core_type)
	{
#if IMP_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_IMP:
			target_buffer = &cl_data->imp[core_num].cl_nop[0];
			clsize        = sizeof(cl_data->imp[core_num].cl_nop);
			break;
#endif
#if IMP_SLIM_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_IMP_SLIM:
			target_buffer = &cl_data->imp_slim[core_num].cl_nop[0];
			clsize        = sizeof(cl_data->imp_slim[core_num].cl_nop);
			break;
#endif
#if OCV_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_OCV:
			target_buffer = &cl_data->ocv[core_num].cl_nop[0U];
			clsize        = sizeof(cl_data->ocv[core_num].cl_nop);
			break;
#endif
#if DMA_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_DMAC:
			target_buffer = &cl_data->dmac[core_num].cl_nop[0U];
			clsize        = sizeof(cl_data->dmac[core_num].cl_nop);
			break;
#endif
#if DMA_SLIM_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_DMAC_SLIM:
			target_buffer = &cl_data->dmac_slim[core_num].cl_nop[0U];
			clsize        = sizeof(cl_data->dmac_slim[core_num].cl_nop);
			break;
#endif
#if PSCEXE_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_PSCEXE:
			target_buffer = &cl_data->pscexe[core_num].cl_nop[0U];
			clsize        = sizeof(cl_data->pscexe[core_num].cl_nop);
			break;
#endif
#if CNN_CORE_MAX > 0
		case IMPDRV_CORE_TYPE_CNN:
			target_buffer = &cl_data->cnn[core_num].cl_nop[0U];
			clsize      = sizeof(cl_data->cnn[core_num].cl_nop);
			break;
#endif
		default:
			LOGPRINT(("<< [ERROR] invalid core.core_type>>\n"));
			break;
	}

    ret = 0;
    if (NULL != target_buffer)
    {
        target_addr = (uintptr_t)target_buffer;
        buffer_top  = (uintptr_t)cl_buffer->cpu_addr;
        buffer_end  = buffer_top + cl_buffer->buffersize;
        if ((buffer_top > target_addr) || (target_addr >= buffer_end))
        {
            LOGPRINT(("<< [ERROR] can not calculate hw_addr>>\n"));
            ret = 1;
        }
        else
        {
            /* offset */
            claddr_phys = (uint32_t)(target_addr - buffer_top);
            claddr_phys += (uint32_t)cl_buffer->hw_addr;

            *p_claddr_phys = claddr_phys;
            *p_clsize      = clsize;
        }
    }
    else
    {
        ret = -1;
    }

    return ret;
}

int8_t set_cl_data_for_wupcovf_imp(const st_impdrv_core_info_t *const core_info, uint32_t *claddr)
{
	uint32_t cldata_write;
	int8_t ret = 0;
	int8_t i;
	
	if ((IMPDRV_CORE_TYPE_IMP == core_info->core_type) && (0U == core_info->core_num))
	{
		cldata_write = 0U;

		/* to sync core: at first execute CL sets: SLP-WUP */
		*(claddr + cldata_write) = FCT_IMP_SLP | (0x0002); //wait wakeup from coremap 1
		cldata_write++;
		*(claddr + cldata_write) = FCT_IMP_WUP | (0x0002); //wakeup coremap 1
		cldata_write++;

		for (i = 0; i < WUPCOVF_IMP_SLP_CNT; i++)
		{
			*(claddr + cldata_write) = FCT_IMP_NOP_2;
			cldata_write++;
		}

		/* after this place. two CL start almost same time */
		/* put this core to infinit halts wait WUP from coremap2 */
		*(claddr + cldata_write) = FCT_IMP_SLP | (0x0004); //wait wakeup from coremap 2
		cldata_write++;
		*(claddr + cldata_write) = FCT_IMP_TRAP;
		cldata_write++;
	}
	else if ((IMPDRV_CORE_TYPE_IMP == core_info->core_type) && (1U == core_info->core_num))
	{
		cldata_write = 0U;

		/* to sync core: at second execute CL sets: WUP-SLP */
		*(claddr + cldata_write) = FCT_IMP_WUP | (0x0001); //wakeup coremap 0
		cldata_write++;
		for (i = 0; i < WUPCOVF_IMP_SLP_CNT; i++)
		{
			*(claddr + cldata_write) = FCT_IMP_NOP_2;
			cldata_write++;
		}

		*(claddr + cldata_write) = FCT_IMP_SLP | (0x0001); //wait wakeup from coremap 0
		cldata_write++;
		for (i = 0; i < WUPCOVF_IMP_SLP_CNT; i++)
		{
			*(claddr + cldata_write) = FCT_IMP_NOP_2;
			cldata_write++;
		}

		/* after this place. two CL start almost same time */
		*(claddr + cldata_write) = FCT_IMP_WUP | (0x0001); //wakeup coremap 0
		cldata_write++;
		for (i = 0; i < WUPCOVF_IMP_SLP_CNT; i++)
		{
			*(claddr + cldata_write) = FCT_IMP_NOP_2;
			cldata_write++;
		}

		*(claddr + cldata_write) = FCT_IMP_WUP | (0x0001); //wakeup coremap 0
		cldata_write++;
		for (i = 0; i < WUPCOVF_IMP_SLP_CNT; i++)
		{
			*(claddr + cldata_write) = FCT_IMP_NOP_2;
			cldata_write++;
		}

		*(claddr + cldata_write) = FCT_IMP_TRAP;
		cldata_write++;
	}
	else if ((IMPDRV_CORE_TYPE_IMP == core_info->core_type) && (2U == core_info->core_num))
	{
		cldata_write = 0U;

		/* to sync core: at second execute CL sets: WUP-SLP */
		*(claddr + cldata_write) = FCT_IMP_WUP | (0x0001); //wakeup coremap 0
		cldata_write++;
		for (i = 0; i < WUPCOVF_IMP_SLP_CNT; i++)
		{
			*(claddr + cldata_write) = FCT_IMP_NOP_2;
			cldata_write++;
		}

		*(claddr + cldata_write) = FCT_IMP_TRAP;
		cldata_write++;
	}
	else
	{
		LOGPRINT(("[ERROR] Not supported cores\n"));
		ret = (-1);
	}
	
	return ret;
}