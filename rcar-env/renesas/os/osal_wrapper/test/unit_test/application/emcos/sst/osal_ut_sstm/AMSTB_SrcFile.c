#ifdef WINAMS_STUB
#ifdef __cplusplus
extern "C" {
#endif

#include "../../src/include/stub.h"
    
#include "target/emcos/r_osal_common.h"
//#include "target/qnx/r_osal_common.h"
//#include "target/common/pma/r_pma_common.h"
//#include "target/common/pma/r_pma.h"
//#include "target/common/pma/statemachine/r_pma_statemachine.h"
//#include "target/common/pma/hw/r_pma_hwdepend.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg_control.h"
//#include "target/emcos/dev_cfg/r_osal_dev_cfg_tbl_v3h1.h"
//#include "target/emcos/dev_cfg/r_osal_dev_cfg_tbl_v3h2.h"
//#include "target/emcos/dev_cfg/r_osal_dev_cfg_tbl_v3m.h"
#include "target/emcos/dev_cfg/r_osal_dev_cfg_tbl_v4h.h"
#include "target/emcos/usr_cfg/r_osal_usr_cfg.h"
#include "target/emcos/usr_cfg/r_osal_usr_cfg_control.h"
#include "rcar-xos/osal/r_osal.h"
//#include "rcar-xos/osal/r_osal_api.h"
//#include "rcar-xos/osal/r_osal_def.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"
//#include "rcar-xos/osal/r_osal_types.h"
#include "rcar-xos/osal/r_osal_version.h"
#include "rcar-xos/memory_allocator/r_meml.h"
#include "rcar-xos/osal_configuration/r_osal_dev_cfg_info.h"
#include "rcar-xos/osal_configuration/r_osal_usr_cfg_info.h"
#include "rcar-xos/osal_configuration/r_osal_configuration.h"


/* WINAMS_VAR_EXT[R_PMA_TSK_HANDLE R_PMA_SM_GxThread] */
// extern R_PMA_TSK_HANDLE R_PMA_SM_GxThread;

/* WINAMS_VAR_EXT[R_PMA_QUE_HANDLE R_PMA_SM_GxTaskQueue] */
// extern R_PMA_QUE_HANDLE R_PMA_SM_GxTaskQueue;

/* WINAMS_VAR_EXT[R_PMA_SEM_HANDLE R_PMA_SM_GxMutex] */
// extern R_PMA_SEM_HANDLE R_PMA_SM_GxMutex;

// /* WINAMS_VAR_EXT[e_pma_hwa_id_t hwa_idmin] */
// extern e_pma_hwa_id_t hwa_idmin;

// /* WINAMS_VAR_EXT[e_pma_hwa_id_t hwa_idmax] */
// extern e_pma_hwa_id_t hwa_idmax;

/* WINAMS_VAR_EXT[const st_hwa_func_tbl_t *R_PMA_SM_GstRegStatChangeFuncTbl] */
//extern const st_hwa_func_tbl_t *R_PMA_SM_GstRegStatChangeFuncTbl;

/* WINAMS_VAR_EXT[e_sysc_domain_id_t sysc_idmin] */
// extern e_sysc_domain_id_t sysc_idmin;

/* WINAMS_VAR_EXT[e_sysc_domain_id_t sysc_idmax] */
// extern e_sysc_domain_id_t sysc_idmax;

/* WINAMS_VAR_EXT[const unsigned long long *sysc_mask_tbl] */
extern const uint64_t *sysc_mask_tbl;

/* WINAMS_VAR_EXT[const int32_t *R_PMA_SYSC_PDR_ID_TABLE] */
extern const int32_t *R_PMA_SYSC_PDR_ID_TABLE;

/* WINAMS_VAR_EXT[e_cpg_domain_id_t cpg_idmin] */
// extern e_cpg_domain_id_t cpg_idmin;

/* WINAMS_VAR_EXT[e_cpg_domain_id_t cpg_idmax] */
// extern e_cpg_domain_id_t cpg_idmax;

/* WINAMS_VAR_EXT[uint64_t pma_soc_type] */
// extern uint64_t pma_soc_type;

/* WINAMS_VAR_EXT[const int *R_PMA_CPG_MOD_ID_TABLE] */
extern const int32_t *R_PMA_CPG_MOD_ID_TABLE;

typedef struct st_osal_inner_mq_control
{
    bool                      send_wait;     /*!< Send wait flag */
    bool                      receive_wait;  /*!< Receive wait flag */
    st_osal_queue_t           *queue;        /*!< Pointer of queue */
    int32_t                   head;          /*!< Element number of the next queue to read */
    int32_t                   tail;          /*!< Element number of the next queue to write */
    pthread_mutex_t           inner_mtx;     /*!< Internal mutex object */
    osal_milli_sec_t          inner_timeout; /*!< Waiting time of lock inner_mtx */
    pthread_cond_t            send_cond;     /*!< Handle of Condition Variable for waiting to send */
    pthread_cond_t            receive_cond;  /*!< Handle of Condition Variable for waiting to receive */
    uint32_t                  numofmsg;      /*!< Number of messages in the queue */
    st_osal_inner_mq_config_t config;        /*!< Structure to Message queue configuration */
}st_osal_inner_mq_control_t;

typedef struct st_osal_inner_thread_control
{
    pthread_t           thread;         /*!< Handle of thread for emcos */
    uint16_t            num;            /*!< Index number containing the handle */
}st_osal_inner_thread_control_t;

typedef struct st_osal_inner_mutexattr
{
    pthread_mutexattr_t attr;
}st_osal_inner_mutexattr_t;

int g_dummy;
int64_t g_return_value;
int32_t g_int32_t;
size_t g_size_t;
uint8_t  g_uint8_t;
uint16_t g_uint16_t;
uint32_t g_uint32_t;
uint64_t g_uint64_t;
uint64_t g_get_value;
uint64_t * g_get_value_p = &g_get_value;
pthread_mutexattr_t g_mutex_attr;
pthread_mutex_t g_mutex;
void* gp_void;
uintptr_t g_uintptr_t;
FILE g_FILE;
void * g_p_buffer_dummy;

uint8_t  array_uint8[10];
uint16_t array_uint16[10];
uint32_t array_uint32[10];
uint64_t array_uint64[10];
size_t   array_sizet[10];
char     str[256];

//st_osal_clock_info_t g_st_osal_clock_info = {{0, 0}, 0, 0};
//st_osal_clock_info_t *g_st_osal_clock_info_p;
struct timespec g_abs_timeout;
st_meml_buffer_t meml_buffer_dummy[1024];
char dummy_buffer_pa[4096];
char dummy_buffer_va[4096];
st_meml_manage_t meml_manager_dummy;
/*st_meml_manage_t meml_manager_dummy =
{
    meml_buffer_dummy,
    dummy_buffer_pa,
    1024,
    NULL,
    NULL,
    {
        &meml_buffer_dummy[0],
        0,
        1024,
        &(meml_manager_dummy.buffer),
        NULL
    }
};*/
st_meml_manage_t *p_meml_manager = &meml_manager_dummy;
st_meml_buffer_t *p_meml_buffer_dummy = meml_buffer_dummy;
st_meml_buffer_t *p_meml_buffer_dummy1 = &meml_buffer_dummy[384];
st_meml_buffer_t *p_meml_buffer_dummy2 = &meml_buffer_dummy[768];
st_meml_buffer_t *p_meml_manager_own = &(meml_manager_dummy.buffer);

st_osal_time_t g_st_osal_time;
st_osal_time_t g_st_osal_time2;
osal_nano_sec_t g_osal_nano_sec_t;
st_osal_thread_control_t g_thread_control_1;
st_osal_thread_control_t g_thread_control_2;

st_osal_thread_control_t g_thread_control;
st_osal_thread_control_t g_thread_control2;
st_osal_thread_control_t *g_thread_control_p;
bool g_bool;
st_osal_thread_config_t g_uts_thread_config;
int64_t g_return_value;
st_osal_inner_thread_control_t * g_st_inner_thread_control_p;
struct st_osal_rcfg_thread_config g_st_osal_rcfg_thread_config;
st_osal_inner_mutexattr_t * g_st_inner_mutexattr_p;
osal_inner_mutexattr_t g_osal_inner_mutexattr_p;

osal_mq_handle_t     g_osal_mq_handle;
st_osal_mq_config_t  g_uts_mq_config;
st_osal_mq_control_t g_mq_control;
st_osal_mq_control_t g_st_osal_mq_control;
st_osal_mq_control_t *g_st_osal_mq_control_p;
struct st_osal_rcfg_mq_config g_st_osal_rcfg_mq_config;
st_osal_mq_config_t g_st_osal_mq_config;
struct st_osal_inner_mq_control g_st_inner_mq_control;
struct st_osal_inner_mq_control * g_st_inner_mq_control_p;

osal_thread_handle_t g_uts_thread_handle;
osal_thread_handle_t *g_uts_thread_handle_p;
osal_mutex_handle_t g_osal_mutex_handle;
osal_cond_handle_t g_osal_cond_handle;
st_osal_mutex_control_t g_st_osal_mutex_control;
struct st_osal_rcfg_mutex_config g_st_osal_rcfg_mutex_config;
st_osal_cond_control_t g_st_osal_cond_control;
st_osal_queue_t g_st_osal_queue[10];
struct st_osal_rcfg_cond_config g_st_osal_rcfg_cond_config;
st_osal_mutex_control_t *g_st_osal_mutex_control_p;
st_osal_cond_control_t *g_st_osal_cond_control_p;
st_osal_inner_thread_attrinfo_t g_osal_inner_thread_attr;
osal_inner_thread_attr_t g_osal_inner_thread_attr_arry[2];

struct st_osal_device_info g_device_info_dummy;
st_osal_device_control_t g_device_control_dummy = {0, true, NULL, 500, 0, 0, NULL, 0, &g_device_info_dummy, NULL, NULL, false, 0};
st_osal_device_control_t *g_device_ppinfo_p = &g_device_control_dummy;
st_osal_device_control_t **g_device_ctrl_pp = &g_device_ppinfo_p;

struct st_osal_rcfg_thread_config g_thread_config_dummy;
st_osal_thread_control_t g_thread_ctrl_dummy = {0, false, NULL, 0, NULL, NULL, NULL, false, NULL, NULL, 0, &g_thread_config_dummy};
st_osal_thread_control_t *g_thread_ppinfo_p = &g_thread_ctrl_dummy;
st_osal_thread_control_t **g_thread_ctrl_pp = &g_thread_ppinfo_p;

struct st_osal_rcfg_mq_config g_mq_config_dummy;
st_osal_mq_control_t g_mq_ctrl_dummy = {.usr_config = &g_mq_config_dummy};
st_osal_mq_control_t *g_mq_ppinfo_p  = &g_mq_ctrl_dummy;
st_osal_mq_control_t **g_mq_ctrl_pp  = &g_mq_ppinfo_p;

struct st_osal_interrupt_thread_config g_interrupt_config_dummy;
st_osal_interrupt_thread_control_t g_interrupt_ctrl_dummy = { .usr_config = &g_interrupt_config_dummy};
st_osal_interrupt_thread_control_t *g_interrupt_ppinfo_p = &g_interrupt_ctrl_dummy;
st_osal_interrupt_thread_control_t **g_interrupt_ctrl_pp = &g_interrupt_ppinfo_p;

bool *gs_osal_threadsync_is_initialized_dummy;
bool *gs_osal_thread_is_initialized_dummy;
bool *gs_osal_interrupt_is_initialized_dummy;
bool *gp_osal_io_is_initialized_dummy;

st_osal_interrupt_thread_control_t g_st_osal_interrupt_thread_control;
st_osal_interrupt_thread_control_t * g_st_osal_interrupt_thread_control_p;
struct st_osal_interrupt_thread_config g_st_osal_interrupt_thread_config;
st_osal_device_control_t g_st_osal_device_control;
st_osal_device_control_t *g_st_osal_device_control_p;
struct st_osal_device_info g_st_osal_device_info;
st_osal_interrupt_info_t g_st_osal_interrupt_info;

 bool *p_gs_osal_interrupt_is_initialized_dummy;

/*
typedef struct st_osal_memory_manager_obj * osal_memory_manager_handle_t;
typedef struct st_osal_memory_buffer_obj * osal_memory_buffer_handle_t;
*/

st_osal_mmngr_config_t g_st_osal_mmngr_config;
osal_memory_manager_t g_osal_memory_manager;
osal_memory_buffer_t g_osal_memory_buffer;
st_osal_mem_mmngr_ptr_self_t g_st_osal_mem_mmngr_ptr_self[30];
st_osal_mem_mmngr_ptr_self_t *g_st_osal_mem_mmngr_ptr_self_p;
st_osal_mem_mmngr_ptr_self_t **g_st_osal_mem_mmngr_ptr_self_pp;
st_osal_mem_monitor_desc_t   g_st_osal_mem_monitor_desc;
osal_memory_manager_handle_t g_memory_manager_handle;
struct st_osal_memory_manager_obj g_st_osal_memory_manager_obj[30];
struct st_osal_memory_manager_obj * g_st_osal_memory_manager_obj_p;
st_osal_mem_buf_ptr_self_t g_osal_mem_buf_ptr_self;
osal_memory_buffer_handle_t g_memory_buffer_handle;
struct st_osal_memory_buffer_obj g_st_osal_memory_buffer_obj;
struct st_osal_memory_buffer_obj * g_osal_memory_buffer_obj;

uint8_t mmngr_buffer[OSAL_MMNGR_GUARD_PAGE_SIZE + 64 + OSAL_MMNGR_GUARD_PAGE_SIZE];

osal_axi_bus_id_t g_osal_axi_bus_id;
st_osal_axi_bus_control_t g_st_osal_axi_bus_control;
st_osal_axi_bus_control_t *g_st_osal_axi_bus_control_p;
st_osal_axi_bus_list_t    g_axi_bus_list[10];
osal_device_handle_t g_osal_device_handle;

e_osal_pm_state_t g_e_osal_pm_state;
st_pma_handle_t g_st_pma_handle;
e_osal_pm_policy_t g_e_osal_pm_policy;
e_osal_pm_required_state_t g_e_osal_pm_required_state;
e_osal_pm_reset_state_t g_e_osal_pm_reset_state;

/* note added 28,Februry ------------------------- */
st_osal_axi_bus_name_info_t g_axi_bus_name_info;
st_osal_axi_bus_name_info_t *g_axi_bus_name_info_p;

st_osal_utlb_assignment_info_t g_st_assignment;
st_osal_utlb_assignment_info_t *g_st_assignment_p;

char char_dummy[32];
char *char_dummy_p;

char dummy_id[256];

size_t dummy_array_sizet[5];

size_t stb_current_utlb = 3;
size_t *stb_current_utlb_p = &stb_current_utlb;

size_t stb_match_count = 3;
size_t *stb_match_count_p = &stb_match_count;

/* case normal */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_1[3]   = {{"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};
st_osal_axi_bus_name_info_t *g_axi_bus_name_info_1_p   = &g_axi_bus_name_info_1;
st_osal_axi_bus_name_info_t **g_axi_bus_name_info_1_pp = &g_axi_bus_name_info_1_p;

/* case gsp_axi_bus_name_info[count].utlb = OSAL_INVALID_UTLB */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_2[3]   = {{"dummy_domain_name", -2, char_dummy, 32},
                                                          {"dummy_domain_name", -2, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};
st_osal_axi_bus_name_info_t *g_axi_bus_name_info_2_p   = &g_axi_bus_name_info_2;
st_osal_axi_bus_name_info_t **g_axi_bus_name_info_2_pp = &g_axi_bus_name_info_2_p;

/* case gsp_axi_bus_name_info[count].utlb = OSAL_INVALID_UTLB to OSAL_AXI_BUS_NAME_LAST */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_3[3]   = {{"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", -2, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};
st_osal_axi_bus_name_info_t *g_axi_bus_name_info_3_p   = &g_axi_bus_name_info_3;
st_osal_axi_bus_name_info_t **g_axi_bus_name_info_3_pp = &g_axi_bus_name_info_3_p;

/* case gsp_axi_bus_name_info[count].utlb = OSAL_AXI_BUS_NAME_LAST */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_4[3]   = {{"dummy_domain_name",  1, char_dummy, 32},
                                                          {"dummy_domain_name",  1, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};
st_osal_axi_bus_name_info_t *g_axi_bus_name_info_4_p   = &g_axi_bus_name_info_4;
st_osal_axi_bus_name_info_t **g_axi_bus_name_info_4_pp = &g_axi_bus_name_info_4_p;

st_osal_axi_bus_name_info_t g_axi_bus_name_info_5[4]   = {{"dummy_domain_name",  1, char_dummy, 32},
                                                          {"dummy_domain_name",  2, char_dummy, 32},
                                                          {"dummy_domain_name",  3, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};
st_osal_axi_bus_name_info_t *g_axi_bus_name_info_5_p   = &g_axi_bus_name_info_5;
st_osal_axi_bus_name_info_t **g_axi_bus_name_info_5_pp = &g_axi_bus_name_info_5_p;

st_osal_mmngr_region_info_t g_st_osal_mmngr_region_info;
st_osal_mmngr_region_info_t * g_st_osal_mmngr_region_info_p = &g_st_osal_mmngr_region_info;
st_osal_mmngr_region_info_t ** g_st_osal_mmngr_region_info_pp = &g_st_osal_mmngr_region_info_p;

st_osal_pdev_entry_t g_osal_pdev_entry;
uint32_t g_interrupt_count;
uint16_t g_interrupt_id;
pdev_memrange_t g_pdevmr;

/* ----------------------------------------------- */

st_pma_user_info_t  dummy_LpUserInfo[16];
//st_hwa_t            dummy_LpHwa[1];
//st_cpg_domain_t *   dummy_pCpgDomain[1];
//st_sysc_domain_t  * dummy_pSyscDomain[1];
//struct st_sysc_domain_t_ * dummy_sysc_pDepDomain[1];
//struct st_cpg_domain_t_  * dummy_cpg_pDepDomain[1];
//st_sysc_domain_t    dummy_SyscDomain;
//st_cpg_domain_t     dummy_CpgDomain;
st_pma_hwa_state_t  dummy_HwaStat;
//st_msg_to_task_t    dummy_MsgToTask[2];
st_pma_param_t    * dummy_pParam;


size_t dummy_size;
struct st_osal_mq_control dummy_st_osal_mq_control;

//sysc_mgr_funcs_t dummy_syschwfuncs ={0,dummy_sysc_init,0,dummy_sysc_power_is_off,dummy_sysc_power_off,dummy_sysc_power_on,0};

// st_osal_rcfg_thread_config_t *g_osal_thread_config_dummy_p = g_osal_thread_config_dummy;
// st_osal_rcfg_thread_config_t *g_osal_thread_config_p = g_osal_thread_config_dummy;
// st_osal_rcfg_thread_config_t *g_osal_thread_config_p = g_osal_thread_config;

//st_pma_user_info_t dummy_LpUserInfo[16];
//st_hwa_t dummy_LpHwa[1];
//st_cpg_domain_t * dummy_pCpgDomain[1];
//st_sysc_domain_t   * dummy_pSyscDomain[1];
//struct st_sysc_domain_t_ * dummy_sysc_pDepDomain[1];
//struct st_cpg_domain_t_  * dummy_cpg_pDepDomain[1];
//st_sysc_domain_t    dummy_SyscDomain;
//st_cpg_domain_t     dummy_CpgDomain;
//st_pma_hwa_state_t  dummy_HwaStat;
//st_msg_to_task_t    dummy_MsgToTask[2];
//st_pma_param_t    * dummy_pParam;

void osal_memory_mngr_cb_dummy(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
	(void)user_arg;
	(void)event;
	(void)error;
	(void)buffer_hndl;
	return;
}

int64_t osal_thread_func_dummy(void* user_arg)
{
	(void)user_arg;
	return 0;
}

void osal_irq_cb_dummy(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
	(void)device_handle;
	(void)irq_channel;
	(void)irq_arg;
	static bool volatile AM_flags;

	if (1 == AM_flags)
	{
		g_st_osal_interrupt_thread_control.irq_enable = true;
	}
	else if (0 == AM_flags)
	{
		g_st_osal_interrupt_thread_control.irq_enable = false;
	}
	AM_flags = 1;

	return;
}

uintptr_t meml_alloc_dummy(st_meml_manage_t* p_manage, size_t size, size_t alignment)
{
	(void)p_manage;
	(void)size;
	(void)alignment;
	static uintptr_t volatile AMIN_return;

	return 1;
}

e_meml_return_t meml_dealloc_dummy(st_meml_manage_t* p_manage, uintptr_t address)
{
	(void)p_manage;
	(void)address;
	static e_meml_return_t volatile AMIN_return;

	return 0;
}

/* WINAMS_STUB[r_osal_power.c:osal_pm_initialize:AMSTB_osal_pm_initialize:inout:::] */
/*    osal_pm_initialize => Stub */
e_osal_return_t AMSTB_osal_pm_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_power.c:R_OSAL_PmInitialize:AMSTB_R_OSAL_PmInitialize:inout:::] */
/*    R_OSAL_PmInitialize => Stub */
e_osal_return_t AMSTB_R_OSAL_PmInitialize()
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_timeclock.c:osal_clock_time_initialize:AMSTB_osal_clock_time_initialize:inout:::] */
/*    osal_clock_time_initialize => Stub */
e_osal_return_t AMSTB_osal_clock_time_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[:printf:AMSTB_printf:inout:::] */
/*    printf => Stub */
void AMSTB_printf()
{
}

/* WINAMS_STUB[stub.c:pthread_mutex_timedlock_monotonic:AMSTB_pthread_mutex_timedlock_monotonic:inout:::] */
/*    pthread_mutex_timedlock_monotonic => Stub */
int AMSTB_pthread_mutex_timedlock_monotonic(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
	static int volatile AMIN_return;
	static int volatile AM_init_ope;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const struct timespec *volatile AMOUT_abs_timeout;
	AMOUT_mutex = mutex;
	AMOUT_abs_timeout = abs_timeout;
	
	if (1 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else
	{
		/* Do nothing */
	}

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_timedlock_monotonic:AMSTB_pthread_mutex_timedlock_monotonic:inout:::] */
/*    pthread_mutex_timedlock_monotonic => Stub */
int AMSTB_pthread_mutex_timedlock_monotonic_multiple(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
#define AMCALLMAX_pthread_mutex_timedlock_monotonic 30
	static int volatile AMIN_return[AMCALLMAX_pthread_mutex_timedlock_monotonic];
	static int volatile AM_init_ope;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const struct timespec *volatile AMOUT_abs_timeout;
	AMOUT_mutex = mutex;
	AMOUT_abs_timeout = abs_timeout;
	static int volatile AM_count;
	
	if (1 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else
	{
		/* Do nothing */
	}
	AM_count++;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_mutex_timedlock:AMSTB_pthread_mutex_timedlock:inout:::] */
/*    pthread_mutex_timedlock => Stub */
int AMSTB_pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
	static int volatile AMIN_return;
	static int volatile AM_init_ope;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const struct timespec *volatile AMOUT_abs_timeout;
	AMOUT_mutex = mutex;
	AMOUT_abs_timeout = abs_timeout;
	
	if (1 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else
	{
		/* Do nothing */
	}

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_timedlock:AMSTB_pthread_mutex_timedlock:inout:::] */
/*    pthread_mutex_timedlock => Stub */
int AMSTB_pthread_mutex_timedlock_multiple(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
#define AMCALLMAX_pthread_mutex_timedlock 30
	static int volatile AMIN_return[AMCALLMAX_pthread_mutex_timedlock];
	static int volatile AM_init_ope;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const struct timespec *volatile AMOUT_abs_timeout;
	AMOUT_mutex = mutex;
	AMOUT_abs_timeout = abs_timeout;
	static int volatile AM_count;
	
	if (1 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else
	{
		/* Do nothing */
	}
	AM_count++;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_cond_signal:AMSTB_pthread_cond_signal:inout:::] */
/*    pthread_cond_signal => Stub */
int AMSTB_pthread_cond_signal(pthread_cond_t *cond)
{
	static int volatile AMIN_return;
	static pthread_cond_t *volatile AMOUT_cond;
	AMOUT_cond = cond;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:ClockCycles:AMSTB_ClockCycles:inout:::] */
/*    ClockCycles => Stub */
uint64_t AMSTB_ClockCycles(void)
{
	static uint64_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_equal:AMSTB_pthread_equal:inout:::] */
/*    pthread_equal => Stub */
int AMSTB_pthread_equal(pthread_t t1, pthread_t t2)
{
	static int volatile AMIN_return;
	static pthread_t volatile AMOUT_t1;
	static pthread_t volatile AMOUT_t2;
	AMOUT_t1 = t1;
	AMOUT_t2 = t2;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:sysconf:AMSTB_sysconf:inout:::] */
/*    sysconf => Stub */
long AMSTB_sysconf(int name)
{
	static long volatile AMIN_return;
	static int volatile AMOUT_name;
	AMOUT_name = name;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_lock:AMSTB_pthread_mutex_lock:inout:::] */
/*    pthread_mutex_lock => Stub */
int AMSTB_pthread_mutex_lock(pthread_mutex_t *mutex)
{
	static int volatile AMIN_return;
	static pthread_mutex_t *volatile AMOUT_mutex;
	AMOUT_mutex = mutex;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_init:AMSTB_pthread_cond_init:inout:::] */
/*    pthread_cond_init => Stub */
int AMSTB_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
#define AMCALLMAX_pthread_cond_init 30
	static int volatile AMIN_return[AMCALLMAX_pthread_cond_init];
	static pthread_cond_t *volatile AMOUT_cond;
	static const pthread_condattr_t *volatile AMOUT_attr;
	static int volatile AM_count;
	AMOUT_cond = cond;
	AMOUT_attr = attr;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_attr_setstacksize:AMSTB_pthread_attr_setstacksize:inout:::] */
/*    pthread_attr_setstacksize => Stub */
int AMSTB_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
	static int volatile AMIN_return;
	static pthread_attr_t *volatile AMOUT_attr;
	static size_t volatile AMOUT_stacksize;
	AMOUT_attr = attr;
	AMOUT_stacksize = stacksize;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:out16:AMSTB_out16:inout:::] */
/*    out16 => Stub */
void AMSTB_out16(uintptr_t port, uint16_t val)
{
	static uintptr_t volatile AMOUT_port;
	static uint16_t volatile AMOUT_val;
	AMOUT_port = port;
	AMOUT_val = val;
}

/* WINAMS_STUB[stub.c:munmap:AMSTB_munmap:inout:::] */
/*    munmap => Stub */
int AMSTB_munmap(void *addr, size_t len)
{
	static int volatile AMIN_return;
	static void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	AMOUT_addr = addr;
	AMOUT_len = len;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:out8:AMSTB_out8:inout:::] */
/*    out8 => Stub */
void AMSTB_out8(uintptr_t port, uint8_t val)
{
	static uintptr_t volatile AMOUT_port;
	static uint8_t volatile AMOUT_val;
	AMOUT_port = port;
	AMOUT_val = val;
}

/* WINAMS_STUB[stub.c:pthread_cond_destroy:AMSTB_pthread_cond_destroy:inout:::] */
/*    pthread_cond_destroy => Stub */
int AMSTB_pthread_cond_destroy(pthread_cond_t *cond)
{
#define AMCALLMAX_pthread_cond_destroy 30
	static int volatile AMIN_return[AMCALLMAX_pthread_cond_destroy];
	static pthread_cond_t *volatile AMOUT_cond;
	static int volatile AM_count;
	AM_count++;

	AMOUT_cond = cond;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_attr_setschedpolicy:AMSTB_pthread_attr_setschedpolicy:inout:::] */
/*    pthread_attr_setschedpolicy => Stub */
int AMSTB_pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
	static int volatile AMIN_return;
	static pthread_attr_t *volatile AMOUT_attr;
	static int volatile AMOUT_policy;
	AMOUT_attr = attr;
	AMOUT_policy = policy;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:munlock:AMSTB_munlock:inout:::] */
/*    munlock => Stub */
int AMSTB_munlock(const void *addr, size_t len)
{
	static int volatile AMIN_return;
	static const void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	AMOUT_addr = addr;
	AMOUT_len = len;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:mmap:AMSTB_mmap:inout:::] */
/*    mmap => Stub */
void *AMSTB_mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
{
	static void *volatile AMIN_return;
	static void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	static int volatile AMOUT_prot;
	static int volatile AMOUT_flags;
	static int volatile AMOUT_fildes;
	static off_t volatile AMOUT_off;
	AMOUT_addr = addr;
	AMOUT_len = len;
	AMOUT_prot = prot;
	AMOUT_flags = flags;
	AMOUT_fildes = fildes;
	AMOUT_off = off;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:InterruptUnmask:AMSTB_InterruptUnmask:inout:::] */
/*    InterruptUnmask => Stub */
int AMSTB_InterruptUnmask(int intr, int id)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_intr;
	static int volatile AMOUT_id;
	AMOUT_intr = intr;
	AMOUT_id = id;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_timedjoin_monotonic:AMSTB_pthread_timedjoin_monotonic:inout:::] */
/*    pthread_timedjoin_monotonic => Stub */
int AMSTB_pthread_timedjoin_monotonic(pthread_t thread, void **value_ptr, const struct timespec *abstime)
{
	static int volatile AMIN_return;
	static void *volatile AMIN_value_ptr;
	static pthread_t volatile AMOUT_thread;
	static void **volatile AMOUT_value_ptr;
	static const struct timespec *volatile AMOUT_abstime;
	AMOUT_thread = thread;
	AMOUT_value_ptr = value_ptr;
	AMOUT_abstime = abstime;

	*value_ptr = AMIN_value_ptr;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutexattr_init:AMSTB_pthread_mutexattr_init:inout:::] */
/*    pthread_mutexattr_init => Stub */
int AMSTB_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_mutexattr_t *volatile AMOUT_attr;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_unlock:AMSTB_pthread_mutex_unlock:inout:::] */
/*    pthread_mutex_unlock => Stub */
int AMSTB_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
#define AMCALLMAX_pthread_mutex_unlock 30
	static int volatile AMIN_return[AMCALLMAX_pthread_mutex_unlock];
	static pthread_mutex_t *volatile AMOUT_mutex;
	static int volatile AM_count;
	static int volatile AM_interrupt;
	AMOUT_mutex = mutex;

	if (1 == AM_interrupt)
	{
		for (int i = 0; i < AMCALLMAX_pthread_mutex_unlock; i++)
		{
			AMIN_return[i] = AMIN_return[0];
			AM_count++;
		}
	}
	else
	{
		AM_count++;
	}

	AM_interrupt = 0;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_join:AMSTB_pthread_join:inout:::] */
/*    pthread_join => Stub */
int AMSTB_pthread_join(pthread_t thr, void **value_ptr)
{
#define AMCALLMAX_pthread_join 30
	static int volatile AMIN_return[AMCALLMAX_pthread_join];
	static void *volatile AMIN_value_ptr;
	static int volatile AM_count;
	static pthread_t volatile AMOUT_thr;
	static void **volatile AMOUT_value_ptr;
	AMOUT_thr = thr;
	AMOUT_value_ptr = value_ptr;

	*value_ptr = AMIN_value_ptr;

	AM_count++;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:error_stub:AMSTB_error_stub:inout:::] */
/*    error_stub => Stub */
int AMSTB_error_stub(void)
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_trylock:AMSTB_pthread_mutex_trylock:inout:::] */
/*    pthread_mutex_trylock => Stub */
int AMSTB_pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	static int volatile AMIN_return;
	static int volatile AM_init_ope;
	static pthread_mutex_t *volatile AMOUT_mutex;
	AMOUT_mutex = mutex;
	
	if (1 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else
	{
		/* Do nothing */
	}

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_broadcast:AMSTB_pthread_cond_broadcast:inout:::] */
/*    pthread_cond_broadcast => Stub */
int AMSTB_pthread_cond_broadcast(pthread_cond_t *cond)
{
	static int volatile AMIN_return;
	static pthread_cond_t *volatile AMOUT_cond;
	AMOUT_cond = cond;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:InterruptMask:AMSTB_InterruptMask:inout:::] */
/*    InterruptMask => Stub */
int AMSTB_InterruptMask(int intr, int id)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_intr;
	static int volatile AMOUT_id;
	AMOUT_intr = intr;
	AMOUT_id = id;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_setschedprio:AMSTB_pthread_setschedprio:inout:::] */
/*    pthread_setschedprio => Stub */
int AMSTB_pthread_setschedprio(pthread_t thr, int prio)
{
	static int volatile AMIN_return;
	static pthread_t volatile AMOUT_thr;
	static int volatile AMOUT_prio;
	AMOUT_thr = thr;
	AMOUT_prio = prio;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_init:AMSTB_pthread_mutex_init:inout:::] */
/*    pthread_mutex_init => Stub */
int AMSTB_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const pthread_mutexattr_t *volatile AMOUT_attr;
	AMOUT_mutex = mutex;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_detach:AMSTB_pthread_detach:inout:::] */
/*    pthread_detach => Stub */
int AMSTB_pthread_detach(pthread_t thread)
{
	static int volatile AMIN_return;
	static pthread_t volatile AMOUT_thread;
	AMOUT_thread = thread;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:mlock:AMSTB_mlock:inout:::] */
/*    mlock => Stub */
int AMSTB_mlock(const void *addr, size_t len)
{
	static int volatile AMIN_return;
	static const void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	AMOUT_addr = addr;
	AMOUT_len = len;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:clock_nanosleep:AMSTB_clock_nanosleep:inout:::] */
/*    clock_nanosleep => Stub */
int AMSTB_clock_nanosleep(clockid_t clock_id, int flags, const struct timespec *rqtp, struct timespec *rmtp)
{
	static int volatile AMIN_return;
	static clockid_t volatile AMOUT_clock_id;
	static int volatile AMOUT_flags;
	static const struct timespec *volatile AMOUT_rqtp;
	static struct timespec *volatile AMOUT_rmtp;
	AMOUT_clock_id = clock_id;
	AMOUT_flags = flags;
	AMOUT_rqtp = rqtp;
	AMOUT_rmtp = rmtp;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:msync:AMSTB_msync:inout:::] */
/*    msync => Stub */
int AMSTB_msync(void *addr, size_t len,int flags)
{
#define AMCALLMAX_msync 30
	static int volatile AMIN_return[AMCALLMAX_msync];
	static void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	static int volatile AMOUT_flags;
	static int volatile AM_count;
	AMOUT_addr = addr;
	AMOUT_len = len;
	AMOUT_flags = flags;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:clock_gettime:AMSTB_clock_gettime:inout:::] */
/*    clock_gettime => Stub */
int AMSTB_clock_gettime(clockid_t clock_id, struct timespec *tp)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_clock_id;
	static struct timespec *volatile AMOUT_tp;
	static long input_tv_sec;
	static long input_tv_nsec;

	tp->tv_sec  = input_tv_sec;
	tp->tv_nsec = input_tv_nsec;

	AMOUT_clock_id = clock_id;
	AMOUT_tp = tp;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:strnlen:AMSTB_strnlen:inout:::] */
/*    strnlen => Stub */
unsigned long AMSTB_strnlen(const char *s, size_t maxlen)
{
#define AMCALLMAX_strnlen 30
	static unsigned long volatile AMIN_return[AMCALLMAX_strnlen];
	static const char *volatile AMOUT_s;
	static size_t volatile AMOUT_maxlen;
	static int volatile AM_count;
	AMOUT_s = s;
	AMOUT_maxlen = maxlen;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_self:AMSTB_pthread_self:inout:::] */
/*    pthread_self => Stub */
pthread_t AMSTB_pthread_self()
{
	static pthread_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutexattr_settype:AMSTB_pthread_mutexattr_settype:inout:::] */
/*    pthread_mutexattr_settype => Stub */
int AMSTB_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
	static int volatile AMIN_return;
	static pthread_mutexattr_t *volatile AMOUT_attr;
	static int volatile AMOUT_type;
	AMOUT_attr = attr;
	AMOUT_type = type;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_getspecific:AMSTB_pthread_getspecific:inout:::] */
/*    pthread_getspecific => Stub */
void *AMSTB_pthread_getspecific(pthread_key_t key)
{
	static void *volatile AMIN_return;
	static pthread_key_t volatile AMOUT_key;
	AMOUT_key = key;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_condattr_destroy:AMSTB_pthread_condattr_destroy:inout:::] */
/*    pthread_condattr_destroy => Stub */
int AMSTB_pthread_condattr_destroy(pthread_condattr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_condattr_t *volatile AMOUT_attr;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_wait:AMSTB_pthread_cond_wait:inout:::] */
/*    pthread_cond_wait => Stub */
int AMSTB_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	static int volatile AMIN_return;
	static pthread_cond_t *volatile AMOUT_cond;
	static pthread_mutex_t *volatile AMOUT_mutex;
	AMOUT_cond = cond;
	AMOUT_mutex = mutex;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_timedwait:AMSTB_pthread_cond_timedwait:inout:::] */
/*    pthread_cond_timedwait => Stub */
int AMSTB_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
	static int volatile AMIN_return;
	static int volatile AM_init_ope;
	static pthread_cond_t *volatile AMOUT_cond;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const struct timespec *volatile AMOUT_abstime;
	AMOUT_cond = cond;
	AMOUT_mutex = mutex;
	AMOUT_abstime = abstime;
	
	if (1 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else
	{
		/* Do nothing */
	}
	
	return AMIN_return;
}

int AMSTB_pthread_cond_timedwait_mqstb(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
#define AMCALLMAX_cond_mq 30
	static int volatile AMIN_return[AMCALLMAX_cond_mq];
	static pthread_cond_t *volatile AMOUT_cond;
	static pthread_mutex_t *volatile AMOUT_mutex;
	static const struct timespec *volatile AMOUT_abstime;
	AMOUT_cond = cond;
	AMOUT_mutex = mutex;
	AMOUT_abstime = abstime;

	static int volatile AM_val;
	static int volatile AM_count;

    if (AM_count != 0)
    {
        g_st_osal_mq_control.numofmsg = AM_val;
        g_st_inner_mq_control.numofmsg = AM_val;
    }

    AM_count++;

	return AMIN_return[AM_count - 1];

}

/* WINAMS_STUB[stub.c:munmap_device_memory:AMSTB_munmap_device_memory:inout:::] */
/*    munmap_device_memory => Stub */
int AMSTB_munmap_device_memory(void *addr, size_t len)
{
	static int volatile AMIN_return;
	static void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	AMOUT_addr = addr;
	AMOUT_len = len;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:close:AMSTB_close:inout:::] */
/*    close => Stub */
int AMSTB_close(int fildes)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_fildes;
	AMOUT_fildes = fildes;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_setspecific:AMSTB_pthread_setspecific:inout:::] */
/*    pthread_setspecific => Stub */
int AMSTB_pthread_setspecific(pthread_key_t key,void *value)
{
	static int volatile AMIN_return;
	static pthread_key_t volatile AMOUT_key;
	static void *volatile AMOUT_value;
	AMOUT_key = key;
	AMOUT_value = value;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:mmap_device_memory:AMSTB_mmap_device_memory:inout:::] */
/*    mmap_device_memory => Stub */
void *AMSTB_mmap_device_memory(void *addr, size_t len, int prot,int flags, uint64_t physical)
{
	static void *volatile AMIN_return;
	static void *volatile AMOUT_addr;
	static size_t volatile AMOUT_len;
	static int volatile AMOUT_prot;
	static int volatile AMOUT_flags;
	static uint64_t volatile AMOUT_physical;
	AMOUT_addr = addr;
	AMOUT_len = len;
	AMOUT_prot = prot;
	AMOUT_flags = flags;
	AMOUT_physical = physical;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:in32:AMSTB_in32:inout:::] */
/*    in32 => Stub */
unsigned int AMSTB_in32(uintptr_t port)
{
	static unsigned int volatile AMIN_return;
	static uintptr_t volatile AMOUT_port;
	AMOUT_port = port;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:TimerTimeout:AMSTB_TimerTimeout:inout:::] */
/*    TimerTimeout => Stub */
int AMSTB_TimerTimeout(clockid_t id, int flags, const struct sigevent *notify, const uint64_t *ntime, uint64_t *otime)
{
	static int volatile AMIN_return;
	static clockid_t volatile AMOUT_id;
	static int volatile AMOUT_flags;
	static const struct sigevent *volatile AMOUT_notify;
	static const uint64_t *volatile AMOUT_ntime;
	static uint64_t *volatile AMOUT_otime;
	AMOUT_id = id;
	AMOUT_flags = flags;
	AMOUT_notify = notify;
	AMOUT_ntime = ntime;
	AMOUT_otime = otime;
	return AMIN_return;
}


/* WINAMS_STUB[stub.c:pthread_mutexattr_destroy:AMSTB_pthread_mutexattr_destroy:inout:::] */
/*    pthread_mutexattr_destroy => Stub */
int AMSTB_pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_mutexattr_t *volatile AMOUT_attr;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:posix_typed_mem_open:AMSTB_posix_typed_mem_open:inout:::] */
/*    posix_typed_mem_open => Stub */
int AMSTB_posix_typed_mem_open(const char *name, int oflag, int tflag)
{
	static int volatile AMIN_return;
	static const char *volatile AMOUT_name;
	static int volatile AMOUT_oflag;
	static int volatile AMOUT_tflag;
	AMOUT_name = name;
	AMOUT_oflag = oflag;
	AMOUT_tflag = tflag;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:in8:AMSTB_in8:inout:::] */
/*    in8 => Stub */
unsigned char AMSTB_in8(uintptr_t port)
{
	static unsigned char volatile AMIN_return;
	static uintptr_t volatile AMOUT_port;
	AMOUT_port = port;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_attr_setschedparam:AMSTB_pthread_attr_setschedparam:inout:::] */
/*    pthread_attr_setschedparam => Stub */
int AMSTB_pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
	static int volatile AMIN_return;
	static pthread_attr_t *volatile AMOUT_attr;
	static const struct sched_param *volatile AMOUT_param;
	AMOUT_attr = attr;
	AMOUT_param = param;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_attr_setinheritsched:AMSTB_pthread_attr_setinheritsched:inout:::] */
/*    pthread_attr_setinheritsched => Stub */
int AMSTB_pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
	static int volatile AMIN_return;
	static pthread_attr_t *volatile AMOUT_attr;
	static int volatile AMOUT_inheritsched;
	AMOUT_attr = attr;
	AMOUT_inheritsched = inheritsched;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_attr_init:AMSTB_pthread_attr_init:inout:::] */
/*    pthread_attr_init => Stub */
int AMSTB_pthread_attr_init(pthread_attr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_attr_t *volatile AMOUT_attr;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:out32:AMSTB_out32:inout:::] */
/*    out32 => Stub */
void AMSTB_out32(uintptr_t port, uint32_t val)
{
	static uintptr_t volatile AMOUT_port;
	static uint32_t volatile AMOUT_val;
	AMOUT_port = port;
	AMOUT_val = val;
}

/* WINAMS_STUB[stub.c:ThreadCtl:AMSTB_ThreadCtl:inout:::] */
/*    ThreadCtl => Stub */
int AMSTB_ThreadCtl(int cmd, void *data)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_cmd;
	static void *volatile AMOUT_data;
	AMOUT_cmd = cmd;
	AMOUT_data = data;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cancel:AMSTB_pthread_cancel:inout:::] */
/*    pthread_cancel => Stub */
int AMSTB_pthread_cancel(pthread_t thr)
{
	static int volatile AMIN_return;
	static pthread_t volatile AMOUT_thr;
	AMOUT_thr = thr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:munmap_device_io:AMSTB_munmap_device_io:inout:::] */
/*    munmap_device_io => Stub */
int AMSTB_munmap_device_io(uintptr_t io, size_t len)
{
	static int volatile AMIN_return;
	static uintptr_t volatile AMOUT_io;
	static size_t volatile AMOUT_len;
	AMOUT_io = io;
	AMOUT_len = len;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:InterruptWait:AMSTB_InterruptWait:inout:::] */
/*    InterruptWait => Stub */
int AMSTB_InterruptWait(int flags, const uint64_t *timeout)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_flags;
	static const uint64_t *volatile AMOUT_timeout;
	AMOUT_flags = flags;
	AMOUT_timeout = timeout;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:InterruptAttachEvent:AMSTB_InterruptAttachEvent:inout:::] */
/*    InterruptAttachEvent => Stub */
int AMSTB_InterruptAttachEvent(int intr, const struct sigevent *event, unsigned flags)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_intr;
	static const struct sigevent *volatile AMOUT_event;
	static unsigned int volatile AMOUT_flags;
	AMOUT_intr = intr;
	AMOUT_event = event;
	AMOUT_flags = flags;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:sched_get_priority_min:AMSTB_sched_get_priority_min:inout:::] */
/*    sched_get_priority_min => Stub */
int AMSTB_sched_get_priority_min(int policy)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_policy;
	AMOUT_policy = policy;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_destroy:AMSTB_pthread_mutex_destroy:inout:::] */
/*    pthread_mutex_destroy => Stub */
int AMSTB_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
#define AMCALLMAX_pthread_mutex_destroy 550
	static int volatile AMIN_return[AMCALLMAX_pthread_mutex_destroy];
	static pthread_mutex_t *volatile AMOUT_mutex;
	static int volatile AM_count;

	if ((0 != AMIN_return[0]) && (-1 != AMIN_return[0]))
	{
		if (0 == AMIN_return[1])
		{
			/* do nothing */
		}
		else
		{
			for (int i = 0; i < AMCALLMAX_pthread_mutex_destroy; i++)
			{
				AMIN_return[i] = 16; /* set EBUSY */
			}
		}
	}

	AMOUT_mutex = mutex;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

int AMSTB_pthread_mutex_destroy_mqstb(pthread_mutex_t *mutex)
{
#define AMCALLMAX_pthread_mutex_destroy 550
	static int volatile AMIN_return[AMCALLMAX_pthread_mutex_destroy];
	static pthread_mutex_t *volatile AMOUT_mutex;
	static int volatile AM_count;
	static int volatile AM_control;

	AMOUT_mutex = mutex;
	AM_count++;

	if (0 != AM_control)
	{
		for (int i = 0; i < AMCALLMAX_pthread_mutex_destroy; i++)
		{
			AMIN_return[i] = 16; /* set EBUSY */
		}
	}

    if (2 == AM_control)
    {
		AMIN_return[5] = 0;
    }
	if (3 == AM_control)
    {
		AMIN_return[5] = -1;
    }
	AM_control = 0;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_key_delete:AMSTB_pthread_key_delete:inout:::] */
/*    pthread_key_delete => Stub */
int AMSTB_pthread_key_delete(pthread_key_t key)
{
	static int volatile AMIN_return;
	static pthread_key_t volatile AMOUT_key;
	AMOUT_key = key;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_key_create:AMSTB_pthread_key_create:inout:::] */
/*    pthread_key_create => Stub */
int AMSTB_pthread_key_create(pthread_key_t *key, void *destructor)
{
	static int volatile AMIN_return;
	static unsigned int *volatile AMOUT_key;
	static void *volatile AMOUT_destructor;
	AMOUT_key = key;
	AMOUT_destructor = destructor;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_create:AMSTB_pthread_create:inout:::] */
/*    pthread_create => Stub */
int AMSTB_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *start_routine, void *arg)
{
	static int volatile AMIN_return;
	static pthread_t *volatile AMOUT_thread;
	static const pthread_attr_t *volatile AMOUT_attr;
	static void *volatile AMOUT_start_routine;
	static void *volatile AMOUT_arg;
	AMOUT_thread = thread;
	AMOUT_attr = attr;
	AMOUT_start_routine = start_routine;
	AMOUT_arg = arg;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_condattr_init:AMSTB_pthread_condattr_init:inout:::] */
/*    pthread_condattr_init => Stub */
int AMSTB_pthread_condattr_init(pthread_condattr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_condattr_t *volatile AMOUT_attr;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_attr_destroy:AMSTB_pthread_attr_destroy:inout:::] */
/*    pthread_attr_destroy => Stub */
int AMSTB_pthread_attr_destroy(pthread_attr_t *attr)
{
	static int volatile AMIN_return;
	static pthread_attr_t *volatile AMOUT_attr;
	AMOUT_attr = attr;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:mmap_device_io:AMSTB_mmap_device_io:inout:::] */
/*    mmap_device_io => Stub */
uintptr_t AMSTB_mmap_device_io(size_t len, uint64_t io)
{
	static uintptr_t volatile AMIN_return;
	static size_t volatile AMOUT_len;
	static uint64_t volatile AMOUT_io;
	AMOUT_len = len;
	AMOUT_io = io;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:in16:AMSTB_in16:inout:::] */
/*    in16 => Stub */
uint16_t AMSTB_in16(uintptr_t port)
{
	static uint16_t volatile AMIN_return;
	static uintptr_t volatile AMOUT_port;
	AMOUT_port = port;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_timeclock.c:osal_clock_time_deinitialize:AMSTB_osal_clock_time_deinitialize:inout:::] */
/*    osal_clock_time_deinitialize => Stub */
e_osal_return_t AMSTB_osal_clock_time_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_interrupt.c:osal_interrupt_deinitialize:AMSTB_osal_interrupt_deinitialize:inout:::] */
/*    osal_interrupt_deinitialize => Stub */
e_osal_return_t AMSTB_osal_interrupt_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_interrupt.c:osal_interrupt_initialize:AMSTB_osal_interrupt_initialize:inout:::] */
/*    osal_interrupt_initialize => Stub */
e_osal_return_t AMSTB_osal_interrupt_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:osal_io_deinitialize:AMSTB_osal_io_deinitialize:inout:::] */
/*    osal_io_deinitialize => Stub */
e_osal_return_t AMSTB_osal_io_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:osal_io_initialize:AMSTB_osal_io_initialize:inout:::] */
/*    osal_io_initialize => Stub */
e_osal_return_t AMSTB_osal_io_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_memory_deinitialize:AMSTB_osal_memory_deinitialize:inout:::] */
/*    osal_memory_deinitialize => Stub */
e_osal_return_t AMSTB_osal_memory_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_memory_initialize:AMSTB_osal_memory_initialize:inout:::] */
/*    osal_memory_initialize => Stub */
e_osal_return_t AMSTB_osal_memory_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_mq_deinitialize:AMSTB_osal_mq_deinitialize:inout:::] */
/*    osal_mq_deinitialize => Stub */
e_osal_return_t AMSTB_osal_mq_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_mq_initialize:AMSTB_osal_mq_initialize:inout:::] */
/*    osal_mq_initialize => Stub */
e_osal_return_t AMSTB_osal_mq_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_thread.c:osal_thread_deinitialize:AMSTB_osal_thread_deinitialize:inout:::] */
/*    osal_thread_deinitialize => Stub */
e_osal_return_t AMSTB_osal_thread_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_thread.c:osal_thread_initialize:AMSTB_osal_thread_initialize:inout:::] */
/*    osal_thread_initialize => Stub */
e_osal_return_t AMSTB_osal_thread_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_threadsync_deinitialize:AMSTB_osal_threadsync_deinitialize:inout:::] */
/*    osal_threadsync_deinitialize => Stub */
e_osal_return_t AMSTB_osal_threadsync_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_threadsync_initialize:AMSTB_osal_threadsync_initialize:inout:::] */
/*    osal_threadsync_initialize => Stub */
e_osal_return_t AMSTB_osal_threadsync_initialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:__cpu_membarrier:AMSTB___cpu_membarrier:inout:::] */
/*    __cpu_membarrier => Stub */
void AMSTB___cpu_membarrier(void)
{
}

/* WINAMS_STUB[:snprintf:AMSTB_snprintf:inout:::] */
int AMSTB_snprintf()
{
#define AMCALLMAX_snprintf 30
	static int volatile AMIN_return[AMCALLMAX_snprintf];
	static int volatile AM_count;
	
	AM_count++;
	return AMIN_return[AM_count -1];
}

/* WINAMS_STUB[:fprintf:AMSTB_fprintf:inout:::] */
/*    fprintf => Stub */
int AMSTB_fprintf()
{
#define AMCALLMAX_fprintf 30
	static int volatile AMIN_return[AMCALLMAX_fprintf];
	static int volatile AM_count;
	AM_count++;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:strlen:AMSTB_strlen:inout:::] */
/*    strlen => Stub */
unsigned long AMSTB_strlen(const char *s)
{
	static unsigned long volatile AMIN_return;
	static const char *volatile AMOUT_s;
	AMOUT_s = s;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:strncpy:AMSTB_strncpy:inout:::] */
/*    strncpy => Stub */
char *AMSTB_strncpy(char *s1, const char *s2,unsigned long n)
{
	static char *volatile AMIN_return;
	static const char *volatile AMOUT_s1;
	static unsigned char *volatile AMOUT_s2;
	static unsigned long volatile AMOUT_n;
	AMOUT_s1 = s1;
	AMOUT_s2 = s2;
	AMOUT_n = n;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:malloc:AMSTB_malloc:inout:::] */
/*    malloc => Stub */
void *AMSTB_malloc(unsigned long size)
{
#define AMCALLMAX_malloc 30

	static void *volatile AMIN_return[AMCALLMAX_malloc];
	static unsigned long volatile AMOUT_size;
	static int volatile AM_count;

	AMOUT_size = size;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:memset:AMSTB_memset:inout:::] */
/*    memset => Stub */
void *AMSTB_memset(void *s, int c, unsigned long n)
{
	static void *volatile AMIN_return;
	static void *volatile AMOUT_s;
	static int volatile AMOUT_c;
	static unsigned long volatile AMOUT_n;
	AMOUT_s = s;
	AMOUT_c = c;
	AMOUT_n = n;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:strncmp:AMSTB_strncmp:inout:::] */
/*    strncmp => Stub */
int AMSTB_strncmp(const char *s1, const char *s2, unsigned long n)
{
#define AMCALLMAX_strncmp 30
	static int volatile AMIN_return[AMCALLMAX_strncmp];
	static const char *volatile AMOUT_s1;
	static const char *volatile AMOUT_s2;
	static unsigned long volatile AMOUT_n;
	static int volatile AM_count;

	AMOUT_s1 = s1;
	AMOUT_s2 = s2;
	AMOUT_n = n;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:strcmp:AMSTB_strcmp:inout:::] */
/*    strcmp => Stub */
int AMSTB_strcmp(const char *s1, const char *s2)
{
	static int volatile AMIN_return;
	static const char *volatile AMOUT_s1;
	static const char *volatile AMOUT_s2;
	AMOUT_s1 = s1;
	AMOUT_s2 = s2;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:strstr:AMSTB_strstr:inout:::] */
/*    strcmp => Stub */
int AMSTB_strstr(const char *s1, const char *s2)
{
#define AMCALLMAX_strstr 30
    static int volatile AMIN_return[AMCALLMAX_strstr];
    static const char *volatile AMOUT_s1;
    static const char *volatile AMOUT_s2;
    AMOUT_s1 = s1;
    AMOUT_s2 = s2;
    static int volatile AM_count;

    AM_count++;
    return AMIN_return[AM_count - 1];
}


/* WINAMS_STUB[stub.c:free:AMSTB_free:inout:::] */
/*    free => Stub */
void AMSTB_free(void *ptr)
{
	static void *volatile AMOUT_ptr;
	AMOUT_ptr = ptr;
}

/* WINAMS_STUB[stub.c:memcpy:AMSTB_memcpy:inout:::] */
/*    memcpy => Stub */
void *AMSTB_memcpy(void *dst, const void *src, unsigned long nbytes)
{
	static void *volatile AMIN_return;
	static void *volatile AMOUT_dst;
	static const void *volatile AMOUT_src;
	static unsigned long volatile AMOUT_nbytes;
	AMOUT_dst = dst;
	AMOUT_src = src;
	AMOUT_nbytes = nbytes;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_Deinitialize_pre:AMSTB_R_OSAL_Internal_Deinitialize_pre:inout:::] */
/*    R_OSAL_Internal_Deinitialize_pre => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_Deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_CheckTimestamp:AMSTB_R_OSAL_Internal_CheckTimestamp:inout:::] */
/*    R_OSAL_Internal_CheckTimestamp => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_CheckTimestamp(const st_osal_time_t *const p_time_stamp)
{
	static e_osal_return_t volatile AMIN_return;
	static const st_osal_time_t const *volatile AMOUT_p_time_stamp;
	AMOUT_p_time_stamp = p_time_stamp;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_GetPointerToClockInfo:AMSTB_R_OSAL_Internal_GetPointerToClockInfo:inout:::] */
/*    R_OSAL_Internal_GetPointerToClockInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_GetPointerToClockInfo(struct st_osal_clock_info **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static int volatile AM_count;
	static struct st_osal_clock_info **volatile AMOUT_pp_info;

//	if (1 == AM_count)
//	{
//		g_st_osal_clock_info_p = NULL;
//	}
//	else
//	{
//		g_st_osal_clock_info_p = &g_st_osal_clock_info;
//	}
//
//	*pp_info = g_st_osal_clock_info_p;
	AMOUT_pp_info = pp_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_GetTimestamp:AMSTB_R_OSAL_Internal_GetTimestamp:inout:::] */
/*    R_OSAL_Internal_GetTimestamp => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_GetTimestamp(struct timespec *const p_time_stamp)
{
	static e_osal_return_t volatile AMIN_return;
	static struct timespec const *volatile AMOUT_p_time_stamp;
	static long input_nsec;
	p_time_stamp->tv_nsec = input_nsec;
	AMOUT_p_time_stamp = p_time_stamp;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MsSleep:AMSTB_R_OSAL_Internal_MsSleep:inout:::] */
/*    R_OSAL_Internal_MsSleep => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_MsSleep(osal_milli_sec_t milliseconds)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_milli_sec_t volatile AMOUT_milliseconds;
	static int volatile AM_interrupt;
	AMOUT_milliseconds = milliseconds;

    if (1 == AM_interrupt)
    {
		g_st_osal_interrupt_thread_control.thread_active = true;
		g_st_osal_interrupt_thread_control.usr_config = &g_st_osal_interrupt_thread_config;
        g_st_osal_interrupt_thread_control_p = &g_st_osal_interrupt_thread_control;
    }
	AM_interrupt = 0;

	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_TimedlockMutex:AMSTB_R_OSAL_Internal_TimedlockMutex:inout:::] */
/*    R_OSAL_Internal_TimedlockMutex => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_TimedlockMutex(pthread_mutex_t *p_mutex, osal_milli_sec_t timeout)
{
#define AMCALLMAX_osal_internal_timedlockmutex 30
	static e_osal_return_t volatile AMIN_return[AMCALLMAX_osal_internal_timedlockmutex];
	static pthread_mutex_t *volatile AMOUT_p_mutex;
	static osal_milli_sec_t volatile AMOUT_timeout;
	static int volatile AM_count;
	static int volatile AM_control;
	static int volatile AM_init_ope;
	static int volatile AM_interrupt;
	AMOUT_p_mutex = p_mutex;
	AMOUT_timeout = timeout;
    int i;
    
    if (AM_control == 1) /* Special case. Buffer fill for check buffer */
    {
        for (i = 0; i < OSAL_MMNGR_GUARD_PAGE_SIZE; i++)
        {
            mmngr_buffer[i] = OSAL_MMNGR_GUARD_PAGE_PATTERN;
            mmngr_buffer[OSAL_MMNGR_GUARD_PAGE_SIZE + 64 + i] = OSAL_MMNGR_GUARD_PAGE_PATTERN;
        }
    }
    else if (AM_control == 2) /* Special case. Buffer clear for check buffer */
    {
        for (i = 0; i < OSAL_MMNGR_GUARD_PAGE_SIZE; i++)
        {
            mmngr_buffer[i] = 0;
            mmngr_buffer[OSAL_MMNGR_GUARD_PAGE_SIZE + 64 + i] = 0;
        }
    }
    AM_control = 0;
    

	/* r_osal_thread.c : gs_osal_thread_is_initialized operation */
	if (1 == AM_init_ope)
	{
		gs_osal_thread_is_initialized_dummy[0] = false;
	}
	else if (2 == AM_init_ope)
	{
		gs_osal_threadsync_is_initialized_dummy[0] = false;
	}
	else if (3 == AM_init_ope)
	{
		if (NULL != gp_osal_io_is_initialized_dummy)
		{
			*gp_osal_io_is_initialized_dummy = false;
		}
	}
	else
	{
		/* Do nothing */
	}
	
	if (1 == AM_interrupt)
	{
		for (int i = 0; i < AMCALLMAX_osal_internal_timedlockmutex; i++)
		{
			AMIN_return[i] = AMIN_return[0];
			AM_count++;
		}
	}
	else
	{
		AM_count++;
	}

	AM_interrupt = 0;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_TimeRoundUp:AMSTB_R_OSAL_Internal_TimeRoundUp:inout:::] */
/*    R_OSAL_Internal_TimeRoundUp => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_TimeRoundUp(struct timespec *p_timespec)
{
	static e_osal_return_t volatile AMIN_return;
	static struct timespec *volatile AMOUT_p_timespec;
	AMOUT_p_timespec = p_timespec;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_TimespecAdd:AMSTB_R_OSAL_Internal_TimespecAdd:inout:::] */
/*    R_OSAL_Internal_TimespecAdd => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_TimespecAdd(struct timespec *p_timespec, osal_milli_sec_t *p_time_period)
{
	static e_osal_return_t volatile AMIN_return;
	static struct timespec *volatile AMOUT_p_timespec;
	static osal_milli_sec_t *volatile AMOUT_p_time_period;
	AMOUT_p_timespec = p_timespec;
	AMOUT_p_time_period = p_time_period;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_CreateThreadKey:AMSTB_R_OSAL_Internal_CreateThreadKey:inout:::] */
/*    R_OSAL_Internal_CreateThreadKey => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_CreateThreadKey(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_DeleteThreadKey:AMSTB_R_OSAL_Internal_DeleteThreadKey:inout:::] */
/*    R_OSAL_Internal_DeleteThreadKey => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_DeleteThreadKey(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_GetThreadSpecific:AMSTB_R_OSAL_Internal_GetThreadSpecific:inout:::] */
/*    R_OSAL_Internal_GetThreadSpecific => Stub */
void *AMSTB_R_OSAL_Internal_GetThreadSpecific(void)
{
	static void *volatile AMIN_return;
	static int volatile AM_num;
	if (1 == AM_num)
	{
		g_get_value = 192;
		g_get_value_p = &g_get_value;
		AMIN_return = g_get_value_p;
	}
	if (2 == AM_num)
	{
		g_get_value = 1;
		g_get_value_p = &g_get_value;
		AMIN_return = g_get_value_p;
	}
	if (3 == AM_num)
	{
		g_get_value_p = NULL;
		AMIN_return = g_get_value_p;
	}
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_SetThreadSpecific:AMSTB_R_OSAL_Internal_SetThreadSpecific:inout:::] */
/*    R_OSAL_Internal_SetThreadSpecific => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_SetThreadSpecific(void *p_set_value)
{
	static e_osal_return_t volatile AMIN_return;
	static void *volatile AMOUT_p_set_value;
	AMOUT_p_set_value = p_set_value;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_GetPmaHandle:AMSTB_R_OSAL_Internal_GetPmaHandle:inout:::] */
/*    R_OSAL_Internal_GetPmaHandle => Stub */
st_pma_handle_t * AMSTB_R_OSAL_Internal_GetPmaHandle(void)
{
	static st_pma_handle_t * volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:clock_getres:AMSTB_clock_getres:inout:::] */
/*    clock_getres => Stub */
int AMSTB_clock_getres(clockid_t clockid, struct timespec *res)
{
	static int volatile AMIN_return;
	static clockid_t volatile AMOUT_clockid;
	static struct timespec *volatile AMOUT_res;
	static long input_nsec;
	AMOUT_clockid = clockid;
	res->tv_nsec = input_nsec;
	AMOUT_res = res;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_CheckUser:AMSTB_R_PMA_IF_CheckUser:inout:::] */
/*    R_PMA_IF_CheckUser => Stub */
e_pma_return_t AMSTB_R_PMA_IF_CheckUser(const st_pma_user_info_t *pUserInfo)
{
	static e_pma_return_t volatile AMIN_return;
	static const st_pma_user_info_t *volatile AMOUT_pUserInfo;
	AMOUT_pUserInfo = pUserInfo;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_SendMessageToTask:AMSTB_R_PMA_IF_SendMessageToTask:inout:::] */
/*    R_PMA_IF_SendMessageToTask => Stub */
//e_result_t AMSTB_R_PMA_IF_SendMessageToTask(st_msg_to_task_t *pMsg)
//{
//	static e_result_t volatile AMIN_return;
//	static struct st_osal_mq_control *volatile AMOUT_R_PMA_SM_GxTaskQueue;
//	static st_msg_to_task_t *volatile AMOUT_pMsg;
//	AMOUT_R_PMA_SM_GxTaskQueue = R_PMA_SM_GxTaskQueue;
//	AMOUT_pMsg = pMsg;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_CheckConfigParam:AMSTB_R_PMA_IF_CheckConfigParam:inout:::] */
/*    R_PMA_IF_CheckConfigParam => Stub */
e_pma_return_t AMSTB_R_PMA_IF_CheckConfigParam(const st_pma_config_t *pConfig,st_pma_param_t *pParam)
{
	static e_pma_return_t volatile AMIN_return;
	static const st_pma_config_t *volatile AMOUT_pConfig;
	static st_pma_param_t *volatile AMOUT_pParam;
	AMOUT_pConfig = pConfig;
	AMOUT_pParam = pParam;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_DefaultInit:AMSTB_R_PMA_IF_DefaultInit:inout:::] */
/*    R_PMA_IF_DefaultInit => Stub */
e_pma_return_t AMSTB_R_PMA_IF_DefaultInit(const st_pma_param_t *pParam)
{
	static e_pma_return_t volatile AMIN_return;
//	static struct st_osal_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
	static const st_pma_param_t *volatile AMOUT_pParam;
//	AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
//	AMOUT_pParam = pParam;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_UnlockHwa:AMSTB_R_PMA_UnlockHwa:inout::array:counter<AM_count>] */
/*    R_PMA_UnlockHwa => Stub */
e_pma_return_t AMSTB_R_PMA_UnlockHwa(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId)
{
#define AMCALLMAX_R_PMA_UnlockHwa 33
	static e_pma_return_t volatile AMIN_return[AMCALLMAX_R_PMA_UnlockHwa];
	static struct st_pma_user_info_t *volatile AMOUT_handle[AMCALLMAX_R_PMA_UnlockHwa];
	static e_pma_hwa_id_t volatile AMOUT_hwaId[AMCALLMAX_R_PMA_UnlockHwa];
	static int volatile AM_count;
	AMOUT_handle[AM_count] = handle;
	AMOUT_hwaId[AM_count] = hwaId;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_GetUserInfo:AMSTB_R_PMA_IF_GetUserInfo:inout::array:counter<AM_count>] */
/*    R_PMA_IF_GetUserInfo => Stub */
st_pma_user_info_t * AMSTB_R_PMA_IF_GetUserInfo(unsigned int userNo)
{
#define AMCALLMAX_R_PMA_IF_GetUserInfo 16
	static st_pma_user_info_t * volatile AMIN_return[AMCALLMAX_R_PMA_IF_GetUserInfo];
	static unsigned int volatile AMOUT_userNo[AMCALLMAX_R_PMA_IF_GetUserInfo];
	static int volatile AM_count;
	AMOUT_userNo[AM_count] = userNo;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetLockHwa:AMSTB_R_PMA_SM_GetLockHwa:inout:::] */
/*    R_PMA_SM_GetLockHwa => Stub */
//e_result_t AMSTB_R_PMA_SM_GetLockHwa(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_UnlockHwa:AMSTB_R_PMA_SM_UnlockHwa:inout:::] */
/*    R_PMA_SM_UnlockHwa => Stub */
//e_result_t AMSTB_R_PMA_SM_UnlockHwa(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_if.c:R_PMA_ReleaseHwa:AMSTB_R_PMA_ReleaseHwa:inout:::] */
/*    R_PMA_ReleaseHwa => Stub */
e_pma_return_t AMSTB_R_PMA_ReleaseHwa(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId)
{
	static e_pma_return_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static struct st_pma_user_info_t *volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetHwaState:AMSTB_R_PMA_SM_GetHwaState:inout:::] */
/*    R_PMA_SM_GetHwaState => Stub */
//e_result_t AMSTB_R_PMA_SM_GetHwaState(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,st_pma_hwa_state_t *pHwaState)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static const int *volatile AMOUT_R_PMA_SYSC_PDR_ID_TABLE;
//	static const int *volatile AMOUT_R_PMA_CPG_MOD_ID_TABLE;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static st_pma_hwa_state_t volatile AMOUT_pHwaState;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_R_PMA_SYSC_PDR_ID_TABLE = R_PMA_SYSC_PDR_ID_TABLE;
//	AMOUT_R_PMA_CPG_MOD_ID_TABLE = R_PMA_CPG_MOD_ID_TABLE;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	pHwaState->enActualClkStat = AMOUT_pHwaState.enActualClkStat;
//	pHwaState->enActualPwrStat = AMOUT_pHwaState.enActualPwrStat;
//	*pHwaState = AMOUT_pHwaState;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_osal_timeclock.c:R_OSAL_ClockTimeGetTimeStamp:AMSTB_R_OSAL_ClockTimeGetTimeStamp:inout:::] */
/*    R_OSAL_ClockTimeGetTimeStamp => Stub */
int AMSTB_R_OSAL_ClockTimeGetTimeStamp(int clock_type,struct st_osal_time *p_time_stamp)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_clock_type;
	static struct st_osal_time *volatile AMOUT_p_time_stamp;
	AMOUT_clock_type = clock_type;
	AMOUT_p_time_stamp = p_time_stamp;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_GetCurrentState:AMSTB_R_PMA_IF_GetCurrentState:inout::array:counter<AM_count>] */
/*    R_PMA_IF_GetCurrentState => Stub */
//e_result_t AMSTB_R_PMA_IF_GetCurrentState(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,const st_pma_hwa_state_t *pHwaState,st_pma_hwa_state_t *pCurHwaState,e_pma_reset_req_t *pCurRstStat)
//{
//#define AMCALLMAX_R_PMA_IF_GetCurrentState 2
//	static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static struct st_osal_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static struct st_pma_user_info_t *volatile AMOUT_handle[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static e_pma_hwa_id_t volatile AMOUT_hwaId[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static const st_pma_hwa_state_t *volatile AMOUT_pHwaState[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static st_pma_hwa_state_t volatile AMOUT_pCurHwaState[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static e_pma_reset_req_t volatile AMOUT_pCurRstStat[AMCALLMAX_R_PMA_IF_GetCurrentState];
//	static int volatile AM_count;
//	AMOUT_R_PMA_SM_GxMutex[AM_count] = R_PMA_SM_GxMutex;
//	AMOUT_handle[AM_count] = handle;
//	AMOUT_hwaId[AM_count] = hwaId;
//	AMOUT_pHwaState[AM_count] = pHwaState;
//	pCurHwaState->enReqState = AMOUT_pCurHwaState[AM_count].enReqState;
//	pCurHwaState->enActualPwrStat = AMOUT_pCurHwaState[AM_count].enActualPwrStat;
//	pCurHwaState->enActualClkStat = AMOUT_pCurHwaState[AM_count].enActualClkStat;
//	*pCurRstStat = AMOUT_pCurRstStat[AM_count];
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}
/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_GetTimeDiffAndSleep:AMSTB_R_PMA_IF_GetTimeDiffAndSleep:inout::array:counter<AM_count>] */
/*    R_PMA_IF_GetTimeDiffAndSleep => Stub */
int AMSTB_R_PMA_IF_GetTimeDiffAndSleep(struct st_osal_time *LstStartTime,long long *pDiffTime,long long sleepTimePeriod)
{
#define AMCALLMAX_R_PMA_IF_GetTimeDiffAndSleep 2
	static int volatile AMIN_return[AMCALLMAX_R_PMA_IF_GetTimeDiffAndSleep];
	static struct st_osal_time *volatile AMOUT_LstStartTime[AMCALLMAX_R_PMA_IF_GetTimeDiffAndSleep];
	static long long *volatile AMOUT_pDiffTime[AMCALLMAX_R_PMA_IF_GetTimeDiffAndSleep];
	static long long volatile AMOUT_sleepTimePeriod[AMCALLMAX_R_PMA_IF_GetTimeDiffAndSleep];
	static int volatile AM_count;
	AMOUT_LstStartTime[AM_count] = LstStartTime;
//	AMOUT_pDiffTime[AM_count] = pDiffTime;
	*pDiffTime = AMOUT_pDiffTime[AM_count];
	AMOUT_sleepTimePeriod[AM_count] = sleepTimePeriod;
	AM_count++;

	return AMIN_return[AM_count - 1];
}
/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetResetState:AMSTB_R_PMA_SM_GetResetState:inout:::] */
/*    R_PMA_SM_GetResetState => Stub */
//e_result_t AMSTB_R_PMA_SM_GetResetState(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstState)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static e_pma_reset_req_t *volatile AMOUT_pRstState;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	AMOUT_pRstState = pRstState;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetLowestPowerState:AMSTB_R_PMA_SM_GetLowestPowerState:inout:::] */
/*    R_PMA_SM_GetLowestPowerState => Stub */
//e_result_t AMSTB_R_PMA_SM_GetLowestPowerState(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,e_pma_power_state_t *pLowestPower)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static e_pma_power_state_t *volatile AMOUT_pLowestPower;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	AMOUT_pLowestPower = pLowestPower;
//
//    return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetPostClock:AMSTB_R_PMA_SM_SetPostClock:inout:::] */
/*    R_PMA_SM_SetPostClock => Stub */
//e_result_t AMSTB_R_PMA_SM_SetPostClock(e_pma_clk_req_t clockEnable,unsigned int chanIdx,unsigned int *pClockDivider)
//{
//	static e_result_t volatile AMIN_return;
//	static struct st_osal_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
//	static e_pma_clk_req_t volatile AMOUT_clockEnable;
//	static unsigned int volatile AMOUT_chanIdx;
//	static unsigned int *volatile AMOUT_pClockDivider;
//	AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
//	AMOUT_clockEnable = clockEnable;
//	AMOUT_chanIdx = chanIdx;
//	AMOUT_pClockDivider = pClockDivider;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_osal_threadsync.c:R_OSAL_ThsyncMutexCreate:AMSTB_R_OSAL_ThsyncMutexCreate:inout:::] */
/*    R_OSAL_ThsyncMutexCreate => Stub */
int AMSTB_R_OSAL_ThsyncMutexCreate(unsigned long long mutex_id,struct st_osal_mutex_control **p_handle)
{
	static int volatile AMIN_return;
	static unsigned long long volatile AMOUT_mutex_id;
//	static struct st_osal_mutex_control **volatile AMOUT_p_handle;
	AMOUT_mutex_id = mutex_id;
//	*p_handle = AMOUT_p_handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_Init:AMSTB_R_PMA_SM_Init:inout:::] */
/*    R_PMA_SM_Init => Stub */
//e_result_t AMSTB_R_PMA_SM_Init(struct st_osal_mq_control **pTaskQueue,struct st_osal_thread_control **pTskHandle,struct st_osal_mutex_control *xMutex,const st_pma_param_t *pParam)
//{
//	static unsigned long long volatile AMIN_pma_soc_type;
//	static e_pma_hwa_id_t volatile AMIN_hwa_idmax;
//	static e_sysc_domain_id_t volatile AMIN_sysc_idmax;
//	static e_cpg_domain_id_t volatile AMIN_cpg_idmax;
//	static e_pma_hwa_id_t volatile AMIN_hwa_idmin;
//	static e_sysc_domain_id_t volatile AMIN_sysc_idmin;
//	static e_cpg_domain_id_t volatile AMIN_cpg_idmin;
//	static e_result_t volatile AMIN_return;
//	static unsigned long long volatile AMOUT_pma_soc_type;
//	static struct st_osal_mq_control **volatile AMOUT_pTaskQueue;
//	static struct st_osal_thread_control **volatile AMOUT_pTskHandle;
//	static struct st_osal_mutex_control *volatile AMOUT_xMutex;
//	static const st_pma_param_t *volatile AMOUT_pParam;
//	AMOUT_pma_soc_type = pma_soc_type;
//	pma_soc_type = AMIN_pma_soc_type;
//	hwa_idmax = AMIN_hwa_idmax;
//	sysc_idmax = AMIN_sysc_idmax;
//	cpg_idmax = AMIN_cpg_idmax;
//	hwa_idmin = AMIN_hwa_idmin;
//	sysc_idmin = AMIN_sysc_idmin;
//	cpg_idmin = AMIN_cpg_idmin;
//	AMOUT_pTaskQueue = pTaskQueue;
//	AMOUT_pTskHandle = pTskHandle;
//	AMOUT_xMutex = xMutex;
//	AMOUT_pParam = pParam;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetPowerPolicy:AMSTB_R_PMA_SM_GetPowerPolicy:inout:::] */
/*    R_PMA_SM_GetPowerPolicy => Stub */
//e_result_t AMSTB_R_PMA_SM_GetPowerPolicy(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,e_pma_power_policy_t *pPolicy)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static e_pma_power_policy_t *volatile AMOUT_pPolicy;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	AMOUT_pPolicy = pPolicy;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetPowerPolicy:AMSTB_R_PMA_SM_SetPowerPolicy:inout:::] */
/*    R_PMA_SM_SetPowerPolicy => Stub */
//e_result_t AMSTB_R_PMA_SM_SetPowerPolicy(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,e_pma_power_policy_t policy)
//{
//	static e_result_t volatile AMIN_return;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static e_pma_power_policy_t volatile AMOUT_policy;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	AMOUT_policy = policy;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_osal_timeclock.c:R_OSAL_ClockTimeCalculateTimeDifference:AMSTB_R_OSAL_ClockTimeCalculateTimeDifference:inout:::] */
/*    R_OSAL_ClockTimeCalculateTimeDifference => Stub */
int AMSTB_R_OSAL_ClockTimeCalculateTimeDifference(struct st_osal_time *p_time2,struct st_osal_time *p_time1,long long *p_time_difference)
{
	static int volatile AMIN_return;
	static struct st_osal_time *volatile AMOUT_p_time2;
	static struct st_osal_time *volatile AMOUT_p_time1;
	static long long *volatile AMOUT_p_time_difference;
	AMOUT_p_time2 = p_time2;
	AMOUT_p_time1 = p_time1;
	AMOUT_p_time_difference = p_time_difference;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetThreadInfo:AMSTB_R_OSAL_RCFG_GetThreadInfo:inout:::] */
/*    R_OSAL_RCFG_GetThreadInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_GetThreadInfo(uint32_t num, st_osal_thread_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static uint32_t volatile AMOUT_num;
	static st_osal_thread_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;

	if (1 == AM_count)
	{
		g_thread_control_p = NULL;
	}
	else if (2 == AM_count)
	{
		g_thread_control.usr_config = &g_st_osal_rcfg_thread_config;
		g_thread_control_p = &g_thread_control;
	}
	else if (3 == AM_count)
	{
		g_thread_control.usr_config = NULL;
		g_thread_control_p = &g_thread_control;
	}
	else
	{
		g_thread_control_p = &g_thread_control;
	}
	
	*pp_info = g_thread_control_p;
	AMOUT_num = num;
	AMOUT_pp_info = pp_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetNumOfMq:AMSTB_R_OSAL_RCFG_GetNumOfMq:inout:::] */
/*    R_OSAL_RCFG_GetNumOfMq => Stub */
size_t AMSTB_R_OSAL_RCFG_GetNumOfMq(void)
{
	static size_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetNumOfInterruptThread:AMSTB_R_OSAL_RCFG_GetNumOfInterruptThread:inout:::] */
/*    R_OSAL_RCFG_GetNumOfInterruptThread => Stub */
size_t AMSTB_R_OSAL_RCFG_GetNumOfInterruptThread(void)
{
	static size_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetCondInfo:AMSTB_R_OSAL_RCFG_GetCondInfo:inout:::] */
/*    R_OSAL_RCFG_GetCondInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_GetCondInfo(uint32_t num, st_osal_cond_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static uint32_t volatile AMOUT_num;
	static st_osal_cond_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;
	static osal_cond_id_t input_cond_id;

	if (1 == AM_count)
	{
		g_st_osal_cond_control_p = NULL;
	}
	else if (2 == AM_count)
	{
		g_st_osal_cond_control.active = false;
		g_st_osal_rcfg_cond_config.id = 1;
		g_st_osal_cond_control.usr_config = &g_st_osal_rcfg_cond_config;
		g_st_osal_cond_control_p = &g_st_osal_cond_control;
	}
	else if (3 == AM_count)
	{
		g_st_osal_cond_control.usr_config = NULL;
		g_st_osal_cond_control_p = &g_st_osal_cond_control;
	}
	else if (4 == AM_count)
	{
		g_st_osal_cond_control.active = true;
		g_st_osal_cond_control.usr_config = &g_st_osal_rcfg_cond_config;
		g_st_osal_cond_control_p = &g_st_osal_cond_control;
	}
	else if (5 == AM_count)
	{
		g_st_osal_cond_control.active = false;
		g_st_osal_rcfg_cond_config.id = 2;
		g_st_osal_cond_control.usr_config = &g_st_osal_rcfg_cond_config;
		g_st_osal_cond_control_p = &g_st_osal_cond_control;
	}
	else
	{
		g_st_osal_cond_control_p = &g_st_osal_cond_control;
	}

	*pp_info = g_st_osal_cond_control_p;

	AMOUT_num = num;
	AMOUT_pp_info = pp_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetNumOfMutex:AMSTB_R_OSAL_RCFG_GetNumOfMutex:inout:::] */
/*    R_OSAL_RCFG_GetNumOfMutex => Stub */
size_t AMSTB_R_OSAL_RCFG_GetNumOfMutex(void)
{
	static size_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetMutexInfo:AMSTB_R_OSAL_RCFG_GetMutexInfo:inout:::] */
/*    R_OSAL_RCFG_GetMutexInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_GetMutexInfo(uint32_t num, st_osal_mutex_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static uint32_t volatile AMOUT_num;
	static st_osal_mutex_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;
	static osal_mutex_id_t input_mutex_id;

	if (1 == AM_count)
	{
		g_st_osal_mutex_control_p = NULL;
	}
	else if (2 == AM_count)
	{
		g_st_osal_mutex_control.active = false;
		g_st_osal_rcfg_mutex_config.id = 1;
		g_st_osal_mutex_control.usr_config = &g_st_osal_rcfg_mutex_config;
		g_st_osal_mutex_control_p          = &g_st_osal_mutex_control;
	}
	else if (3 == AM_count)
	{
		g_st_osal_mutex_control.usr_config = NULL;
		g_st_osal_mutex_control_p          = &g_st_osal_mutex_control;
	}
	else if (4 == AM_count)
	{
		g_st_osal_mutex_control.active = true;
		g_st_osal_mutex_control.usr_config = &g_st_osal_rcfg_mutex_config;
		g_st_osal_mutex_control_p = &g_st_osal_mutex_control;
	}
	else if (5 == AM_count)
	{
		g_st_osal_mutex_control.active = false;
		g_st_osal_rcfg_mutex_config.id = 2;
		g_st_osal_mutex_control.usr_config = &g_st_osal_rcfg_mutex_config;
		g_st_osal_mutex_control_p = &g_st_osal_mutex_control;
	}
	else
	{
		g_st_osal_mutex_control_p = &g_st_osal_mutex_control;
	}

	*pp_info = g_st_osal_mutex_control_p;

	AMOUT_num = num;
	AMOUT_pp_info = pp_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetNumOfThread:AMSTB_R_OSAL_RCFG_GetNumOfThread:inout:::] */
/*    R_OSAL_RCFG_GetNumOfThread => Stub */
size_t AMSTB_R_OSAL_RCFG_GetNumOfThread(void)
{
	static size_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetNumOfCond:AMSTB_R_OSAL_RCFG_GetNumOfCond:inout:::] */
/*    R_OSAL_RCFG_GetNumOfCond => Stub */
size_t AMSTB_R_OSAL_RCFG_GetNumOfCond(void)
{
	static size_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetMqInfo:AMSTB_R_OSAL_RCFG_GetMqInfo:inout:::] */
/*    R_OSAL_RCFG_GetMqInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_GetMqInfo(uint32_t num, st_osal_mq_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static uint32_t volatile AMOUT_num;
	static st_osal_mq_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;
	
	if (1 == AM_count)
	{
		g_st_osal_mq_control_p = NULL;
	}
	else if (2 == AM_count)
	{
		g_st_osal_rcfg_mq_config.id = 0;
		g_st_osal_rcfg_mq_config.config = g_st_osal_mq_config;
		g_st_osal_mq_control.usr_config = &g_st_osal_rcfg_mq_config;
		g_st_osal_mq_control.queue      = &g_st_osal_queue;
		g_st_osal_mq_control_p          = &g_st_osal_mq_control;
	}
	else if (3 == AM_count)
	{
		g_st_osal_mq_control.usr_config = NULL;
		g_st_osal_mq_control_p = &g_st_osal_mq_control;
	}
	else if (4 == AM_count)
	{
		g_st_osal_rcfg_mq_config.id = 0;
		g_st_osal_rcfg_mq_config.config = g_st_osal_mq_config;
		g_st_osal_mq_control.usr_config = &g_st_osal_rcfg_mq_config;
		g_st_osal_mq_control.queue      = NULL;
		g_st_osal_mq_control_p          = &g_st_osal_mq_control;
	}
	else
	{
		g_st_osal_mq_control_p = &g_st_osal_mq_control;
	}

	*pp_info = g_st_osal_mq_control_p;
	
	AMOUT_num = num;
	AMOUT_pp_info = pp_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetInterruptThreadInfo:AMSTB_R_OSAL_RCFG_GetInterruptThreadInfo:inout:::] */
/*    R_OSAL_RCFG_GetInterruptThreadInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_GetInterruptThreadInfo(uint32_t num, st_osal_interrupt_thread_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static uint32_t volatile AMOUT_num;
	static st_osal_interrupt_thread_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;
	static int volatile AM_interrupt;
	AMOUT_num = num;

    if (1 == AM_count)
    {
        g_st_osal_interrupt_thread_control_p = NULL;
    }
    else if (2 == AM_count)
    {
		g_st_osal_interrupt_thread_control.usr_config = NULL;
		g_st_osal_interrupt_thread_control_p = &g_st_osal_interrupt_thread_control;
	}
    else
    {
		g_st_osal_interrupt_thread_control.irq_channel = 192;
		g_st_osal_interrupt_thread_control.usr_config = &g_st_osal_interrupt_thread_config;
        g_st_osal_interrupt_thread_control_p = &g_st_osal_interrupt_thread_control;
    }

    if (1 == AM_interrupt)
    {
		g_st_osal_interrupt_thread_control.thread_active = true;
		g_st_osal_interrupt_thread_control.usr_config = &g_st_osal_interrupt_thread_config;
        g_st_osal_interrupt_thread_control_p = &g_st_osal_interrupt_thread_control;
    }
    if (2 == AM_interrupt)
    {
		g_st_osal_interrupt_thread_control.thread_active = false;
		g_st_osal_interrupt_thread_control.usr_config = &g_st_osal_interrupt_thread_config;
        g_st_osal_interrupt_thread_control_p = &g_st_osal_interrupt_thread_control;
    }
	AM_interrupt = 0;

	*pp_info = g_st_osal_interrupt_thread_control_p;

    AMOUT_pp_info = pp_info;
	return AMIN_return;
}

e_osal_return_t AMSTB_osal_internal_compare_id(const char *p_str1, const char *p_str2, size_t len)
{
	static e_osal_return_t volatile AMIN_return;
	static const char *volatile AMOUT_p_str1;
	static const char *volatile AMOUT_p_str2;
	static size_t volatile AMOUT_len;
	AMOUT_p_str1 = p_str1;
	AMOUT_p_str2 = p_str2;
	AMOUT_len = len;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetNumOfDevice:AMSTB_R_OSAL_DCFG_GetNumOfDevice:inout:::] */
/*    R_OSAL_DCFG_GetNumOfDevice => Stub */
size_t AMSTB_R_OSAL_DCFG_GetNumOfDevice(void)
{
	static size_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_Init:AMSTB_R_OSAL_DCFG_Init:inout:::] */
/*    R_OSAL_DCFG_Init => Stub */
void AMSTB_R_OSAL_DCFG_Init(void)
{
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetPMAConfiguration:AMSTB_R_OSAL_DCFG_GetPMAConfiguration:inout:::] */
/*    R_OSAL_DCFG_GetPMAConfiguration => Stub */
void AMSTB_R_OSAL_DCFG_GetPMAConfiguration(st_pma_config_t *const p_config)
{
	static st_pma_config_t *const volatile AMOUT_p_config;
	*AMOUT_p_config = *p_config;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetAxiBusInfo:AMSTB_R_OSAL_DCFG_GetAxiBusInfo:inout:::] */
/*    R_OSAL_DCFG_GetAxiBusInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_GetAxiBusInfo(st_osal_axi_bus_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_axi_bus_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;
	AMOUT_pp_info = pp_info;

    if (0 == AM_count)
    {
        g_st_osal_axi_bus_control_p = &g_st_osal_axi_bus_control;
        g_st_osal_axi_bus_control.axi_bus_list = g_axi_bus_list;
    }
    else if (1 == AM_count)
    {
        g_st_osal_axi_bus_control_p = NULL;
    }
    else if (2 == AM_count)
    {
        g_st_osal_axi_bus_control_p = &g_st_osal_axi_bus_control;
        g_st_osal_axi_bus_control.axi_bus_list = NULL;
    }
    else
    {
        g_st_osal_axi_bus_control_p = &g_st_osal_axi_bus_control;
        g_st_osal_axi_bus_control.axi_bus_list = g_axi_bus_list;
    }

    *pp_info = g_st_osal_axi_bus_control_p;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetSocType:AMSTB_R_OSAL_DCFG_GetSocType:inout:::] */
/*    R_OSAL_DCFG_GetSocType => Stub */
uint64_t AMSTB_R_OSAL_DCFG_GetSocType(void)
{
	static uint64_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetMaxConfigOfMemory:AMSTB_R_OSAL_DCFG_GetMaxConfigOfMemory:inout:::] */
/*    R_OSAL_DCFG_GetMaxConfigOfMemory => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_GetMaxConfigOfMemory(st_osal_mmngr_config_t * const p_config, osal_memory_region_idx_t region_idx)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mmngr_config_t * volatile AMOUT_p_config;
	// *p_config = g_st_osal_mmngr_config;
	AMOUT_p_config = p_config;

    if (NULL != p_config)
    {
        p_config->mode                        = g_osal_memory_max_configration.mode;
        p_config->memory_limit                = g_osal_memory_max_configration.memory_limit;
        p_config->max_allowed_allocations     = g_osal_memory_max_configration.max_allowed_allocations;
        p_config->max_registered_monitors_cbs = g_osal_memory_max_configration.max_registered_monitors_cbs;
    }
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_Deinit:AMSTB_R_OSAL_DCFG_Deinit:inout:::] */
/*    R_OSAL_DCFG_Deinit => Stub */
void AMSTB_R_OSAL_DCFG_Deinit(void)
{
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetDeviceInfo:AMSTB_R_OSAL_DCFG_GetDeviceInfo:inout:::] */
/*    R_OSAL_DCFG_GetDeviceInfo => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_GetDeviceInfo(uint32_t num, st_osal_device_control_t **pp_info)
{
	static e_osal_return_t volatile AMIN_return;
	static uint32_t volatile AMOUT_num;
	static st_osal_device_control_t **volatile AMOUT_pp_info;
	static int volatile AM_count;
	AMOUT_num = num;
	
	if (1 == AM_count)
	{
		g_st_osal_device_control_p = NULL;
	}
	else if (2 == AM_count)
	{
		g_st_osal_device_control.dev_info = NULL;
		g_st_osal_device_control_p = &g_st_osal_device_control;
	}
	else
	{
		g_st_osal_device_control.dev_info = &g_st_osal_device_info;
		g_st_osal_device_control_p = &g_st_osal_device_control;
	}
	
	*pp_info = g_st_osal_device_control_p;
	AMOUT_pp_info = pp_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:osal_internal_io_deinitailize_pre:AMSTB_osal_internal_io_deinitailize_pre:inout:::] */
/*    osal_internal_io_deinitailize_pre => Stub */
e_osal_return_t AMSTB_osal_internal_io_deinitailize_pre(uint16_t *p_init_flg)
{
	static e_osal_return_t volatile AMIN_return;
	static uint16_t *volatile AMOUT_p_init_flg;
	AMOUT_p_init_flg = p_init_flg;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_interrupt.c:osal_interrupt_deinitialize_pre:AMSTB_osal_interrupt_deinitialize_pre:inout:::] */
/*    osal_interrupt_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_interrupt_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:osal_io_deinitialize_pre:AMSTB_osal_io_deinitialize_pre:inout:::] */
/*    osal_io_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_io_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_memory_deinitialize_pre:AMSTB_osal_memory_deinitialize_pre:inout:::] */
/*    osal_memory_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_memory_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_mq_deinitialize_pre:AMSTB_osal_mq_deinitialize_pre:inout:::] */
/*    osal_mq_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_mq_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_power.c:osal_pm_deinitialize_pre:AMSTB_osal_pm_deinitialize_pre:inout:::] */
/*    osal_pm_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_pm_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_thread.c:osal_thread_deinitialize_pre:AMSTB_osal_thread_deinitialize_pre:inout:::] */
/*    osal_thread_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_thread_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_threadsync_deinitialize_pre:AMSTB_osal_threadsync_deinitialize_pre:inout:::] */
/*    osal_threadsync_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_threadsync_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_timeclock.c:osal_clock_time_deinitialize_pre:AMSTB_osal_clock_time_deinitialize_pre:inout:::] */
/*    osal_clock_time_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_clock_time_deinitialize_pre(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_internal_mq_rcv_is_isr:AMSTB_osal_internal_mq_rcv_is_isr:inout:::] */
/*    osal_internal_mq_rcv_is_isr => Stub */
e_osal_return_t AMSTB_osal_internal_mq_rcv_is_isr(osal_mq_handle_t handle, void *p_buffer)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_mq_handle_t volatile AMOUT_handle;
	static void *volatile AMOUT_p_buffer;
	AMOUT_handle = handle;
	AMOUT_p_buffer = p_buffer;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_internal_mq_rcv_not_isr:AMSTB_osal_internal_mq_rcv_not_isr:inout:::] */
/*    osal_internal_mq_rcv_not_isr => Stub */
e_osal_return_t AMSTB_osal_internal_mq_rcv_not_isr(osal_mq_handle_t handle, struct timespec *p_time_stamp, void *p_buffer)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_mq_handle_t volatile AMOUT_handle;
	static struct timespec *volatile AMOUT_p_time_stamp;
	static void *volatile AMOUT_p_buffer;
	AMOUT_handle = handle;
	AMOUT_p_time_stamp = p_time_stamp;
	AMOUT_p_buffer = p_buffer;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_internal_mq_snd_is_isr:AMSTB_osal_internal_mq_snd_is_isr:inout:::] */
/*    osal_internal_mq_snd_is_isr => Stub */
e_osal_return_t AMSTB_osal_internal_mq_snd_is_isr(osal_mq_handle_t handle, void *p_buffer, size_t buffer_size)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_mq_handle_t volatile AMOUT_handle;
	static void *volatile AMOUT_p_buffer;
	static size_t volatile AMOUT_buffer_size;
	AMOUT_handle = handle;
	AMOUT_p_buffer = p_buffer;
	AMOUT_buffer_size = buffer_size;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:AMSTB_osal_internal_mq_snd_not_isr:AMSTB_AMSTB_osal_internal_mq_snd_not_isr:inout:::] */
/*    AMSTB_osal_internal_mq_snd_not_isr => Stub */
e_osal_return_t AMSTB_osal_internal_mq_snd_not_isr(osal_mq_handle_t handle, struct timespec *p_time_stamp, void *p_buffer, size_t buffer_size)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_mq_handle_t volatile AMOUT_handle;
	static struct timespec *volatile AMOUT_p_time_stamp;
	static void *volatile AMOUT_p_buffer;
	static size_t volatile AMOUT_buffer_size;
	AMOUT_handle = handle;
	AMOUT_p_time_stamp = p_time_stamp;
	AMOUT_p_buffer = p_buffer;
	AMOUT_buffer_size = buffer_size;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_open:AMSTB_osal_internal_mem_open:inout:::] */
/*    osal_internal_mem_open => Stub */
e_osal_return_t AMSTB_osal_internal_mem_open(const st_osal_mmngr_config_t * const p_usr_config, osal_memory_region_idx_t region_idx)
{
	static e_osal_return_t volatile AMIN_return;
	static const st_osal_mmngr_config_t * const volatile AMOUT_p_usr_config;
	// *AMOUT_p_usr_config = *p_usr_config;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_open_map:AMSTB_osal_internal_mem_open_map:inout:::] */
/*    osal_internal_mem_open_map => Stub */
e_osal_return_t AMSTB_osal_internal_mem_open_map(st_osal_mem_mmngr_ptr_self_t *p_mngr_control, const st_osal_mmngr_config_t * const p_usr_config, osal_memory_region_idx_t region_idx)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mem_mmngr_ptr_self_t *volatile AMOUT_p_mngr_control;
	// static const st_osal_mmngr_config_t * const volatile AMOUT_p_usr_config;
	AMOUT_p_mngr_control = p_mngr_control;
	// *AMOUT_p_usr_config = *p_usr_config;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_open_malloc:AMSTB_osal_internal_mem_open_malloc:inout:::] */
/*    osal_internal_mem_open_malloc => Stub */
e_osal_return_t AMSTB_osal_internal_mem_open_malloc(st_osal_mem_mmngr_ptr_self_t * p_mngr_control, const st_osal_mmngr_config_t * const p_usr_config)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mem_mmngr_ptr_self_t *volatile AMOUT_p_mngr_control;
	// static const st_osal_mmngr_config_t * const volatile AMOUT_p_usr_config;
	AMOUT_p_mngr_control = p_mngr_control;
	// *AMOUT_p_usr_config = *p_usr_config;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_close:AMSTB_osal_internal_mem_close:inout:::] */
/*    osal_internal_mem_close => Stub */
e_osal_return_t AMSTB_osal_internal_mem_close(osal_memory_manager_handle_t hndl_mmngr)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_memory_manager_handle_t volatile AMOUT_hndl_mmngr;
	AMOUT_hndl_mmngr = hndl_mmngr;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_conv_unit_and_str:AMSTB_osal_internal_mem_conv_unit_and_str:inout:::] */
/*    osal_internal_mem_conv_unit_and_str => Stub */
e_osal_return_t AMSTB_osal_internal_mem_conv_unit_and_str(size_t size, char * p_output, size_t array_size)
{
#define AMCALLMAX_osal_internal_mem_conv_unit_and_str 30
	static e_osal_return_t volatile AMIN_return[AMCALLMAX_osal_internal_mem_conv_unit_and_str];
	static size_t volatile AMOUT_size;
	static char *volatile AMOUT_p_output;
	static size_t volatile AMOUT_array_size;
	static int volatile AM_count;
	AM_count++;
	AMOUT_size = size;
	AMOUT_p_output = p_output;
	AMOUT_array_size = array_size;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_send_monitor_event:AMSTB_osal_internal_mem_send_monitor_event:inout:::] */
/*    osal_internal_mem_send_monitor_event => Stub */
e_osal_return_t AMSTB_osal_internal_mem_send_monitor_event(pthread_mutex_t *p_mutex_id, e_osal_mmngr_event_t event_id, e_osal_return_t error_code, st_osal_mem_mmngr_ptr_self_t const * p_mngr_control, osal_memory_buffer_handle_t buffer_hndl)
{
	static e_osal_return_t volatile AMIN_return;
	static pthread_mutex_t *volatile AMOUT_p_mutex_id;
	static e_osal_mmngr_event_t volatile AMOUT_event_id;
	static e_osal_return_t volatile AMOUT_error_code;
	static st_osal_mem_mmngr_ptr_self_t const * volatile AMOUT_p_mngr_control;
	static osal_memory_buffer_handle_t volatile AMOUT_buffer_hndl;
	AMOUT_p_mutex_id = p_mutex_id;
	AMOUT_event_id = event_id;
	AMOUT_error_code = error_code;
	AMOUT_p_mngr_control = p_mngr_control;
	AMOUT_buffer_hndl = buffer_hndl;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_dealloc_proc:AMSTB_osal_internal_mem_dealloc_proc:inout:::] */
/*    osal_internal_mem_dealloc_proc => Stub */
e_osal_return_t AMSTB_osal_internal_mem_dealloc_proc(st_osal_mem_mmngr_ptr_self_t *p_mngr_control, osal_memory_buffer_handle_t buffer_obj)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mem_mmngr_ptr_self_t *volatile AMOUT_p_mngr_control;
	static osal_memory_buffer_handle_t volatile AMOUT_buffer_obj;
	AMOUT_p_mngr_control = p_mngr_control;
	AMOUT_buffer_obj = buffer_obj;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_alloc_fill_guard_area:AMSTB_osal_internal_mem_alloc_fill_guard_area:inout:::] */
/*    osal_internal_mem_alloc_fill_guard_area => Stub */
e_osal_return_t AMSTB_osal_internal_mem_alloc_fill_guard_area(e_osal_mmngr_allocator_mode_t mode, st_osal_mem_buf_ptr_self_t * p_buffer_info)
{
	static e_osal_return_t volatile AMIN_return;
	static e_osal_mmngr_allocator_mode_t volatile AMOUT_mode;
	static st_osal_mem_buf_ptr_self_t * volatile AMOUT_p_buffer_info;
	AMOUT_mode = mode;
	AMOUT_p_buffer_info = p_buffer_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_alloc_map:AMSTB_osal_internal_mem_alloc_map:inout:::] */
/*    osal_internal_mem_alloc_map => Stub */
e_osal_return_t AMSTB_osal_internal_mem_alloc_map(st_osal_mem_buf_ptr_self_t * p_buffer_info, st_osal_mmngr_config_t * p_usr_config, size_t bytes, size_t alignment)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mem_buf_ptr_self_t * volatile AMOUT_p_buffer_info;
	static st_osal_mmngr_config_t * volatile AMOUT_p_usr_config;
	static size_t volatile AMOUT_bytes;
	static size_t volatile AMOUT_alignment;
	AMOUT_p_buffer_info = p_buffer_info;
	AMOUT_p_usr_config = p_usr_config;
	AMOUT_bytes = bytes;
	AMOUT_alignment = alignment;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_alloc_lib:AMSTB_osal_internal_mem_alloc_lib:inout:::] */
/*    osal_internal_mem_alloc_lib => Stub */
e_osal_return_t AMSTB_osal_internal_mem_alloc_lib(st_osal_mem_mmngr_ptr_self_t * p_mngr_control, st_osal_mem_buf_ptr_self_t * p_buffer_info, size_t bytes, size_t alignment)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mem_mmngr_ptr_self_t * volatile AMOUT_p_mngr_control;
	static st_osal_mem_buf_ptr_self_t * volatile AMOUT_p_buffer_info;
	static size_t volatile AMOUT_bytes;
	static size_t volatile AMOUT_alignment;
	AMOUT_p_mngr_control = p_mngr_control;
	AMOUT_p_buffer_info = p_buffer_info;
	AMOUT_bytes = bytes;
	AMOUT_alignment = alignment;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_memory.c:osal_internal_mem_find_free_buffer_info:AMSTB_osal_internal_mem_find_free_buffer_info:inout:::] */
/*    osal_internal_mem_find_free_buffer_info => Stub */
e_osal_return_t AMSTB_osal_internal_mem_find_free_buffer_info(st_osal_mem_mmngr_ptr_self_t * p_mngr_control, st_osal_mem_buf_ptr_self_t ** pp_buffer_info)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_mem_mmngr_ptr_self_t * volatile AMOUT_p_mngr_control;
	static st_osal_mem_buf_ptr_self_t ** volatile AMOUT_pp_buffer_info;
	AMOUT_p_mngr_control = p_mngr_control;
	AMOUT_pp_buffer_info = pp_buffer_info;
	static int volatile AM_control;
    
    if (AM_control != 0)
    {
        *pp_buffer_info = &g_osal_mem_buf_ptr_self;
    }
    else
    {
        *pp_buffer_info = NULL;
    }
    
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:osal_internal_compare_string:AMSTB_osal_internal_compare_string:inout:::] */
/*    osal_internal_compare_string => Stub */
e_osal_return_t AMSTB_osal_internal_compare_string(const char *p_str1, const char *p_str2, size_t len)
{
	static e_osal_return_t volatile AMIN_return;
	static const char *volatile AMOUT_p_str1;
	static const char *volatile AMOUT_p_str2;
	static size_t volatile AMOUT_len;
	AMOUT_p_str1 = p_str1;
	AMOUT_p_str2 = p_str2;
	AMOUT_len = len;
	return AMIN_return;
}

e_osal_return_t AMSTB_osal_internal_create_axibus_info(const st_osal_device_control_t *p_device_control)
{
    static e_osal_return_t volatile AMIN_return;
    
    return AMIN_return;
}

e_osal_return_t AMSTB_osal_internal_register_axi_bus_info_last_data(void)
{
    static e_osal_return_t volatile AMIN_return;
    
    return AMIN_return;
}

e_osal_return_t AMSTB_osal_internal_add_utlb_list(const char* p_domain_name, osal_axi_bus_id_t* pp_list_buffer, size_t list_size,
                                                  size_t *match_count, int32_t *current_utlb, const char *p_num_str)
{
    static e_osal_return_t    volatile AMIN_return;
    static const char*        volatile AMOUT_domain_name;
    static osal_axi_bus_id_t* volatile AMOUT_list_buffer;
    static size_t             volatile AMOUT_list_size;
    static size_t             *volatile AMOUT_match_count;
    static int32_t            *volatile AMOUT_current_utlb;
    static const char         *volatile AMOUT_num_str;
    
    static size_t volatile AMIN_match_count;
    *match_count = AMIN_match_count;
    AMOUT_match_count = match_count;

    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToCG:AMSTB_R_PMA_SM_ChangeToCG:inout:::] */
/*    R_PMA_SM_ChangeToCG => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToCG(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToPGtypeA:AMSTB_R_PMA_SM_ChangeToPGtypeA:inout:::] */
/*    R_PMA_SM_ChangeToPGtypeA => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToPGtypeA(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToPGtypeB:AMSTB_R_PMA_SM_ChangeToPGtypeB:inout:::] */
/*    R_PMA_SM_ChangeToPGtypeB => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToPGtypeB(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_InitCpgDomain:AMSTB_R_PMA_SM_InitCpgDomain:inout:::] */
/*    R_PMA_SM_InitCpgDomain => Stub */
//e_result_t AMSTB_R_PMA_SM_InitCpgDomain(void)
//{
//	static const int *volatile AMIN_R_PMA_CPG_MOD_ID_TABLE;
//	static e_result_t volatile AMIN_return;
//	static unsigned long long volatile AMOUT_pma_soc_type;
//	static e_cpg_domain_id_t volatile AMOUT_cpg_idmax;
//	static e_cpg_domain_id_t volatile AMOUT_cpg_idmin;
//	AMOUT_pma_soc_type = pma_soc_type;
//	AMOUT_cpg_idmax = cpg_idmax;
//	AMOUT_cpg_idmin = cpg_idmin;
//	R_PMA_CPG_MOD_ID_TABLE = AMIN_R_PMA_CPG_MOD_ID_TABLE;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToPGtypeC:AMSTB_R_PMA_SM_ChangeToPGtypeC:inout:::] */
/*    R_PMA_SM_ChangeToPGtypeC => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToPGtypeC(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_InitHwa:AMSTB_R_PMA_SM_InitHwa:inout:::] */
/*    R_PMA_SM_InitHwa => Stub */
//e_result_t AMSTB_R_PMA_SM_InitHwa(void)
//{
//	static const st_hwa_func_tbl_t *volatile AMIN_R_PMA_SM_GstRegStatChangeFuncTbl;
//	static e_result_t volatile AMIN_return;
//	static unsigned long long volatile AMOUT_pma_soc_type;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmin;
//	AMOUT_pma_soc_type = pma_soc_type;
//	AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_hwa_idmin = hwa_idmin;
//	R_PMA_SM_GstRegStatChangeFuncTbl = AMIN_R_PMA_SM_GstRegStatChangeFuncTbl;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_InitSyscDomain:AMSTB_R_PMA_SM_InitSyscDomain:inout:::] */
/*    R_PMA_SM_InitSyscDomain => Stub */
//e_result_t AMSTB_R_PMA_SM_InitSyscDomain(void)
//{
//	static const int *volatile AMIN_R_PMA_SYSC_PDR_ID_TABLE;
//	static const unsigned long long *volatile AMIN_sysc_mask_tbl;
//	static e_result_t volatile AMIN_return;
//	static unsigned long long volatile AMOUT_pma_soc_type;
//	static e_sysc_domain_id_t volatile AMOUT_sysc_idmax;
//	static e_sysc_domain_id_t volatile AMOUT_sysc_idmin;
//	AMOUT_pma_soc_type = pma_soc_type;
//	AMOUT_sysc_idmax = sysc_idmax;
//	AMOUT_sysc_idmin = sysc_idmin;
//	R_PMA_SYSC_PDR_ID_TABLE = AMIN_R_PMA_SYSC_PDR_ID_TABLE;
//	sysc_mask_tbl = AMIN_sysc_mask_tbl;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToREADY:AMSTB_R_PMA_SM_ChangeToREADY:inout:::] */
/*    R_PMA_SM_ChangeToREADY => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToREADY(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToREADYwithoutPwr:AMSTB_R_PMA_SM_ChangeToREADYwithoutPwr:inout:::] */
/*    R_PMA_SM_ChangeToREADYwithoutPwr => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToREADYwithoutPwr(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToREADYtypeA:AMSTB_R_PMA_SM_ChangeToREADYtypeA:inout:::] */
/*    R_PMA_SM_ChangeToREADYtypeA => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToREADYtypeA(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_SM_ChangeToREADYtypeC:AMSTB_R_PMA_SM_ChangeToREADYtypeC:inout:::] */
/*    R_PMA_SM_ChangeToREADYtypeC => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangeToREADYtypeC(const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[stub.c:sysc_mgr_getfuncs:AMSTB_sysc_mgr_getfuncs:inout:::] */
/*    sysc_mgr_getfuncs => Stub */
int AMSTB_sysc_mgr_getfuncs(struct sysc_mgr_funcs_t *funcs,int tabsize)
{
	static int volatile AMIN_return;
	static struct sysc_mgr_funcs_t *volatile AMOUT_funcs;
	static int volatile AMOUT_tabsize;
	AMOUT_funcs = funcs;
	AMOUT_tabsize = tabsize;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqDelete:AMSTB_R_OSAL_Internal_MqDelete:inout:::] */
/*    R_OSAL_Internal_MqDelete => Stub */
int AMSTB_R_OSAL_Internal_MqDelete(struct st_osal_inner_mq_control *handle)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_mq_control *volatile AMOUT_handle;
	AMOUT_handle = handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexDestroy:AMSTB_R_OSAL_Internal_MutexDestroy:inout:::] */
/*    R_OSAL_Internal_MutexDestroy => Stub */
int AMSTB_R_OSAL_Internal_MutexDestroy(struct st_osal_inner_mutex_control *handle)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_mutex_control *volatile AMOUT_handle;
	AMOUT_handle = handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_ThreadTimedJoin:AMSTB_R_OSAL_Internal_ThreadTimedJoin:inout:::] */
/*    R_OSAL_Internal_ThreadTimedJoin => Stub */
int AMSTB_R_OSAL_Internal_ThreadTimedJoin(struct st_osal_inner_thread_control *handle,long long *p_return_value,long long time_period)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_thread_control *volatile AMOUT_handle;
	static long long *volatile AMOUT_p_return_value;
	static long long volatile AMOUT_time_period;
	AMOUT_handle = handle;
	AMOUT_p_return_value = p_return_value;
	AMOUT_time_period = time_period;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_SetPostClock:AMSTB_R_PMA_SetPostClock:inout:::] */
/*    R_PMA_SetPostClock => Stub */
e_pma_return_t AMSTB_R_PMA_SetPostClock(st_pma_handle_t const handle, e_pma_clk_req_t clockEnable, uint32_t chanIdx, uint32_t *pClockDivider)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t const volatile AMOUT_handle;
    static e_pma_clk_req_t volatile AMOUT_clockEnable;
    static uint32_t volatile AMOUT_chanIdx;
    static uint32_t *volatile AMOUT_pClockDivider;
    // AMOUT_handle = handle;
    AMOUT_clockEnable = clockEnable;
    AMOUT_chanIdx = chanIdx;
    AMOUT_pClockDivider = pClockDivider;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_Exchange_ErrCode:AMSTB_R_PMA_IF_Exchange_ErrCode:inout:::] */
/*    R_PMA_IF_Exchange_ErrCode => Stub */
//e_pma_return_t AMSTB_R_PMA_IF_Exchange_ErrCode(e_result_t innerErr)
//{
//	static e_pma_return_t volatile AMIN_return;
//	static e_result_t volatile AMOUT_innerErr;
//	AMOUT_innerErr = innerErr;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexInit:AMSTB_R_OSAL_Internal_MutexInit:inout:::] */
/*    R_OSAL_Internal_MutexInit => Stub */
int AMSTB_R_OSAL_Internal_MutexInit(struct st_osal_inner_mutexattr **p_mutexattr,struct st_osal_inner_mutex_control **p_handle)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_mutexattr **volatile AMOUT_p_mutexattr;
//	static struct st_osal_inner_mutex_control **volatile AMOUT_p_handle;
	AMOUT_p_mutexattr = p_mutexattr;
//	AMOUT_p_handle = p_handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_SetMutexAttr:AMSTB_R_OSAL_Internal_SetMutexAttr:inout:::] */
/*    R_OSAL_Internal_SetMutexAttr => Stub */
int AMSTB_R_OSAL_Internal_SetMutexAttr(struct st_osal_inner_mutexattr **p_mutexattr)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_mutexattr **volatile AMOUT_p_mutexattr;
	AMOUT_p_mutexattr = p_mutexattr;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqTimedSend:AMSTB_R_OSAL_Internal_MqTimedSend:inout:::] */
/*    R_OSAL_Internal_MqTimedSend => Stub */
int AMSTB_R_OSAL_Internal_MqTimedSend(struct st_osal_inner_mq_control *handle,long long time_period,void *p_buffer,unsigned int buffer_size)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_mq_control *volatile AMOUT_handle;
	static long long volatile AMOUT_time_period;
	static void *volatile AMOUT_p_buffer;
	static unsigned int volatile AMOUT_buffer_size;
	AMOUT_handle = handle;
	AMOUT_time_period = time_period;
	AMOUT_p_buffer = p_buffer;
	AMOUT_buffer_size = buffer_size;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexUnlock:AMSTB_R_OSAL_Internal_MutexUnlock:inout:::] */
/*    R_OSAL_Internal_MutexUnlock => Stub */
e_osal_return_t AMSTB_R_OSAL_Internal_MutexUnlock(struct st_osal_inner_mutex_control *handle)
{
	static e_osal_return_t volatile AMIN_return;
	static struct st_osal_inner_mutex_control *volatile AMOUT_handle;
	AMOUT_handle = handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexTimedLock:AMSTB_R_OSAL_Internal_MutexTimedLock:inout::array:counter<AM_count>] */
/*    R_OSAL_Internal_MutexTimedLock => Stub */
int AMSTB_R_OSAL_Internal_MutexTimedLock(struct st_osal_inner_mutex_control *handle,long long time_period)
{
#define AMCALLMAX_R_OSAL_Internal_MutexTimedLock 2
	static int volatile AMIN_return[AMCALLMAX_R_OSAL_Internal_MutexTimedLock];
	static struct st_osal_inner_mutex_control *volatile AMOUT_handle[AMCALLMAX_R_OSAL_Internal_MutexTimedLock];
	static long long volatile AMOUT_time_period[AMCALLMAX_R_OSAL_Internal_MutexTimedLock];
	static int volatile AM_count;
	AMOUT_handle[AM_count] = handle;
	AMOUT_time_period[AM_count] = time_period;

	AM_count++;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_init:AMSTB_dummy_init:inout:::] */
/*    dummy_init => Stub */
int AMSTB_dummy_init(int vbose)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_vbose;
	AMOUT_vbose = vbose;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_SetThreadAttr:AMSTB_R_OSAL_Internal_SetThreadAttr:inout:::] */
/*    R_OSAL_Internal_SetThreadAttr => Stub */
int AMSTB_R_OSAL_Internal_SetThreadAttr(struct st_osal_inner_thread_attrinfo *p_attr_info,struct st_osal_inner_thread_attr **p_attr)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_thread_attrinfo *volatile AMOUT_p_attr_info;
	static struct st_osal_inner_thread_attr **volatile AMOUT_p_attr;
	AMOUT_p_attr_info = p_attr_info;
	AMOUT_p_attr = p_attr;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_ThreadCreate:AMSTB_R_OSAL_Internal_ThreadCreate:inout:::] */
/*    R_OSAL_Internal_ThreadCreate => Stub */
int AMSTB_R_OSAL_Internal_ThreadCreate(struct st_osal_inner_thread_config *p_config,struct st_osal_inner_thread_control **p_handle)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_thread_config *volatile AMOUT_p_config;
//	static struct st_osal_inner_thread_control **volatile AMOUT_p_handle;
	AMOUT_p_config = p_config;
//	AMOUT_p_handle = p_handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqCreate:AMSTB_R_OSAL_Internal_MqCreate:inout:::] */
/*    R_OSAL_Internal_MqCreate => Stub */
int AMSTB_R_OSAL_Internal_MqCreate(struct st_osal_inner_mq_config *p_config,struct st_osal_inner_mq_control **p_handle)
{
	static int volatile AMIN_return;
	static struct st_osal_inner_mq_config *volatile AMOUT_p_config;
//	static struct st_osal_inner_mq_control **volatile AMOUT_p_handle;
	AMOUT_p_config = p_config;
//	AMOUT_p_handle = p_handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqTimedReceive:AMSTB_R_OSAL_Internal_MqTimedReceive:inout::array:counter<AM_count>] */
/*    R_OSAL_Internal_MqTimedReceive => Stub */
int AMSTB_R_OSAL_Internal_MqTimedReceive(struct st_osal_inner_mq_control *handle,long long time_period,void *p_buffer,unsigned int buffer_size)
{
#define AMCALLMAX_R_OSAL_Internal_MqTimedReceive 2
	static int volatile AMIN_return[AMCALLMAX_R_OSAL_Internal_MqTimedReceive];
	static struct st_osal_inner_mq_control *volatile AMOUT_handle[AMCALLMAX_R_OSAL_Internal_MqTimedReceive];
	static long long volatile AMOUT_time_period[AMCALLMAX_R_OSAL_Internal_MqTimedReceive];
	static void *volatile AMOUT_p_buffer[AMCALLMAX_R_OSAL_Internal_MqTimedReceive];
	static unsigned int volatile AMOUT_buffer_size[AMCALLMAX_R_OSAL_Internal_MqTimedReceive];
	static int volatile AM_count;
	AMOUT_handle[AM_count] = handle;
	AMOUT_time_period[AM_count] = time_period;
	AMOUT_p_buffer[AM_count] = p_buffer;
	AMOUT_buffer_size[AM_count] = buffer_size;

//	st_msg_to_task_t * pMsgTotask = p_buffer;
//	pMsgTotask->enProcId = dummy_MsgToTask[AM_count].enProcId;
//
	AM_count++;
	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetPowerPolicy:AMSTB_R_PMA_GetPowerPolicy:inout:::] */
/*    R_PMA_GetPowerPolicy => Stub */
e_pma_return_t AMSTB_R_PMA_GetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_power_policy_t *volatile AMOUT_pPolicy;
	static e_pma_power_policy_t volatile AMIN_Policy;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	AMOUT_pPolicy = pPolicy;

    *pPolicy = AMIN_Policy;

    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_ReleaseResetHwa:AMSTB_R_PMA_ReleaseResetHwa:inout:::] */
/*    R_PMA_ReleaseResetHwa => Stub */
e_pma_return_t AMSTB_R_PMA_ReleaseResetHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_WaitForState:AMSTB_R_PMA_WaitForState:inout:::] */
/*    R_PMA_WaitForState => Stub */
e_pma_return_t AMSTB_R_PMA_WaitForState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const * pHwaState, e_pma_reset_req_t rstStat, osal_milli_sec_t sleepTimePeriod, osal_milli_sec_t timeoutMsPeriod, osal_nano_sec_t timeoutNsPeriod)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static st_pma_hwa_state_t const * volatile AMOUT_pHwaState;
	static e_pma_reset_req_t volatile AMOUT_rstStat;
	static osal_milli_sec_t volatile AMOUT_sleepTimePeriod;
	static osal_milli_sec_t volatile AMOUT_timeoutMsPeriod;
	static osal_nano_sec_t volatile AMOUT_timeoutNsPeriod;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	AMOUT_pHwaState = pHwaState;
	AMOUT_rstStat = rstStat;
	AMOUT_sleepTimePeriod = sleepTimePeriod;
	AMOUT_timeoutMsPeriod = timeoutMsPeriod;
	AMOUT_timeoutNsPeriod = timeoutNsPeriod;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_RequestHwa:AMSTB_R_PMA_RequestHwa:inout:::] */
/*    R_PMA_RequestHwa => Stub */
e_pma_return_t AMSTB_R_PMA_RequestHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_ApplyResetHwa:AMSTB_R_PMA_ApplyResetHwa:inout:::] */
/*    R_PMA_ApplyResetHwa => Stub */
e_pma_return_t AMSTB_R_PMA_ApplyResetHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetLockHwa:AMSTB_R_PMA_GetLockHwa:inout:::] */
/*    R_PMA_GetLockHwa => Stub */
e_pma_return_t AMSTB_R_PMA_GetLockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetReset:AMSTB_R_PMA_GetReset:inout:::] */
/*    R_PMA_GetReset => Stub */
e_pma_return_t AMSTB_R_PMA_GetReset(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	// static struct st_osal_inner_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_reset_req_t *volatile AMOUT_pRstStat;
	static e_pma_reset_req_t volatile AMIN_RstStat;
	// AMOUT_hwa_idmax = hwa_idmax;
	// AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	AMOUT_pRstStat = pRstStat;
	*pRstStat = AMIN_RstStat;

    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetLowestPowerState:AMSTB_R_PMA_GetLowestPowerState:inout:::] */
/*    R_PMA_GetLowestPowerState => Stub */
e_pma_return_t AMSTB_R_PMA_GetLowestPowerState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	// static struct st_osal_inner_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_power_state_t *volatile AMOUT_pLowestPower;
	static e_pma_power_state_t volatile AMIN_LowestPower;
	// AMOUT_hwa_idmax = hwa_idmax;
	// AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	AMOUT_pLowestPower = pLowestPower;

	*pLowestPower = AMIN_LowestPower;

    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_SetPowerPolicy:AMSTB_R_PMA_SetPowerPolicy:inout:::] */
/*    R_PMA_SetPowerPolicy => Stub */
e_pma_return_t AMSTB_R_PMA_SetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_power_policy_t volatile AMOUT_policy;
	// AMOUT_hwa_idmax = hwa_idmax;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	AMOUT_policy = policy;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_Quit:AMSTB_R_PMA_Quit:inout:::] */
/*    R_PMA_Quit => Stub */
e_pma_return_t AMSTB_R_PMA_Quit(st_pma_handle_t handle)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	// static struct st_osal_inner_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmin;
	// static struct st_osal_inner_thread_control *volatile AMOUT_R_PMA_SM_GxThread;
	// static struct st_osal_inner_mq_control *volatile AMOUT_R_PMA_SM_GxTaskQueue;
	static st_pma_handle_t volatile AMOUT_handle;
	// AMOUT_hwa_idmax = hwa_idmax;
	// AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
	// AMOUT_hwa_idmin = hwa_idmin;
	// AMOUT_R_PMA_SM_GxThread = R_PMA_SM_GxThread;
	// AMOUT_R_PMA_SM_GxTaskQueue = R_PMA_SM_GxTaskQueue;
	AMOUT_handle = handle;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_Init:AMSTB_R_PMA_Init:inout:::] */
/*    R_PMA_Init => Stub */
e_pma_return_t AMSTB_R_PMA_Init(st_pma_handle_t *pHandle, const st_pma_config_t *pConfig)
{
	static e_pma_return_t volatile AMIN_return;
	static st_pma_handle_t *volatile AMOUT_pHandle;
	static const st_pma_config_t *volatile AMOUT_pConfig;
	AMOUT_pHandle = pHandle;
	AMOUT_pConfig = pConfig;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetHwaState:AMSTB_R_PMA_GetHwaState:inout:::] */
/*    R_PMA_GetHwaState => Stub */
e_pma_return_t AMSTB_R_PMA_GetHwaState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState)
{
	static e_pma_return_t volatile AMIN_return;
	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
	// static struct st_osal_inner_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
	static st_pma_handle_t volatile AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static st_pma_hwa_state_t *volatile AMOUT_pHwaState;
	static e_pma_hwa_req_t volatile input_enReqState;
	static e_pma_onoff_t   volatile input_enActualPwrStat;
	static e_pma_onoff_t   volatile input_enActualClkStat;

	pHwaState->enReqState      = input_enReqState;
	pHwaState->enActualPwrStat = input_enActualPwrStat;
	pHwaState->enActualClkStat = input_enActualClkStat;

	// AMOUT_hwa_idmax = hwa_idmax;
	// AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	AMOUT_pHwaState = pHwaState;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetHwa:AMSTB_R_PMA_SM_GetHwa:inout:::] */
/*    R_PMA_SM_GetHwa => Stub */
//st_hwa_t * AMSTB_R_PMA_SM_GetHwa(e_pma_hwa_id_t hwaId)
//{
//	static st_hwa_t * volatile AMIN_return;
//	// static e_pma_hwa_id_t volatile AMOUT_hwa_idmax;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	// AMOUT_hwa_idmax = hwa_idmax;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetClockStat:AMSTB_R_PMA_SM_SetClockStat:inout:::] */
/*    R_PMA_SM_SetClockStat => Stub */
//e_result_t AMSTB_R_PMA_SM_SetClockStat(st_cpg_domain_t * pCpgDomain, e_pma_clock_onoff_t enstat)
//{
#//define AMCALLMAX_R_PMA_SM_SetClockStat 6
//	static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_SM_SetClockStat];
//	static const int *volatile AMOUT_R_PMA_CPG_MOD_ID_TABLE[AMCALLMAX_R_PMA_SM_SetClockStat];
//	static struct st_cpg_domain_t_ *volatile AMOUT_pCpgDomain[AMCALLMAX_R_PMA_SM_SetClockStat];
//	static e_pma_clock_onoff_t volatile AMOUT_enstat[AMCALLMAX_R_PMA_SM_SetClockStat];
//	static int volatile AM_count;
//	AMOUT_R_PMA_CPG_MOD_ID_TABLE[AM_count] = R_PMA_CPG_MOD_ID_TABLE;
//	AMOUT_pCpgDomain[AM_count] = pCpgDomain;
//	AMOUT_enstat[AM_count] = enstat;
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetupQueue:AMSTB_R_PMA_SM_SetupQueue:inout:::] */
/*    R_PMA_SM_SetupQueue => Stub */
//e_result_t AMSTB_R_PMA_SM_SetupQueue(R_PMA_QUE_HANDLE *pTaskQueue)
//{
//	static e_result_t volatile AMIN_return;
//	// static struct st_osal_inner_mq_control *volatile AMOUT_R_PMA_SM_GxTaskQueue;
//	static R_PMA_QUE_HANDLE *volatile AMOUT_pTaskQueue;
//	// AMOUT_R_PMA_SM_GxTaskQueue = R_PMA_SM_GxTaskQueue;
//	AMOUT_pTaskQueue = pTaskQueue;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ReleaseHWA:AMSTB_R_PMA_SM_ReleaseHWA:inout:::] */
/*    R_PMA_SM_ReleaseHWA => Stub */
//e_result_t AMSTB_R_PMA_SM_ReleaseHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
//{
//	static e_result_t volatile AMIN_return;
//	// static const st_hwa_func_tbl_t *volatile AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	// AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl = R_PMA_SM_GstRegStatChangeFuncTbl;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetPowerStat:AMSTB_R_PMA_SM_SetPowerStat:inout:::] */
/*    R_PMA_SM_SetPowerStat => Stub */
//e_result_t AMSTB_R_PMA_SM_SetPowerStat(st_pma_handle_t handle, st_sysc_domain_t * pSyscDomain, e_pma_power_onoff_t enstat)
//{
//#define AMCALLMAX_R_PMA_SM_SetPowerStat 6
//	static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_SM_SetPowerStat];
//	static const int *volatile AMOUT_R_PMA_SYSC_PDR_ID_TABLE[AMCALLMAX_R_PMA_SM_SetPowerStat];
//	static const unsigned long long *volatile AMOUT_sysc_mask_tbl[AMCALLMAX_R_PMA_SM_SetPowerStat];
//	static struct st_pma_user_info_t *volatile AMOUT_handle[AMCALLMAX_R_PMA_SM_SetPowerStat];
//	static struct st_sysc_domain_t_ *volatile AMOUT_pSyscDomain[AMCALLMAX_R_PMA_SM_SetPowerStat];
//	static e_pma_power_onoff_t volatile AMOUT_enstat[AMCALLMAX_R_PMA_SM_SetPowerStat];
//	static int volatile AM_count;
//	AMOUT_R_PMA_SYSC_PDR_ID_TABLE[AM_count] = R_PMA_SYSC_PDR_ID_TABLE;
//	AMOUT_sysc_mask_tbl[AM_count] = sysc_mask_tbl;
//	AMOUT_handle[AM_count] = handle;
//	AMOUT_pSyscDomain[AM_count] = pSyscDomain;
//	AMOUT_enstat[AM_count] = enstat;
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetHwa:AMSTB_3R_PMA_SM_GetHwaState_R_PMA_SM_GetHwa:inout:csv_local:array:counter<AM_count>] */
/*    R_PMA_SM_GetHwaState -- R_PMA_SM_GetHwa => Stub */
//st_hwa_t * AMSTB_3R_PMA_SM_GetHwaState_R_PMA_SM_GetHwa(e_pma_hwa_id_t hwaId)
//{
//#define AMCALLMAX_3R_PMA_SM_GetHwaState_R_PMA_SM_GetHwa 1
//	static st_hwa_t * volatile AMIN_return[AMCALLMAX_3R_PMA_SM_GetHwaState_R_PMA_SM_GetHwa];
//	static e_pma_hwa_id_t volatile AMOUT_hwa_idmax[AMCALLMAX_3R_PMA_SM_GetHwaState_R_PMA_SM_GetHwa];
//	static e_pma_hwa_id_t volatile AMOUT_hwaId[AMCALLMAX_3R_PMA_SM_GetHwaState_R_PMA_SM_GetHwa];
//	static int volatile AM_count;
//	AMOUT_hwa_idmax[AM_count] = hwa_idmax;
//	AMOUT_hwaId[AM_count] = hwaId;
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}
//st_cpg_domain_t dummy_CpgDomain2;
/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_PolicyToStatusWithoutPower:AMSTB_R_PMA_SM_PolicyToStatusWithoutPower:inout:::] */
/*    R_PMA_SM_PolicyToStatusWithoutPower => Stub */
//e_result_t AMSTB_R_PMA_SM_PolicyToStatusWithoutPower(e_pma_hwa_id_t hwaId, const st_hwa_t *pHwa, e_pma_power_policy_t policy)
//{
//	static e_result_t volatile AMIN_return;
//	// static const st_hwa_func_tbl_t *volatile AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	static e_pma_power_policy_t volatile AMOUT_policy;
//	// AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl = R_PMA_SM_GstRegStatChangeFuncTbl;
//	AMOUT_hwaId = hwaId;
//	AMOUT_pHwa = pHwa;
//pHwa->pCpgDomain->refCnt = dummy_CpgDomain2.refCnt;
//	AMOUT_policy = policy;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_Task:AMSTB_R_PMA_SM_Task:inout:::] */
/*    R_PMA_SM_Task => Stub */
void *AMSTB_R_PMA_SM_Task(void *pvParameters)
{
	static void *volatile AMIN_return;
	// static struct st_osal_inner_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex;
	// static struct st_osal_inner_mq_control *volatile AMOUT_R_PMA_SM_GxTaskQueue;
	static void *volatile AMOUT_pvParameters;
	// AMOUT_R_PMA_SM_GxMutex = R_PMA_SM_GxMutex;
	// AMOUT_R_PMA_SM_GxTaskQueue = R_PMA_SM_GxTaskQueue;
	AMOUT_pvParameters = pvParameters;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ReleaseResetHWA:AMSTB_R_PMA_SM_ReleaseResetHWA:inout:::] */
/*    R_PMA_SM_ReleaseResetHWA => Stub */
//e_result_t AMSTB_R_PMA_SM_ReleaseResetHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
//{
//	static e_result_t volatile AMIN_return;
//	// static const int *volatile AMOUT_R_PMA_CPG_MOD_ID_TABLE;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	// AMOUT_R_PMA_CPG_MOD_ID_TABLE = R_PMA_CPG_MOD_ID_TABLE;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_CheckUserMatchHwa:AMSTB_R_PMA_SM_CheckUserMatchHwa:inout:::] */
/*    R_PMA_SM_CheckUserMatchHwa => Stub */
//e_result_t AMSTB_R_PMA_SM_CheckUserMatchHwa(st_pma_handle_t handle, const st_hwa_t *pHwa)
//{
//	static e_result_t volatile AMIN_return;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	AMOUT_handle = handle;
//	AMOUT_pHwa = pHwa;
//	return AMIN_return;
//}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_disable:AMSTB_dummy_clk_disable:inout:::] */
/*    dummy_clk_disable => Stub */
int AMSTB_dummy_clk_disable(int id,int idx)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_id;
	static int volatile AMOUT_idx;
	AMOUT_id = id;
	AMOUT_idx = idx;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_divider_get:AMSTB_dummy_clk_divider_get:inout:::] */
/*    dummy_clk_divider_get => Stub */
int AMSTB_dummy_clk_divider_get(int id,int idx,unsigned int *div)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_id;
	static int volatile AMOUT_idx;
	static unsigned int *volatile AMOUT_div;
	AMOUT_id = id;
	AMOUT_idx = idx;
	AMOUT_div = div;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_divider_set:AMSTB_dummy_clk_divider_set:inout:::] */
/*    dummy_clk_divider_set => Stub */
int AMSTB_dummy_clk_divider_set(int id,int idx,unsigned int *div)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_id;
	static int volatile AMOUT_idx;
	static unsigned int *volatile AMOUT_div;
	AMOUT_id = id;
	AMOUT_idx = idx;
	AMOUT_div = div;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_enable:AMSTB_dummy_clk_enable:inout:::] */
/*    dummy_clk_enable => Stub */
int AMSTB_dummy_clk_enable(int id,int idx)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_id;
	static int volatile AMOUT_idx;
	AMOUT_id = id;
	AMOUT_idx = idx;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_is_enabled:AMSTB_dummy_clk_is_enabled:inout:::] */
/*    dummy_clk_is_enabled => Stub */
//int AMSTB_dummy_clk_is_enabled(int id,int idx)
//{
//	static int volatile AMIN_return;
//	static int volatile AMOUT_id;
//	static int volatile AMOUT_idx;
//	AMOUT_id = id;
//	AMOUT_idx = idx;
//	return AMIN_return;
//}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_rate_get:AMSTB_dummy_clk_rate_get:inout:::] */
/*    dummy_clk_rate_get => Stub */
int AMSTB_dummy_clk_rate_get(int id,int idx,unsigned int *rate)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_id;
	static int volatile AMOUT_idx;
	static unsigned int *volatile AMOUT_rate;
	AMOUT_id = id;
	AMOUT_idx = idx;
	AMOUT_rate = rate;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_clk_rate_set:AMSTB_dummy_clk_rate_set:inout:::] */
/*    dummy_clk_rate_set => Stub */
int AMSTB_dummy_clk_rate_set(int id,int idx,unsigned int *rate)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_id;
	static int volatile AMOUT_idx;
	static unsigned int *volatile AMOUT_rate;
	AMOUT_id = id;
	AMOUT_idx = idx;
	AMOUT_rate = rate;
	return AMIN_return;
}


/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_deinit:AMSTB_dummy_deinit:inout:::] */
/*    dummy_deinit => Stub */
int AMSTB_dummy_deinit()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_assert:AMSTB_dummy_mstp_assert:inout:::] */
/*    dummy_mstp_assert => Stub */
int AMSTB_dummy_mstp_assert(int mod_id)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_mod_id;
	AMOUT_mod_id = mod_id;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_deassert:AMSTB_dummy_mstp_deassert:inout:::] */
/*    dummy_mstp_deassert => Stub */
int AMSTB_dummy_mstp_deassert(int mod_id)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_mod_id;
	AMOUT_mod_id = mod_id;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_disable:AMSTB_dummy_mstp_disable:inout:::] */
/*    dummy_mstp_disable => Stub */
int AMSTB_dummy_mstp_disable(int mod_id)
{
#define AMCALLMAX_dummy_mstp_disable 34
	static int volatile AMIN_return[AMCALLMAX_dummy_mstp_disable];
	static int volatile AMOUT_mod_id[AMCALLMAX_dummy_mstp_disable];
	static int volatile AM_count;
	AMOUT_mod_id[AM_count] = mod_id;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_enable:AMSTB_dummy_mstp_enable:inout:::] */
/*    dummy_mstp_enable => Stub */
int AMSTB_dummy_mstp_enable(int mod_id)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_mod_id;
	AMOUT_mod_id = mod_id;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_is_enabled:AMSTB_dummy_mstp_is_enabled:inout:::] */
/*    dummy_mstp_is_enabled => Stub */
int AMSTB_dummy_mstp_is_enabled(int mod_id)
{
#define AMCALLMAX_dummy_mstp_is_enabled 34
	static int volatile AMIN_return[AMCALLMAX_dummy_mstp_is_enabled];
	static int volatile AMOUT_mod_id[AMCALLMAX_dummy_mstp_is_enabled];
	static int volatile AM_count;
	AMOUT_mod_id[AM_count] = mod_id;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_reset:AMSTB_dummy_mstp_reset:inout:::] */
/*    dummy_mstp_reset => Stub */
int AMSTB_dummy_mstp_reset(int mod_id)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_mod_id;
	AMOUT_mod_id = mod_id;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_sysc_deinit:AMSTB_dummy_sysc_deinit:inout:::] */
/*    dummy_sysc_deinit => Stub */
void AMSTB_dummy_sysc_deinit()
{
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_sysc_init:AMSTB_dummy_sysc_init:inout:::] */
/*    dummy_sysc_init => Stub */
int AMSTB_dummy_sysc_init()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* AMSTB_dummy_sysc_power_is_off:inout:::] */
/*    dummy_sysc_power_is_off => Stub */
int AMSTB_dummy_sysc_power_is_off(int pdr_number)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_pdr_number;
	AMOUT_pdr_number = pdr_number;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_sysc_power_off:AMSTB_dummy_sysc_power_off:inout:::] */
/*    dummy_sysc_power_off => Stub */
int AMSTB_dummy_sysc_power_off(int pdr_number)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_pdr_number;
	AMOUT_pdr_number = pdr_number;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_sysc_power_on:AMSTB_dummy_sysc_power_on:inout:::] */
/*    dummy_sysc_power_on => Stub */
int AMSTB_dummy_sysc_power_on(int pdr_number)
{
	static int volatile AMIN_return;
	static int volatile AMOUT_pdr_number;
	AMOUT_pdr_number = pdr_number;
	return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_sysc_read_pdmode:AMSTB_dummy_sysc_read_pdmode:inout:::] */
/*    dummy_sysc_read_pdmode => Stub */
int AMSTB_dummy_sysc_read_pdmode()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetSyscDomain:AMSTB_R_PMA_SM_GetSyscDomain:inout:::] */
/*    R_PMA_SM_GetSyscDomain => Stub */
//st_sysc_domain_t *AMSTB_R_PMA_SM_GetSyscDomain(e_sysc_domain_id_t syscId)
//{
//#define AMCALLMAX_R_PMA_SM_GetSyscDomain 36
//	static struct st_sysc_domain_t_ *volatile AMIN_return[AMCALLMAX_R_PMA_SM_GetSyscDomain];
//	static e_sysc_domain_id_t volatile AMOUT_sysc_idmax[AMCALLMAX_R_PMA_SM_GetSyscDomain];
//	static e_sysc_domain_id_t volatile AMOUT_syscId[AMCALLMAX_R_PMA_SM_GetSyscDomain];
//	static int volatile AM_count;
//	AMOUT_sysc_idmax[AM_count] = sysc_idmax;
//	AMOUT_syscId[AM_count] = syscId;
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_UpdateLowestPowerState:AMSTB_R_PMA_SM_UpdateLowestPowerState:inout::array:counter<AM_count>] */
/*    R_PMA_SM_UpdateLowestPowerState => Stub */
//e_result_t AMSTB_R_PMA_SM_UpdateLowestPowerState(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaid)
//{
//#define AMCALLMAX_R_PMA_SM_UpdateLowestPowerState 64
//	static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_SM_UpdateLowestPowerState];
//	static struct st_pma_user_info_t *volatile AMOUT_handle[AMCALLMAX_R_PMA_SM_UpdateLowestPowerState];
//	static e_pma_hwa_id_t volatile AMOUT_hwaid[AMCALLMAX_R_PMA_SM_UpdateLowestPowerState];
//	static int volatile AM_count;
//	AMOUT_handle[AM_count] = handle;
//	AMOUT_hwaid[AM_count] = hwaid;
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_PolicyToStatus:AMSTB_R_PMA_SM_PolicyToStatus:inout:::] */
/*    R_PMA_SM_PolicyToStatus => Stub */
//e_result_t AMSTB_R_PMA_SM_PolicyToStatus(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, const st_hwa_t *pHwa, e_pma_power_policy_t policy)
//{
//	static e_result_t volatile AMIN_return;
//	static const st_hwa_func_tbl_t *volatile AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl;
//	static struct st_pma_user_info_t *volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static const st_hwa_t *volatile AMOUT_pHwa;
//	static e_pma_power_policy_t volatile AMOUT_policy;
//	AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl = R_PMA_SM_GstRegStatChangeFuncTbl;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	AMOUT_pHwa = pHwa;
//	AMOUT_policy = policy;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetCpgDomain:AMSTB_R_PMA_SM_GetCpgDomain:inout:::] */
/*    R_PMA_SM_GetCpgDomain => Stub */
//st_cpg_domain_t *AMSTB_R_PMA_SM_GetCpgDomain(e_cpg_domain_id_t cpgId)
//{
//#define AMCALLMAX_R_PMA_SM_GetCpgDomain 66
//	static struct st_cpg_domain_t_ *volatile AMIN_return[AMCALLMAX_R_PMA_SM_GetCpgDomain];
//	static e_cpg_domain_id_t volatile AMOUT_cpg_idmax[AMCALLMAX_R_PMA_SM_GetCpgDomain];
//	static e_cpg_domain_id_t volatile AMOUT_cpgId[AMCALLMAX_R_PMA_SM_GetCpgDomain];
//	static int volatile AM_count;
//	AMOUT_cpg_idmax[AM_count] = cpg_idmax;
//	AMOUT_cpgId[AM_count] = cpgId;
//	AM_count++;
//
//	return AMIN_return[AM_count - 1];
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetupTask:AMSTB_R_PMA_SM_SetupTask:inout:::] */
/*    R_PMA_SM_SetupTask => Stub */
//e_result_t AMSTB_R_PMA_SM_SetupTask(R_PMA_PRIORITY taskPriority)
//{
//	static e_result_t volatile AMIN_return;
//	// static struct st_osal_inner_thread_control *volatile AMOUT_R_PMA_SM_GxThread;
//	static R_PMA_PRIORITY volatile AMOUT_taskPriority;
//	// AMOUT_R_PMA_SM_GxThread = R_PMA_SM_GxThread;
//	AMOUT_taskPriority = taskPriority;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_RequestHWA:AMSTB_R_PMA_SM_RequestHWA:inout:::] */
/*    R_PMA_SM_RequestHWA => Stub */
//e_result_t AMSTB_R_PMA_SM_RequestHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
//{
//	static e_result_t volatile AMIN_return;
//	// static const st_hwa_func_tbl_t *volatile AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	// AMOUT_R_PMA_SM_GstRegStatChangeFuncTbl = R_PMA_SM_GstRegStatChangeFuncTbl;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetLowestPowerState:AMSTB_R_PMA_SM_SetLowestPowerState:inout:::] */
/*    R_PMA_SM_SetLowestPowerState => Stub */
//e_result_t AMSTB_R_PMA_SM_SetLowestPowerState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
//{
//	static e_result_t volatile AMIN_return;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ExChangeErrFromBsp:AMSTB_R_PMA_SM_ExChangeErrFromBsp:inout:::] */
/*    R_PMA_SM_ExChangeErrFromBsp => Stub */
//e_result_t AMSTB_R_PMA_SM_ExChangeErrFromBsp(int32_t errNo)
//{
//	static e_result_t volatile AMIN_return;
//	static int32_t volatile AMOUT_errNo;
//	AMOUT_errNo = errNo;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ApplyResetHWA:AMSTB_R_PMA_SM_ApplyResetHWA:inout:::] */
/*    R_PMA_SM_ApplyResetHWA => Stub */
 //e_result_t AMSTB_R_PMA_SM_ApplyResetHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
 //{
// 	static e_result_t volatile AMIN_return;
 //	static const int *volatile AMOUT_R_PMA_CPG_MOD_ID_TABLE;
 //	static st_pma_handle_t volatile AMOUT_handle;
 //	static e_pma_hwa_id_t volatile AMOUT_hwaId;
 //	AMOUT_R_PMA_CPG_MOD_ID_TABLE = R_PMA_CPG_MOD_ID_TABLE;
 //	AMOUT_handle = handle;
 //	AMOUT_hwaId = hwaId;
 //	return AMIN_return;
 //}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_CheckDepState:AMSTB_R_PMA_SM_CheckDepState:inout:::] */
/*    R_PMA_SM_CheckDepState => Stub */
//e_result_t AMSTB_R_PMA_SM_CheckDepState(st_pma_handle_t handle, uint64_t mask)
//{
//	static e_result_t volatile AMIN_return;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static uint64_t AMOUT_mask;
//	AMOUT_handle = handle;
//	AMOUT_mask = mask;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ChangePolicyHWA:AMSTB_R_PMA_SM_ChangePolicyHWA:inout:::] */
/*    R_PMA_SM_ChangePolicyHWA => Stub */
//e_result_t AMSTB_R_PMA_SM_ChangePolicyHWA(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy)
//{
//	static e_result_t volatile AMIN_return;
//	static st_pma_handle_t volatile AMOUT_handle;
//	static e_pma_hwa_id_t volatile AMOUT_hwaId;
//	static e_pma_power_policy_t volatile AMOUT_policy;
//	AMOUT_handle = handle;
//	AMOUT_hwaId = hwaId;
//	AMOUT_policy = policy;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_osal_power.c:osal_internal_pm_get_state:AMSTB_osal_internal_pm_get_state:inout:::] */
/*    osal_internal_pm_get_state => Stub */
e_osal_return_t AMSTB_osal_internal_pm_get_state(st_pma_handle_t pma_handle, e_pma_hwa_id_t pma_hwaId, e_osal_pm_state_t * const p_state)
{
	static e_osal_return_t volatile AMIN_return;
	static st_pma_handle_t volatile AMOUT_pma_handle;
	static e_pma_hwa_id_t volatile AMOUT_pma_hwaId;
	static e_osal_pm_state_t *volatile AMOUT_p_state;
	static e_osal_pm_state_t volatile AMIN_state;
	
	AMOUT_pma_handle = pma_handle;
	AMOUT_pma_hwaId = pma_hwaId;
	AMOUT_p_state = p_state;
	*p_state = AMIN_state;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_power.c:osal_pm_deinitialize:AMSTB_osal_pm_deinitialize:inout:::] */
/*    osal_pm_deinitialize => Stub */
e_osal_return_t AMSTB_osal_pm_deinitialize(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:osal_internal_free_attr_info:AMSTB_osal_internal_free_attr_info:inout:::] */
/*    osal_internal_free_attr_info => Stub */
e_osal_return_t AMSTB_osal_internal_free_attr_info(void)
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_interrupt.c:osal_internal_timedlock_mutex:AMSTB_osal_internal_timedlock_mutex:inout:::] */
/*    osal_internal_timedlock_mutex => Stub */
e_osal_return_t AMSTB_osal_internal_timedlock_mutex(st_osal_interrupt_thread_control_t *p_info)
{
	static e_osal_return_t volatile AMIN_return;
	static st_osal_interrupt_thread_control_t *volatile AMOUT_p_info;
	static int volatile AM_init_ope;
	
	if (1 == AM_init_ope)
	{
		gs_osal_interrupt_is_initialized_dummy[0] = false;
	}
	AM_init_ope = 0;
	AMOUT_p_info = p_info;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_interrupt.c:osal_internal_unlock_mutex:AMSTB_osal_internal_unlock_mutex:inout:::] */
/*    osal_internal_unlock_mutex => Stub */
e_osal_return_t AMSTB_osal_internal_unlock_mutex(st_osal_interrupt_thread_control_t *p_info)
{
	static int volatile AMIN_return;
	static st_osal_interrupt_thread_control_t *volatile AMOUT_p_info;
	AMOUT_p_info = p_info;
	return AMIN_return;
}

void AMSTB_osal_internal_interrupt_execute_userfunction_irq_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
	static osal_device_handle_t volatile AMOUT_device_handle;
	static uint64_t volatile AMOUT_irq_channel;
	static void* volatile AMOUT_irq_arg;
	return;
}

/* WINAMS_STUB[r_osal_interrupt.c:osal_internal_interrupt_initialize:AMSTB_osal_internal_interrupt_initialize:inout:::] */
/*    osal_internal_interrupt_initialize => Stub */
e_osal_return_t AMSTB_osal_internal_interrupt_initialize(st_osal_interrupt_thread_control_t *p_info, st_osal_device_control_t *p_device_handle, pthread_mutexattr_t *p_mutex_attr)
{
	static int volatile AMIN_return;
	static st_osal_interrupt_thread_control_t *volatile AMOUT_p_info;
	static st_osal_device_control_t *volatile AMOUT_p_device_handle;
	static pthread_mutexattr_t *volatile AMOUT_p_mutex_attr;
	AMOUT_p_info = p_info;
	AMOUT_p_device_handle = p_device_handle;
	AMOUT_p_mutex_attr = p_mutex_attr;
	return AMIN_return;
}

/* Common stub for internal memory functions */
e_osal_return_t AMSTB_osal_mem_internal()
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_GetVersion:AMSTB_R_MEML_GetVersion:inout:::] */
/*    R_MEML_GetVersion => Stub */
const st_meml_version_t * AMSTB_R_MEML_GetVersion(void)
{
	static const st_meml_version_t * volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_Alloc:AMSTB_R_MEML_Alloc:inout:::] */
/*    R_MEML_Alloc => Stub */
uintptr_t AMSTB_R_MEML_Alloc(st_meml_manage_t* const p_manage, size_t size, size_t alignment)
{
	static uintptr_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	static size_t volatile AMOUT_size;
	static size_t volatile AMOUT_alignment;
	*AMOUT_p_manage = *p_manage;
	AMOUT_size = size;
	AMOUT_alignment = alignment;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_Deinitialize:AMSTB_R_MEML_Deinitialize:inout:::] */
/*    R_MEML_Deinitialize => Stub */
e_meml_return_t AMSTB_R_MEML_Deinitialize(st_meml_manage_t* const p_manage)
{
	static e_meml_return_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	*AMOUT_p_manage = *p_manage;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:memory_lib_internal_alloc_stack:AMSTB_memory_lib_internal_alloc_stack:inout:::] */
/*    memory_lib_internal_alloc_stack => Stub */
uintptr_t AMSTB_memory_lib_internal_alloc_stack(st_meml_manage_t * const p_manage, size_t size, size_t alignment)
{
	static uintptr_t volatile AMIN_return;
	static st_meml_manage_t * const volatile AMOUT_p_manage;
	static size_t volatile AMOUT_size;
	static size_t volatile AMOUT_alignment;
	*AMOUT_p_manage = *p_manage;
	AMOUT_size = size;
	AMOUT_alignment = alignment;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_Dealloc:AMSTB_R_MEML_Dealloc:inout:::] */
/*    R_MEML_Dealloc => Stub */
e_meml_return_t AMSTB_R_MEML_Dealloc(st_meml_manage_t* const p_manage, uintptr_t address)
{
	static e_meml_return_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	static uintptr_t volatile AMOUT_address;
	*AMOUT_p_manage = *p_manage;
	AMOUT_address = address;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:memory_lib_internal_alloc_freelist:AMSTB_memory_lib_internal_alloc_freelist:inout:::] */
/*    memory_lib_internal_alloc_freelist => Stub */
uintptr_t AMSTB_memory_lib_internal_alloc_freelist(st_meml_manage_t* const p_manage, size_t size, size_t alignment)
{
	static uintptr_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	static size_t volatile AMOUT_size;
	static size_t volatile AMOUT_alignment;
	*AMOUT_p_manage = *p_manage;
	AMOUT_size = size;
	AMOUT_alignment = alignment;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:memory_lib_internal_dealloc_stack:AMSTB_memory_lib_internal_dealloc_stack:inout:::] */
/*    memory_lib_internal_dealloc_stack => Stub */
e_meml_return_t AMSTB_memory_lib_internal_dealloc_stack(st_meml_manage_t* const p_manage, uintptr_t address)
{
	static e_meml_return_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	static uintptr_t volatile AMOUT_address;
	*AMOUT_p_manage = *p_manage;
	AMOUT_address = address;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_Initialize:AMSTB_R_MEML_Initialize:inout:::] */
/*    R_MEML_Initialize => Stub */
e_meml_return_t AMSTB_R_MEML_Initialize(st_meml_manage_t* const p_manage, uintptr_t pa, uintptr_t va, size_t size, e_meml_alloc_mode_t mode)
{
	static e_meml_return_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	static uintptr_t volatile AMOUT_pa;
	static uintptr_t volatile AMOUT_va;
	static size_t volatile AMOUT_size;
	static e_meml_alloc_mode_t volatile AMOUT_mode;
	*AMOUT_p_manage = *p_manage;
	AMOUT_pa = pa;
	AMOUT_va = va;
	AMOUT_size = size;
	AMOUT_mode = mode;
	return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:memory_lib_internal_dealloc_freelist:AMSTB_memory_lib_internal_dealloc_freelist:inout:::] */
/*    memory_lib_internal_dealloc_freelist => Stub */
e_meml_return_t AMSTB_memory_lib_internal_dealloc_freelist(st_meml_manage_t* const p_manage, uintptr_t address)
{
	static e_meml_return_t volatile AMIN_return;
	static st_meml_manage_t* const volatile AMOUT_p_manage;
	static uintptr_t volatile AMOUT_address;
	*AMOUT_p_manage = *p_manage;
	AMOUT_address = address;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_internal_mq_snd:AMSTB_osal_internal_mq_snd:inout:::] */
/*    osal_internal_mq_snd => Stub */
e_osal_return_t AMSTB_osal_internal_mq_snd(osal_mq_handle_t handle, const void *p_buffer, size_t buffer_size)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_mq_handle_t volatile AMOUT_handle;
	static const void *volatile AMOUT_p_buffer;
	static size_t volatile AMOUT_buffer_size;
	AMOUT_handle = handle;
	AMOUT_p_buffer = p_buffer;
	AMOUT_buffer_size = buffer_size;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_message.c:osal_internal_mq_rcv:AMSTB_osal_internal_mq_rcv:inout:::] */
/*    osal_internal_mq_rcv => Stub */
e_osal_return_t AMSTB_osal_internal_mq_rcv(osal_mq_handle_t handle, void *p_buffer, size_t buffer_size)
{
	static e_osal_return_t volatile AMIN_return;
	static osal_mq_handle_t volatile AMOUT_handle;
	static void *volatile AMOUT_p_buffer;
	static size_t volatile AMOUT_buffer_size;
	AMOUT_handle = handle;
	AMOUT_p_buffer = p_buffer;
	AMOUT_buffer_size = buffer_size;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:realloc:AMSTB_realloc:inout:::] */
/*    realloc => Stub */
void *AMSTB_realloc(void *__ptr, size_t __size)
{
#define AMCALLMAX_realloc 30
    static void *volatile AMIN_return[AMCALLMAX_realloc];
	static void *volatile AMOUT___ptr;
	static size_t volatile AMOUT___size;
    static int volatile AM_count;

	AMOUT___ptr = __ptr;
	AMOUT___size = __size;
    AM_count++;
    
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:cpg_mgr_getfuncs:AMSTB_cpg_mgr_getfuncs:inout:::] */
/*    cpg_mgr_getfuncs => Stub */
int AMSTB_cpg_mgr_getfuncs(struct cpg_mgr_funcs_t *funcs,int tabsize)
{
	static int volatile AMIN_return;
	static struct cpg_mgr_funcs_t *volatile AMOUT_funcs;
	static int volatile AMOUT_tabsize;
	AMOUT_funcs = funcs;
	AMOUT_tabsize = tabsize;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_CalcTimeDifference:AMSTB_R_OSAL_Internal_CalcTimeDifference:inout:::] */
/*    R_OSAL_Internal_CalcTimeDifference => Stub */
int AMSTB_R_OSAL_Internal_CalcTimeDifference(struct st_osal_time *p_time2,struct st_osal_time *p_time1,long long *p_time_difference)
{
	static int volatile AMIN_return;
	static struct st_osal_time *volatile AMOUT_p_time2;
	static struct st_osal_time *volatile AMOUT_p_time1;
	static long long *volatile AMOUT_p_time_difference;
	AMOUT_p_time2 = p_time2;
	AMOUT_p_time1 = p_time1;
	AMOUT_p_time_difference = p_time_difference;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_GetHighResoTimeStamp:AMSTB_R_OSAL_Internal_GetHighResoTimeStamp:inout:::] */
/*    R_OSAL_Internal_GetHighResoTimeStamp => Stub */
int AMSTB_R_OSAL_Internal_GetHighResoTimeStamp(struct st_osal_time *p_time_stamp)
{
	static int volatile AMIN_return;
	static struct st_osal_time *volatile AMOUT_p_time_stamp;
	AMOUT_p_time_stamp = p_time_stamp;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_internal_cond_initialize:AMSTB_osal_internal_cond_initialize:inout:::] */
/*    osal_internal_cond_initialize => Stub */
int AMSTB_osal_internal_cond_initialize()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_internal_cond_deinitialize_pre:AMSTB_osal_internal_cond_deinitialize_pre:inout:::] */
/*    osal_internal_cond_deinitialize_pre => Stub */
int AMSTB_osal_internal_cond_deinitialize_pre()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_internal_cond_deinitialize:AMSTB_osal_internal_cond_deinitialize:inout:::] */
/*    osal_internal_cond_deinitialize => Stub */
int AMSTB_osal_internal_cond_deinitialize()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_internal_mutex_initialize:AMSTB_osal_internal_mutex_initialize:inout:::] */
/*    osal_internal_mutex_initialize => Stub */
int AMSTB_osal_internal_mutex_initialize()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_internal_mutex_deinitialize_pre:AMSTB_osal_internal_mutex_deinitialize_pre:inout:::] */
/*    osal_internal_mutex_deinitialize_pre => Stub */
int AMSTB_osal_internal_mutex_deinitialize_pre()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_threadsync.c:osal_internal_mutex_deinitialize:AMSTB_osal_internal_mutex_deinitialize:inout:::] */
/*    osal_internal_mutex_deinitialize => Stub */
int AMSTB_osal_internal_mutex_deinitialize()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:osal_internal_deinitialize_pre:AMSTB_osal_internal_deinitialize_pre:inout:::] */
/*    osal_internal_deinitialize_pre => Stub */
e_osal_return_t AMSTB_osal_internal_deinitialize_pre()
{
	static e_osal_return_t volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:osal_internal_io_deinitialize:AMSTB_osal_internal_io_deinitialize:inout:::] */
/*    osal_internal_io_deinitialize => Stub */
int AMSTB_osal_internal_io_deinitialize()
{
	static int volatile AMIN_return;
	return AMIN_return;
}

/* WINAMS_STUB[stub.c:InterruptWait_r:AMSTB_InterruptWait_r:inout:::] */
/*    InterruptWait_r => Stub */
int AMSTB_InterruptWait_r(int __flags,unsigned long long *__timeout)
{
#define AMCALLMAX_InterruptWait_r 30
	static int volatile AMIN_return[AMCALLMAX_InterruptWait_r];
	static int volatile AMOUT___flags;
	static unsigned long long *volatile AMOUT___timeout;
	static int volatile AM_count;
	AMOUT___flags = __flags;
	AMOUT___timeout = __timeout;

	AM_count++;
	return AMIN_return[AM_count -1];
}

/* WINAMS_STUB[stub.c:R_OSAL_DCFG_GetDefaultRegionInfo:AMSTB_R_OSAL_DCFG_GetDefaultRegionInfo:inout:::] */
/*    R_OSAL_DCFG_GetDefaultRegionInfo => Stub */
size_t AMSTB_R_OSAL_DCFG_GetDefaultRegionInfo(void)
{
	static size_t volatile AMIN_return;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:R_OSAL_DCFG_GetNumOfMemoryRegionInfo:AMSTB_R_OSAL_DCFG_GetNumOfMemoryRegionInfo:inout:::] */
/*    R_OSAL_DCFG_GetNumOfMemoryRegionInfo => Stub */
osal_memory_region_idx_t AMSTB_R_OSAL_DCFG_GetNumOfMemoryRegionInfo(void)
{
	static osal_memory_region_idx_t volatile AMIN_return;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:posix_devctl:AMSTB_posix_devctl:inout:::] */
/*    posix_devctl => Stub */
int AMSTB_posix_devctl(int fno, int dcmd, void* data, size_t size, int* info)
{
#define AMCALLMAX_posix_devctl 30
	static int volatile AMIN_return[AMCALLMAX_posix_devctl];
	static void * volatile AMIN_data[AMCALLMAX_posix_devctl];
	static int volatile AM_count;
	pdev_memrange_t * p_tmp;

	if (NULL != data)
	{
		if (2U == size)
		{
			*(uint16_t*)data = *(uint16_t*)AMIN_data[AM_count];
		}
		else if (4U == size)
		{
			*(uint32_t*)data = *(uint32_t*)AMIN_data[AM_count];
		}
		else if (sizeof(pdev_memrange_t) == size)
		{
			p_tmp = (pdev_memrange_t*)data;
			p_tmp->base = g_pdevmr.base;
			p_tmp->len  = g_pdevmr.len;
		}
		else
		{
			/* do nothing */
		}
	}

	AM_count++;
	return AMIN_return[AM_count -1];
}

/* WINAMS_STUB[stub.c:uring_interrupt_create:AMSTB_uring_interrupt_create:inout:::] */
/*    uring_interrupt_create => Stub */
int32_t AMSTB_uring_interrupt_create(uring_t *r, int32_t interrupts_count)
{
	static int32_t volatile AMIN_return;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:uring_destroy:AMSTB_uring_destroy:inout:::] */
/*    uring_destroy => Stub */
int32_t AMSTB_uring_destroy(uring_t r)
{
	static int32_t volatile AMIN_return;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:uring_interrupt_notify:AMSTB_uring_interrupt_notify:inout:::] */
/*    uring_interrupt_notify => Stub */
int32_t AMSTB_uring_interrupt_notify(uring_t r, const void *data, size_t data_size)
{
	static int32_t volatile AMIN_return;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:uring_interrupt_receive:AMSTB_uring_interrupt_receive:inout:::] */
/*    uring_interrupt_receive => Stub */
int32_t AMSTB_uring_interrupt_receive(uring_t r, void *data, size_t data_size)
{
	static int32_t volatile AMIN_return;
    static struct lfs_devgetdone volatile AMIN_data;
    struct lfs_devgetdone *data_tmp;
    static int volatile AM_init_ope=0;

    if (1 == AM_init_ope)
    {
        p_gs_osal_interrupt_is_initialized_dummy[0] = 0;
        AM_init_ope = 0;
    }

    if (NULL != data)
    {
        data_tmp = (struct lfs_devgetdone *)data;
        data_tmp->code = AMIN_data.code;
        data_tmp->type = AMIN_data.type;
    }

    return AMIN_return;
}

/* WINAMS_STUB[stub.c:R_OSAL_DCFG_GetNumOfPdevEntry:AMSTB_R_OSAL_DCFG_GetNumOfPdevEntry:inout:::] */
/*    R_OSAL_DCFG_GetNumOfPdevEntry => Stub */
size_t AMSTB_R_OSAL_DCFG_GetNumOfPdevEntry(void)
{
	static size_t volatile AMIN_return;

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:R_OSAL_DCFG_GetPdevEntryName:AMSTB_R_OSAL_DCFG_GetPdevEntryName:inout:::] */
/*    R_OSAL_DCFG_GetPdevEntryName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_GetPdevEntryName(uint32_t num, const char **name)
{
	static e_osal_return_t volatile AMIN_return;
	static char * volatile AMIN_name;

	if (NULL != name)
	{
		*name = AMIN_name;
	}

	return AMIN_return;
}

/* WINAMS_STUB[stub.c:open:AMSTB_open:inout:::] */
/*    open => Stub */
int AMSTB_open(const char *pathname, int flags)
{
	static int volatile AMIN_return;

	return AMIN_return;
}

st_osal_device_info_t gs_osal_device_info_dummy[OSAL_DCFG_DEVICE_MAX_NUM + 1] = /* 384+1 */
{
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 10 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 20 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 30 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 40 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 50 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 60 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 70 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 80 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 90 */
    {.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 100 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 110 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 120 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 130 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 140 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 150 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 160 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 170 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 180 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 190 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 200 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 210 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 220 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 230 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 240 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 250 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 260 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 270 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 280 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 290 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 300 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 310 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 320 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 330 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 340 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 350 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 360 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 370 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4},	{.id = 5},	{.id = 6},	{.id = 7},	{.id = 8},	{.id = 9},	{.id = 10}, /* 380 */
	{.id = 1},	{.id = 2},	{.id = 3},	{.id = 4}, /* 384 */
	{.id = OSAL_DEVICE_INVALID_ID},
};

const st_osal_rcfg_thread_config_t g_osal_thread_config_dummy[] =
{
    /* thread id,                   fucn, userarg, priority,                 task_name,          stack_size */
    {0U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {3U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {4U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {5U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {6U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {7U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {8U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {9U,                            {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {10U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {11U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {12U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {13U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {14U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {15U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {16U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {17U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {18U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {19U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {20U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {21U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {22U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {23U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {24U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {25U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {26U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {27U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {28U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {29U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {30U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {31U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {32U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {33U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {34U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {35U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {36U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {37U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {38U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {39U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {40U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {41U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {42U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {43U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {44U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {45U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {46U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {47U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {48U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {49U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {50U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {51U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {52U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {53U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {54U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {55U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {56U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {57U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {58U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {59U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {60U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {61U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {62U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {63U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {64U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {65U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {66U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {67U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {68U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {69U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {70U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {71U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {72U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {73U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {74U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {75U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {76U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {77U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {78U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {79U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {80U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {81U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {82U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {83U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {84U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {85U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {86U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {87U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {88U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {89U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {90U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {91U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {92U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {93U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {94U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {95U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {96U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {97U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {98U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {99U,                           {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {100U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {101U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {102U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {103U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {104U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {105U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {106U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {107U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {108U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {109U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {110U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {111U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {112U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {113U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {114U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {115U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {116U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {117U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {118U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {119U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {120U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {121U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {122U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {123U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {124U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {125U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {126U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {127U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {128U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {129U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {130U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {131U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {132U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {133U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {134U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {135U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {136U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {137U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {138U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {139U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {140U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {141U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {142U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {143U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {144U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {145U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {146U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {147U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {148U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {149U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {150U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {151U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {152U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {153U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {154U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {155U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {156U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {157U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {158U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {159U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {160U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {161U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {162U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {163U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {164U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {165U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {166U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {167U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {168U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {169U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {170U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {171U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {172U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {173U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {174U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {175U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {176U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {177U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {178U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {179U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {180U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {181U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {182U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {183U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {184U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {185U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {186U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {187U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {188U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {189U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {190U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {191U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {192U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {193U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {194U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {195U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {196U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {197U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {198U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {199U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {200U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {201U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {202U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {203U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {204U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {205U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {206U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {207U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {208U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {209U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {210U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {211U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {212U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {213U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {214U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {215U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {216U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {217U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {218U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {219U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {220U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {221U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {222U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {223U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {224U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {225U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {226U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {227U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {228U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {229U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {230U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {231U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {232U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {233U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {234U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {235U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {236U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {237U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {238U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {239U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {240U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {241U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {242U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {243U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {244U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {245U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {246U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {247U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {248U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {249U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {250U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {251U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {252U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {253U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {254U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {255U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {256U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {257U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {258U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {259U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {260U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {261U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {262U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {263U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {264U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {265U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {266U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {267U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {268U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {269U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {270U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {271U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {272U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {273U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {274U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {275U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {276U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {277U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {278U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {279U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {280U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {281U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {282U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {283U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {284U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {285U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {286U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {287U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {288U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {289U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {290U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {291U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {292U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {293U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {294U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {295U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {296U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {297U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {298U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {299U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {300U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {301U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {302U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {303U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {304U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {305U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {306U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {307U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {308U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {309U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {310U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {311U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {312U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {313U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {314U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {315U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {316U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {317U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {318U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {319U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {320U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {321U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {322U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {323U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {324U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {325U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {326U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {327U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {328U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {329U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {330U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {331U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {332U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {333U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {334U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {335U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {336U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {337U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {338U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {339U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {340U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {341U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {342U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {343U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {344U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {345U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {346U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {347U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {348U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {349U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {350U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {351U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {352U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {353U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {354U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {355U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {356U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {357U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {358U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {359U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {360U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {361U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {362U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {363U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {364U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {365U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {366U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {367U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {368U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {369U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {370U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {371U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {372U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {373U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {374U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {375U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {376U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {377U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {378U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {379U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {380U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {381U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {382U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {383U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {384U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {385U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {386U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {387U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {388U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {389U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {390U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {391U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {392U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {393U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {394U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {395U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {396U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {397U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {398U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {399U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {400U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {401U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {402U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {403U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {404U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {405U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {406U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {407U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {408U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {409U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {410U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {411U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {412U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {413U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {414U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {415U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {416U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {417U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {418U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {419U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {420U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {421U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {422U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {423U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {424U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {425U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {426U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {427U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {428U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {429U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {430U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {431U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {432U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {433U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {434U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {435U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {436U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {437U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {438U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {439U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {440U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {441U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {442U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {443U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {444U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {445U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {446U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {447U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {448U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {449U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {450U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {451U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {452U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {453U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {454U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {455U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {456U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {457U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {458U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {459U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {460U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {461U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {462U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {463U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {464U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {465U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {466U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {467U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {468U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {469U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {470U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {471U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {472U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {473U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {474U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {475U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {476U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {477U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {478U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {479U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {480U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {481U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {482U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {483U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {484U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {485U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {486U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {487U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {488U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {489U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {490U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {491U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {492U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {493U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {494U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {495U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {496U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {497U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {498U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {499U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {500U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {501U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {502U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {503U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {504U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {505U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {506U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {507U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {508U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {509U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {510U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {511U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {512U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {513U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {514U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {515U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {516U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {517U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {518U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {519U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {520U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {521U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {522U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {523U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {524U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {525U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {526U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {527U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {528U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {529U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {530U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {531U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {532U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {533U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {534U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {535U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {536U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {537U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {538U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {539U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {540U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {541U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {542U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {543U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {544U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {545U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {546U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {547U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {548U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {549U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {550U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {551U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {552U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {553U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {554U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {555U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {556U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {557U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {558U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {559U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {560U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {561U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {562U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {563U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {564U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {565U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {566U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {567U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {568U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {569U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {570U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {571U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {572U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {573U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {574U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {575U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {576U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {577U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {578U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {579U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {580U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {581U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {582U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {583U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {584U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {585U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {586U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {587U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {588U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {589U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {590U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {591U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {592U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {593U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {594U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {595U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {596U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {597U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {598U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {599U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {600U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {601U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {602U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {603U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {604U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {605U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {606U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {607U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {608U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {609U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {610U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {611U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {612U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {613U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {614U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {615U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {616U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {617U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {618U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {619U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {620U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {621U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {622U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {623U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {624U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {625U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {626U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {627U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {628U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {629U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {630U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {631U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {632U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {633U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {634U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {635U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {636U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {637U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {638U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {639U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {640U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {641U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {642U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {643U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {644U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {645U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {646U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {647U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {648U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {649U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {650U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {651U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {652U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {653U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {654U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {655U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {656U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {657U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {658U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {659U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {660U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {661U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {662U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {663U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {664U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {665U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {666U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {667U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {668U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {669U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {670U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {671U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {672U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {673U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {674U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {675U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {676U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {677U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {678U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {679U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {680U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {681U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {682U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {683U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {684U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {685U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {686U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {687U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {688U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {689U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {690U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {691U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {692U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {693U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {694U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {695U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {696U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {697U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {698U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {699U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {700U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {701U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {702U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {703U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {704U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {705U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {706U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {707U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {708U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {709U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {710U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {711U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {712U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {713U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {714U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {715U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {716U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {717U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {718U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {719U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {720U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {721U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {722U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {723U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {724U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {725U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {726U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {727U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {728U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {729U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {730U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {731U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {732U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {733U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {734U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {735U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {736U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {737U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {738U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {739U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {740U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {741U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {742U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {743U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {744U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {745U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {746U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {747U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {748U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {749U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {750U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {751U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {752U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {753U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {754U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {755U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {756U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {757U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {758U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {759U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {760U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {761U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {762U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {763U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {764U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {765U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {766U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {767U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {768U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {769U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {770U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {771U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {772U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {773U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {774U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {775U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {776U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {777U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {778U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {779U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {780U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {781U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {782U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {783U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {784U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {785U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {786U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {787U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {788U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {789U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {790U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {791U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {792U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {793U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {794U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {795U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {796U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {797U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {798U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {799U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {800U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {801U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {802U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {803U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {804U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {805U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {806U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {807U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {808U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {809U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {810U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {811U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {812U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {813U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {814U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {815U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {816U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {817U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {818U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {819U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {820U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {821U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {822U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {823U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {824U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {825U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {826U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {827U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {828U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {829U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {830U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {831U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {832U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {833U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {834U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {835U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {836U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {837U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {838U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {839U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {840U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {841U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {842U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {843U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {844U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {845U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {846U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {847U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {848U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {849U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {850U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {851U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {852U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {853U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {854U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {855U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {856U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {857U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {858U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {859U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {860U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {861U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {862U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {863U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {864U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {865U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {866U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {867U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {868U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {869U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {870U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {871U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {872U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {873U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {874U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {875U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {876U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {877U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {878U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {879U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {880U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {881U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {882U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {883U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {884U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {885U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {886U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {887U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {888U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {889U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {890U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {891U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {892U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {893U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {894U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {895U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {896U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {897U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {898U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {899U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {900U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {901U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {902U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {903U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {904U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {905U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {906U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {907U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {908U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {909U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {910U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {911U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {912U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {913U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {914U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {915U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {916U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {917U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {918U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {919U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {920U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {921U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {922U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {923U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {924U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {925U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {926U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {927U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {928U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {929U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {930U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {931U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {932U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {933U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {934U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {935U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {936U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {937U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {938U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {939U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {940U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {941U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {942U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {943U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {944U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {945U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {946U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {947U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {948U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {949U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {950U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {951U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {952U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {953U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {954U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {955U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {956U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {957U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {958U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {959U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {960U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {961U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {962U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {963U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {964U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {965U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {966U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {967U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {968U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {969U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {970U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {971U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {972U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {973U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {974U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {975U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {976U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {977U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {978U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {979U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {980U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {981U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {982U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {983U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {984U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {985U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {986U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {987U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {988U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {989U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {990U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {991U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {992U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {993U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {994U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {995U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {996U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {997U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {998U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {999U,                          {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1000U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1001U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1002U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1003U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1004U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1005U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1006U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1007U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1008U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1009U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1010U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1011U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1012U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1013U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1014U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1015U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1016U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1017U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1018U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1019U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1020U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1021U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1022U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1023U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1024U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1025U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1026U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1027U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1028U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1029U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1030U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1031U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1032U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1033U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1034U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1035U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1036U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1037U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1038U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1039U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1040U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1041U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1042U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1043U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1044U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1045U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1046U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1047U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1048U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1049U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1050U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1051U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1052U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1053U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1054U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1055U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1056U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1057U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1058U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1059U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1060U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1061U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1062U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1063U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1064U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1065U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1066U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1067U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1068U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1069U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1070U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1071U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1072U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1073U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1074U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1075U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1076U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1077U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1078U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1079U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1080U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1081U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1082U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1083U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1084U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1085U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1086U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1087U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1088U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1089U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1090U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1091U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1092U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1093U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1094U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1095U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1096U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1097U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1098U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1099U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1100U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1101U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1102U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1103U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1104U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1105U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1106U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1107U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1108U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1109U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1110U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1111U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1112U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1113U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1114U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1115U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1116U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1117U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1118U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1119U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1120U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1121U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1122U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1123U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1124U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1125U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1126U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1127U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1128U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1129U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1130U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1131U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1132U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1133U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1134U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1135U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1136U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1137U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1138U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1139U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1140U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1141U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1142U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1143U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1144U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1145U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1146U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1147U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1148U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1149U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1150U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1151U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1152U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1153U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1154U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1155U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1156U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1157U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1158U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1159U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1160U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1161U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1162U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1163U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1164U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1165U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1166U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1167U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1168U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1169U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1170U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1171U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1172U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1173U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1174U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1175U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1176U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1177U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1178U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1179U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1180U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1181U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1182U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1183U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1184U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1185U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1186U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1187U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1188U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1189U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1190U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1191U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1192U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1193U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1194U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1195U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1196U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1197U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1198U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1199U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1200U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1201U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1202U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1203U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1204U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1205U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1206U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1207U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1208U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1209U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1210U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1211U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1212U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1213U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1214U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1215U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1216U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1217U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1218U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1219U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1220U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1221U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1222U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1223U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1224U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1225U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1226U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1227U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1228U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1229U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1230U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1231U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1232U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1233U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1234U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1235U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1236U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1237U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1238U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1239U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1240U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1241U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1242U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1243U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1244U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1245U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1246U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1247U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1248U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1249U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1250U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1251U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1252U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1253U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1254U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1255U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1256U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1257U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1258U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1259U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1260U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1261U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1262U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1263U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1264U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1265U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1266U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1267U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1268U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1269U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1270U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1271U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1272U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1273U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1274U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1275U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1276U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1277U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1278U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1279U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1280U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1281U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1282U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1283U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1284U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1285U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1286U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1287U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1288U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1289U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1290U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1291U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1292U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1293U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1294U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1295U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1296U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1297U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1298U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1299U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1300U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1301U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1302U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1303U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1304U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1305U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1306U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1307U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1308U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1309U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1310U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1311U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1312U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1313U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1314U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1315U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1316U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1317U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1318U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1319U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1320U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1321U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1322U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1323U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1324U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1325U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1326U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1327U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1328U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1329U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1330U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1331U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1332U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1333U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1334U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1335U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1336U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1337U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1338U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1339U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1340U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1341U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1342U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1343U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1344U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1345U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1346U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1347U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1348U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1349U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1350U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1351U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1352U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1353U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1354U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1355U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1356U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1357U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1358U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1359U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1360U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1361U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1362U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1363U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1364U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1365U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1366U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1367U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1368U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1369U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1370U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1371U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1372U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1373U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1374U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1375U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1376U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1377U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1378U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1379U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1380U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1381U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1382U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1383U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1384U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1385U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1386U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1387U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1388U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1389U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1390U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1391U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1392U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1393U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1394U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1395U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1396U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1397U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1398U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1399U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1400U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1401U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1402U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1403U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1404U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1405U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1406U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1407U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1408U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1409U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1410U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1411U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1412U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1413U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1414U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1415U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1416U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1417U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1418U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1419U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1420U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1421U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1422U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1423U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1424U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1425U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1426U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1427U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1428U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1429U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1430U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1431U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1432U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1433U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1434U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1435U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1436U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1437U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1438U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1439U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1440U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1441U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1442U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1443U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1444U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1445U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1446U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1447U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1448U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1449U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1450U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1451U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1452U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1453U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1454U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1455U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1456U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1457U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1458U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1459U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1460U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1461U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1462U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1463U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1464U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1465U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1466U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1467U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1468U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1469U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1470U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1471U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1472U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1473U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1474U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1475U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1476U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1477U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1478U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1479U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1480U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1481U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1482U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1483U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1484U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1485U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1486U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1487U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1488U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1489U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1490U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1491U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1492U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1493U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1494U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1495U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1496U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1497U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1498U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1499U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1500U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1501U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1502U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1503U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1504U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1505U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1506U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1507U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1508U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1509U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1510U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1511U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1512U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1513U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1514U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1515U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1516U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1517U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1518U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1519U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1520U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1521U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1522U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1523U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1524U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1525U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1526U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1527U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1528U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1529U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1530U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1531U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1532U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1533U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1534U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1535U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1536U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1537U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1538U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1539U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1540U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1541U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1542U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1543U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1544U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1545U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1546U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1547U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1548U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1549U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1550U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1551U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1552U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1553U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1554U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1555U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1556U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1557U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1558U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1559U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1560U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1561U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1562U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1563U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1564U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1565U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1566U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1567U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1568U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1569U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1570U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1571U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1572U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1573U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1574U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1575U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1576U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1577U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1578U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1579U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1580U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1581U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1582U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1583U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1584U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1585U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1586U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1587U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1588U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1589U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1590U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1591U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1592U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1593U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1594U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1595U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1596U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1597U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1598U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1599U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1600U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1601U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1602U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1603U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1604U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1605U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1606U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1607U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1608U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1609U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1610U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1611U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1612U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1613U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1614U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1615U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1616U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1617U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1618U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1619U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1620U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1621U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1622U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1623U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1624U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1625U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1626U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1627U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1628U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1629U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1630U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1631U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1632U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1633U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1634U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1635U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1636U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1637U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1638U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1639U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1640U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1641U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1642U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1643U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1644U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1645U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1646U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1647U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1648U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1649U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1650U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1651U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1652U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1653U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1654U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1655U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1656U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1657U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1658U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1659U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1660U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1661U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1662U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1663U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1664U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1665U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1666U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1667U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1668U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1669U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1670U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1671U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1672U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1673U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1674U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1675U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1676U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1677U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1678U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1679U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1680U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1681U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1682U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1683U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1684U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1685U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1686U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1687U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1688U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1689U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1690U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1691U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1692U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1693U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1694U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1695U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1696U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1697U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1698U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1699U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1700U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1701U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1702U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1703U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1704U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1705U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1706U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1707U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1708U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1709U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1710U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1711U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1712U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1713U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1714U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1715U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1716U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1717U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1718U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1719U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1720U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1721U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1722U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1723U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1724U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1725U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1726U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1727U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1728U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1729U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1730U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1731U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1732U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1733U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1734U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1735U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1736U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1737U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1738U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1739U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1740U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1741U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1742U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1743U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1744U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1745U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1746U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1747U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1748U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1749U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1750U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1751U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1752U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1753U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1754U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1755U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1756U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1757U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1758U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1759U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1760U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1761U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1762U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1763U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1764U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1765U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1766U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1767U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1768U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1769U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1770U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1771U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1772U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1773U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1774U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1775U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1776U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1777U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1778U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1779U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1780U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1781U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1782U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1783U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1784U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1785U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1786U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1787U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1788U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1789U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1790U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1791U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1792U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1793U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1794U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1795U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1796U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1797U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1798U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1799U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1800U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1801U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1802U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1803U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1804U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1805U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1806U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1807U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1808U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1809U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1810U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1811U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1812U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1813U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1814U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1815U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1816U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1817U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1818U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1819U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1820U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1821U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1822U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1823U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1824U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1825U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1826U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1827U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1828U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1829U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1830U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1831U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1832U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1833U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1834U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1835U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1836U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1837U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1838U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1839U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1840U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1841U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1842U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1843U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1844U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1845U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1846U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1847U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1848U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1849U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1850U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1851U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1852U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1853U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1854U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1855U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1856U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1857U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1858U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1859U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1860U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1861U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1862U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1863U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1864U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1865U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1866U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1867U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1868U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1869U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1870U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1871U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1872U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1873U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1874U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1875U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1876U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1877U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1878U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1879U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1880U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1881U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1882U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1883U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1884U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1885U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1886U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1887U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1888U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1889U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1890U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1891U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1892U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1893U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1894U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1895U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1896U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1897U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1898U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1899U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1900U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1901U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1902U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1903U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1904U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1905U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1906U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1907U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1908U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1909U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1910U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1911U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1912U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1913U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1914U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1915U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1916U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1917U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1918U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1919U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1920U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1921U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1922U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1923U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1924U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1925U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1926U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1927U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1928U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1929U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1930U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1931U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1932U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1933U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1934U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1935U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1936U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1937U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1938U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1939U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1940U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1941U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1942U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1943U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1944U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1945U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1946U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1947U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1948U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1949U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1950U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1951U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1952U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1953U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1954U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1955U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1956U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1957U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1958U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1959U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1960U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1961U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1962U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1963U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1964U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1965U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1966U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1967U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1968U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1969U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1970U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1971U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1972U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1973U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1974U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1975U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1976U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1977U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1978U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1979U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1980U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1981U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1982U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1983U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1984U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1985U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1986U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1987U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1988U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1989U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1990U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1991U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1992U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1993U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1994U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1995U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1996U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1997U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1998U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {1999U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2000U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2001U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2002U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2003U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2004U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2005U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2006U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2007U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2008U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2009U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2010U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2011U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2012U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2013U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2014U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2015U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2016U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2017U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2018U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2019U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2020U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2021U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2022U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2023U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2024U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2025U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2026U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2027U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2028U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2029U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2030U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2031U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2032U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2033U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2034U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2035U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2036U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2037U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2038U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2039U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2040U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2041U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2042U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2043U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2044U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2045U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2046U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2047U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    {2048U,                         {NULL, NULL, OSAL_THREAD_PRIORITY_TYPE10, "dummy_task",          0x2000} },
    /* greater than OSAL_RCFG_THREAD_MAX_NUM (2048) */

    {.id = OSAL_THREAD_INVALID_ID}
};

const st_osal_rcfg_mutex_config_t g_osal_mutex_config_dummy[] =
{
    /* mutex id               */
    {0U                         },
    {1U                         },
    {2U                         },
    {3U                         },
    {4U                         },
    {5U                         },
    {6U                         },
    {7U                         },
    {8U                         },
    {9U                         },
    {10U                        },
    {11U                        },
    {12U                        },
    {13U                        },
    {14U                        },
    {15U                        },
    {16U                        },
    {17U                        },
    {18U                        },
    {19U                        },
    {20U                        },
    {21U                        },
    {22U                        },
    {23U                        },
    {24U                        },
    {25U                        },
    {26U                        },
    {27U                        },
    {28U                        },
    {29U                        },
    {30U                        },
    {31U                        },
    {32U                        },
    {33U                        },
    {34U                        },
    {35U                        },
    {36U                        },
    {37U                        },
    {38U                        },
    {39U                        },
    {40U                        },
    {41U                        },
    {42U                        },
    {43U                        },
    {44U                        },
    {45U                        },
    {46U                        },
    {47U                        },
    {48U                        },
    {49U                        },
    {50U                        },
    {51U                        },
    {52U                        },
    {53U                        },
    {54U                        },
    {55U                        },
    {56U                        },
    {57U                        },
    {58U                        },
    {59U                        },
    {60U                        },
    {61U                        },
    {62U                        },
    {63U                        },
    {64U                        },
    {65U                        },
    {66U                        },
    {67U                        },
    {68U                        },
    {69U                        },
    {70U                        },
    {71U                        },
    {72U                        },
    {73U                        },
    {74U                        },
    {75U                        },
    {76U                        },
    {77U                        },
    {78U                        },
    {79U                        },
    {80U                        },
    {81U                        },
    {82U                        },
    {83U                        },
    {84U                        },
    {85U                        },
    {86U                        },
    {87U                        },
    {88U                        },
    {89U                        },
    {90U                        },
    {91U                        },
    {92U                        },
    {93U                        },
    {94U                        },
    {95U                        },
    {96U                        },
    {97U                        },
    {98U                        },
    {99U                        },
    {100U                       },
    {101U                       },
    {102U                       },
    {103U                       },
    {104U                       },
    {105U                       },
    {106U                       },
    {107U                       },
    {108U                       },
    {109U                       },
    {110U                       },
    {111U                       },
    {112U                       },
    {113U                       },
    {114U                       },
    {115U                       },
    {116U                       },
    {117U                       },
    {118U                       },
    {119U                       },
    {120U                       },
    {121U                       },
    {122U                       },
    {123U                       },
    {124U                       },
    {125U                       },
    {126U                       },
    {127U                       },
    {128U                       },
    {129U                       },
    {130U                       },
    {131U                       },
    {132U                       },
    {133U                       },
    {134U                       },
    {135U                       },
    {136U                       },
    {137U                       },
    {138U                       },
    {139U                       },
    {140U                       },
    {141U                       },
    {142U                       },
    {143U                       },
    {144U                       },
    {145U                       },
    {146U                       },
    {147U                       },
    {148U                       },
    {149U                       },
    {150U                       },
    {151U                       },
    {152U                       },
    {153U                       },
    {154U                       },
    {155U                       },
    {156U                       },
    {157U                       },
    {158U                       },
    {159U                       },
    {160U                       },
    {161U                       },
    {162U                       },
    {163U                       },
    {164U                       },
    {165U                       },
    {166U                       },
    {167U                       },
    {168U                       },
    {169U                       },
    {170U                       },
    {171U                       },
    {172U                       },
    {173U                       },
    {174U                       },
    {175U                       },
    {176U                       },
    {177U                       },
    {178U                       },
    {179U                       },
    {180U                       },
    {181U                       },
    {182U                       },
    {183U                       },
    {184U                       },
    {185U                       },
    {186U                       },
    {187U                       },
    {188U                       },
    {189U                       },
    {190U                       },
    {191U                       },
    {192U                       },
    {193U                       },
    {194U                       },
    {195U                       },
    {196U                       },
    {197U                       },
    {198U                       },
    {199U                       },
    {200U                       },
    {201U                       },
    {202U                       },
    {203U                       },
    {204U                       },
    {205U                       },
    {206U                       },
    {207U                       },
    {208U                       },
    {209U                       },
    {210U                       },
    {211U                       },
    {212U                       },
    {213U                       },
    {214U                       },
    {215U                       },
    {216U                       },
    {217U                       },
    {218U                       },
    {219U                       },
    {220U                       },
    {221U                       },
    {222U                       },
    {223U                       },
    {224U                       },
    {225U                       },
    {226U                       },
    {227U                       },
    {228U                       },
    {229U                       },
    {230U                       },
    {231U                       },
    {232U                       },
    {233U                       },
    {234U                       },
    {235U                       },
    {236U                       },
    {237U                       },
    {238U                       },
    {239U                       },
    {240U                       },
    {241U                       },
    {242U                       },
    {243U                       },
    {244U                       },
    {245U                       },
    {246U                       },
    {247U                       },
    {248U                       },
    {249U                       },
    {250U                       },
    {251U                       },
    {252U                       },
    {253U                       },
    {254U                       },
    {255U                       },
    {256U                       },
    {257U                       },
    {258U                       },
    {259U                       },
    {260U                       },
    {261U                       },
    {262U                       },
    {263U                       },
    {264U                       },
    {265U                       },
    {266U                       },
    {267U                       },
    {268U                       },
    {269U                       },
    {270U                       },
    {271U                       },
    {272U                       },
    {273U                       },
    {274U                       },
    {275U                       },
    {276U                       },
    {277U                       },
    {278U                       },
    {279U                       },
    {280U                       },
    {281U                       },
    {282U                       },
    {283U                       },
    {284U                       },
    {285U                       },
    {286U                       },
    {287U                       },
    {288U                       },
    {289U                       },
    {290U                       },
    {291U                       },
    {292U                       },
    {293U                       },
    {294U                       },
    {295U                       },
    {296U                       },
    {297U                       },
    {298U                       },
    {299U                       },
    {300U                       },
    {301U                       },
    {302U                       },
    {303U                       },
    {304U                       },
    {305U                       },
    {306U                       },
    {307U                       },
    {308U                       },
    {309U                       },
    {310U                       },
    {311U                       },
    {312U                       },
    {313U                       },
    {314U                       },
    {315U                       },
    {316U                       },
    {317U                       },
    {318U                       },
    {319U                       },
    {320U                       },
    {321U                       },
    {322U                       },
    {323U                       },
    {324U                       },
    {325U                       },
    {326U                       },
    {327U                       },
    {328U                       },
    {329U                       },
    {330U                       },
    {331U                       },
    {332U                       },
    {333U                       },
    {334U                       },
    {335U                       },
    {336U                       },
    {337U                       },
    {338U                       },
    {339U                       },
    {340U                       },
    {341U                       },
    {342U                       },
    {343U                       },
    {344U                       },
    {345U                       },
    {346U                       },
    {347U                       },
    {348U                       },
    {349U                       },
    {350U                       },
    {351U                       },
    {352U                       },
    {353U                       },
    {354U                       },
    {355U                       },
    {356U                       },
    {357U                       },
    {358U                       },
    {359U                       },
    {360U                       },
    {361U                       },
    {362U                       },
    {363U                       },
    {364U                       },
    {365U                       },
    {366U                       },
    {367U                       },
    {368U                       },
    {369U                       },
    {370U                       },
    {371U                       },
    {372U                       },
    {373U                       },
    {374U                       },
    {375U                       },
    {376U                       },
    {377U                       },
    {378U                       },
    {379U                       },
    {380U                       },
    {381U                       },
    {382U                       },
    {383U                       },
    {384U                       },
    {385U                       },
    {386U                       },
    {387U                       },
    {388U                       },
    {389U                       },
    {390U                       },
    {391U                       },
    {392U                       },
    {393U                       },
    {394U                       },
    {395U                       },
    {396U                       },
    {397U                       },
    {398U                       },
    {399U                       },
    {400U                       },
    {401U                       },
    {402U                       },
    {403U                       },
    {404U                       },
    {405U                       },
    {406U                       },
    {407U                       },
    {408U                       },
    {409U                       },
    {410U                       },
    {411U                       },
    {412U                       },
    {413U                       },
    {414U                       },
    {415U                       },
    {416U                       },
    {417U                       },
    {418U                       },
    {419U                       },
    {420U                       },
    {421U                       },
    {422U                       },
    {423U                       },
    {424U                       },
    {425U                       },
    {426U                       },
    {427U                       },
    {428U                       },
    {429U                       },
    {430U                       },
    {431U                       },
    {432U                       },
    {433U                       },
    {434U                       },
    {435U                       },
    {436U                       },
    {437U                       },
    {438U                       },
    {439U                       },
    {440U                       },
    {441U                       },
    {442U                       },
    {443U                       },
    {444U                       },
    {445U                       },
    {446U                       },
    {447U                       },
    {448U                       },
    {449U                       },
    {450U                       },
    {451U                       },
    {452U                       },
    {453U                       },
    {454U                       },
    {455U                       },
    {456U                       },
    {457U                       },
    {458U                       },
    {459U                       },
    {460U                       },
    {461U                       },
    {462U                       },
    {463U                       },
    {464U                       },
    {465U                       },
    {466U                       },
    {467U                       },
    {468U                       },
    {469U                       },
    {470U                       },
    {471U                       },
    {472U                       },
    {473U                       },
    {474U                       },
    {475U                       },
    {476U                       },
    {477U                       },
    {478U                       },
    {479U                       },
    {480U                       },
    {481U                       },
    {482U                       },
    {483U                       },
    {484U                       },
    {485U                       },
    {486U                       },
    {487U                       },
    {488U                       },
    {489U                       },
    {490U                       },
    {491U                       },
    {492U                       },
    {493U                       },
    {494U                       },
    {495U                       },
    {496U                       },
    {497U                       },
    {498U                       },
    {499U                       },
    {500U                       },
    {501U                       },
    {502U                       },
    {503U                       },
    {504U                       },
    {505U                       },
    {506U                       },
    {507U                       },
    {508U                       },
    {509U                       },
    {510U                       },
    {511U                       },
    {512U                       },
    {513U                       },
    {514U                       },
    {515U                       },
    {516U                       },
    {517U                       },
    {518U                       },
    {519U                       },
    {520U                       },
    {521U                       },
    {522U                       },
    {523U                       },
    {524U                       },
    {525U                       },
    {526U                       },
    {527U                       },
    {528U                       },
    {529U                       },
    {530U                       },
    {531U                       },
    {532U                       },
    {533U                       },
    {534U                       },
    {535U                       },
    {536U                       },
    {537U                       },
    {538U                       },
    {539U                       },
    {540U                       },
    {541U                       },
    {542U                       },
    {543U                       },
    {544U                       },
    {545U                       },
    {546U                       },
    {547U                       },
    {548U                       },
    {549U                       },
    {550U                       },
    {551U                       },
    {552U                       },
    {553U                       },
    {554U                       },
    {555U                       },
    {556U                       },
    {557U                       },
    {558U                       },
    {559U                       },
    {560U                       },
    {561U                       },
    {562U                       },
    {563U                       },
    {564U                       },
    {565U                       },
    {566U                       },
    {567U                       },
    {568U                       },
    {569U                       },
    {570U                       },
    {571U                       },
    {572U                       },
    {573U                       },
    {574U                       },
    {575U                       },
    {576U                       },
    {577U                       },
    {578U                       },
    {579U                       },
    {580U                       },
    {581U                       },
    {582U                       },
    {583U                       },
    {584U                       },
    {585U                       },
    {586U                       },
    {587U                       },
    {588U                       },
    {589U                       },
    {590U                       },
    {591U                       },
    {592U                       },
    {593U                       },
    {594U                       },
    {595U                       },
    {596U                       },
    {597U                       },
    {598U                       },
    {599U                       },
    {600U                       },
    {601U                       },
    {602U                       },
    {603U                       },
    {604U                       },
    {605U                       },
    {606U                       },
    {607U                       },
    {608U                       },
    {609U                       },
    {610U                       },
    {611U                       },
    {612U                       },
    {613U                       },
    {614U                       },
    {615U                       },
    {616U                       },
    {617U                       },
    {618U                       },
    {619U                       },
    {620U                       },
    {621U                       },
    {622U                       },
    {623U                       },
    {624U                       },
    {625U                       },
    {626U                       },
    {627U                       },
    {628U                       },
    {629U                       },
    {630U                       },
    {631U                       },
    {632U                       },
    {633U                       },
    {634U                       },
    {635U                       },
    {636U                       },
    {637U                       },
    {638U                       },
    {639U                       },
    {640U                       },
    {641U                       },
    {642U                       },
    {643U                       },
    {644U                       },
    {645U                       },
    {646U                       },
    {647U                       },
    {648U                       },
    {649U                       },
    {650U                       },
    {651U                       },
    {652U                       },
    {653U                       },
    {654U                       },
    {655U                       },
    {656U                       },
    {657U                       },
    {658U                       },
    {659U                       },
    {660U                       },
    {661U                       },
    {662U                       },
    {663U                       },
    {664U                       },
    {665U                       },
    {666U                       },
    {667U                       },
    {668U                       },
    {669U                       },
    {670U                       },
    {671U                       },
    {672U                       },
    {673U                       },
    {674U                       },
    {675U                       },
    {676U                       },
    {677U                       },
    {678U                       },
    {679U                       },
    {680U                       },
    {681U                       },
    {682U                       },
    {683U                       },
    {684U                       },
    {685U                       },
    {686U                       },
    {687U                       },
    {688U                       },
    {689U                       },
    {690U                       },
    {691U                       },
    {692U                       },
    {693U                       },
    {694U                       },
    {695U                       },
    {696U                       },
    {697U                       },
    {698U                       },
    {699U                       },
    {700U                       },
    {701U                       },
    {702U                       },
    {703U                       },
    {704U                       },
    {705U                       },
    {706U                       },
    {707U                       },
    {708U                       },
    {709U                       },
    {710U                       },
    {711U                       },
    {712U                       },
    {713U                       },
    {714U                       },
    {715U                       },
    {716U                       },
    {717U                       },
    {718U                       },
    {719U                       },
    {720U                       },
    {721U                       },
    {722U                       },
    {723U                       },
    {724U                       },
    {725U                       },
    {726U                       },
    {727U                       },
    {728U                       },
    {729U                       },
    {730U                       },
    {731U                       },
    {732U                       },
    {733U                       },
    {734U                       },
    {735U                       },
    {736U                       },
    {737U                       },
    {738U                       },
    {739U                       },
    {740U                       },
    {741U                       },
    {742U                       },
    {743U                       },
    {744U                       },
    {745U                       },
    {746U                       },
    {747U                       },
    {748U                       },
    {749U                       },
    {750U                       },
    {751U                       },
    {752U                       },
    {753U                       },
    {754U                       },
    {755U                       },
    {756U                       },
    {757U                       },
    {758U                       },
    {759U                       },
    {760U                       },
    {761U                       },
    {762U                       },
    {763U                       },
    {764U                       },
    {765U                       },
    {766U                       },
    {767U                       },
    {768U                       },
    {769U                       },
    {770U                       },
    {771U                       },
    {772U                       },
    {773U                       },
    {774U                       },
    {775U                       },
    {776U                       },
    {777U                       },
    {778U                       },
    {779U                       },
    {780U                       },
    {781U                       },
    {782U                       },
    {783U                       },
    {784U                       },
    {785U                       },
    {786U                       },
    {787U                       },
    {788U                       },
    {789U                       },
    {790U                       },
    {791U                       },
    {792U                       },
    {793U                       },
    {794U                       },
    {795U                       },
    {796U                       },
    {797U                       },
    {798U                       },
    {799U                       },
    {800U                       },
    {801U                       },
    {802U                       },
    {803U                       },
    {804U                       },
    {805U                       },
    {806U                       },
    {807U                       },
    {808U                       },
    {809U                       },
    {810U                       },
    {811U                       },
    {812U                       },
    {813U                       },
    {814U                       },
    {815U                       },
    {816U                       },
    {817U                       },
    {818U                       },
    {819U                       },
    {820U                       },
    {821U                       },
    {822U                       },
    {823U                       },
    {824U                       },
    {825U                       },
    {826U                       },
    {827U                       },
    {828U                       },
    {829U                       },
    {830U                       },
    {831U                       },
    {832U                       },
    {833U                       },
    {834U                       },
    {835U                       },
    {836U                       },
    {837U                       },
    {838U                       },
    {839U                       },
    {840U                       },
    {841U                       },
    {842U                       },
    {843U                       },
    {844U                       },
    {845U                       },
    {846U                       },
    {847U                       },
    {848U                       },
    {849U                       },
    {850U                       },
    {851U                       },
    {852U                       },
    {853U                       },
    {854U                       },
    {855U                       },
    {856U                       },
    {857U                       },
    {858U                       },
    {859U                       },
    {860U                       },
    {861U                       },
    {862U                       },
    {863U                       },
    {864U                       },
    {865U                       },
    {866U                       },
    {867U                       },
    {868U                       },
    {869U                       },
    {870U                       },
    {871U                       },
    {872U                       },
    {873U                       },
    {874U                       },
    {875U                       },
    {876U                       },
    {877U                       },
    {878U                       },
    {879U                       },
    {880U                       },
    {881U                       },
    {882U                       },
    {883U                       },
    {884U                       },
    {885U                       },
    {886U                       },
    {887U                       },
    {888U                       },
    {889U                       },
    {890U                       },
    {891U                       },
    {892U                       },
    {893U                       },
    {894U                       },
    {895U                       },
    {896U                       },
    {897U                       },
    {898U                       },
    {899U                       },
    {900U                       },
    {901U                       },
    {902U                       },
    {903U                       },
    {904U                       },
    {905U                       },
    {906U                       },
    {907U                       },
    {908U                       },
    {909U                       },
    {910U                       },
    {911U                       },
    {912U                       },
    {913U                       },
    {914U                       },
    {915U                       },
    {916U                       },
    {917U                       },
    {918U                       },
    {919U                       },
    {920U                       },
    {921U                       },
    {922U                       },
    {923U                       },
    {924U                       },
    {925U                       },
    {926U                       },
    {927U                       },
    {928U                       },
    {929U                       },
    {930U                       },
    {931U                       },
    {932U                       },
    {933U                       },
    {934U                       },
    {935U                       },
    {936U                       },
    {937U                       },
    {938U                       },
    {939U                       },
    {940U                       },
    {941U                       },
    {942U                       },
    {943U                       },
    {944U                       },
    {945U                       },
    {946U                       },
    {947U                       },
    {948U                       },
    {949U                       },
    {950U                       },
    {951U                       },
    {952U                       },
    {953U                       },
    {954U                       },
    {955U                       },
    {956U                       },
    {957U                       },
    {958U                       },
    {959U                       },
    {960U                       },
    {961U                       },
    {962U                       },
    {963U                       },
    {964U                       },
    {965U                       },
    {966U                       },
    {967U                       },
    {968U                       },
    {969U                       },
    {970U                       },
    {971U                       },
    {972U                       },
    {973U                       },
    {974U                       },
    {975U                       },
    {976U                       },
    {977U                       },
    {978U                       },
    {979U                       },
    {980U                       },
    {981U                       },
    {982U                       },
    {983U                       },
    {984U                       },
    {985U                       },
    {986U                       },
    {987U                       },
    {988U                       },
    {989U                       },
    {990U                       },
    {991U                       },
    {992U                       },
    {993U                       },
    {994U                       },
    {995U                       },
    {996U                       },
    {997U                       },
    {998U                       },
    {999U                       },
    {1000U                      },
    {1001U                      },
    {1002U                      },
    {1003U                      },
    {1004U                      },
    {1005U                      },
    {1006U                      },
    {1007U                      },
    {1008U                      },
    {1009U                      },
    {1010U                      },
    {1011U                      },
    {1012U                      },
    {1013U                      },
    {1014U                      },
    {1015U                      },
    {1016U                      },
    {1017U                      },
    {1018U                      },
    {1019U                      },
    {1020U                      },
    {1021U                      },
    {1022U                      },
    {1023U                      },
    {1024U                      },
    /* greater than OSAL_RCFG_MUTEX_MAX_NUM (1024) */

    {.id = OSAL_MUTEX_INVALID_ID}
};

const st_osal_rcfg_cond_config_t g_osal_cond_config_dummy[] =
{
    /* cond id               */
    {0U                       },
    {1U                       },
    {2U                       },
    {3U                       },
    {4U                       },
    {5U                       },
    {6U                       },
    {7U                       },
    {8U                       },
    {9U                       },
    {10U                      },
    {11U                      },
    {12U                      },
    {13U                      },
    {14U                      },
    {15U                      },
    {16U                      },
    {17U                      },
    {18U                      },
    {19U                      },
    {20U                      },
    {21U                      },
    {22U                      },
    {23U                      },
    {24U                      },
    {25U                      },
    {26U                      },
    {27U                      },
    {28U                      },
    {29U                      },
    {30U                      },
    {31U                      },
    {32U                      },
    {33U                      },
    {34U                      },
    {35U                      },
    {36U                      },
    {37U                      },
    {38U                      },
    {39U                      },
    {40U                      },
    {41U                      },
    {42U                      },
    {43U                      },
    {44U                      },
    {45U                      },
    {46U                      },
    {47U                      },
    {48U                      },
    {49U                      },
    {50U                      },
    {51U                      },
    {52U                      },
    {53U                      },
    {54U                      },
    {55U                      },
    {56U                      },
    {57U                      },
    {58U                      },
    {59U                      },
    {60U                      },
    {61U                      },
    {62U                      },
    {63U                      },
    {64U                      },
    {65U                      },
    {66U                      },
    {67U                      },
    {68U                      },
    {69U                      },
    {70U                      },
    {71U                      },
    {72U                      },
    {73U                      },
    {74U                      },
    {75U                      },
    {76U                      },
    {77U                      },
    {78U                      },
    {79U                      },
    {80U                      },
    {81U                      },
    {82U                      },
    {83U                      },
    {84U                      },
    {85U                      },
    {86U                      },
    {87U                      },
    {88U                      },
    {89U                      },
    {90U                      },
    {91U                      },
    {92U                      },
    {93U                      },
    {94U                      },
    {95U                      },
    {96U                      },
    {97U                      },
    {98U                      },
    {99U                      },
    {100U                     },
    {101U                     },
    {102U                     },
    {103U                     },
    {104U                     },
    {105U                     },
    {106U                     },
    {107U                     },
    {108U                     },
    {109U                     },
    {110U                     },
    {111U                     },
    {112U                     },
    {113U                     },
    {114U                     },
    {115U                     },
    {116U                     },
    {117U                     },
    {118U                     },
    {119U                     },
    {120U                     },
    {121U                     },
    {122U                     },
    {123U                     },
    {124U                     },
    {125U                     },
    {126U                     },
    {127U                     },
    {128U                     },
    {129U                     },
    {130U                     },
    {131U                     },
    {132U                     },
    {133U                     },
    {134U                     },
    {135U                     },
    {136U                     },
    {137U                     },
    {138U                     },
    {139U                     },
    {140U                     },
    {141U                     },
    {142U                     },
    {143U                     },
    {144U                     },
    {145U                     },
    {146U                     },
    {147U                     },
    {148U                     },
    {149U                     },
    {150U                     },
    {151U                     },
    {152U                     },
    {153U                     },
    {154U                     },
    {155U                     },
    {156U                     },
    {157U                     },
    {158U                     },
    {159U                     },
    {160U                     },
    {161U                     },
    {162U                     },
    {163U                     },
    {164U                     },
    {165U                     },
    {166U                     },
    {167U                     },
    {168U                     },
    {169U                     },
    {170U                     },
    {171U                     },
    {172U                     },
    {173U                     },
    {174U                     },
    {175U                     },
    {176U                     },
    {177U                     },
    {178U                     },
    {179U                     },
    {180U                     },
    {181U                     },
    {182U                     },
    {183U                     },
    {184U                     },
    {185U                     },
    {186U                     },
    {187U                     },
    {188U                     },
    {189U                     },
    {190U                     },
    {191U                     },
    {192U                     },
    {193U                     },
    {194U                     },
    {195U                     },
    {196U                     },
    {197U                     },
    {198U                     },
    {199U                     },
    {200U                     },
    {201U                     },
    {202U                     },
    {203U                     },
    {204U                     },
    {205U                     },
    {206U                     },
    {207U                     },
    {208U                     },
    {209U                     },
    {210U                     },
    {211U                     },
    {212U                     },
    {213U                     },
    {214U                     },
    {215U                     },
    {216U                     },
    {217U                     },
    {218U                     },
    {219U                     },
    {220U                     },
    {221U                     },
    {222U                     },
    {223U                     },
    {224U                     },
    {225U                     },
    {226U                     },
    {227U                     },
    {228U                     },
    {229U                     },
    {230U                     },
    {231U                     },
    {232U                     },
    {233U                     },
    {234U                     },
    {235U                     },
    {236U                     },
    {237U                     },
    {238U                     },
    {239U                     },
    {240U                     },
    {241U                     },
    {242U                     },
    {243U                     },
    {244U                     },
    {245U                     },
    {246U                     },
    {247U                     },
    {248U                     },
    {249U                     },
    {250U                     },
    {251U                     },
    {252U                     },
    {253U                     },
    {254U                     },
    {255U                     },
    {256U                     },
    {257U                     },
    {258U                     },
    {259U                     },
    {260U                     },
    {261U                     },
    {262U                     },
    {263U                     },
    {264U                     },
    {265U                     },
    {266U                     },
    {267U                     },
    {268U                     },
    {269U                     },
    {270U                     },
    {271U                     },
    {272U                     },
    {273U                     },
    {274U                     },
    {275U                     },
    {276U                     },
    {277U                     },
    {278U                     },
    {279U                     },
    {280U                     },
    {281U                     },
    {282U                     },
    {283U                     },
    {284U                     },
    {285U                     },
    {286U                     },
    {287U                     },
    {288U                     },
    {289U                     },
    {290U                     },
    {291U                     },
    {292U                     },
    {293U                     },
    {294U                     },
    {295U                     },
    {296U                     },
    {297U                     },
    {298U                     },
    {299U                     },
    {300U                     },
    {301U                     },
    {302U                     },
    {303U                     },
    {304U                     },
    {305U                     },
    {306U                     },
    {307U                     },
    {308U                     },
    {309U                     },
    {310U                     },
    {311U                     },
    {312U                     },
    {313U                     },
    {314U                     },
    {315U                     },
    {316U                     },
    {317U                     },
    {318U                     },
    {319U                     },
    {320U                     },
    {321U                     },
    {322U                     },
    {323U                     },
    {324U                     },
    {325U                     },
    {326U                     },
    {327U                     },
    {328U                     },
    {329U                     },
    {330U                     },
    {331U                     },
    {332U                     },
    {333U                     },
    {334U                     },
    {335U                     },
    {336U                     },
    {337U                     },
    {338U                     },
    {339U                     },
    {340U                     },
    {341U                     },
    {342U                     },
    {343U                     },
    {344U                     },
    {345U                     },
    {346U                     },
    {347U                     },
    {348U                     },
    {349U                     },
    {350U                     },
    {351U                     },
    {352U                     },
    {353U                     },
    {354U                     },
    {355U                     },
    {356U                     },
    {357U                     },
    {358U                     },
    {359U                     },
    {360U                     },
    {361U                     },
    {362U                     },
    {363U                     },
    {364U                     },
    {365U                     },
    {366U                     },
    {367U                     },
    {368U                     },
    {369U                     },
    {370U                     },
    {371U                     },
    {372U                     },
    {373U                     },
    {374U                     },
    {375U                     },
    {376U                     },
    {377U                     },
    {378U                     },
    {379U                     },
    {380U                     },
    {381U                     },
    {382U                     },
    {383U                     },
    {384U                     },
    {385U                     },
    {386U                     },
    {387U                     },
    {388U                     },
    {389U                     },
    {390U                     },
    {391U                     },
    {392U                     },
    {393U                     },
    {394U                     },
    {395U                     },
    {396U                     },
    {397U                     },
    {398U                     },
    {399U                     },
    {400U                     },
    {401U                     },
    {402U                     },
    {403U                     },
    {404U                     },
    {405U                     },
    {406U                     },
    {407U                     },
    {408U                     },
    {409U                     },
    {410U                     },
    {411U                     },
    {412U                     },
    {413U                     },
    {414U                     },
    {415U                     },
    {416U                     },
    {417U                     },
    {418U                     },
    {419U                     },
    {420U                     },
    {421U                     },
    {422U                     },
    {423U                     },
    {424U                     },
    {425U                     },
    {426U                     },
    {427U                     },
    {428U                     },
    {429U                     },
    {430U                     },
    {431U                     },
    {432U                     },
    {433U                     },
    {434U                     },
    {435U                     },
    {436U                     },
    {437U                     },
    {438U                     },
    {439U                     },
    {440U                     },
    {441U                     },
    {442U                     },
    {443U                     },
    {444U                     },
    {445U                     },
    {446U                     },
    {447U                     },
    {448U                     },
    {449U                     },
    {450U                     },
    {451U                     },
    {452U                     },
    {453U                     },
    {454U                     },
    {455U                     },
    {456U                     },
    {457U                     },
    {458U                     },
    {459U                     },
    {460U                     },
    {461U                     },
    {462U                     },
    {463U                     },
    {464U                     },
    {465U                     },
    {466U                     },
    {467U                     },
    {468U                     },
    {469U                     },
    {470U                     },
    {471U                     },
    {472U                     },
    {473U                     },
    {474U                     },
    {475U                     },
    {476U                     },
    {477U                     },
    {478U                     },
    {479U                     },
    {480U                     },
    {481U                     },
    {482U                     },
    {483U                     },
    {484U                     },
    {485U                     },
    {486U                     },
    {487U                     },
    {488U                     },
    {489U                     },
    {490U                     },
    {491U                     },
    {492U                     },
    {493U                     },
    {494U                     },
    {495U                     },
    {496U                     },
    {497U                     },
    {498U                     },
    {499U                     },
    {500U                     },
    {501U                     },
    {502U                     },
    {503U                     },
    {504U                     },
    {505U                     },
    {506U                     },
    {507U                     },
    {508U                     },
    {509U                     },
    {510U                     },
    {511U                     },
    {512U                     },
    {513U                     },
    {514U                     },
    {515U                     },
    {516U                     },
    {517U                     },
    {518U                     },
    {519U                     },
    {520U                     },
    {521U                     },
    {522U                     },
    {523U                     },
    {524U                     },
    {525U                     },
    {526U                     },
    {527U                     },
    {528U                     },
    {529U                     },
    {530U                     },
    {531U                     },
    {532U                     },
    {533U                     },
    {534U                     },
    {535U                     },
    {536U                     },
    {537U                     },
    {538U                     },
    {539U                     },
    {540U                     },
    {541U                     },
    {542U                     },
    {543U                     },
    {544U                     },
    {545U                     },
    {546U                     },
    {547U                     },
    {548U                     },
    {549U                     },
    {550U                     },
    {551U                     },
    {552U                     },
    {553U                     },
    {554U                     },
    {555U                     },
    {556U                     },
    {557U                     },
    {558U                     },
    {559U                     },
    {560U                     },
    {561U                     },
    {562U                     },
    {563U                     },
    {564U                     },
    {565U                     },
    {566U                     },
    {567U                     },
    {568U                     },
    {569U                     },
    {570U                     },
    {571U                     },
    {572U                     },
    {573U                     },
    {574U                     },
    {575U                     },
    {576U                     },
    {577U                     },
    {578U                     },
    {579U                     },
    {580U                     },
    {581U                     },
    {582U                     },
    {583U                     },
    {584U                     },
    {585U                     },
    {586U                     },
    {587U                     },
    {588U                     },
    {589U                     },
    {590U                     },
    {591U                     },
    {592U                     },
    {593U                     },
    {594U                     },
    {595U                     },
    {596U                     },
    {597U                     },
    {598U                     },
    {599U                     },
    {600U                     },
    {601U                     },
    {602U                     },
    {603U                     },
    {604U                     },
    {605U                     },
    {606U                     },
    {607U                     },
    {608U                     },
    {609U                     },
    {610U                     },
    {611U                     },
    {612U                     },
    {613U                     },
    {614U                     },
    {615U                     },
    {616U                     },
    {617U                     },
    {618U                     },
    {619U                     },
    {620U                     },
    {621U                     },
    {622U                     },
    {623U                     },
    {624U                     },
    {625U                     },
    {626U                     },
    {627U                     },
    {628U                     },
    {629U                     },
    {630U                     },
    {631U                     },
    {632U                     },
    {633U                     },
    {634U                     },
    {635U                     },
    {636U                     },
    {637U                     },
    {638U                     },
    {639U                     },
    {640U                     },
    {641U                     },
    {642U                     },
    {643U                     },
    {644U                     },
    {645U                     },
    {646U                     },
    {647U                     },
    {648U                     },
    {649U                     },
    {650U                     },
    {651U                     },
    {652U                     },
    {653U                     },
    {654U                     },
    {655U                     },
    {656U                     },
    {657U                     },
    {658U                     },
    {659U                     },
    {660U                     },
    {661U                     },
    {662U                     },
    {663U                     },
    {664U                     },
    {665U                     },
    {666U                     },
    {667U                     },
    {668U                     },
    {669U                     },
    {670U                     },
    {671U                     },
    {672U                     },
    {673U                     },
    {674U                     },
    {675U                     },
    {676U                     },
    {677U                     },
    {678U                     },
    {679U                     },
    {680U                     },
    {681U                     },
    {682U                     },
    {683U                     },
    {684U                     },
    {685U                     },
    {686U                     },
    {687U                     },
    {688U                     },
    {689U                     },
    {690U                     },
    {691U                     },
    {692U                     },
    {693U                     },
    {694U                     },
    {695U                     },
    {696U                     },
    {697U                     },
    {698U                     },
    {699U                     },
    {700U                     },
    {701U                     },
    {702U                     },
    {703U                     },
    {704U                     },
    {705U                     },
    {706U                     },
    {707U                     },
    {708U                     },
    {709U                     },
    {710U                     },
    {711U                     },
    {712U                     },
    {713U                     },
    {714U                     },
    {715U                     },
    {716U                     },
    {717U                     },
    {718U                     },
    {719U                     },
    {720U                     },
    {721U                     },
    {722U                     },
    {723U                     },
    {724U                     },
    {725U                     },
    {726U                     },
    {727U                     },
    {728U                     },
    {729U                     },
    {730U                     },
    {731U                     },
    {732U                     },
    {733U                     },
    {734U                     },
    {735U                     },
    {736U                     },
    {737U                     },
    {738U                     },
    {739U                     },
    {740U                     },
    {741U                     },
    {742U                     },
    {743U                     },
    {744U                     },
    {745U                     },
    {746U                     },
    {747U                     },
    {748U                     },
    {749U                     },
    {750U                     },
    {751U                     },
    {752U                     },
    {753U                     },
    {754U                     },
    {755U                     },
    {756U                     },
    {757U                     },
    {758U                     },
    {759U                     },
    {760U                     },
    {761U                     },
    {762U                     },
    {763U                     },
    {764U                     },
    {765U                     },
    {766U                     },
    {767U                     },
    {768U                     },
    {769U                     },
    {770U                     },
    {771U                     },
    {772U                     },
    {773U                     },
    {774U                     },
    {775U                     },
    {776U                     },
    {777U                     },
    {778U                     },
    {779U                     },
    {780U                     },
    {781U                     },
    {782U                     },
    {783U                     },
    {784U                     },
    {785U                     },
    {786U                     },
    {787U                     },
    {788U                     },
    {789U                     },
    {790U                     },
    {791U                     },
    {792U                     },
    {793U                     },
    {794U                     },
    {795U                     },
    {796U                     },
    {797U                     },
    {798U                     },
    {799U                     },
    {800U                     },
    {801U                     },
    {802U                     },
    {803U                     },
    {804U                     },
    {805U                     },
    {806U                     },
    {807U                     },
    {808U                     },
    {809U                     },
    {810U                     },
    {811U                     },
    {812U                     },
    {813U                     },
    {814U                     },
    {815U                     },
    {816U                     },
    {817U                     },
    {818U                     },
    {819U                     },
    {820U                     },
    {821U                     },
    {822U                     },
    {823U                     },
    {824U                     },
    {825U                     },
    {826U                     },
    {827U                     },
    {828U                     },
    {829U                     },
    {830U                     },
    {831U                     },
    {832U                     },
    {833U                     },
    {834U                     },
    {835U                     },
    {836U                     },
    {837U                     },
    {838U                     },
    {839U                     },
    {840U                     },
    {841U                     },
    {842U                     },
    {843U                     },
    {844U                     },
    {845U                     },
    {846U                     },
    {847U                     },
    {848U                     },
    {849U                     },
    {850U                     },
    {851U                     },
    {852U                     },
    {853U                     },
    {854U                     },
    {855U                     },
    {856U                     },
    {857U                     },
    {858U                     },
    {859U                     },
    {860U                     },
    {861U                     },
    {862U                     },
    {863U                     },
    {864U                     },
    {865U                     },
    {866U                     },
    {867U                     },
    {868U                     },
    {869U                     },
    {870U                     },
    {871U                     },
    {872U                     },
    {873U                     },
    {874U                     },
    {875U                     },
    {876U                     },
    {877U                     },
    {878U                     },
    {879U                     },
    {880U                     },
    {881U                     },
    {882U                     },
    {883U                     },
    {884U                     },
    {885U                     },
    {886U                     },
    {887U                     },
    {888U                     },
    {889U                     },
    {890U                     },
    {891U                     },
    {892U                     },
    {893U                     },
    {894U                     },
    {895U                     },
    {896U                     },
    {897U                     },
    {898U                     },
    {899U                     },
    {900U                     },
    {901U                     },
    {902U                     },
    {903U                     },
    {904U                     },
    {905U                     },
    {906U                     },
    {907U                     },
    {908U                     },
    {909U                     },
    {910U                     },
    {911U                     },
    {912U                     },
    {913U                     },
    {914U                     },
    {915U                     },
    {916U                     },
    {917U                     },
    {918U                     },
    {919U                     },
    {920U                     },
    {921U                     },
    {922U                     },
    {923U                     },
    {924U                     },
    {925U                     },
    {926U                     },
    {927U                     },
    {928U                     },
    {929U                     },
    {930U                     },
    {931U                     },
    {932U                     },
    {933U                     },
    {934U                     },
    {935U                     },
    {936U                     },
    {937U                     },
    {938U                     },
    {939U                     },
    {940U                     },
    {941U                     },
    {942U                     },
    {943U                     },
    {944U                     },
    {945U                     },
    {946U                     },
    {947U                     },
    {948U                     },
    {949U                     },
    {950U                     },
    {951U                     },
    {952U                     },
    {953U                     },
    {954U                     },
    {955U                     },
    {956U                     },
    {957U                     },
    {958U                     },
    {959U                     },
    {960U                     },
    {961U                     },
    {962U                     },
    {963U                     },
    {964U                     },
    {965U                     },
    {966U                     },
    {967U                     },
    {968U                     },
    {969U                     },
    {970U                     },
    {971U                     },
    {972U                     },
    {973U                     },
    {974U                     },
    {975U                     },
    {976U                     },
    {977U                     },
    {978U                     },
    {979U                     },
    {980U                     },
    {981U                     },
    {982U                     },
    {983U                     },
    {984U                     },
    {985U                     },
    {986U                     },
    {987U                     },
    {988U                     },
    {989U                     },
    {990U                     },
    {991U                     },
    {992U                     },
    {993U                     },
    {994U                     },
    {995U                     },
    {996U                     },
    {997U                     },
    {998U                     },
    {999U                     },
    {1000U                    },
    {1001U                    },
    {1002U                    },
    {1003U                    },
    {1004U                    },
    {1005U                    },
    {1006U                    },
    {1007U                    },
    {1008U                    },
    {1009U                    },
    {1010U                    },
    {1011U                    },
    {1012U                    },
    {1013U                    },
    {1014U                    },
    {1015U                    },
    {1016U                    },
    {1017U                    },
    {1018U                    },
    {1019U                    },
    {1020U                    },
    {1021U                    },
    {1022U                    },
    {1023U                    },
    {1024U                    },
    /* greater than OSAL_RCFG_COND_MAX_NUM (1024) */

    {.id = OSAL_COND_INVALID_ID}
};

const st_osal_mq_rcfg_config_t g_osal_mq_config_dummy[] =
{
    /* mq id,                       max_num_msg, msg_size */
    {0U,                                     {1, 4}    },
    {1U,                                     {1, 4}    },
    {2U,                                     {1, 4}    },
    {3U,                                     {1, 4}    },
    {4U,                                     {1, 4}    },
    {5U,                                     {1, 4}    },
    {6U,                                     {1, 4}    },
    {7U,                                     {1, 4}    },
    {8U,                                     {1, 4}    },
    {9U,                                     {1, 4}    },
    {10U,                                    {1, 4}    },
    {11U,                                    {1, 4}    },
    {12U,                                    {1, 4}    },
    {13U,                                    {1, 4}    },
    {14U,                                    {1, 4}    },
    {15U,                                    {1, 4}    },
    {16U,                                    {1, 4}    },
    {17U,                                    {1, 4}    },
    {18U,                                    {1, 4}    },
    {19U,                                    {1, 4}    },
    {20U,                                    {1, 4}    },
    {21U,                                    {1, 4}    },
    {22U,                                    {1, 4}    },
    {23U,                                    {1, 4}    },
    {24U,                                    {1, 4}    },
    {25U,                                    {1, 4}    },
    {26U,                                    {1, 4}    },
    {27U,                                    {1, 4}    },
    {28U,                                    {1, 4}    },
    {29U,                                    {1, 4}    },
    {30U,                                    {1, 4}    },
    {31U,                                    {1, 4}    },
    {32U,                                    {1, 4}    },
    {33U,                                    {1, 4}    },
    {34U,                                    {1, 4}    },
    {35U,                                    {1, 4}    },
    {36U,                                    {1, 4}    },
    {37U,                                    {1, 4}    },
    {38U,                                    {1, 4}    },
    {39U,                                    {1, 4}    },
    {40U,                                    {1, 4}    },
    {41U,                                    {1, 4}    },
    {42U,                                    {1, 4}    },
    {43U,                                    {1, 4}    },
    {44U,                                    {1, 4}    },
    {45U,                                    {1, 4}    },
    {46U,                                    {1, 4}    },
    {47U,                                    {1, 4}    },
    {48U,                                    {1, 4}    },
    {49U,                                    {1, 4}    },
    {50U,                                    {1, 4}    },
    {51U,                                    {1, 4}    },
    {52U,                                    {1, 4}    },
    {53U,                                    {1, 4}    },
    {54U,                                    {1, 4}    },
    {55U,                                    {1, 4}    },
    {56U,                                    {1, 4}    },
    {57U,                                    {1, 4}    },
    {58U,                                    {1, 4}    },
    {59U,                                    {1, 4}    },
    {60U,                                    {1, 4}    },
    {61U,                                    {1, 4}    },
    {62U,                                    {1, 4}    },
    {63U,                                    {1, 4}    },
    {64U,                                    {1, 4}    },
    {65U,                                    {1, 4}    },
    {66U,                                    {1, 4}    },
    {67U,                                    {1, 4}    },
    {68U,                                    {1, 4}    },
    {69U,                                    {1, 4}    },
    {70U,                                    {1, 4}    },
    {71U,                                    {1, 4}    },
    {72U,                                    {1, 4}    },
    {73U,                                    {1, 4}    },
    {74U,                                    {1, 4}    },
    {75U,                                    {1, 4}    },
    {76U,                                    {1, 4}    },
    {77U,                                    {1, 4}    },
    {78U,                                    {1, 4}    },
    {79U,                                    {1, 4}    },
    {80U,                                    {1, 4}    },
    {81U,                                    {1, 4}    },
    {82U,                                    {1, 4}    },
    {83U,                                    {1, 4}    },
    {84U,                                    {1, 4}    },
    {85U,                                    {1, 4}    },
    {86U,                                    {1, 4}    },
    {87U,                                    {1, 4}    },
    {88U,                                    {1, 4}    },
    {89U,                                    {1, 4}    },
    {90U,                                    {1, 4}    },
    {91U,                                    {1, 4}    },
    {92U,                                    {1, 4}    },
    {93U,                                    {1, 4}    },
    {94U,                                    {1, 4}    },
    {95U,                                    {1, 4}    },
    {96U,                                    {1, 4}    },
    {97U,                                    {1, 4}    },
    {98U,                                    {1, 4}    },
    {99U,                                    {1, 4}    },
    {100U,                                   {1, 4}    },
    {101U,                                   {1, 4}    },
    {102U,                                   {1, 4}    },
    {103U,                                   {1, 4}    },
    {104U,                                   {1, 4}    },
    {105U,                                   {1, 4}    },
    {106U,                                   {1, 4}    },
    {107U,                                   {1, 4}    },
    {108U,                                   {1, 4}    },
    {109U,                                   {1, 4}    },
    {110U,                                   {1, 4}    },
    {111U,                                   {1, 4}    },
    {112U,                                   {1, 4}    },
    {113U,                                   {1, 4}    },
    {114U,                                   {1, 4}    },
    {115U,                                   {1, 4}    },
    {116U,                                   {1, 4}    },
    {117U,                                   {1, 4}    },
    {118U,                                   {1, 4}    },
    {119U,                                   {1, 4}    },
    {120U,                                   {1, 4}    },
    {121U,                                   {1, 4}    },
    {122U,                                   {1, 4}    },
    {123U,                                   {1, 4}    },
    {124U,                                   {1, 4}    },
    {125U,                                   {1, 4}    },
    {126U,                                   {1, 4}    },
    {127U,                                   {1, 4}    },
    {128U,                                   {1, 4}    },
    {129U,                                   {1, 4}    },
    {130U,                                   {1, 4}    },
    {131U,                                   {1, 4}    },
    {132U,                                   {1, 4}    },
    {133U,                                   {1, 4}    },
    {134U,                                   {1, 4}    },
    {135U,                                   {1, 4}    },
    {136U,                                   {1, 4}    },
    {137U,                                   {1, 4}    },
    {138U,                                   {1, 4}    },
    {139U,                                   {1, 4}    },
    {140U,                                   {1, 4}    },
    {141U,                                   {1, 4}    },
    {142U,                                   {1, 4}    },
    {143U,                                   {1, 4}    },
    {144U,                                   {1, 4}    },
    {145U,                                   {1, 4}    },
    {146U,                                   {1, 4}    },
    {147U,                                   {1, 4}    },
    {148U,                                   {1, 4}    },
    {149U,                                   {1, 4}    },
    {150U,                                   {1, 4}    },
    {151U,                                   {1, 4}    },
    {152U,                                   {1, 4}    },
    {153U,                                   {1, 4}    },
    {154U,                                   {1, 4}    },
    {155U,                                   {1, 4}    },
    {156U,                                   {1, 4}    },
    {157U,                                   {1, 4}    },
    {158U,                                   {1, 4}    },
    {159U,                                   {1, 4}    },
    {160U,                                   {1, 4}    },
    {161U,                                   {1, 4}    },
    {162U,                                   {1, 4}    },
    {163U,                                   {1, 4}    },
    {164U,                                   {1, 4}    },
    {165U,                                   {1, 4}    },
    {166U,                                   {1, 4}    },
    {167U,                                   {1, 4}    },
    {168U,                                   {1, 4}    },
    {169U,                                   {1, 4}    },
    {170U,                                   {1, 4}    },
    {171U,                                   {1, 4}    },
    {172U,                                   {1, 4}    },
    {173U,                                   {1, 4}    },
    {174U,                                   {1, 4}    },
    {175U,                                   {1, 4}    },
    {176U,                                   {1, 4}    },
    {177U,                                   {1, 4}    },
    {178U,                                   {1, 4}    },
    {179U,                                   {1, 4}    },
    {180U,                                   {1, 4}    },
    {181U,                                   {1, 4}    },
    {182U,                                   {1, 4}    },
    {183U,                                   {1, 4}    },
    {184U,                                   {1, 4}    },
    {185U,                                   {1, 4}    },
    {186U,                                   {1, 4}    },
    {187U,                                   {1, 4}    },
    {188U,                                   {1, 4}    },
    {189U,                                   {1, 4}    },
    {190U,                                   {1, 4}    },
    {191U,                                   {1, 4}    },
    {192U,                                   {1, 4}    },
    {193U,                                   {1, 4}    },
    {194U,                                   {1, 4}    },
    {195U,                                   {1, 4}    },
    {196U,                                   {1, 4}    },
    {197U,                                   {1, 4}    },
    {198U,                                   {1, 4}    },
    {199U,                                   {1, 4}    },
    {200U,                                   {1, 4}    },
    {201U,                                   {1, 4}    },
    {202U,                                   {1, 4}    },
    {203U,                                   {1, 4}    },
    {204U,                                   {1, 4}    },
    {205U,                                   {1, 4}    },
    {206U,                                   {1, 4}    },
    {207U,                                   {1, 4}    },
    {208U,                                   {1, 4}    },
    {209U,                                   {1, 4}    },
    {210U,                                   {1, 4}    },
    {211U,                                   {1, 4}    },
    {212U,                                   {1, 4}    },
    {213U,                                   {1, 4}    },
    {214U,                                   {1, 4}    },
    {215U,                                   {1, 4}    },
    {216U,                                   {1, 4}    },
    {217U,                                   {1, 4}    },
    {218U,                                   {1, 4}    },
    {219U,                                   {1, 4}    },
    {220U,                                   {1, 4}    },
    {221U,                                   {1, 4}    },
    {222U,                                   {1, 4}    },
    {223U,                                   {1, 4}    },
    {224U,                                   {1, 4}    },
    {225U,                                   {1, 4}    },
    {226U,                                   {1, 4}    },
    {227U,                                   {1, 4}    },
    {228U,                                   {1, 4}    },
    {229U,                                   {1, 4}    },
    {230U,                                   {1, 4}    },
    {231U,                                   {1, 4}    },
    {232U,                                   {1, 4}    },
    {233U,                                   {1, 4}    },
    {234U,                                   {1, 4}    },
    {235U,                                   {1, 4}    },
    {236U,                                   {1, 4}    },
    {237U,                                   {1, 4}    },
    {238U,                                   {1, 4}    },
    {239U,                                   {1, 4}    },
    {240U,                                   {1, 4}    },
    {241U,                                   {1, 4}    },
    {242U,                                   {1, 4}    },
    {243U,                                   {1, 4}    },
    {244U,                                   {1, 4}    },
    {245U,                                   {1, 4}    },
    {246U,                                   {1, 4}    },
    {247U,                                   {1, 4}    },
    {248U,                                   {1, 4}    },
    {249U,                                   {1, 4}    },
    {250U,                                   {1, 4}    },
    {251U,                                   {1, 4}    },
    {252U,                                   {1, 4}    },
    {253U,                                   {1, 4}    },
    {254U,                                   {1, 4}    },
    {255U,                                   {1, 4}    },
    {256U,                                   {1, 4}    },
    {257U,                                   {1, 4}    },
    {258U,                                   {1, 4}    },
    {259U,                                   {1, 4}    },
    {260U,                                   {1, 4}    },
    {261U,                                   {1, 4}    },
    {262U,                                   {1, 4}    },
    {263U,                                   {1, 4}    },
    {264U,                                   {1, 4}    },
    {265U,                                   {1, 4}    },
    {266U,                                   {1, 4}    },
    {267U,                                   {1, 4}    },
    {268U,                                   {1, 4}    },
    {269U,                                   {1, 4}    },
    {270U,                                   {1, 4}    },
    {271U,                                   {1, 4}    },
    {272U,                                   {1, 4}    },
    {273U,                                   {1, 4}    },
    {274U,                                   {1, 4}    },
    {275U,                                   {1, 4}    },
    {276U,                                   {1, 4}    },
    {277U,                                   {1, 4}    },
    {278U,                                   {1, 4}    },
    {279U,                                   {1, 4}    },
    {280U,                                   {1, 4}    },
    {281U,                                   {1, 4}    },
    {282U,                                   {1, 4}    },
    {283U,                                   {1, 4}    },
    {284U,                                   {1, 4}    },
    {285U,                                   {1, 4}    },
    {286U,                                   {1, 4}    },
    {287U,                                   {1, 4}    },
    {288U,                                   {1, 4}    },
    {289U,                                   {1, 4}    },
    {290U,                                   {1, 4}    },
    {291U,                                   {1, 4}    },
    {292U,                                   {1, 4}    },
    {293U,                                   {1, 4}    },
    {294U,                                   {1, 4}    },
    {295U,                                   {1, 4}    },
    {296U,                                   {1, 4}    },
    {297U,                                   {1, 4}    },
    {298U,                                   {1, 4}    },
    {299U,                                   {1, 4}    },
    {300U,                                   {1, 4}    },
    {301U,                                   {1, 4}    },
    {302U,                                   {1, 4}    },
    {303U,                                   {1, 4}    },
    {304U,                                   {1, 4}    },
    {305U,                                   {1, 4}    },
    {306U,                                   {1, 4}    },
    {307U,                                   {1, 4}    },
    {308U,                                   {1, 4}    },
    {309U,                                   {1, 4}    },
    {310U,                                   {1, 4}    },
    {311U,                                   {1, 4}    },
    {312U,                                   {1, 4}    },
    {313U,                                   {1, 4}    },
    {314U,                                   {1, 4}    },
    {315U,                                   {1, 4}    },
    {316U,                                   {1, 4}    },
    {317U,                                   {1, 4}    },
    {318U,                                   {1, 4}    },
    {319U,                                   {1, 4}    },
    {320U,                                   {1, 4}    },
    {321U,                                   {1, 4}    },
    {322U,                                   {1, 4}    },
    {323U,                                   {1, 4}    },
    {324U,                                   {1, 4}    },
    {325U,                                   {1, 4}    },
    {326U,                                   {1, 4}    },
    {327U,                                   {1, 4}    },
    {328U,                                   {1, 4}    },
    {329U,                                   {1, 4}    },
    {330U,                                   {1, 4}    },
    {331U,                                   {1, 4}    },
    {332U,                                   {1, 4}    },
    {333U,                                   {1, 4}    },
    {334U,                                   {1, 4}    },
    {335U,                                   {1, 4}    },
    {336U,                                   {1, 4}    },
    {337U,                                   {1, 4}    },
    {338U,                                   {1, 4}    },
    {339U,                                   {1, 4}    },
    {340U,                                   {1, 4}    },
    {341U,                                   {1, 4}    },
    {342U,                                   {1, 4}    },
    {343U,                                   {1, 4}    },
    {344U,                                   {1, 4}    },
    {345U,                                   {1, 4}    },
    {346U,                                   {1, 4}    },
    {347U,                                   {1, 4}    },
    {348U,                                   {1, 4}    },
    {349U,                                   {1, 4}    },
    {350U,                                   {1, 4}    },
    {351U,                                   {1, 4}    },
    {352U,                                   {1, 4}    },
    {353U,                                   {1, 4}    },
    {354U,                                   {1, 4}    },
    {355U,                                   {1, 4}    },
    {356U,                                   {1, 4}    },
    {357U,                                   {1, 4}    },
    {358U,                                   {1, 4}    },
    {359U,                                   {1, 4}    },
    {360U,                                   {1, 4}    },
    {361U,                                   {1, 4}    },
    {362U,                                   {1, 4}    },
    {363U,                                   {1, 4}    },
    {364U,                                   {1, 4}    },
    {365U,                                   {1, 4}    },
    {366U,                                   {1, 4}    },
    {367U,                                   {1, 4}    },
    {368U,                                   {1, 4}    },
    {369U,                                   {1, 4}    },
    {370U,                                   {1, 4}    },
    {371U,                                   {1, 4}    },
    {372U,                                   {1, 4}    },
    {373U,                                   {1, 4}    },
    {374U,                                   {1, 4}    },
    {375U,                                   {1, 4}    },
    {376U,                                   {1, 4}    },
    {377U,                                   {1, 4}    },
    {378U,                                   {1, 4}    },
    {379U,                                   {1, 4}    },
    {380U,                                   {1, 4}    },
    {381U,                                   {1, 4}    },
    {382U,                                   {1, 4}    },
    {383U,                                   {1, 4}    },
    {384U,                                   {1, 4}    },
    {385U,                                   {1, 4}    },
    {386U,                                   {1, 4}    },
    {387U,                                   {1, 4}    },
    {388U,                                   {1, 4}    },
    {389U,                                   {1, 4}    },
    {390U,                                   {1, 4}    },
    {391U,                                   {1, 4}    },
    {392U,                                   {1, 4}    },
    {393U,                                   {1, 4}    },
    {394U,                                   {1, 4}    },
    {395U,                                   {1, 4}    },
    {396U,                                   {1, 4}    },
    {397U,                                   {1, 4}    },
    {398U,                                   {1, 4}    },
    {399U,                                   {1, 4}    },
    {400U,                                   {1, 4}    },
    {401U,                                   {1, 4}    },
    {402U,                                   {1, 4}    },
    {403U,                                   {1, 4}    },
    {404U,                                   {1, 4}    },
    {405U,                                   {1, 4}    },
    {406U,                                   {1, 4}    },
    {407U,                                   {1, 4}    },
    {408U,                                   {1, 4}    },
    {409U,                                   {1, 4}    },
    {410U,                                   {1, 4}    },
    {411U,                                   {1, 4}    },
    {412U,                                   {1, 4}    },
    {413U,                                   {1, 4}    },
    {414U,                                   {1, 4}    },
    {415U,                                   {1, 4}    },
    {416U,                                   {1, 4}    },
    {417U,                                   {1, 4}    },
    {418U,                                   {1, 4}    },
    {419U,                                   {1, 4}    },
    {420U,                                   {1, 4}    },
    {421U,                                   {1, 4}    },
    {422U,                                   {1, 4}    },
    {423U,                                   {1, 4}    },
    {424U,                                   {1, 4}    },
    {425U,                                   {1, 4}    },
    {426U,                                   {1, 4}    },
    {427U,                                   {1, 4}    },
    {428U,                                   {1, 4}    },
    {429U,                                   {1, 4}    },
    {430U,                                   {1, 4}    },
    {431U,                                   {1, 4}    },
    {432U,                                   {1, 4}    },
    {433U,                                   {1, 4}    },
    {434U,                                   {1, 4}    },
    {435U,                                   {1, 4}    },
    {436U,                                   {1, 4}    },
    {437U,                                   {1, 4}    },
    {438U,                                   {1, 4}    },
    {439U,                                   {1, 4}    },
    {440U,                                   {1, 4}    },
    {441U,                                   {1, 4}    },
    {442U,                                   {1, 4}    },
    {443U,                                   {1, 4}    },
    {444U,                                   {1, 4}    },
    {445U,                                   {1, 4}    },
    {446U,                                   {1, 4}    },
    {447U,                                   {1, 4}    },
    {448U,                                   {1, 4}    },
    {449U,                                   {1, 4}    },
    {450U,                                   {1, 4}    },
    {451U,                                   {1, 4}    },
    {452U,                                   {1, 4}    },
    {453U,                                   {1, 4}    },
    {454U,                                   {1, 4}    },
    {455U,                                   {1, 4}    },
    {456U,                                   {1, 4}    },
    {457U,                                   {1, 4}    },
    {458U,                                   {1, 4}    },
    {459U,                                   {1, 4}    },
    {460U,                                   {1, 4}    },
    {461U,                                   {1, 4}    },
    {462U,                                   {1, 4}    },
    {463U,                                   {1, 4}    },
    {464U,                                   {1, 4}    },
    {465U,                                   {1, 4}    },
    {466U,                                   {1, 4}    },
    {467U,                                   {1, 4}    },
    {468U,                                   {1, 4}    },
    {469U,                                   {1, 4}    },
    {470U,                                   {1, 4}    },
    {471U,                                   {1, 4}    },
    {472U,                                   {1, 4}    },
    {473U,                                   {1, 4}    },
    {474U,                                   {1, 4}    },
    {475U,                                   {1, 4}    },
    {476U,                                   {1, 4}    },
    {477U,                                   {1, 4}    },
    {478U,                                   {1, 4}    },
    {479U,                                   {1, 4}    },
    {480U,                                   {1, 4}    },
    {481U,                                   {1, 4}    },
    {482U,                                   {1, 4}    },
    {483U,                                   {1, 4}    },
    {484U,                                   {1, 4}    },
    {485U,                                   {1, 4}    },
    {486U,                                   {1, 4}    },
    {487U,                                   {1, 4}    },
    {488U,                                   {1, 4}    },
    {489U,                                   {1, 4}    },
    {490U,                                   {1, 4}    },
    {491U,                                   {1, 4}    },
    {492U,                                   {1, 4}    },
    {493U,                                   {1, 4}    },
    {494U,                                   {1, 4}    },
    {495U,                                   {1, 4}    },
    {496U,                                   {1, 4}    },
    {497U,                                   {1, 4}    },
    {498U,                                   {1, 4}    },
    {499U,                                   {1, 4}    },
    {500U,                                   {1, 4}    },
    {501U,                                   {1, 4}    },
    {502U,                                   {1, 4}    },
    {503U,                                   {1, 4}    },
    {504U,                                   {1, 4}    },
    {505U,                                   {1, 4}    },
    {506U,                                   {1, 4}    },
    {507U,                                   {1, 4}    },
    {508U,                                   {1, 4}    },
    {509U,                                   {1, 4}    },
    {510U,                                   {1, 4}    },
    {511U,                                   {1, 4}    },
    {512U,                                   {1, 4}    },
    {513U,                                   {1, 4}    },
    {514U,                                   {1, 4}    },
    {515U,                                   {1, 4}    },
    {516U,                                   {1, 4}    },
    {517U,                                   {1, 4}    },
    {518U,                                   {1, 4}    },
    {519U,                                   {1, 4}    },
    {520U,                                   {1, 4}    },
    {521U,                                   {1, 4}    },
    {522U,                                   {1, 4}    },
    {523U,                                   {1, 4}    },
    {524U,                                   {1, 4}    },
    {525U,                                   {1, 4}    },
    {526U,                                   {1, 4}    },
    {527U,                                   {1, 4}    },
    {528U,                                   {1, 4}    },
    {529U,                                   {1, 4}    },
    {530U,                                   {1, 4}    },
    {531U,                                   {1, 4}    },
    {532U,                                   {1, 4}    },
    {533U,                                   {1, 4}    },
    {534U,                                   {1, 4}    },
    {535U,                                   {1, 4}    },
    {536U,                                   {1, 4}    },
    {537U,                                   {1, 4}    },
    {538U,                                   {1, 4}    },
    {539U,                                   {1, 4}    },
    {540U,                                   {1, 4}    },
    {541U,                                   {1, 4}    },
    {542U,                                   {1, 4}    },
    {543U,                                   {1, 4}    },
    {544U,                                   {1, 4}    },
    {545U,                                   {1, 4}    },
    {546U,                                   {1, 4}    },
    {547U,                                   {1, 4}    },
    {548U,                                   {1, 4}    },
    {549U,                                   {1, 4}    },
    {550U,                                   {1, 4}    },
    {551U,                                   {1, 4}    },
    {552U,                                   {1, 4}    },
    {553U,                                   {1, 4}    },
    {554U,                                   {1, 4}    },
    {555U,                                   {1, 4}    },
    {556U,                                   {1, 4}    },
    {557U,                                   {1, 4}    },
    {558U,                                   {1, 4}    },
    {559U,                                   {1, 4}    },
    {560U,                                   {1, 4}    },
    {561U,                                   {1, 4}    },
    {562U,                                   {1, 4}    },
    {563U,                                   {1, 4}    },
    {564U,                                   {1, 4}    },
    {565U,                                   {1, 4}    },
    {566U,                                   {1, 4}    },
    {567U,                                   {1, 4}    },
    {568U,                                   {1, 4}    },
    {569U,                                   {1, 4}    },
    {570U,                                   {1, 4}    },
    {571U,                                   {1, 4}    },
    {572U,                                   {1, 4}    },
    {573U,                                   {1, 4}    },
    {574U,                                   {1, 4}    },
    {575U,                                   {1, 4}    },
    {576U,                                   {1, 4}    },
    {577U,                                   {1, 4}    },
    {578U,                                   {1, 4}    },
    {579U,                                   {1, 4}    },
    {580U,                                   {1, 4}    },
    {581U,                                   {1, 4}    },
    {582U,                                   {1, 4}    },
    {583U,                                   {1, 4}    },
    {584U,                                   {1, 4}    },
    {585U,                                   {1, 4}    },
    {586U,                                   {1, 4}    },
    {587U,                                   {1, 4}    },
    {588U,                                   {1, 4}    },
    {589U,                                   {1, 4}    },
    {590U,                                   {1, 4}    },
    {591U,                                   {1, 4}    },
    {592U,                                   {1, 4}    },
    {593U,                                   {1, 4}    },
    {594U,                                   {1, 4}    },
    {595U,                                   {1, 4}    },
    {596U,                                   {1, 4}    },
    {597U,                                   {1, 4}    },
    {598U,                                   {1, 4}    },
    {599U,                                   {1, 4}    },
    {600U,                                   {1, 4}    },
    {601U,                                   {1, 4}    },
    {602U,                                   {1, 4}    },
    {603U,                                   {1, 4}    },
    {604U,                                   {1, 4}    },
    {605U,                                   {1, 4}    },
    {606U,                                   {1, 4}    },
    {607U,                                   {1, 4}    },
    {608U,                                   {1, 4}    },
    {609U,                                   {1, 4}    },
    {610U,                                   {1, 4}    },
    {611U,                                   {1, 4}    },
    {612U,                                   {1, 4}    },
    {613U,                                   {1, 4}    },
    {614U,                                   {1, 4}    },
    {615U,                                   {1, 4}    },
    {616U,                                   {1, 4}    },
    {617U,                                   {1, 4}    },
    {618U,                                   {1, 4}    },
    {619U,                                   {1, 4}    },
    {620U,                                   {1, 4}    },
    {621U,                                   {1, 4}    },
    {622U,                                   {1, 4}    },
    {623U,                                   {1, 4}    },
    {624U,                                   {1, 4}    },
    {625U,                                   {1, 4}    },
    {626U,                                   {1, 4}    },
    {627U,                                   {1, 4}    },
    {628U,                                   {1, 4}    },
    {629U,                                   {1, 4}    },
    {630U,                                   {1, 4}    },
    {631U,                                   {1, 4}    },
    {632U,                                   {1, 4}    },
    {633U,                                   {1, 4}    },
    {634U,                                   {1, 4}    },
    {635U,                                   {1, 4}    },
    {636U,                                   {1, 4}    },
    {637U,                                   {1, 4}    },
    {638U,                                   {1, 4}    },
    {639U,                                   {1, 4}    },
    {640U,                                   {1, 4}    },
    {641U,                                   {1, 4}    },
    {642U,                                   {1, 4}    },
    {643U,                                   {1, 4}    },
    {644U,                                   {1, 4}    },
    {645U,                                   {1, 4}    },
    {646U,                                   {1, 4}    },
    {647U,                                   {1, 4}    },
    {648U,                                   {1, 4}    },
    {649U,                                   {1, 4}    },
    {650U,                                   {1, 4}    },
    {651U,                                   {1, 4}    },
    {652U,                                   {1, 4}    },
    {653U,                                   {1, 4}    },
    {654U,                                   {1, 4}    },
    {655U,                                   {1, 4}    },
    {656U,                                   {1, 4}    },
    {657U,                                   {1, 4}    },
    {658U,                                   {1, 4}    },
    {659U,                                   {1, 4}    },
    {660U,                                   {1, 4}    },
    {661U,                                   {1, 4}    },
    {662U,                                   {1, 4}    },
    {663U,                                   {1, 4}    },
    {664U,                                   {1, 4}    },
    {665U,                                   {1, 4}    },
    {666U,                                   {1, 4}    },
    {667U,                                   {1, 4}    },
    {668U,                                   {1, 4}    },
    {669U,                                   {1, 4}    },
    {670U,                                   {1, 4}    },
    {671U,                                   {1, 4}    },
    {672U,                                   {1, 4}    },
    {673U,                                   {1, 4}    },
    {674U,                                   {1, 4}    },
    {675U,                                   {1, 4}    },
    {676U,                                   {1, 4}    },
    {677U,                                   {1, 4}    },
    {678U,                                   {1, 4}    },
    {679U,                                   {1, 4}    },
    {680U,                                   {1, 4}    },
    {681U,                                   {1, 4}    },
    {682U,                                   {1, 4}    },
    {683U,                                   {1, 4}    },
    {684U,                                   {1, 4}    },
    {685U,                                   {1, 4}    },
    {686U,                                   {1, 4}    },
    {687U,                                   {1, 4}    },
    {688U,                                   {1, 4}    },
    {689U,                                   {1, 4}    },
    {690U,                                   {1, 4}    },
    {691U,                                   {1, 4}    },
    {692U,                                   {1, 4}    },
    {693U,                                   {1, 4}    },
    {694U,                                   {1, 4}    },
    {695U,                                   {1, 4}    },
    {696U,                                   {1, 4}    },
    {697U,                                   {1, 4}    },
    {698U,                                   {1, 4}    },
    {699U,                                   {1, 4}    },
    {700U,                                   {1, 4}    },
    {701U,                                   {1, 4}    },
    {702U,                                   {1, 4}    },
    {703U,                                   {1, 4}    },
    {704U,                                   {1, 4}    },
    {705U,                                   {1, 4}    },
    {706U,                                   {1, 4}    },
    {707U,                                   {1, 4}    },
    {708U,                                   {1, 4}    },
    {709U,                                   {1, 4}    },
    {710U,                                   {1, 4}    },
    {711U,                                   {1, 4}    },
    {712U,                                   {1, 4}    },
    {713U,                                   {1, 4}    },
    {714U,                                   {1, 4}    },
    {715U,                                   {1, 4}    },
    {716U,                                   {1, 4}    },
    {717U,                                   {1, 4}    },
    {718U,                                   {1, 4}    },
    {719U,                                   {1, 4}    },
    {720U,                                   {1, 4}    },
    {721U,                                   {1, 4}    },
    {722U,                                   {1, 4}    },
    {723U,                                   {1, 4}    },
    {724U,                                   {1, 4}    },
    {725U,                                   {1, 4}    },
    {726U,                                   {1, 4}    },
    {727U,                                   {1, 4}    },
    {728U,                                   {1, 4}    },
    {729U,                                   {1, 4}    },
    {730U,                                   {1, 4}    },
    {731U,                                   {1, 4}    },
    {732U,                                   {1, 4}    },
    {733U,                                   {1, 4}    },
    {734U,                                   {1, 4}    },
    {735U,                                   {1, 4}    },
    {736U,                                   {1, 4}    },
    {737U,                                   {1, 4}    },
    {738U,                                   {1, 4}    },
    {739U,                                   {1, 4}    },
    {740U,                                   {1, 4}    },
    {741U,                                   {1, 4}    },
    {742U,                                   {1, 4}    },
    {743U,                                   {1, 4}    },
    {744U,                                   {1, 4}    },
    {745U,                                   {1, 4}    },
    {746U,                                   {1, 4}    },
    {747U,                                   {1, 4}    },
    {748U,                                   {1, 4}    },
    {749U,                                   {1, 4}    },
    {750U,                                   {1, 4}    },
    {751U,                                   {1, 4}    },
    {752U,                                   {1, 4}    },
    {753U,                                   {1, 4}    },
    {754U,                                   {1, 4}    },
    {755U,                                   {1, 4}    },
    {756U,                                   {1, 4}    },
    {757U,                                   {1, 4}    },
    {758U,                                   {1, 4}    },
    {759U,                                   {1, 4}    },
    {760U,                                   {1, 4}    },
    {761U,                                   {1, 4}    },
    {762U,                                   {1, 4}    },
    {763U,                                   {1, 4}    },
    {764U,                                   {1, 4}    },
    {765U,                                   {1, 4}    },
    {766U,                                   {1, 4}    },
    {767U,                                   {1, 4}    },
    {768U,                                   {1, 4}    },
    {769U,                                   {1, 4}    },
    {770U,                                   {1, 4}    },
    {771U,                                   {1, 4}    },
    {772U,                                   {1, 4}    },
    {773U,                                   {1, 4}    },
    {774U,                                   {1, 4}    },
    {775U,                                   {1, 4}    },
    {776U,                                   {1, 4}    },
    {777U,                                   {1, 4}    },
    {778U,                                   {1, 4}    },
    {779U,                                   {1, 4}    },
    {780U,                                   {1, 4}    },
    {781U,                                   {1, 4}    },
    {782U,                                   {1, 4}    },
    {783U,                                   {1, 4}    },
    {784U,                                   {1, 4}    },
    {785U,                                   {1, 4}    },
    {786U,                                   {1, 4}    },
    {787U,                                   {1, 4}    },
    {788U,                                   {1, 4}    },
    {789U,                                   {1, 4}    },
    {790U,                                   {1, 4}    },
    {791U,                                   {1, 4}    },
    {792U,                                   {1, 4}    },
    {793U,                                   {1, 4}    },
    {794U,                                   {1, 4}    },
    {795U,                                   {1, 4}    },
    {796U,                                   {1, 4}    },
    {797U,                                   {1, 4}    },
    {798U,                                   {1, 4}    },
    {799U,                                   {1, 4}    },
    {800U,                                   {1, 4}    },
    {801U,                                   {1, 4}    },
    {802U,                                   {1, 4}    },
    {803U,                                   {1, 4}    },
    {804U,                                   {1, 4}    },
    {805U,                                   {1, 4}    },
    {806U,                                   {1, 4}    },
    {807U,                                   {1, 4}    },
    {808U,                                   {1, 4}    },
    {809U,                                   {1, 4}    },
    {810U,                                   {1, 4}    },
    {811U,                                   {1, 4}    },
    {812U,                                   {1, 4}    },
    {813U,                                   {1, 4}    },
    {814U,                                   {1, 4}    },
    {815U,                                   {1, 4}    },
    {816U,                                   {1, 4}    },
    {817U,                                   {1, 4}    },
    {818U,                                   {1, 4}    },
    {819U,                                   {1, 4}    },
    {820U,                                   {1, 4}    },
    {821U,                                   {1, 4}    },
    {822U,                                   {1, 4}    },
    {823U,                                   {1, 4}    },
    {824U,                                   {1, 4}    },
    {825U,                                   {1, 4}    },
    {826U,                                   {1, 4}    },
    {827U,                                   {1, 4}    },
    {828U,                                   {1, 4}    },
    {829U,                                   {1, 4}    },
    {830U,                                   {1, 4}    },
    {831U,                                   {1, 4}    },
    {832U,                                   {1, 4}    },
    {833U,                                   {1, 4}    },
    {834U,                                   {1, 4}    },
    {835U,                                   {1, 4}    },
    {836U,                                   {1, 4}    },
    {837U,                                   {1, 4}    },
    {838U,                                   {1, 4}    },
    {839U,                                   {1, 4}    },
    {840U,                                   {1, 4}    },
    {841U,                                   {1, 4}    },
    {842U,                                   {1, 4}    },
    {843U,                                   {1, 4}    },
    {844U,                                   {1, 4}    },
    {845U,                                   {1, 4}    },
    {846U,                                   {1, 4}    },
    {847U,                                   {1, 4}    },
    {848U,                                   {1, 4}    },
    {849U,                                   {1, 4}    },
    {850U,                                   {1, 4}    },
    {851U,                                   {1, 4}    },
    {852U,                                   {1, 4}    },
    {853U,                                   {1, 4}    },
    {854U,                                   {1, 4}    },
    {855U,                                   {1, 4}    },
    {856U,                                   {1, 4}    },
    {857U,                                   {1, 4}    },
    {858U,                                   {1, 4}    },
    {859U,                                   {1, 4}    },
    {860U,                                   {1, 4}    },
    {861U,                                   {1, 4}    },
    {862U,                                   {1, 4}    },
    {863U,                                   {1, 4}    },
    {864U,                                   {1, 4}    },
    {865U,                                   {1, 4}    },
    {866U,                                   {1, 4}    },
    {867U,                                   {1, 4}    },
    {868U,                                   {1, 4}    },
    {869U,                                   {1, 4}    },
    {870U,                                   {1, 4}    },
    {871U,                                   {1, 4}    },
    {872U,                                   {1, 4}    },
    {873U,                                   {1, 4}    },
    {874U,                                   {1, 4}    },
    {875U,                                   {1, 4}    },
    {876U,                                   {1, 4}    },
    {877U,                                   {1, 4}    },
    {878U,                                   {1, 4}    },
    {879U,                                   {1, 4}    },
    {880U,                                   {1, 4}    },
    {881U,                                   {1, 4}    },
    {882U,                                   {1, 4}    },
    {883U,                                   {1, 4}    },
    {884U,                                   {1, 4}    },
    {885U,                                   {1, 4}    },
    {886U,                                   {1, 4}    },
    {887U,                                   {1, 4}    },
    {888U,                                   {1, 4}    },
    {889U,                                   {1, 4}    },
    {890U,                                   {1, 4}    },
    {891U,                                   {1, 4}    },
    {892U,                                   {1, 4}    },
    {893U,                                   {1, 4}    },
    {894U,                                   {1, 4}    },
    {895U,                                   {1, 4}    },
    {896U,                                   {1, 4}    },
    {897U,                                   {1, 4}    },
    {898U,                                   {1, 4}    },
    {899U,                                   {1, 4}    },
    {900U,                                   {1, 4}    },
    {901U,                                   {1, 4}    },
    {902U,                                   {1, 4}    },
    {903U,                                   {1, 4}    },
    {904U,                                   {1, 4}    },
    {905U,                                   {1, 4}    },
    {906U,                                   {1, 4}    },
    {907U,                                   {1, 4}    },
    {908U,                                   {1, 4}    },
    {909U,                                   {1, 4}    },
    {910U,                                   {1, 4}    },
    {911U,                                   {1, 4}    },
    {912U,                                   {1, 4}    },
    {913U,                                   {1, 4}    },
    {914U,                                   {1, 4}    },
    {915U,                                   {1, 4}    },
    {916U,                                   {1, 4}    },
    {917U,                                   {1, 4}    },
    {918U,                                   {1, 4}    },
    {919U,                                   {1, 4}    },
    {920U,                                   {1, 4}    },
    {921U,                                   {1, 4}    },
    {922U,                                   {1, 4}    },
    {923U,                                   {1, 4}    },
    {924U,                                   {1, 4}    },
    {925U,                                   {1, 4}    },
    {926U,                                   {1, 4}    },
    {927U,                                   {1, 4}    },
    {928U,                                   {1, 4}    },
    {929U,                                   {1, 4}    },
    {930U,                                   {1, 4}    },
    {931U,                                   {1, 4}    },
    {932U,                                   {1, 4}    },
    {933U,                                   {1, 4}    },
    {934U,                                   {1, 4}    },
    {935U,                                   {1, 4}    },
    {936U,                                   {1, 4}    },
    {937U,                                   {1, 4}    },
    {938U,                                   {1, 4}    },
    {939U,                                   {1, 4}    },
    {940U,                                   {1, 4}    },
    {941U,                                   {1, 4}    },
    {942U,                                   {1, 4}    },
    {943U,                                   {1, 4}    },
    {944U,                                   {1, 4}    },
    {945U,                                   {1, 4}    },
    {946U,                                   {1, 4}    },
    {947U,                                   {1, 4}    },
    {948U,                                   {1, 4}    },
    {949U,                                   {1, 4}    },
    {950U,                                   {1, 4}    },
    {951U,                                   {1, 4}    },
    {952U,                                   {1, 4}    },
    {953U,                                   {1, 4}    },
    {954U,                                   {1, 4}    },
    {955U,                                   {1, 4}    },
    {956U,                                   {1, 4}    },
    {957U,                                   {1, 4}    },
    {958U,                                   {1, 4}    },
    {959U,                                   {1, 4}    },
    {960U,                                   {1, 4}    },
    {961U,                                   {1, 4}    },
    {962U,                                   {1, 4}    },
    {963U,                                   {1, 4}    },
    {964U,                                   {1, 4}    },
    {965U,                                   {1, 4}    },
    {966U,                                   {1, 4}    },
    {967U,                                   {1, 4}    },
    {968U,                                   {1, 4}    },
    {969U,                                   {1, 4}    },
    {970U,                                   {1, 4}    },
    {971U,                                   {1, 4}    },
    {972U,                                   {1, 4}    },
    {973U,                                   {1, 4}    },
    {974U,                                   {1, 4}    },
    {975U,                                   {1, 4}    },
    {976U,                                   {1, 4}    },
    {977U,                                   {1, 4}    },
    {978U,                                   {1, 4}    },
    {979U,                                   {1, 4}    },
    {980U,                                   {1, 4}    },
    {981U,                                   {1, 4}    },
    {982U,                                   {1, 4}    },
    {983U,                                   {1, 4}    },
    {984U,                                   {1, 4}    },
    {985U,                                   {1, 4}    },
    {986U,                                   {1, 4}    },
    {987U,                                   {1, 4}    },
    {988U,                                   {1, 4}    },
    {989U,                                   {1, 4}    },
    {990U,                                   {1, 4}    },
    {991U,                                   {1, 4}    },
    {992U,                                   {1, 4}    },
    {993U,                                   {1, 4}    },
    {994U,                                   {1, 4}    },
    {995U,                                   {1, 4}    },
    {996U,                                   {1, 4}    },
    {997U,                                   {1, 4}    },
    {998U,                                   {1, 4}    },
    {999U,                                   {1, 4}    },
    {1000U,                                  {1, 4}    },
    {1001U,                                  {1, 4}    },
    {1002U,                                  {1, 4}    },
    {1003U,                                  {1, 4}    },
    {1004U,                                  {1, 4}    },
    {1005U,                                  {1, 4}    },
    {1006U,                                  {1, 4}    },
    {1007U,                                  {1, 4}    },
    {1008U,                                  {1, 4}    },
    {1009U,                                  {1, 4}    },
    {1010U,                                  {1, 4}    },
    {1011U,                                  {1, 4}    },
    {1012U,                                  {1, 4}    },
    {1013U,                                  {1, 4}    },
    {1014U,                                  {1, 4}    },
    {1015U,                                  {1, 4}    },
    {1016U,                                  {1, 4}    },
    {1017U,                                  {1, 4}    },
    {1018U,                                  {1, 4}    },
    {1019U,                                  {1, 4}    },
    {1020U,                                  {1, 4}    },
    {1021U,                                  {1, 4}    },
    {1022U,                                  {1, 4}    },
    {1023U,                                  {1, 4}    },
    {1024U,                                  {1, 4}    },
    /* greater than OSAL_RCFG_MQ_MAX_NUM (1024) */

    {.id = OSAL_MQ_INVALID_ID}
};

const st_osal_interrupt_thread_config_t  g_osal_interrupt_thread_config_dummy[] =
{
    /* irq, device_id               interrupt priority            task_name,              stack_size,  interrupt_timeout */
    {1,   "dummy_001",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {2,   "dummy_002",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {3,   "dummy_003",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {4,   "dummy_004",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {5,   "dummy_005",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {6,   "dummy_006",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {7,   "dummy_007",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {8,   "dummy_008",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {9,   "dummy_009",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {10,  "dummy_010",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {11,  "dummy_011",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {12,  "dummy_012",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {13,  "dummy_013",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {14,  "dummy_014",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {15,  "dummy_015",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {16,  "dummy_016",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {17,  "dummy_017",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {18,  "dummy_018",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {19,  "dummy_019",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {20,  "dummy_020",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {21,  "dummy_021",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {22,  "dummy_022",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {23,  "dummy_023",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {24,  "dummy_024",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {25,  "dummy_025",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {26,  "dummy_026",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {27,  "dummy_027",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {28,  "dummy_028",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {29,  "dummy_029",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {30,  "dummy_030",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {31,  "dummy_031",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {32,  "dummy_032",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {33,  "dummy_033",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {34,  "dummy_034",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {35,  "dummy_035",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {36,  "dummy_036",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {37,  "dummy_037",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {38,  "dummy_038",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {39,  "dummy_039",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {40,  "dummy_040",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {41,  "dummy_041",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {42,  "dummy_042",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {43,  "dummy_043",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {44,  "dummy_044",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {45,  "dummy_045",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {46,  "dummy_046",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {47,  "dummy_047",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {48,  "dummy_048",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {49,  "dummy_049",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {50,  "dummy_050",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {51,  "dummy_051",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {52,  "dummy_052",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {53,  "dummy_053",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {54,  "dummy_054",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {55,  "dummy_055",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {56,  "dummy_056",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {57,  "dummy_057",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {58,  "dummy_058",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {59,  "dummy_059",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {60,  "dummy_060",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {61,  "dummy_061",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {62,  "dummy_062",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {63,  "dummy_063",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {64,  "dummy_064",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {65,  "dummy_065",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {66,  "dummy_066",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {67,  "dummy_067",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {68,  "dummy_068",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {69,  "dummy_069",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {70,  "dummy_070",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {71,  "dummy_071",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {72,  "dummy_072",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {73,  "dummy_073",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {74,  "dummy_074",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {75,  "dummy_075",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {76,  "dummy_076",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {77,  "dummy_077",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {78,  "dummy_078",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {79,  "dummy_079",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {80,  "dummy_080",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {81,  "dummy_081",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {82,  "dummy_082",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {83,  "dummy_083",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {84,  "dummy_084",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {85,  "dummy_085",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {86,  "dummy_086",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {87,  "dummy_087",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {88,  "dummy_088",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {89,  "dummy_089",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {90,  "dummy_090",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {91,  "dummy_091",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {92,  "dummy_092",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {93,  "dummy_093",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {94,  "dummy_094",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {95,  "dummy_095",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {96,  "dummy_096",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {97,  "dummy_097",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {98,  "dummy_098",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {99,  "dummy_099",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {100, "dummy_100",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {101, "dummy_101",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {102, "dummy_102",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {103, "dummy_103",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {104, "dummy_104",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {105, "dummy_105",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {106, "dummy_106",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {107, "dummy_107",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {108, "dummy_108",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {109, "dummy_109",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {110, "dummy_110",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {111, "dummy_111",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {112, "dummy_112",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {113, "dummy_113",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {114, "dummy_114",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {115, "dummy_115",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {116, "dummy_116",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {117, "dummy_117",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {118, "dummy_118",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {119, "dummy_119",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {120, "dummy_120",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {121, "dummy_121",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {122, "dummy_122",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {123, "dummy_123",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {124, "dummy_124",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {125, "dummy_125",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {126, "dummy_126",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {127, "dummy_127",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {128, "dummy_128",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {129, "dummy_129",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {130, "dummy_130",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {131, "dummy_131",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {132, "dummy_132",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {133, "dummy_133",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {134, "dummy_134",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {135, "dummy_135",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {136, "dummy_136",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {137, "dummy_137",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {138, "dummy_138",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {139, "dummy_139",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {140, "dummy_140",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {141, "dummy_141",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {142, "dummy_142",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {143, "dummy_143",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {144, "dummy_144",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {145, "dummy_145",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {146, "dummy_146",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {147, "dummy_147",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {148, "dummy_148",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {149, "dummy_149",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {150, "dummy_150",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {151, "dummy_151",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {152, "dummy_152",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {153, "dummy_153",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {154, "dummy_154",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {155, "dummy_155",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {156, "dummy_156",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {157, "dummy_157",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {158, "dummy_158",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {159, "dummy_159",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {160, "dummy_160",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {161, "dummy_161",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {162, "dummy_162",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {163, "dummy_163",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {164, "dummy_164",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {165, "dummy_165",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {166, "dummy_166",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {167, "dummy_167",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {168, "dummy_168",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {169, "dummy_169",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {170, "dummy_170",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {171, "dummy_171",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {172, "dummy_172",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {173, "dummy_173",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {174, "dummy_174",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {175, "dummy_175",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {176, "dummy_176",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {177, "dummy_177",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {178, "dummy_178",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {179, "dummy_179",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {180, "dummy_180",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {181, "dummy_181",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {182, "dummy_182",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {183, "dummy_183",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {184, "dummy_184",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {185, "dummy_185",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {186, "dummy_186",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {187, "dummy_187",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {188, "dummy_188",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {189, "dummy_189",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {190, "dummy_190",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {191, "dummy_191",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {192, "dummy_192",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {193, "dummy_193",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {194, "dummy_194",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {195, "dummy_195",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {196, "dummy_196",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {197, "dummy_197",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {198, "dummy_198",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {199, "dummy_199",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {200, "dummy_200",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {201, "dummy_201",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {202, "dummy_202",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {203, "dummy_203",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {204, "dummy_204",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {205, "dummy_205",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {206, "dummy_206",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {207, "dummy_207",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {208, "dummy_208",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {209, "dummy_209",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {210, "dummy_210",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {211, "dummy_211",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {212, "dummy_212",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {213, "dummy_213",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {214, "dummy_214",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {215, "dummy_215",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {216, "dummy_216",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {217, "dummy_217",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {218, "dummy_218",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {219, "dummy_219",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {220, "dummy_220",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {221, "dummy_221",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {222, "dummy_222",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {223, "dummy_223",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {224, "dummy_224",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {225, "dummy_225",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {226, "dummy_226",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {227, "dummy_227",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {228, "dummy_228",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {229, "dummy_229",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {230, "dummy_230",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {231, "dummy_231",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {232, "dummy_232",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {233, "dummy_233",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {234, "dummy_234",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {235, "dummy_235",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {236, "dummy_236",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {237, "dummy_237",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {238, "dummy_238",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {239, "dummy_239",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {240, "dummy_240",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {241, "dummy_241",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {242, "dummy_242",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {243, "dummy_243",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {244, "dummy_244",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {245, "dummy_245",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {246, "dummy_246",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {247, "dummy_247",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {248, "dummy_248",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {249, "dummy_249",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {250, "dummy_250",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {251, "dummy_251",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {252, "dummy_252",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {253, "dummy_253",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {254, "dummy_254",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {255, "dummy_255",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {256, "dummy_256",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {257, "dummy_257",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {258, "dummy_258",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {259, "dummy_259",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {260, "dummy_260",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {261, "dummy_261",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {262, "dummy_262",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {263, "dummy_263",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {264, "dummy_264",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {265, "dummy_265",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {266, "dummy_266",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {267, "dummy_267",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {268, "dummy_268",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {269, "dummy_269",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {270, "dummy_270",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {271, "dummy_271",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {272, "dummy_272",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {273, "dummy_273",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {274, "dummy_274",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {275, "dummy_275",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {276, "dummy_276",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {277, "dummy_277",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {278, "dummy_278",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {279, "dummy_279",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {280, "dummy_280",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {281, "dummy_281",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {282, "dummy_282",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {283, "dummy_283",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {284, "dummy_284",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {285, "dummy_285",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {286, "dummy_286",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {287, "dummy_287",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {288, "dummy_288",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {289, "dummy_289",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {290, "dummy_290",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {291, "dummy_291",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {292, "dummy_292",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {293, "dummy_293",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {294, "dummy_294",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {295, "dummy_295",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {296, "dummy_296",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {297, "dummy_297",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {298, "dummy_298",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {299, "dummy_299",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {300, "dummy_300",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {301, "dummy_301",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {302, "dummy_302",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {303, "dummy_303",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {304, "dummy_304",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {305, "dummy_305",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {306, "dummy_306",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {307, "dummy_307",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {308, "dummy_308",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {309, "dummy_309",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {310, "dummy_310",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {311, "dummy_311",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {312, "dummy_312",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {313, "dummy_313",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {314, "dummy_314",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {315, "dummy_315",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {316, "dummy_316",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {317, "dummy_317",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {318, "dummy_318",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {319, "dummy_319",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {320, "dummy_320",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {321, "dummy_321",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {322, "dummy_322",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {323, "dummy_323",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {324, "dummy_324",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {325, "dummy_325",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {326, "dummy_326",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {327, "dummy_327",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {328, "dummy_328",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {329, "dummy_329",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {330, "dummy_330",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {331, "dummy_331",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {332, "dummy_332",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {333, "dummy_333",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {334, "dummy_334",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {335, "dummy_335",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {336, "dummy_336",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {337, "dummy_337",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {338, "dummy_338",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {339, "dummy_339",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {340, "dummy_340",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {341, "dummy_341",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {342, "dummy_342",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {343, "dummy_343",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {344, "dummy_344",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {345, "dummy_345",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {346, "dummy_346",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {347, "dummy_347",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {348, "dummy_348",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {349, "dummy_349",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {350, "dummy_350",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {351, "dummy_351",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {352, "dummy_352",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {353, "dummy_353",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {354, "dummy_354",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {355, "dummy_355",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {356, "dummy_356",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {357, "dummy_357",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {358, "dummy_358",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {359, "dummy_359",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {360, "dummy_360",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {361, "dummy_361",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {362, "dummy_362",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {363, "dummy_363",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {364, "dummy_364",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {365, "dummy_365",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {366, "dummy_366",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {367, "dummy_367",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {368, "dummy_368",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {369, "dummy_369",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {370, "dummy_370",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {371, "dummy_371",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {372, "dummy_372",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {373, "dummy_373",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {374, "dummy_374",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {375, "dummy_375",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {376, "dummy_376",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {377, "dummy_377",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {378, "dummy_378",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {379, "dummy_379",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {380, "dummy_380",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {381, "dummy_381",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {382, "dummy_382",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {383, "dummy_383",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {384, "dummy_384",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    {385, "dummy_385",              OSAL_INTERRUPT_PRIORITY_TYPE0, "int_task",             0x0400,      60000000},
    /* greater than OSAL_RCFG_INTERRUPT_MAX_NUM (384) */

    {.id = OSAL_INTERRUPT_INVALID_ID}
};

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_HwStateOff:AMSTB_R_PMA_SM_HwStateOff:inout:::] */
/*    R_PMA_SM_HwStateOff => Stub */
//e_result_t AMSTB_R_PMA_SM_HwStateOff()
//{
//	static e_result_t volatile AMIN_return;
//	static e_sysc_domain_id_t volatile AMOUT_sysc_idmax;
//	static const int *volatile AMOUT_R_PMA_SYSC_PDR_ID_TABLE;
//	static e_cpg_domain_id_t volatile AMOUT_cpg_idmax;
//	static e_sysc_domain_id_t volatile AMOUT_sysc_idmin;
//	static e_cpg_domain_id_t volatile AMOUT_cpg_idmin;
//	static const int *volatile AMOUT_R_PMA_CPG_MOD_ID_TABLE;
//	AMOUT_sysc_idmax = sysc_idmax;
//	AMOUT_R_PMA_SYSC_PDR_ID_TABLE = R_PMA_SYSC_PDR_ID_TABLE;
//	AMOUT_cpg_idmax = cpg_idmax;
//	AMOUT_sysc_idmin = sysc_idmin;
//	AMOUT_cpg_idmin = cpg_idmin;
//	AMOUT_R_PMA_CPG_MOD_ID_TABLE = R_PMA_CPG_MOD_ID_TABLE;
//	return AMIN_return;
//}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_PowerONA3IR:AMSTB_R_PMA_SM_PowerONA3IR:inout:::] */
/*    R_PMA_SM_PowerONA3IR => Stub */
//e_result_t AMSTB_R_PMA_SM_PowerONA3IR()
//{
//	static e_result_t volatile AMIN_return;
//	static const int *volatile AMOUT_R_PMA_SYSC_PDR_ID_TABLE;
////	static e_pma_hwa_id_t volatile AMOUT_hwa_idimpr;
//	AMOUT_R_PMA_SYSC_PDR_ID_TABLE = R_PMA_SYSC_PDR_ID_TABLE;
////	AMOUT_hwa_idimpr = hwa_idimpr;
//	return AMIN_return;
//}
#ifdef __cplusplus
}
#endif
#endif /* WINAMS_STUB */
