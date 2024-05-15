#ifndef __IMPDRV_IT_UTIL_H__
#define __IMPDRV_IT_UTIL_H__

#include <stdint.h>
#include "r_impdrv_private.h"
#include "r_impdrv_genctl.h"
#include "soc_common.h"

/*** memory size ***/
#define CL_SIZE				(256)
#define LONG_CL_SIZE		(256 * 1024)

#define ALIGN64(a)      	(((a) + 63U) & (~63U))

/* CL size */
#define MSCO_CL_SIZE		(0x6000U)
#define USINT_CL_SIZE		(0x3000U)
#define APIPINT_CL_SIZE		(0x3000U)
#define HPINT_CL_SIZE		(0x3000U)

#define IMAGE_COPY_COUNT	(2u)

#define INT_CL_SIZE			(0x2000u)     /* 4096 */

/* APIPINT info */
#define APIPINT_IMAGE_SIZE	(0x100u)
#define APIPINT_IMAGE_NUM	(4u)

/* HPINT info */
#define HPINT_IMAGE_SIZE	(0x100u)
#define HPINT_IMAGE_NUM		(2u)

/* MSCO info */
#define MSCO_INTERNAL_CL_SIZE		(0x5000U)
#define MSCO_INTERNAL_SUB_CL_SIZE	(0x100U)

#define DMAI0_OFFSET				(MSCO_INTERNAL_CL_SIZE + MSCO_INTERNAL_SUB_CL_SIZE * 0)
#define DMA3DC0_OFFSET				(MSCO_INTERNAL_CL_SIZE + MSCO_INTERNAL_SUB_CL_SIZE * 1)
#define DMAO0_31_OFFSET				(MSCO_INTERNAL_CL_SIZE + MSCO_INTERNAL_SUB_CL_SIZE * 2)

/* END info */
#define END_CL_SIZE		(256u * sizeof(uint32_t))
#define END_ALIGNMENT	(128u)
#define IMAGE_WIDTH     (16)
#define IMAGE_HEIGHT    (16)
#define IMAGE_BPP       (1)       /* bpp (byte per pix) */
#define IMAGE_TYPE      (R_ATMLIB_IMG_8U)
#define DST0_WIDTH      (12)
#define DST0_HEIGHT     (12)
#define DST1_WIDTH      (8)
#define DST1_HEIGHT     (12)
#define DST2_WIDTH      (12)
#define DST2_HEIGHT     (8)
#define DST3_WIDTH      (8)
#define DST3_HEIGHT     (8)
#define IMAGE_SIZE      (IMAGE_HEIGHT * IMAGE_WIDTH * IMAGE_BPP)
#define DST0_SIZE       (DST0_HEIGHT * DST0_WIDTH * IMAGE_BPP)
#define DST1_SIZE       (DST1_HEIGHT * DST1_WIDTH * IMAGE_BPP)
#define DST2_SIZE       (DST2_HEIGHT * DST2_WIDTH * IMAGE_BPP)
#define DST3_SIZE       (DST3_HEIGHT * DST3_WIDTH * IMAGE_BPP)

/* WUPCOVF info */
#define WUPCOVF_CL_SIZE                 (0x1000u)
#define WUPCOVF_CORE_NUM                (5)
#define WUPCOVF_IMP_SLP_CNT             (1U) /* assume 1 */

#define IMP_MALLOC_HW                   (0x00U)
#define NOPCLSIZE                       (102400U)

/**********************************************************************************************************************
 Global Typedef definitions
 *********************************************************************************************************************/
/* declare typedef */
typedef struct {
    uint32_t                       buffersize;
    void                           *cpu_addr;
    uintptr_t                      hw_addr;
} st_buffer_t;

typedef struct END_cl_data {
	void *cl_cpu_addr;
    void *imga_cpu_addr;
    void *dst0_cpu_addr;
    void *dst1_cpu_addr;
    void *dst2_cpu_addr;
    void *dst3_cpu_addr
} END_cl_data_t;

typedef enum e_impdemo_testcase
{
	TESTCASE_IMP_HPINT = 0,
	TESTCASE_IMP_APINT,
	TESTCASE_OCV_USINT,
	TESTCASE_CNN_MSCO,
} e_impdemo_testcase_t;

typedef struct {
    uint32_t cl_nop[NOPCLSIZE];
} st_cldata_t;

typedef struct {
#if IMP_CORE_MAX > 0
    st_cldata_t imp[IMP_CORE_MAX];
#endif
#if IMP_SLIM_CORE_MAX > 0
    st_cldata_t imp_slim[IMP_SLIM_CORE_MAX];
#endif
#if OCV_CORE_MAX > 0
    st_cldata_t ocv[OCV_CORE_MAX];
#endif
#if DMA_CORE_MAX > 0
    st_cldata_t dmac[DMA_CORE_MAX];
#endif
#if DMA_SLIM_CORE_MAX > 0
    st_cldata_t dmac_slim[DMA_SLIM_CORE_MAX];
#endif
#if PSCEXE_CORE_MAX > 0
    st_cldata_t pscexe[PSCEXE_CORE_MAX];
#endif
#if CNN_CORE_MAX > 0
    st_cldata_t cnn[CNN_CORE_MAX];
#endif
} st_imp_buffer_t;

/*** function ***/
int32_t create_msg(void **handle, uint32_t id);
void destroy_msg(void *handle);
int32_t send_msg(void *handle, int32_t send_data);
int32_t receive_msg(void *handle, int32_t *msg);
int32_t receive_msg_time(void *handle, int32_t *msg, int32_t time);
int32_t reset_msg(void *handle);

int32_t OsDepInit(osal_memory_manager_handle_t *osal_mmngr);
int32_t OsDepDeinit(osal_memory_manager_handle_t *osal_mmngr);
void util_mem_flush(void *addr);
void util_mem_invalidate(void *addr);
osal_memory_buffer_handle_t util_mem_get_handle(void *addr);
uintptr_t util_mem_get_paddr(void *addr);
void *util_mem_get_vaddr(void *addr);
size_t util_mem_get_size(void *addr);
uint64_t util_counttousec(uint64_t count);
uint64_t util_getcount(void);

void *MemAlloc(osal_memory_manager_handle_t osal_mmngr, size_t size, size_t align, uint32_t flags);
void MemFree(osal_memory_manager_handle_t osal_mmngr, void *addr);
void StartHardwareAccess(void *addr, size_t size);
void EndHardwareAccess(void *addr, size_t size);
void *createCL_SBOtest(uint32_t threthold);
void *createCL_USIER(uint32_t threthold);
void createCL_APIPINTtest(const void *const cpu_addr);
void createCL_HPINTtest(const void *const cpu_addr);
void createCL_USINTtest(void *const cpu_addr);
int8_t createCL_MSCOtest(void *const cpu_addr);
int8_t createCL_ENDtest(END_cl_data_t *p_data);
int32_t createCLpair_WUPCOVFtest(void *const cpu_addr, uint32_t sync_id);
int32_t createCL_WUPCOVFtest(void *const cpu_addr, const e_impdrv_core_type_t core_type, uint32_t sync_id);
void irq_group_createCLdata(st_buffer_t *buffer);
int32_t CLgethw_addr(const st_buffer_t *const cl_buffer, const st_impdrv_core_info_t *core, uint32_t *p_claddr_phys, uint32_t *p_clsize);
int32_t set_intpbcovf_commandlist(uint32_t type, void *addr, uint32_t tcode);
int32_t set_pbcovf_commandlist(uint32_t type, void *addr, uint32_t tcode);
int8_t set_cl_data_for_wupcovf_imp(const st_impdrv_core_info_t *const core_info, uint32_t *claddr);

char *type_to_name(uint32_t type);
char *impdrv_err_to_name(e_impdrv_errorcode_t err);
char *osal_err_to_name(e_osal_return_t osal_err);
char *policy_to_name(e_impdrv_pm_policy_t target_policy);
char *state_to_name(e_impdrv_state_t state);
char *instance_to_name(e_impdrv_instance_t instance_num);
char *irq_group_to_name(e_impdrv_irq_group_t irq_group);
char *irq_to_name(uint32_t irq_name);
int8_t get_test_noexec_core(uint32_t count, st_impdrv_core_info_t *core_info);
const st_impdrv_core_info_t *get_core_info(const st_impdrv_core_info_t *core_list, const uint32_t range, const e_impdrv_core_type_t chk_core_type, const uint32_t chk_core_num);
int32_t check_state(const st_impdrv_initdata_t *const p_data, const st_impdrv_coretypectl_t coretype_ctl[], const e_impdrv_state_t state, uint32_t num);
int32_t compare_callback(st_impdrv_core_info_t *core, int32_t ercd, int32_t code, int32_t cercd, int32_t ccode);
int32_t wait_and_check_callback(void *msgcnf, st_impdrv_core_info_t *core_info, int32_t ercd, int32_t code);

extern osal_memory_manager_handle_t	osal_mmngr;
extern osal_axi_bus_id_t            imp_dev_axi_bus_id;
extern volatile bool				msco_int_flg;

#endif /* __IMPDRV_IT_UTIL_H__ */