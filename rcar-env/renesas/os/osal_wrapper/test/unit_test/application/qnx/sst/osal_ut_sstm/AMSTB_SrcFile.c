#ifdef WINAMS_STUB
#ifdef __cplusplus
extern "C" {
#endif

#ifdef WINAMS_STUB
#ifdef __cplusplus
extern "C" {
#endif

/* WINAMS_VAR_EXT[void *qtime] */
extern void *qtime;

/* WINAMS_VAR_EXT[int g_context_key] */
extern int g_context_key;

/* WINAMS_VAR_EXT[unsigned long long g_osal_num_of_device_table] */
extern unsigned long long g_osal_num_of_device_table;

/* WINAMS_VAR_EXT[struct st_osal_device_control *gp_osal_device_control] */
extern struct st_osal_device_control *gp_osal_device_control;

#include <pthread.h>
// #include "target/qnx/r_osal_common.h"
#include "target/common/r_osal_common.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"
#include "target/common/res_cfg/static/r_osal_res_cfg.h"
#include "target/common/res_cfg/r_osal_res_cfg_memory.h"
#include "target/qnx/r_osal_os_type.h"
#include "target/common/helper/r_osal_helper_if.h"
#include "hw/rcar_cpg.h"
#include "hw/rcar_sysc.h"

/* ------------------------------------------------ */
typedef enum e_osal_inner_thread_policy
{
    OSAL_SCHED_INVALID = 0,
    OSAL_SCHED_FIFO,
    OSAL_SCHED_RR
} e_osal_inner_thread_policy_t;

typedef struct st_osal_inner_thread_attr *osal_inner_thread_attr_t;

typedef struct st_osal_inner_thread_control *osal_inner_thread_handle_t;

typedef struct st_osal_inner_mutexattr *osal_inner_mutexattr_t;

typedef struct st_osal_inner_mutex_control *osal_inner_mutex_handle_t;

typedef struct st_osal_inner_cond_control *osal_inner_cond_handle_t;

typedef struct st_osal_inner_mq_control *osal_inner_mq_handle_t;

typedef void* (*p_osal_inner_thread_func_t)(void* user_arg);

typedef struct st_osal_clock_info
{
    struct timespec initial_monotonic_time;    /*!< Monotonic time set by osal_clock_time_initialize() */
    uint64_t        cycles_per_sec;            /*!< The number of ClockCycles() increments per second for the system. */
    uint64_t        initial_cycle;             /*!< Value of a free-running 64-bit cycle counter set by osal_clock_time_initialize(). */
}st_osal_clock_info_t;

typedef struct st_osal_inner_thread_attrinfo
{
    e_osal_inner_thread_policy_t policy;
    e_osal_thread_priority_t     priority;
    size_t                       stack_size;
} st_osal_inner_thread_attrinfo_t;

typedef struct st_osal_inner_thread_config
{
    osal_inner_thread_attr_t    *attr;
    p_osal_inner_thread_func_t  func;      /*!< Worker function */
    void*                       userarg;   /*!< Pointer of Argument for worker function */
} st_osal_inner_thread_config_t;

typedef struct st_osal_inner_mq_config
{
    uint64_t max_num_msg;
    uint64_t msg_size;
} st_osal_inner_mq_config_t;

typedef struct
{
    st_osal_os_thread_t self_thread;
    pthread_mutex_t     self_mutex;
    bool                use_flag;
}st_osal_thread_self_control_t;

typedef struct st_osal_interrupt_thread_control
{
    uint64_t              handle_id;            /*!< Fixed value: OSAL_INTERRUPT_HANDLE_ID */
    bool                  register_signal;      /*!< Registration notification flag */
    bool                  thread_active;        /*!< interrupt thread active flag(true: active interrupt thread is active, false: interrupt thread is inactive) */
    pthread_cond_t        inner_cond;           /*!< Internal condition variable object */
    pthread_mutex_t       inner_mtx;            /*!< Internal mutex object */
    pthread_mutex_t       inner_mtx_for_cond;   /*!< Internal mutex object for waiting for condition variables  */
    osal_milli_sec_t      inner_timeout;        /*!< Waiting time of lock inner_mtx */
    osal_device_handle_t  device_handle;        /*!< Handle of device according to interrupt */
    pthread_t             thread_id;            /*!< Handle of interrupt thread for QNX */
    p_osal_isr_func_t     irq_handler;          /*!< Pointer of Worker function of Interrupt thread */
    void                  *irq_arg;             /*!< Pointer of Worker function argument */
    uint64_t              irq_channel;          /*!< irq number channel */
    bool                  irq_enable;           /*!< interrupt status (true: Enable interrupt, false: Disable interrupt) */
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                   event_id;         /*!< Attach event ID */

    const struct st_osal_interrupt_thread_config *usr_config; /*!< User configuration for message queue */
} st_osal_interrupt_thread_control_t;

typedef struct
{
    const st_osal_interrupt_thread_affinity_t *p_affinity_info;
} st_osal_interrupt_thread_affinity_addr_t;

typedef struct
{
    const st_osal_thread_affinity_t *p_affinity_info;
} st_osal_thread_affinity_addr_t;

/* ----------------------------------------------- */

#include "target/common/pma/r_pma_common.h"
#include "target/common/pma/manager/r_pma_manager.h"
#include "target/common/pma/statemachine/r_pma_statemachine.h"
#include "target/common/pma/hw/r_pma_hwdepend.h"
#include "target/common/pma/bsp_wrapper/r_pma_bsp_wrapper.h"
#include "target/common/pma/r_pma.h"

#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>

/* WINAMS_VAR_EXT[e_pma_hwa_id_t g_pma_max_hwa_id_num] */
extern e_pma_hwa_id_t g_pma_max_hwa_id_num;
/* -------- xos3 UT stub --------------------------------------- */
st_pma_handle_t         dummy_handle;
st_hwa_t                dummy_pHwa[1];
e_pma_power_policy_t    dummy_pPolicy;
st_pma_hwa_state_t      dummy_HwaStat;
e_pma_reset_req_t       dummy_pRstState;
e_pma_power_state_t     dummy_pLowestPower;
st_statemachine_t       dummy_pStatemachine;
st_hwa_t                dummy_targetHwa;
e_result_t dummy_NoticeChange(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId){}
e_result_t dummy_pFunc(e_pma_hwa_id_t hwaId){}
/* Target HW Dependdent Table */
uint64_t                dummy_g_pma_hw_dep_hwa_bit_tbl[333];
e_pma_hwa_id_t          dummy_g_pma_hw_dep_hwa_bit_to_id_tbl[333];
uint64_t                dummy_g_pma_hw_dep_sysc_ref_tbl[46];
uint64_t                dummy_g_pma_hw_dep_cpg_ref_tbl[638];
st_hwa_t                dummy_g_pma_hw_dep_hwa_info_tbl[333];
st_hwa_dependent_flag_t dummy_g_pma_hw_dep_hwa_dependency_tbl[333];
st_osal_time_t g_st_osal_dif_time;

e_pma_onoff_t dummy_pClkStat;
e_pma_onoff_t dummy_pPwrStat;
e_pma_clock_onoff_t dummy_modState;
uint32_t dummy_pClockDivider;


size_t dummy_size;
int dummy_init(int vbose){}
int dummy_deinit(void){}
int dummy_mstp_is_enabled(int mod_id){}
int dummy_mstp_enable(int mod_id){}
int dummy_mstp_disable(int mod_id){}
int dummy_mstp_reset(int mod_id){}
int dummy_mstp_assert(int mod_id){}
int dummy_mstp_deassert(int mod_id){}
int dummy_clk_is_enabled(cpg_clk_id_t id, int idx){}
int dummy_clk_enable(cpg_clk_id_t id, int idx){}
int dummy_clk_disable(cpg_clk_id_t id, int idx){}
int dummy_clk_rate_get(cpg_clk_id_t id, int idx, uint32_t *rate){}
int dummy_clk_rate_set(cpg_clk_id_t id, int idx, uint32_t *rate){}
int dummy_clk_divider_get(cpg_clk_id_t id, int idx, uint32_t *div){}
int dummy_clk_divider_set(cpg_clk_id_t id, int idx, uint32_t *div){}
int dummy_mstp_reset_state(int mod_id){}

cpg_mgr_funcs_t dummy_clkhwfuncs = {0,dummy_init,0,dummy_mstp_is_enabled, dummy_mstp_enable, dummy_mstp_disable,0, dummy_mstp_assert, dummy_mstp_deassert,dummy_clk_is_enabled, dummy_clk_enable, dummy_clk_disable,0,0,0,dummy_clk_divider_set,dummy_mstp_reset_state};

int dummy_sysc_init(void){}
void dummy_sysc_deinit(void){}
int dummy_sysc_power_is_off(int pdr_number){}
int dummy_sysc_power_off(int pdr_number){}
int dummy_sysc_power_on(int pdr_number){}
int dummy_sysc_read_pdmode(void){}
sysc_mgr_funcs_t dummy_syschwfuncs ={0,dummy_sysc_init,0,dummy_sysc_power_is_off,dummy_sysc_power_off,dummy_sysc_power_on,0};


st_pma_user_info_t      dummy_user_info[16];

const int * dummy_g_pma_bsp_wrap_sysc_id_tbl;
const int * dummy_g_pma_bsp_wrap_cpg_id_tbl;

/* ----------------------------------------------- */
int       g_dummy;
bool      g_bool;
int       g_key;
int       g_array[10];
void*     gp_void;
FILE      g_FILE;
uintptr_t g_uintptr_t;
int32_t   g_int32_t;
uint8_t   g_uint8_t;
uint16_t  g_uint16_t;
uint32_t  g_uint32_t;
uint64_t  g_uint64_t;
uint8_t   g_uint8_array[10];
uint16_t  g_uint16_array[10];
uint32_t  g_uint32_array[10];
uint64_t  g_uint64_array[10];
uint64_t  g_irq_channel;
struct timespec g_os_time;
struct timespec g_os_restime;
/* for generic variable */
uint8_t array_uint8[10];
uint16_t array_uint16[10];
uint32_t array_uint32[10];
uint64_t array_uint64[10];
size_t   array_sizet[10];
bool * p_result_common;
uint8_t g_uint8_t;
uint16_t g_uint16_t;
uint32_t g_uint32_t;
uint64_t g_uint64_t;
size_t g_size_t;
bool g_bool;
void* gp_void;
uintptr_t g_uintptr_t;
FILE g_FILE;


/* for Helper Cond */
st_osal_os_cond_t g_osal_common_cond;
st_osal_os_mutex_t g_osal_common_mutex;

/* for Common Clock&Time */
st_osal_time_t g_st_osal_time;
osal_nano_sec_t g_osal_nano_sec_t;

/* OS IF */
st_osal_os_cond_t g_os_cond;
st_osal_os_condattr_t g_os_cond_attr;
st_osal_os_mutex_t g_os_mutex;
st_osal_os_mutexattr_t g_os_mutex_attr;
st_osal_thread_self_control_t g_st_osal_thread_self_control[10];
st_osal_thread_config_t g_uts_thread_config;
st_osal_thread_affinity_addr_t g_st_osal_thread_affinity_addr_t[10];
st_osal_os_thread_t g_osal_os_thread;
st_osal_os_threadattr_t g_osal_os_threadattr;
st_osal_device_config_addr_t g_device_config_addr[400];
st_osal_device_control_t g_osal_device_control[10];
struct st_osal_device_info g_osal_device_config[10];
st_osal_interrupt_info_t g_st_osal_interrupt_info[10];
st_osal_interrupt_thread_control_t g_osal_interrupt_thread_control[10];
st_osal_interrupt_thread_affinity_addr_t g_st_osal_interrupt_thread_affinity_addr_t[10];
bool dummy_flag;
bool * gp_osal_os_interrupt_init_flag = &dummy_flag;

int AMSTB_compare_function(const void* p_cmp_info1, const void* p_cmp_info2){return 1;}
void* AMSTB_osal_os_thread_func(void* p_arg){return NULL;}
void AMSTB_irq_cb_dummy(osal_device_handle_t device_handle, uint64_t irq_channel, void *irq_arg)
{
    static bool volatile AM_flags;
    static bool volatile AM_active;
    g_osal_interrupt_thread_control[0].irq_enable = AM_flags;
    return;
}

/* DCFG */
st_osal_device_control_t g_st_osal_device_control;
st_osal_device_control_t g_st_osal_device_control_array[10];
struct st_osal_device_info g_st_osal_device_info;

/* DCFG power */
st_pma_handle_t   g_st_pma_handle;
e_osal_pm_state_t g_e_osal_pm_state;

/* DCFG io */
typedef struct
{
    int32_t     current_utlb;
    size_t      match_count;
} st_osal_utlb_list_control_t;

size_t   g_size_t;
size_t   array_sizet[10];
char     str[256];
char     char_dummy[32] = {"12\n3"};
uint8_t  array_uint8[10];
uint64_t array_uint64[10];
size_t   dummy_array_sizet[5];

st_osal_utlb_list_control_t    g_st_osal_utlb_list_control;
osal_axi_bus_id_t              g_osal_axi_bus_id;
st_osal_axi_bus_name_info_t    g_axi_bus_name_info_array[10];
st_osal_axi_bus_name_info_t    *g_axi_bus_name_info_p;
st_osal_axi_bus_list_t         g_st_osal_axi_bus_list[10];
st_osal_os_mutex_t             g_osal_os_mutex;
st_osal_utlb_assignment_info_t g_st_assignment;
osal_device_handle_t           g_osal_device_handle;
struct st_osal_device_config_addr g_osal_device_config_addr_array[10];

/* case normal */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_1[3]   = {{"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};

/* case gsp_axi_bus_name_info[count].utlb = OSAL_INVALID_UTLB */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_2[3]   = {{"dummy_domain_name", -2, char_dummy, 32},
                                                          {"dummy_domain_name", -2, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};

/* case gsp_axi_bus_name_info[count].utlb = OSAL_INVALID_UTLB to OSAL_AXI_BUS_NAME_LAST */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_3[3]   = {{"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", -2, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};

/* case gsp_axi_bus_name_info[count].utlb = OSAL_AXI_BUS_NAME_LAST */
st_osal_axi_bus_name_info_t g_axi_bus_name_info_4[3]   = {{"dummy_domain_name",  1, char_dummy, 32},
                                                          {"dummy_domain_name",  1, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};

st_osal_axi_bus_name_info_t g_axi_bus_name_info_5[4]   = {{"dummy_domain_name",  1, char_dummy, 32},
                                                          {"dummy_domain_name",  2, char_dummy, 32},
                                                          {"dummy_domain_name",  3, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};

st_osal_axi_bus_name_info_t g_axi_bus_name_info_6[6]   = {{"dummy_domain_name",  0, char_dummy, 32},
                                                          {"dummy_domain_name", 10, char_dummy, 32},
                                                          {"dummy_domain_name", 11, char_dummy, 32},
                                                          {"dummy_domain_name", 12, char_dummy, 32},
                                                          {"dummy_domain_name", 20, char_dummy, 32},
                                                          {"dummy_domain_name", -1, char_dummy, 32},};

/* RCFG memory */
typedef struct
{
    e_osal_init_status_t            osal_memory_is_opened;
    osal_memory_manager_handle_t    osal_memory_manager;
} st_osal_memory_manager_table_t;

osal_memory_buffer_handle_t g_memory_buffer_handle;
osal_memory_manager_handle_t g_memory_manager_handle;

st_osal_mmngr_config_t g_st_osal_mmngr_config;
st_osal_mem_mmngr_ptr_self_t      g_st_osal_mem_mmngr_ptr_self[30];
st_osal_mem_mmngr_ptr_self_t      *g_st_osal_mem_mmngr_ptr_self_pp[30];

struct st_osal_memory_buffer_obj  g_st_osal_memory_buffer_obj;
st_osal_mem_buf_ptr_self_t        g_osal_mem_buf_ptr_self;
struct st_osal_memory_manager_obj g_st_osal_memory_manager_obj[30];
st_osal_memory_manager_table_t    g_st_osal_memory_manager_table;
st_osal_mem_monitor_desc_t        g_st_osal_mem_monitor_desc;
st_osal_mem_monitor_desc_t        g_st_osal_mem_monitor_desc_array[10];
struct st_osal_memory_buffer_obj * g_osal_memory_buffer_obj;

uint8_t mmngr_buffer[OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE + 64 + OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE];

void osal_memory_mngr_cb_dummy(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)event;
    (void)error;
    (void)buffer_hndl;
    return;
}

/* Common stub for internal memory functions */
e_osal_return_t AMSTB_osal_mem_internal()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* RCFG ThreadSync */
typedef struct
{
    const struct st_osal_rcfg_cond_config *p_usr_config;
}st_osal_cond_config_addr_t;

osal_cond_id_t g_cond_id;
osal_cond_handle_t g_osal_cond_handle;
st_osal_os_cond_t             g_osal_os_cond;
st_osal_cond_control_t g_st_osal_cond_control;
st_osal_cond_control_t g_st_osal_cond_control_array[10];
struct st_osal_rcfg_cond_config g_st_osal_rcfg_cond_config;
struct st_osal_rcfg_cond_config g_st_osal_rcfg_cond_config_2;
st_osal_cond_config_addr_t g_st_osal_cond_config_addr1;
st_osal_cond_config_addr_t g_st_osal_cond_config_addr2;

typedef struct
{
    const struct st_osal_rcfg_mutex_config *p_usr_config;
}st_osal_mutex_config_addr_t;

osal_mutex_id_t g_mutex_id;
osal_mutex_handle_t g_osal_mutex_handle;
st_osal_mutex_control_t g_st_osal_mutex_control;
st_osal_mutex_control_t g_st_osal_mutex_control_array[10];
struct st_osal_rcfg_mutex_config g_st_osal_rcfg_mutex_config;
struct st_osal_rcfg_mutex_config g_st_osal_rcfg_mutex_config_2;
st_osal_mutex_config_addr_t g_st_osal_mutex_config_addr1;
st_osal_mutex_config_addr_t g_st_osal_mutex_config_addr2;

/* start of RCFG Thread */

typedef struct
{
    const struct st_osal_rcfg_thread_config *p_usr_config;
}st_osal_thread_config_addr_t;

st_osal_rcfg_thread_config_t         g_st_osal_rcfg_thread_config[10];
st_osal_rcfg_thread_config_t *       gp_osal_thread_config;
st_osal_thread_config_addr_t         g_st_osal_thread_config_addr;
st_osal_thread_config_addr_t         g_st_osal_thread_config_addr1[10];
st_osal_rcfg_thread_config_t         g_st_osal_rcfg_thread_config1[10];
st_osal_thread_config_addr_t         g_st_osal_thread_config_addr2[10];
st_osal_rcfg_thread_config_t         g_st_osal_rcfg_thread_config2[10];
st_osal_thread_control_t             g_st_osal_thread_control[10];
st_osal_thread_control_t             g_thread_control;
st_osal_thread_control_t             g_st_osal_thread_control_0[10];
st_osal_thread_control_t             g_st_osal_thread_control_1[10];
st_osal_thread_control_t             g_thread_control_1[10];
st_osal_thread_control_t             g_thread_control_2[10];
st_osal_thread_control_t *           gp_thread_control_dummy;
st_osal_thread_control_t **          gpp_thread_control_dummy = &gp_thread_control_dummy;
struct st_osal_os_thread *           g_thread_id;
st_osal_thread_config_t              g_st_osal_thread_config[10];
osal_thread_handle_t*                g_uts_thread_handle;
int64_t* const                       g_return_value;

p_osal_thread_func_t osal_thread_func_dummy(void* user_arg)
{
    (void)user_arg;
    return 0;
}

/* end of RCFG Thread */

/* start of RCFG Message */

typedef struct
{
    const struct st_osal_rcfg_mq_config *p_usr_config;
}st_osal_mq_config_addr_t;


st_osal_mq_rcfg_config_t g_st_osal_mq_rcfg_config[10];
st_osal_mq_rcfg_config_t g_st_osal_mq_rcfg_config_1[10];
st_osal_mq_rcfg_config_t g_st_osal_mq_rcfg_config_2[10];
st_osal_mq_config_addr_t g_st_osal_mq_config_addr[10];
st_osal_mq_config_addr_t g_st_osal_mq_config_addr_1[10];
st_osal_mq_config_addr_t g_st_osal_mq_config_addr_2[10];
st_osal_mq_config_t      g_st_osal_mq_config[10];
st_osal_mq_control_t     g_st_osal_mq_control[10];
st_osal_mq_control_t *   gp_st_osal_mq_control;
st_osal_mq_control_t **  gpp_st_osal_mq_control = &gp_st_osal_mq_control;
st_osal_queue_t          g_st_osal_queue[10];
osal_mq_handle_t         g_mq_control;
uint32_t                 array_uint32[10];
osal_mq_id_t             osal_mq_id;

/* end of RCFG Message */

/* for Common Thread */
osal_thread_handle_t* const g_uts_common_thread_handle;
st_osal_thread_config_t* const g_uts_common_thread_config;
st_osal_thread_control_t g_thread_common_control_1;
st_osal_thread_control_t g_thread_common_control_2;

/* for Common ThreadSync */
osal_cond_handle_t g_osal_common_cond_handle;
st_osal_cond_control_t g_st_osal_common_cond_control;
osal_mutex_handle_t g_osal_common_mutex_handle;
st_osal_mutex_control_t g_st_osal_common_mutex_control;

/* for Common Message Queue */
st_osal_mq_config_t g_st_osal_common_mq_config;
osal_mq_handle_t    g_osal_common_mq_handle;
st_osal_mq_control_t g_st_osal_common_mq_control;

/* for Common related device */
size_t   array_sizet[10];
char     str[256];
osal_device_handle_t g_osal_common_device_handle;
st_osal_device_control_t g_st_osal_common_device_control;
st_osal_device_control_t g_st_osal_common_device_control_array[10];
osal_axi_bus_id_t g_osal_common_axi_bus_id;

/* for Common Memory */
st_osal_mmngr_config_t g_st_osal_common_mmngr_config;
osal_memory_manager_handle_t g_osal_common_memory_manager_handle;
struct st_osal_memory_manager_obj g_st_osal_common_memory_manager_obj;
osal_memory_buffer_handle_t g_osal_common_memory_buffer_handle;
struct st_osal_memory_buffer_obj g_st_osal_common_memory_buffer_obj;
osal_memory_manager_t g_osal_common_memory_manager;
osal_memory_buffer_t g_osal_common_memory_buffer;
struct st_osal_memory_buffer_obj * g_osal_common_memory_buffer_obj;

/* for Common Power */
struct st_osal_device_info g_st_osal_common_device_info;
st_pma_handle_t   g_st_pma_common_handle;

/* for MEML */
st_meml_buffer_t meml_buffer_dummy[1024];
char dummy_buffer_pa[4096];
char dummy_buffer_va[4096];
st_meml_manage_t meml_manager_dummy;
st_meml_manage_t *p_meml_manager = &meml_manager_dummy;
st_meml_buffer_t *p_meml_buffer_dummy = meml_buffer_dummy;
st_meml_buffer_t *p_meml_buffer_dummy1 = &meml_buffer_dummy[384];
st_meml_buffer_t *p_meml_buffer_dummy2 = &meml_buffer_dummy[768];
st_meml_buffer_t *p_meml_manager_own = &(meml_manager_dummy.buffer);

/* for OSIF Message */
osal_os_mqattr_t         g_osal_os_mqattr[10];
osal_os_mqattr_t *       gp_osal_os_mqattr;
st_osal_os_mq_t          g_st_osal_os_mq[10];
st_osal_os_mq_t *        gp_st_osal_os_mq[10];
st_osal_os_mq_t **       gpp_st_osal_os_mq[10];
pthread_mutex_t          g_mutex;

/* ----------------------------------------------- */
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

/* ----------------------------------------------- */

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

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetPowerPolicy:AMSTB_R_PMA_GetPowerPolicy:inout:::] */
/*    R_PMA_GetPowerPolicy => Stub */
e_pma_return_t AMSTB_R_PMA_GetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t volatile AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_policy_t *volatile AMOUT_pPolicy;
    static e_pma_power_policy_t volatile AMIN_Policy;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pPolicy = pPolicy;

    *pPolicy = AMIN_Policy;

    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetLockHwa:AMSTB_R_PMA_MNG_GetLockHwa:inout:::] */
/*    R_PMA_MNG_GetLockHwa => Stub */
e_result_t AMSTB_R_PMA_MNG_GetLockHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_CheckUser:AMSTB_R_PMA_IF_CheckUser:inout:::] */
/*    R_PMA_IF_CheckUser => Stub */
e_pma_return_t AMSTB_R_PMA_IF_CheckUser(st_pma_handle_t pUserInfo)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_pUserInfo;
    AMOUT_pUserInfo = pUserInfo;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_Exchange_ErrCode:AMSTB_R_PMA_IF_Exchange_ErrCode:inout:::] */
/*    R_PMA_IF_Exchange_ErrCode => Stub */
e_pma_return_t AMSTB_R_PMA_IF_Exchange_ErrCode(e_result_t innerErr)
{
    static e_pma_return_t volatile AMIN_return;
    static e_result_t volatile AMOUT_innerErr;
    AMOUT_innerErr = innerErr;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_Init:AMSTB_R_PMA_MNG_Init:inout:::] */
/*    R_PMA_MNG_Init => Stub */
e_result_t AMSTB_R_PMA_MNG_Init(const st_pma_config_t *pConfig)
{
    static e_pma_hwa_id_t volatile AMIN_g_pma_max_hwa_id_num;
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static const st_pma_config_t *volatile AMOUT_pConfig;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    g_pma_max_hwa_id_num = AMIN_g_pma_max_hwa_id_num;
    AMOUT_pConfig = pConfig;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_IF_GetUserInfo:AMSTB_R_PMA_IF_GetUserInfo:inout::array:counter<AM_count>] */
/*    R_PMA_IF_GetUserInfo => Stub */
st_pma_handle_t AMSTB_R_PMA_IF_GetUserInfo(unsigned int userNo)
{
#define AMCALLMAX_R_PMA_IF_GetUserInfo 16
    static st_pma_handle_t volatile AMIN_return[AMCALLMAX_R_PMA_IF_GetUserInfo];
    static unsigned int volatile AMOUT_userNo[AMCALLMAX_R_PMA_IF_GetUserInfo];
    static int volatile AM_count;
    AMOUT_userNo[AM_count] = userNo;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_Quit:AMSTB_R_PMA_MNG_Quit:inout:::] */
/*    R_PMA_MNG_Quit => Stub */
e_result_t AMSTB_R_PMA_MNG_Quit()
{
    static e_result_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_SendMessageToTask:AMSTB_R_PMA_MNG_SendMessageToTask:inout:::] */
/*    R_PMA_MNG_SendMessageToTask => Stub */
e_result_t AMSTB_R_PMA_MNG_SendMessageToTask(st_msg_to_task_t *pMsg)
{
    static e_result_t volatile AMIN_return;
    static st_msg_to_task_t *volatile AMOUT_pMsg;
    AMOUT_pMsg = pMsg;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_UnlockHwa:AMSTB_R_PMA_MNG_UnlockHwa:inout:::] */
/*    R_PMA_MNG_UnlockHwa => Stub */
e_result_t AMSTB_R_PMA_MNG_UnlockHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_ReleaseHwa:AMSTB_R_PMA_ReleaseHwa:inout:::] */
/*    R_PMA_ReleaseHwa => Stub */
e_pma_return_t AMSTB_R_PMA_ReleaseHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetReset:AMSTB_R_PMA_MNG_GetReset:inout:::] */
/*    R_PMA_MNG_GetReset => Stub */
e_result_t AMSTB_R_PMA_MNG_GetReset(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstState)
{
    static e_result_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_reset_req_t *volatile AMOUT_pRstState;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pRstState = pRstState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetPowerPolicy:AMSTB_R_PMA_MNG_GetPowerPolicy:inout:::] */
/*    R_PMA_MNG_GetPowerPolicy => Stub */
e_result_t AMSTB_R_PMA_MNG_GetPowerPolicy(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_power_policy_t *pPolicy)
{
    static e_result_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_policy_t *volatile AMOUT_pPolicy;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pPolicy = pPolicy;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetLowestPowerState:AMSTB_R_PMA_MNG_GetLowestPowerState:inout:::] */
/*    R_PMA_MNG_GetLowestPowerState => Stub */
e_result_t AMSTB_R_PMA_MNG_GetLowestPowerState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_power_state_t *pLowestPower)
{
    static e_result_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_state_t *volatile AMOUT_pLowestPower;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pLowestPower = pLowestPower;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetHwaState:AMSTB_R_PMA_MNG_GetHwaState:inout:::] */
/*    R_PMA_MNG_GetHwaState => Stub */
e_result_t AMSTB_R_PMA_MNG_GetHwaState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,st_pma_hwa_state_t *pHwaState)
{
    static e_result_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static st_pma_hwa_state_t *volatile AMOUT_pHwaState;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pHwaState = pHwaState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_SetPostClock:AMSTB_R_PMA_MNG_SetPostClock:inout:::] */
/*    R_PMA_MNG_SetPostClock => Stub */
e_result_t AMSTB_R_PMA_MNG_SetPostClock(e_pma_clk_req_t clockEnable,unsigned int chanIdx,unsigned int *pClockDivider)
{
    static e_result_t volatile AMIN_return;
    static e_pma_clk_req_t volatile AMOUT_clockEnable;
    static unsigned int volatile AMOUT_chanIdx;
    static unsigned int *volatile AMOUT_pClockDivider;
    AMOUT_clockEnable = clockEnable;
    AMOUT_chanIdx = chanIdx;
    AMOUT_pClockDivider = pClockDivider;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_WaitForState:AMSTB_R_PMA_MNG_WaitForState:inout:::] */
/*    R_PMA_MNG_WaitForState => Stub */
e_result_t AMSTB_R_PMA_MNG_WaitForState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,const st_pma_hwa_state_t *pHwaState,e_pma_reset_req_t rstStat,long long sleepTimePeriod,long long timeoutMsPeriod,long long timeoutNsPeriod)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static const st_pma_hwa_state_t *volatile AMOUT_pHwaState;
    static e_pma_reset_req_t volatile AMOUT_rstStat;
    static long long volatile AMOUT_sleepTimePeriod;
    static long long volatile AMOUT_timeoutMsPeriod;
    static long long volatile AMOUT_timeoutNsPeriod;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pHwaState = pHwaState;
    AMOUT_rstStat = rstStat;
    AMOUT_sleepTimePeriod = sleepTimePeriod;
    AMOUT_timeoutMsPeriod = timeoutMsPeriod;
    AMOUT_timeoutNsPeriod = timeoutNsPeriod;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg.c:R_OSAL_DCFG_GetSocType:AMSTB_R_OSAL_DCFG_GetSocType:inout:::] */
/*    R_OSAL_DCFG_GetSocType => Stub */
unsigned long long AMSTB_R_OSAL_DCFG_GetSocType()
{
    static unsigned long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_Init:AMSTB_R_PMA_BSP_WRAP_Init:inout:::] */
/*    R_PMA_BSP_WRAP_Init => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_Init(unsigned long long pma_soc_type)
{
    static const int *volatile AMIN_g_pma_bsp_wrap_cpg_id_tbl;
    static const int *volatile AMIN_g_pma_bsp_wrap_sysc_id_tbl;
    static e_result_t volatile AMIN_return;
    static unsigned long long volatile AMOUT_pma_soc_type;
    AMOUT_pma_soc_type = pma_soc_type;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_Init:AMSTB_R_PMA_HW_DEP_Init:inout:::] */
/*    R_PMA_HW_DEP_Init => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_Init(unsigned long long pma_soc_type,p_sm_stat_chg_notice_func_t pFunc)
{
    static e_result_t volatile AMIN_return;
    static unsigned long long volatile AMOUT_pma_soc_type;
    static p_sm_stat_chg_notice_func_t volatile AMOUT_pFunc;
    AMOUT_pma_soc_type = pma_soc_type;
    AMOUT_pFunc = pFunc;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_Init:AMSTB_R_PMA_SM_Init:inout:::] */
/*    R_PMA_SM_Init => Stub */
e_result_t AMSTB_R_PMA_SM_Init(e_pma_hwa_id_t maxHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_maxHwaId;
    AMOUT_maxHwaId = maxHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_CheckConfigParam:AMSTB_R_PMA_MNG_CheckConfigParam:inout:::] */
/*    R_PMA_MNG_CheckConfigParam => Stub */
e_result_t AMSTB_R_PMA_MNG_CheckConfigParam(const st_pma_config_t *pConfig,st_pma_param_t *pParam)
{
    static e_result_t volatile AMIN_return;
    static const st_pma_config_t *volatile AMOUT_pConfig;
    static st_pma_param_t volatile AMOUT_pParam;
    AMOUT_pConfig = pConfig;
    pParam->timeout_ticks = AMOUT_pParam.timeout_ticks;
    pParam->task_priority = AMOUT_pParam.task_priority;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_SetupQueue:AMSTB_R_PMA_MNG_SetupQueue:inout:::] */
/*    R_PMA_MNG_SetupQueue => Stub */
e_result_t AMSTB_R_PMA_MNG_SetupQueue(struct st_osal_inner_mq_control **pTaskQueue)
{
    static e_result_t volatile AMIN_return;
    static struct st_osal_inner_mq_control **volatile AMOUT_pTaskQueue;
    AMOUT_pTaskQueue = pTaskQueue;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_SetupTask:AMSTB_R_PMA_MNG_SetupTask:inout:::] */
/*    R_PMA_MNG_SetupTask => Stub */
e_result_t AMSTB_R_PMA_MNG_SetupTask(int taskPriority,struct st_osal_inner_thread_control **pThread)
{
    static e_result_t volatile AMIN_return;
    static int volatile AMOUT_taskPriority;
    static struct st_osal_inner_thread_control **volatile AMOUT_pThread;
    AMOUT_taskPriority = taskPriority;
    AMOUT_pThread = pThread;
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

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_Quit:AMSTB_R_PMA_BSP_WRAP_Quit:inout:::] */
/*    R_PMA_BSP_WRAP_Quit => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_Quit()
{
    static e_result_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_Quit:AMSTB_R_PMA_SM_Quit:inout:::] */
/*    R_PMA_SM_Quit => Stub */
void AMSTB_R_PMA_SM_Quit()
{
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    static int volatile AM_count;
    AM_count++;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqTimedSend:AMSTB_R_OSAL_Internal_MqTimedSend:inout:::] */
/*    R_OSAL_Internal_MqTimedSend => Stub */
int AMSTB_R_OSAL_Internal_MqTimedSend(struct st_osal_inner_mq_control *handle,long long time_period,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_inner_mq_control *volatile AMOUT_handle;
    static long long volatile AMOUT_time_period;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_time_period = time_period;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexUnlock:AMSTB_R_OSAL_Internal_MutexUnlock:inout:::] */
/*    R_OSAL_Internal_MutexUnlock => Stub */
int AMSTB_R_OSAL_Internal_MutexUnlock(struct st_osal_inner_mutex_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_inner_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetLockHwa:AMSTB_R_PMA_SM_GetLockHwa:inout:::] */
/*    R_PMA_SM_GetLockHwa => Stub */
e_result_t AMSTB_R_PMA_SM_GetLockHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_UnlockHwa:AMSTB_R_PMA_SM_UnlockHwa:inout:::] */
/*    R_PMA_SM_UnlockHwa => Stub */
e_result_t AMSTB_R_PMA_SM_UnlockHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetHwaState:AMSTB_R_PMA_SM_GetHwaState:inout:::] */
/*    R_PMA_SM_GetHwaState => Stub */
e_result_t AMSTB_R_PMA_SM_GetHwaState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,st_pma_hwa_state_t *pHwaState)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static st_pma_hwa_state_t *volatile AMOUT_pHwaState;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pHwaState = pHwaState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetPowerPolicy:AMSTB_R_PMA_SM_GetPowerPolicy:inout:::] */
/*    R_PMA_SM_GetPowerPolicy => Stub */
e_result_t AMSTB_R_PMA_SM_GetPowerPolicy(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_power_policy_t *pPolicy)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_policy_t *volatile AMOUT_pPolicy;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pPolicy = pPolicy;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetResetState:AMSTB_R_PMA_SM_GetResetState:inout:::] */
/*    R_PMA_SM_GetResetState => Stub */
e_result_t AMSTB_R_PMA_SM_GetResetState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstState)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_reset_req_t *volatile AMOUT_pRstState;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pRstState = pRstState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_GetLowestPowerState:AMSTB_R_PMA_SM_GetLowestPowerState:inout:::] */
/*    R_PMA_SM_GetLowestPowerState => Stub */
e_result_t AMSTB_R_PMA_SM_GetLowestPowerState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_power_state_t *pLowestPower)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_state_t *volatile AMOUT_pLowestPower;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pLowestPower = pLowestPower;
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

/* WINAMS_STUB[stub.c:memset:AMSTB_memset:inout:::] */
/*    memset => Stub */
void *AMSTB_memset(void *s,int c,unsigned long long n)
{
    static void *volatile AMIN_return;
    static void *volatile AMOUT_s;
    static int volatile AMOUT_c;
    static unsigned long long volatile AMOUT_n;
    AMOUT_s = s;
    AMOUT_c = c;
    AMOUT_n = n;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetCurrentState:AMSTB_R_PMA_MNG_GetCurrentState:inout:::counter<AM_count>] */
/*    R_PMA_MNG_GetCurrentState => Stub */
e_result_t AMSTB_R_PMA_MNG_GetCurrentState(struct st_pma_user_info_t *handle,e_pma_hwa_id_t hwaId,const st_pma_hwa_state_t *pHwaState,st_pma_hwa_state_t *pCurHwaState,e_pma_reset_req_t *pCurRstStat)
{
#define AMCALLMAX_R_PMA_IF_GetCurrentState 2
    static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static struct st_osal_mutex_control *volatile AMOUT_R_PMA_SM_GxMutex[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static struct st_pma_user_info_t *volatile AMOUT_handle[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static e_pma_hwa_id_t volatile AMOUT_hwaId[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static const st_pma_hwa_state_t *volatile AMOUT_pHwaState[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static st_pma_hwa_state_t volatile AMOUT_pCurHwaState[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static e_pma_reset_req_t volatile AMOUT_pCurRstStat[AMCALLMAX_R_PMA_IF_GetCurrentState];
    static int volatile AM_count;
    AMOUT_handle[AM_count] = handle;
    AMOUT_hwaId[AM_count] = hwaId;
    AMOUT_pHwaState[AM_count] = pHwaState;
    pCurHwaState->enReqState = AMOUT_pCurHwaState[AM_count].enReqState;
    pCurHwaState->enActualPwrStat = AMOUT_pCurHwaState[AM_count].enActualPwrStat;
    pCurHwaState->enActualClkStat = AMOUT_pCurHwaState[AM_count].enActualClkStat;
    *pCurRstStat = AMOUT_pCurRstStat[AM_count];
    AM_count++;

    return AMIN_return[AM_count - 1];
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

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MsSleep:AMSTB_R_OSAL_Internal_MsSleep:inout:::] */
/*    R_OSAL_Internal_MsSleep => Stub */
int AMSTB_R_OSAL_Internal_MsSleep(long long milliseconds)
{
    static int volatile AMIN_return;
    static long long volatile AMOUT_milliseconds;
    AMOUT_milliseconds = milliseconds;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_DisablePostClock:AMSTB_R_PMA_BSP_WRAP_DisablePostClock:inout:::] */
/*    R_PMA_BSP_WRAP_DisablePostClock => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_DisablePostClock(unsigned int chanIdx)
{
    static e_result_t volatile AMIN_return;
    static unsigned int volatile AMOUT_chanIdx;
    AMOUT_chanIdx = chanIdx;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_EnablePostClock:AMSTB_R_PMA_BSP_WRAP_EnablePostClock:inout:::] */
/*    R_PMA_BSP_WRAP_EnablePostClock => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_EnablePostClock(unsigned int chanIdx)
{
    static e_result_t volatile AMIN_return;
    static unsigned int volatile AMOUT_chanIdx;
    AMOUT_chanIdx = chanIdx;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_SetPostDivider:AMSTB_R_PMA_BSP_WRAP_SetPostDivider:inout:::] */
/*    R_PMA_BSP_WRAP_SetPostDivider => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_SetPostDivider(unsigned int chanIdx,unsigned int *pClockDivider)
{
    static e_result_t volatile AMIN_return;
    static unsigned int volatile AMOUT_chanIdx;
    static unsigned int *volatile AMOUT_pClockDivider;
    AMOUT_chanIdx = chanIdx;
    AMOUT_pClockDivider = pClockDivider;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_SetMutexAttr:AMSTB_R_OSAL_Internal_SetMutexAttr:inout:::] */
/*    R_OSAL_Internal_SetMutexAttr => Stub */
int AMSTB_R_OSAL_Internal_SetMutexAttr(struct st_osal_inner_mutexattr **p_mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_inner_mutexattr **volatile AMOUT_p_mutex_attr;
    AMOUT_p_mutex_attr = p_mutex_attr;
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
    static struct st_osal_inner_thread_control **volatile AMOUT_p_handle;
    AMOUT_p_config = p_config;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}


/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ApplyResetHWA:AMSTB_R_PMA_SM_ApplyResetHWA:inout:::] */
/*    R_PMA_SM_ApplyResetHWA => Stub */
e_result_t AMSTB_R_PMA_SM_ApplyResetHWA(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ChangePolicyHWA:AMSTB_R_PMA_SM_ChangePolicyHWA:inout:::] */
/*    R_PMA_SM_ChangePolicyHWA => Stub */
e_result_t AMSTB_R_PMA_SM_ChangePolicyHWA(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_power_policy_t policy)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_policy_t volatile AMOUT_policy;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_policy = policy;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ReleaseHWA:AMSTB_R_PMA_SM_ReleaseHWA:inout:::] */
/*    R_PMA_SM_ReleaseHWA => Stub */
e_result_t AMSTB_R_PMA_SM_ReleaseHWA(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ReleaseResetHWA:AMSTB_R_PMA_SM_ReleaseResetHWA:inout:::] */
/*    R_PMA_SM_ReleaseResetHWA => Stub */
e_result_t AMSTB_R_PMA_SM_ReleaseResetHWA(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_RequestHWA:AMSTB_R_PMA_SM_RequestHWA:inout:::] */
/*    R_PMA_SM_RequestHWA => Stub */
e_result_t AMSTB_R_PMA_SM_RequestHWA(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqTimedReceive:AMSTB_R_OSAL_Internal_MqTimedReceive:inout::array:counter<AM_count>] */
/*    R_OSAL_Internal_MqTimedReceive => Stub */
st_msg_to_task_t    dummy_MsgToTask[2];

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

    st_msg_to_task_t * pMsgTotask = p_buffer;
    pMsgTotask->enProcId = dummy_MsgToTask[AM_count].enProcId;
    pMsgTotask->enHwaId = dummy_MsgToTask[AM_count].enHwaId;

    AM_count++;
    return AMIN_return[AM_count - 1];
}


/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexTimedLock:AMSTB_R_OSAL_Internal_MutexTimedLock:inout:::] */
/*    R_OSAL_Internal_MutexTimedLock => Stub */
int AMSTB_R_OSAL_Internal_MutexTimedLock(struct st_osal_inner_mutex_control *handle,long long time_period)
{
    static int volatile AMIN_return;
    static struct st_osal_inner_mutex_control *volatile AMOUT_handle;
    static long long volatile AMOUT_time_period;
    AMOUT_handle = handle;
    AMOUT_time_period = time_period;
    return AMIN_return;
}


/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexTimedLock:AMSTB_3R_PMA_MNG_Task_R_OSAL_Internal_MutexTimedLock:inout:csv_local:array:counter<AM_count>] */
/*    R_PMA_MNG_Task -- R_OSAL_Internal_MutexTimedLock => Stub */
int AMSTB_3R_PMA_MNG_Task_R_OSAL_Internal_MutexTimedLock(struct st_osal_inner_mutex_control *handle,long long time_period)
{
#define AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_Internal_MutexTimedLock 2
    static int volatile AMIN_return[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_Internal_MutexTimedLock];
    static struct st_osal_inner_mutex_control *volatile AMOUT_handle[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_Internal_MutexTimedLock];
    static long long volatile AMOUT_time_period[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_Internal_MutexTimedLock];
    static int volatile AM_count;
    AMOUT_handle[AM_count] = handle;
    AMOUT_time_period[AM_count] = time_period;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_GetHwaObject:AMSTB_R_PMA_HW_DEP_GetHwaObject:inout:::] */
/*    R_PMA_HW_DEP_GetHwaObject => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_GetHwaObject(e_pma_hwa_id_t hwaId,st_hwa_t **pHwa)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static st_hwa_t **volatile AMOUT_pHwa;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    *pHwa = AMOUT_pHwa;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_GetHwaState:AMSTB_R_PMA_HW_DEP_GetHwaState:inout:::] */
/*    R_PMA_HW_DEP_GetHwaState => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_GetHwaState(e_pma_hwa_id_t hwaId,e_pma_onoff_t *pClkStat,e_pma_onoff_t *pPwrStat)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_onoff_t  *volatile AMOUT_pClkStat;
    static e_pma_onoff_t  *volatile AMOUT_pPwrStat;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    *pClkStat = AMOUT_pClkStat;
    *pPwrStat = AMOUT_pPwrStat;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ChangeStateToCG:AMSTB_R_PMA_SM_ChangeStateToCG:inout:::] */
/*    R_PMA_SM_ChangeStateToCG => Stub */
e_result_t AMSTB_R_PMA_SM_ChangeStateToCG(st_statemachine_t *pStatemachine)
{
    static e_result_t volatile AMIN_return;
    static st_statemachine_t *volatile AMOUT_pStatemachine;
    AMOUT_pStatemachine = pStatemachine;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ChangeStateToHP:AMSTB_R_PMA_SM_ChangeStateToHP:inout:::] */
/*    R_PMA_SM_ChangeStateToHP => Stub */
e_result_t AMSTB_R_PMA_SM_ChangeStateToHP(st_statemachine_t *pStatemachine)
{
    static e_result_t volatile AMIN_return;
    static st_statemachine_t *volatile AMOUT_pStatemachine;
    AMOUT_pStatemachine = pStatemachine;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_ChangeStateToPG:AMSTB_R_PMA_SM_ChangeStateToPG:inout:::] */
/*    R_PMA_SM_ChangeStateToPG => Stub */
e_result_t AMSTB_R_PMA_SM_ChangeStateToPG(st_statemachine_t *pStatemachine)
{
    static e_result_t volatile AMIN_return;
    static st_statemachine_t *volatile AMOUT_pStatemachine;
    AMOUT_pStatemachine = pStatemachine;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_ApplyReset:AMSTB_R_PMA_HW_DEP_ApplyReset:inout:::] */
/*    R_PMA_HW_DEP_ApplyReset => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_ApplyReset(e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_ReleaseReset:AMSTB_R_PMA_HW_DEP_ReleaseReset:inout:::] */
/*    R_PMA_HW_DEP_ReleaseReset => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_ReleaseReset(e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_NoticeChange:AMSTB_dummy_NoticeChange:inout:::] */
/*    dummy_NoticeChange => Stub */
e_result_t AMSTB_dummy_NoticeChange(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_refHwaId;
    static int volatile AMOUT_targetHwaId;
    static int volatile AM_count;
    AM_count++;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
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
int AMSTB_dummy_clk_is_enabled(int id,int idx)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_id;
    static int volatile AMOUT_idx;
    AMOUT_id = id;
    AMOUT_idx = idx;
    return AMIN_return;
}

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
    static int volatile AM_count;
    static int volatile AMIN_return[2];
    AM_count++;
    return AMIN_return[AM_count - 1];
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
    static int volatile AMIN_return;
    static int volatile AMOUT_mod_id;
    AMOUT_mod_id = mod_id;
    return AMIN_return;
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
    static int volatile AMIN_return;
    static int volatile AMOUT_mod_id;
    static int volatile AM_count;
    AMOUT_mod_id = mod_id;

    return AMIN_return;
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

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_mstp_reset_state:AMSTB_dummy_mstp_reset_state:inout:::] */
/*    dummy_mstp_reset_state => Stub */
int AMSTB_dummy_mstp_reset_state(int mod_id)
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
    static int volatile AM_count;
    AM_count++;
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

/* WINAMS_STUB[stub.c:cpg_mgr_getfuncs:AMSTB_cpg_mgr_getfuncs:inout:::] */
/*    cpg_mgr_getfuncs => Stub */
int AMSTB_cpg_mgr_getfuncs(cpg_mgr_funcs_t *funcs,int tabsize)
{
    static int volatile AMIN_return;
    static cpg_mgr_funcs_t *volatile AMOUT_funcs;
    static int volatile AMOUT_tabsize;
    AMOUT_funcs = funcs;
    AMOUT_tabsize = tabsize;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:sysc_mgr_getfuncs:AMSTB_sysc_mgr_getfuncs:inout:::] */
/*    sysc_mgr_getfuncs => Stub */
int AMSTB_sysc_mgr_getfuncs(sysc_mgr_funcs_t *funcs,int tabsize)
{
    static int volatile AMIN_return;
    static sysc_mgr_funcs_t *volatile AMOUT_funcs;
    static int volatile AMOUT_tabsize;
    AMOUT_funcs = funcs;
    AMOUT_tabsize = tabsize;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_Exchange_ErrCode:AMSTB_R_PMA_BSP_WRAP_Exchange_ErrCode:inout:::] */
/*    R_PMA_BSP_WRAP_Exchange_ErrCode => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_Exchange_ErrCode(int errNo)
{
    static e_result_t volatile AMIN_return;
    static int volatile AMOUT_errNo;
    AMOUT_errNo = errNo;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_ReadPdmode:AMSTB_R_PMA_BSP_WRAP_ReadPdmode:inout:::] */
/*    R_PMA_BSP_WRAP_ReadPdmode => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_ReadPdmode(e_pma_pd_mode_t *pdMode)
{
    static e_result_t volatile AMIN_return;
    static e_pma_pd_mode_t *volatile AMOUT_pdMode;
    *pdMode = AMOUT_pdMode;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_GetCpgState:AMSTB_R_PMA_BSP_WRAP_GetCpgState:inout:::] */
/*    R_PMA_BSP_WRAP_GetCpgState => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_GetCpgState(e_cpg_domain_id_t modId,e_pma_clock_onoff_t *modState)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_cpg_id_tbl;
    static e_cpg_domain_id_t volatile AMOUT_modId;
    static e_pma_clock_onoff_t volatile AMOUT_modState;
    AMOUT_modId = modId;
    *modState = AMOUT_modState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_GetSyscState:AMSTB_R_PMA_BSP_WRAP_GetSyscState:inout:::] */
/*    R_PMA_BSP_WRAP_GetSyscState => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_GetSyscState(e_sysc_domain_id_t modId,e_pma_power_onoff_t *modState)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_sysc_id_tbl;
    static e_sysc_domain_id_t volatile AMOUT_modId;
    static e_pma_power_onoff_t volatile AMOUT_modState;
    AMOUT_modId = modId;
    *modState = AMOUT_modState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_AssertReset:AMSTB_R_PMA_BSP_WRAP_AssertReset:inout:::] */
/*    R_PMA_BSP_WRAP_AssertReset => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_AssertReset(e_cpg_domain_id_t modId)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_cpg_id_tbl;
    static e_cpg_domain_id_t volatile AMOUT_modId;
    AMOUT_modId = modId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_CheckResetDepChild:AMSTB_R_PMA_HW_DEP_CheckResetDepChild:inout:::] */
/*    R_PMA_HW_DEP_CheckResetDepChild => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_CheckResetDepChild(e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_CheckResetDepParent:AMSTB_R_PMA_HW_DEP_CheckResetDepParent:inout:::] */
/*    R_PMA_HW_DEP_CheckResetDepParent => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_CheckResetDepParent(e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_DeassertReset:AMSTB_R_PMA_BSP_WRAP_DeassertReset:inout:::] */
/*    R_PMA_BSP_WRAP_DeassertReset => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_DeassertReset(e_cpg_domain_id_t modId)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_cpg_id_tbl;
    static e_cpg_domain_id_t volatile AMOUT_modId;
    AMOUT_modId = modId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeToCpgOff:AMSTB_R_PMA_HW_DEP_NoticeToCpgOff:inout:::] */
/*    R_PMA_HW_DEP_NoticeToCpgOff => Stub */
void AMSTB_R_PMA_HW_DEP_NoticeToCpgOff(e_pma_hwa_id_t hwaId,e_cpg_domain_id_t cpgId)
{
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_cpg_domain_id_t volatile AMOUT_cpgId;
    AMOUT_hwaId = hwaId;
    AMOUT_cpgId = cpgId;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeToSyscOff:AMSTB_R_PMA_HW_DEP_NoticeToSyscOff:inout::array:counter<AM_count>] */
/*    R_PMA_HW_DEP_NoticeToSyscOff => Stub */
void AMSTB_R_PMA_HW_DEP_NoticeToSyscOff(e_pma_hwa_id_t hwaId,e_sysc_domain_id_t syscId)
{
#define AMCALLMAX_R_PMA_HW_DEP_NoticeToSyscOff 2
    static e_pma_hwa_id_t volatile AMOUT_hwaId[AMCALLMAX_R_PMA_HW_DEP_NoticeToSyscOff];
    static e_sysc_domain_id_t volatile AMOUT_syscId[AMCALLMAX_R_PMA_HW_DEP_NoticeToSyscOff];
    static int volatile AM_count;
    AMOUT_hwaId[AM_count] = hwaId;
    AMOUT_syscId[AM_count] = syscId;
    AM_count++;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_SetCpgOff:AMSTB_R_PMA_HW_DEP_SetCpgOff:inout::array:counter<AM_count>] */
/*    R_PMA_HW_DEP_SetCpgOff => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_SetCpgOff(e_cpg_domain_id_t modId)
{
#define AMCALLMAX_R_PMA_HW_DEP_SetCpgOff 5
    static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_HW_DEP_SetCpgOff];
    static e_cpg_domain_id_t volatile AMOUT_modId[AMCALLMAX_R_PMA_HW_DEP_SetCpgOff];
    static int volatile AM_count;
    AMOUT_modId[AM_count] = modId;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_SetCpgOn:AMSTB_R_PMA_HW_DEP_SetCpgOn:inout::array:counter<AM_count>] */
/*    R_PMA_HW_DEP_SetCpgOn => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_SetCpgOn(e_cpg_domain_id_t modId)
{
#define AMCALLMAX_R_PMA_HW_DEP_SetCpgOn 5
    static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_HW_DEP_SetCpgOn];
    static e_cpg_domain_id_t volatile AMOUT_modId[AMCALLMAX_R_PMA_HW_DEP_SetCpgOn];
    static int volatile AM_count;
    AMOUT_modId[AM_count] = modId;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_SetSyscOff:AMSTB_R_PMA_HW_DEP_SetSyscOff:inout::array:counter<AM_count>] */
/*    R_PMA_HW_DEP_SetSyscOff => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_SetSyscOff(e_sysc_domain_id_t modId)
{
#define AMCALLMAX_R_PMA_HW_DEP_SetSyscOff 5
    static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_HW_DEP_SetSyscOff];
    static e_sysc_domain_id_t volatile AMOUT_modId[AMCALLMAX_R_PMA_HW_DEP_SetSyscOff];
    static int volatile AM_count;
    AMOUT_modId[AM_count] = modId;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[AMSTB_SrcFile.c:dummy_pFunc:AMSTB_dummy_pFunc:inout:::] */
/*    dummy_pFunc => Stub */
int AMSTB_dummy_pFunc(int hwaId)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_hwaId;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_SetCpgON:AMSTB_R_PMA_BSP_WRAP_SetCpgON:inout:::] */
/*    R_PMA_BSP_WRAP_SetCpgON => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_SetCpgON(e_cpg_domain_id_t modId)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_cpg_id_tbl;
    static e_cpg_domain_id_t volatile AMOUT_modId;
    AMOUT_modId = modId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_SetCpgOFF:AMSTB_R_PMA_BSP_WRAP_SetCpgOFF:inout:::] */
/*    R_PMA_BSP_WRAP_SetCpgOFF => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_SetCpgOFF(e_cpg_domain_id_t modId)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_cpg_id_tbl;
    static e_cpg_domain_id_t volatile AMOUT_modId;
    AMOUT_modId = modId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_SetSyscON:AMSTB_R_PMA_BSP_WRAP_SetSyscON:inout:::] */
/*    R_PMA_BSP_WRAP_SetSyscON => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_SetSyscON(e_sysc_domain_id_t modId)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_sysc_id_tbl;
    static e_sysc_domain_id_t volatile AMOUT_modId;
    AMOUT_modId = modId;
    return AMIN_return;
}


/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_SetSyscOFF:AMSTB_R_PMA_BSP_WRAP_SetSyscOFF:inout:::] */
/*    R_PMA_BSP_WRAP_SetSyscOFF => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_SetSyscOFF(e_sysc_domain_id_t modId)
{
    static e_result_t volatile AMIN_return;
    static const int *volatile AMOUT_g_pma_bsp_wrap_sysc_id_tbl;
    static e_sysc_domain_id_t volatile AMOUT_modId;
    AMOUT_modId = modId;
    return AMIN_return;
}


/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeToCpgOn:AMSTB_R_PMA_HW_DEP_NoticeToCpgOn:inout:::] */
/*    R_PMA_HW_DEP_NoticeToCpgOn => Stub */
void AMSTB_R_PMA_HW_DEP_NoticeToCpgOn(e_pma_hwa_id_t hwaId,e_cpg_domain_id_t cpgId)
{
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_cpg_domain_id_t volatile AMOUT_cpgId;
    static int volatile AM_count;
    AMOUT_hwaId = hwaId;
    AMOUT_cpgId = cpgId;
    AM_count++;
}


/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeToSyscOn:AMSTB_R_PMA_HW_DEP_NoticeToSyscOn:inout::array:counter<AM_count>] */
/*    R_PMA_HW_DEP_NoticeToSyscOn => Stub */
void AMSTB_R_PMA_HW_DEP_NoticeToSyscOn(e_pma_hwa_id_t hwaId,e_sysc_domain_id_t syscId)
{
#define AMCALLMAX_R_PMA_HW_DEP_NoticeToSyscOn 5
    static e_pma_hwa_id_t volatile AMOUT_hwaId[AMCALLMAX_R_PMA_HW_DEP_NoticeToSyscOn];
    static e_sysc_domain_id_t volatile AMOUT_syscId[AMCALLMAX_R_PMA_HW_DEP_NoticeToSyscOn];
    static int volatile AM_count;
    AMOUT_hwaId[AM_count] = hwaId;
    AMOUT_syscId[AM_count] = syscId;
    AM_count++;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_SetSyscOn:AMSTB_R_PMA_HW_DEP_SetSyscOn:inout:::] */
/*    R_PMA_HW_DEP_SetSyscOn => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_SetSyscOn(e_sysc_domain_id_t modId)
{
#define AMCALLMAX_R_PMA_HW_DEP_SetSyscOn 5
    static e_result_t volatile AMIN_return[AMCALLMAX_R_PMA_HW_DEP_SetSyscOn];
    static e_sysc_domain_id_t volatile AMOUT_modId[AMCALLMAX_R_PMA_HW_DEP_SetSyscOn];
    static int volatile AM_count;
    AMOUT_modId[AM_count] = modId;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MutexInit:AMSTB_R_OSAL_Internal_MutexInit:inout:::] */
/*    R_OSAL_Internal_MutexInit => Stub */
int AMSTB_R_OSAL_Internal_MutexInit(struct st_osal_inner_mutexattr **p_mutex_attr,struct st_osal_inner_mutex_control **p_handle)
{
    static int volatile AMIN_return;
    static struct st_osal_inner_mutexattr **volatile AMOUT_p_mutex_attr;
    static struct st_osal_inner_mutex_control **volatile AMOUT_p_handle;
    AMOUT_p_mutex_attr = p_mutex_attr;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_Internal_MqCreate:AMSTB_R_OSAL_Internal_MqCreate:inout:::] */
/*    R_OSAL_Internal_MqCreate => Stub */
int AMSTB_R_OSAL_Internal_MqCreate(struct st_osal_inner_mq_config *p_config,struct st_osal_inner_mq_control **p_handle)
{
    static int volatile AMIN_return;
    static struct st_osal_inner_mq_config *volatile AMOUT_p_config;
    static struct st_osal_inner_mq_control **volatile AMOUT_p_handle;
    AMOUT_p_config = p_config;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeGetInitStatus:AMSTB_R_OSAL_RCFG_ClockTimeGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ClockTimeGetInitStatus => Stub */
int AMSTB_R_OSAL_RCFG_ClockTimeGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqGetInitStatus:AMSTB_R_OSAL_RCFG_MqGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MqGetInitStatus => Stub */
int AMSTB_R_OSAL_RCFG_MqGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqCheckBusy:AMSTB_R_OSAL_RCFG_MqCheckBusy:inout:::] */
/*    R_OSAL_RCFG_MqCheckBusy => Stub */
int AMSTB_R_OSAL_RCFG_MqCheckBusy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncGetInitStatus:AMSTB_R_OSAL_RCFG_ThsyncGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThsyncGetInitStatus => Stub */
int AMSTB_R_OSAL_RCFG_ThsyncGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncCheckBusy:AMSTB_R_OSAL_RCFG_ThsyncCheckBusy:inout:::] */
/*    R_OSAL_RCFG_ThsyncCheckBusy => Stub */
int AMSTB_R_OSAL_RCFG_ThsyncCheckBusy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptGetInitStatus:AMSTB_R_OSAL_DCFG_InterruptGetInitStatus:inout:::] */
/*    R_OSAL_DCFG_InterruptGetInitStatus => Stub */
int AMSTB_R_OSAL_DCFG_InterruptGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetInitStatus:AMSTB_R_OSAL_DCFG_PmGetInitStatus:inout:::] */
/*    R_OSAL_DCFG_PmGetInitStatus => Stub */
int AMSTB_R_OSAL_DCFG_PmGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmCheckBusy:AMSTB_R_OSAL_DCFG_PmCheckBusy:inout:::] */
/*    R_OSAL_DCFG_PmCheckBusy => Stub */
int AMSTB_R_OSAL_DCFG_PmCheckBusy()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetInitStatus:AMSTB_R_OSAL_DCFG_IoGetInitStatus:inout:::] */
/*    R_OSAL_DCFG_IoGetInitStatus => Stub */
int AMSTB_R_OSAL_DCFG_IoGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoCheckBusy:AMSTB_R_OSAL_DCFG_IoCheckBusy:inout:::] */
/*    R_OSAL_DCFG_IoCheckBusy => Stub */
int AMSTB_R_OSAL_DCFG_IoCheckBusy()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadGetInitStatus:AMSTB_R_OSAL_RCFG_ThreadGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThreadGetInitStatus => Stub */
int AMSTB_R_OSAL_RCFG_ThreadGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadCheckBusy:AMSTB_R_OSAL_RCFG_ThreadCheckBusy:inout:::] */
/*    R_OSAL_RCFG_ThreadCheckBusy => Stub */
int AMSTB_R_OSAL_RCFG_ThreadCheckBusy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqControlDeinit:AMSTB_R_OSAL_RCFG_MqControlDeinit:inout:::] */
/*    R_OSAL_RCFG_MqControlDeinit => Stub */
int AMSTB_R_OSAL_RCFG_MqControlDeinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncControlDeinit:AMSTB_R_OSAL_RCFG_ThsyncControlDeinit:inout:::] */
/*    R_OSAL_RCFG_ThsyncControlDeinit => Stub */
int AMSTB_R_OSAL_RCFG_ThsyncControlDeinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptControlDeinit:AMSTB_R_OSAL_DCFG_InterruptControlDeinit:inout:::] */
/*    R_OSAL_DCFG_InterruptControlDeinit => Stub */
int AMSTB_R_OSAL_DCFG_InterruptControlDeinit()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmControlDeinit:AMSTB_R_OSAL_DCFG_PmControlDeinit:inout:::] */
/*    R_OSAL_DCFG_PmControlDeinit => Stub */
int AMSTB_R_OSAL_DCFG_PmControlDeinit()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoControlDeinit:AMSTB_R_OSAL_DCFG_IoControlDeinit:inout:::] */
/*    R_OSAL_DCFG_IoControlDeinit => Stub */
int AMSTB_R_OSAL_DCFG_IoControlDeinit()
{
    static unsigned long long volatile AMIN_g_osal_num_of_device_table;
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    g_osal_num_of_device_table = AMIN_g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrControlDeinit:AMSTB_R_OSAL_RCFG_MmngrControlDeinit:inout:::] */
/*    R_OSAL_RCFG_MmngrControlDeinit => Stub */
int AMSTB_R_OSAL_RCFG_MmngrControlDeinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadControlDeinit:AMSTB_R_OSAL_RCFG_ThreadControlDeinit:inout:::] */
/*    R_OSAL_RCFG_ThreadControlDeinit => Stub */
int AMSTB_R_OSAL_RCFG_ThreadControlDeinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetInitStatus:AMSTB_R_OSAL_RCFG_MmngrGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MmngrGetInitStatus => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetInitStatus()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeControlInit:AMSTB_R_OSAL_RCFG_ClockTimeControlInit:inout:::] */
/*    R_OSAL_RCFG_ClockTimeControlInit => Stub */
int AMSTB_R_OSAL_RCFG_ClockTimeControlInit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadControlInit:AMSTB_R_OSAL_RCFG_ThreadControlInit:inout:::] */
/*    R_OSAL_RCFG_ThreadControlInit => Stub */
int AMSTB_R_OSAL_RCFG_ThreadControlInit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncControlInit:AMSTB_R_OSAL_RCFG_ThsyncControlInit:inout:::] */
/*    R_OSAL_RCFG_ThsyncControlInit => Stub */
int AMSTB_R_OSAL_RCFG_ThsyncControlInit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqControlInit:AMSTB_R_OSAL_RCFG_MqControlInit:inout:::] */
/*    R_OSAL_RCFG_MqControlInit => Stub */
int AMSTB_R_OSAL_RCFG_MqControlInit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoControlInit:AMSTB_R_OSAL_DCFG_IoControlInit:inout:::] */
/*    R_OSAL_DCFG_IoControlInit => Stub */
int AMSTB_R_OSAL_DCFG_IoControlInit()
{
    static struct st_osal_device_control *volatile AMIN_gp_osal_device_control;
    static unsigned long long volatile AMIN_g_osal_num_of_device_table;
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    gp_osal_device_control = AMIN_gp_osal_device_control;
    g_osal_num_of_device_table = AMIN_g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmControlInit:AMSTB_R_OSAL_DCFG_PmControlInit:inout:::] */
/*    R_OSAL_DCFG_PmControlInit => Stub */
int AMSTB_R_OSAL_DCFG_PmControlInit()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptControlInit:AMSTB_R_OSAL_DCFG_InterruptControlInit:inout:::] */
/*    R_OSAL_DCFG_InterruptControlInit => Stub */
int AMSTB_R_OSAL_DCFG_InterruptControlInit()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrControlInit:AMSTB_R_OSAL_RCFG_MmngrControlInit:inout:::] */
/*    R_OSAL_RCFG_MmngrControlInit => Stub */
int AMSTB_R_OSAL_RCFG_MmngrControlInit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeGetTimeStamp:AMSTB_R_OSAL_RCFG_ClockTimeGetTimeStamp:inout:::] */
/*    R_OSAL_RCFG_ClockTimeGetTimeStamp => Stub */
int AMSTB_R_OSAL_RCFG_ClockTimeGetTimeStamp(int clock_type,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_clock_type;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_clock_type = clock_type;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB:AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforTypeB(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC:AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforTypeAC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptSetInitStatus:AMSTB_R_OSAL_DCFG_InterruptSetInitStatus:inout:::] */
/*    R_OSAL_DCFG_InterruptSetInitStatus => Stub */
void AMSTB_R_OSAL_DCFG_InterruptSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoWrite:AMSTB_R_OSAL_DCFG_IoWrite:inout:::] */
/*    R_OSAL_DCFG_IoWrite => Stub */
int AMSTB_R_OSAL_DCFG_IoWrite(struct st_osal_device_control *handle,unsigned long long offset,unsigned long long size,unsigned long long unit_size,void *p_data)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static unsigned long long volatile AMOUT_unit_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_unit_size = unit_size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

int AMSTB_R_OSAL_IoBlockWrite8(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}
int AMSTB_R_OSAL_IoBlockWrite16(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}
int AMSTB_R_OSAL_IoBlockWrite32(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}
int AMSTB_R_OSAL_IoBlockWrite64(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

int AMSTB_R_OSAL_IoBlockRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}
int AMSTB_R_OSAL_IoBlockRead16(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}
int AMSTB_R_OSAL_IoBlockRead32(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}
int AMSTB_R_OSAL_IoBlockRead64(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetDeviceAxiBusId:AMSTB_R_OSAL_DCFG_IoGetDeviceAxiBusId:inout:::] */
/*    R_OSAL_DCFG_IoGetDeviceAxiBusId => Stub */
int AMSTB_R_OSAL_DCFG_IoGetDeviceAxiBusId(struct st_osal_device_control *device,unsigned long long *p_axi_id)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_device;
    static unsigned long long *volatile AMOUT_p_axi_id;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_device = device;
    AMOUT_p_axi_id = p_axi_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrClose:AMSTB_R_OSAL_RCFG_MmngrClose:inout:::] */
/*    R_OSAL_RCFG_MmngrClose => Stub */
int AMSTB_R_OSAL_RCFG_MmngrClose(struct st_osal_memory_manager_obj *hndl_mmngr)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    AMOUT_hndl_mmngr = hndl_mmngr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqSetInitStatus:AMSTB_R_OSAL_RCFG_MqSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MqSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_MqSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrFlush:AMSTB_R_OSAL_OS_MmngrFlush:inout:::] */
/*    R_OSAL_OS_MmngrFlush => Stub */
int AMSTB_R_OSAL_OS_MmngrFlush(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long addr,unsigned long long offset,unsigned long long size)
{
#define AMCALLMAX_R_OSAL_OS_MmngrFlush 30
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MmngrFlush];
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_addr;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static int volatile AM_count;

    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_addr = addr;
    AMOUT_offset = offset;
    AMOUT_size = size;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:strstr:AMSTB_strstr:inout:::] */
/*    strstr => Stub */
char *AMSTB_strstr(const char *s1, const char *s2)
{
#define AMCALLMAX_strstr 30
    static char *volatile AMIN_return[AMCALLMAX_strstr];
    static const char *volatile AMOUT_s1;
    static const char *volatile AMOUT_s2;
    AMOUT_s1 = s1;
    AMOUT_s2 = s2;
    static int volatile AM_count;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoWrite:AMSTB_R_OSAL_OS_IoWrite:inout:::] */
/*    R_OSAL_OS_IoWrite => Stub */
int AMSTB_R_OSAL_OS_IoWrite(struct st_osal_device_control *handle,unsigned long long offset,unsigned long long size,unsigned long long unit_size,void *p_data)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static unsigned long long volatile AMOUT_unit_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_unit_size = unit_size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoRead:AMSTB_R_OSAL_OS_IoRead:inout:::] */
/*    R_OSAL_OS_IoRead => Stub */
int AMSTB_R_OSAL_OS_IoRead(struct st_osal_device_control *handle,unsigned long long offset,unsigned long long size,unsigned long long unit_size,void *p_data)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static unsigned long long volatile AMOUT_unit_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_unit_size = unit_size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_search:AMSTB_r_osal_dcfg_io_search:inout:::] */
/*    r_osal_dcfg_io_search => Stub */
int AMSTB_r_osal_dcfg_io_search(void *p_key,void *p_search_info)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_key;
    static void *volatile AMOUT_p_search_info;
    AMOUT_p_key = p_key;
    AMOUT_p_search_info = p_search_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_register_axibus_info_last_data:AMSTB_r_osal_dcfg_io_register_axibus_info_last_data:inout:::] */
/*    r_osal_dcfg_io_register_axibus_info_last_data => Stub */
int AMSTB_r_osal_dcfg_io_register_axibus_info_last_data()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_check_axiidlist_for_domainname:AMSTB_r_osal_dcfg_io_check_axiidlist_for_domainname:inout:::] */
/*    r_osal_dcfg_io_check_axiidlist_for_domainname => Stub */
int AMSTB_r_osal_dcfg_io_check_axiidlist_for_domainname(unsigned char *p_axi_bus_name,unsigned char *p_domain_name,unsigned long long *p_list_buffer,unsigned long long list_size,unsigned long long *p_num_of_axi_ids)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_axi_bus_name;
    static unsigned char *volatile AMOUT_p_domain_name;
    static unsigned long long *volatile AMOUT_p_list_buffer;
    static unsigned long long volatile AMOUT_list_size;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    static unsigned long long volatile AMIN_p_num_of_axi_ids;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_p_domain_name = p_domain_name;
    AMOUT_p_list_buffer = p_list_buffer;
    AMOUT_list_size = list_size;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    *p_num_of_axi_ids = AMIN_p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_add_utlb_list_single:AMSTB_r_osal_dcfg_io_add_utlb_list_single:inout:::] */
/*    r_osal_dcfg_io_check_axiidlist_for_domainname => Stub */
int AMSTB_r_osal_dcfg_io_add_utlb_list_single(int32_t utlb, unsigned char* p_domain_name, unsigned long long list_size, unsigned long long *p_list_buffer, st_osal_utlb_list_control_t *p_utlb_control)
{
    static int volatile AMIN_return;
    static int32_t volatile AMOUT_utlb;
    static unsigned char *volatile AMOUT_p_domain_name;
    static unsigned long long volatile AMOUT_list_size;
    static unsigned long long *volatile AMOUT_p_list_buffer;
    static st_osal_utlb_list_control_t *volatile AMOUT_p_utlb_control;
    AMOUT_utlb = utlb;
    AMOUT_p_domain_name = p_domain_name;
    AMOUT_list_size = list_size;
    AMOUT_p_list_buffer = p_list_buffer;
    AMOUT_p_utlb_control = p_utlb_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_create_axiid_list:AMSTB_r_osal_dcfg_create_axiid_list:inout:::] */
/*    r_osal_dcfg_create_axiid_list => Stub */
void AMSTB_r_osal_dcfg_create_axiid_list(unsigned char *p_device_name,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_axi_ids,unsigned long long *p_num_of_invalid_ids)
{
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_size;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    static unsigned long long *volatile AMOUT_p_num_of_invalid_ids;
    AMOUT_p_device_name = p_device_name;
    AMOUT_p_array = p_array;
    AMOUT_array_size = array_size;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    AMOUT_p_num_of_invalid_ids = p_num_of_invalid_ids;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_search:AMSTB_r_osal_rcfg_cond_search:inout:::] */
/*    r_osal_rcfg_cond_search => Stub */
int AMSTB_r_osal_rcfg_cond_search(void *p_key,void *p_search_info)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_key;
    static void *volatile AMOUT_p_search_info;
    AMOUT_p_key = p_key;
    AMOUT_p_search_info = p_search_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_convert_unit:AMSTB_r_osal_rcfg_mmngr_convert_unit:inout:::] */
/*    r_osal_rcfg_mmngr_convert_unit => Stub */
int AMSTB_r_osal_rcfg_mmngr_convert_unit(unsigned long long size,unsigned char *p_output,unsigned long long array_size)
{
#define AMCALLMAX_r_osal_rcfg_mmngr_convert_unit 30
    static int volatile AMIN_return[AMCALLMAX_r_osal_rcfg_mmngr_convert_unit];
    static unsigned long long volatile AMOUT_size;
    static unsigned char *volatile AMOUT_p_output;
    static unsigned long long volatile AMOUT_array_size;
    static int volatile AM_count;

    AMOUT_size = size;
    AMOUT_p_output = p_output;
    AMOUT_array_size = array_size;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_cmn_get_handle_state:AMSTB_r_osal_rcfg_mmngr_cmn_get_handle_state:inout:::] */
/*    r_osal_rcfg_mmngr_cmn_get_handle_state => Stub */
int AMSTB_r_osal_rcfg_mmngr_cmn_get_handle_state(struct st_osal_memory_manager_obj *hndl_mmngr,int *p_state)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static int *volatile AMOUT_p_state;
    static int volatile AMIN_p_state;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_p_state = p_state;
    *p_state = AMIN_p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_proc:AMSTB_r_osal_rcfg_mmngr_alloc_proc:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_proc => Stub */
int AMSTB_r_osal_rcfg_mmngr_alloc_proc(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,unsigned long long bytes,unsigned long long alignment,struct st_osal_mem_buf_ptr_self *p_buffer_info)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static unsigned long long volatile AMOUT_bytes;
    static unsigned long long volatile AMOUT_alignment;
    static struct st_osal_mem_buf_ptr_self *volatile AMOUT_p_buffer_info;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_bytes = bytes;
    AMOUT_alignment = alignment;
    AMOUT_p_buffer_info = p_buffer_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_compare:AMSTB_r_osal_rcfg_mq_compare:inout:::] */
/*    r_osal_rcfg_mq_compare => Stub */
int AMSTB_r_osal_rcfg_mq_compare(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_wait_user_thread:AMSTB_r_osal_rcfg_thread_wait_user_thread:inout:::] */
/*    r_osal_rcfg_thread_wait_user_thread => Stub */
int AMSTB_r_osal_rcfg_thread_wait_user_thread(struct st_osal_thread_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptGetNumOfIrqChannels:AMSTB_R_OSAL_DCFG_InterruptGetNumOfIrqChannels:inout:::] */
/*    R_OSAL_DCFG_InterruptGetNumOfIrqChannels => Stub */
int AMSTB_R_OSAL_DCFG_InterruptGetNumOfIrqChannels(struct st_osal_device_control *handle,unsigned long long *p_numOfChannels)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long *volatile AMOUT_p_numOfChannels;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_p_numOfChannels = p_numOfChannels;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexLockUntilTimeStamp:AMSTB_R_OSAL_RCFG_MutexLockUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_MutexLockUntilTimeStamp => Stub */
int AMSTB_R_OSAL_RCFG_MutexLockUntilTimeStamp(struct st_osal_mutex_control *handle,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondSignal:AMSTB_R_OSAL_RCFG_CondSignal:inout:::] */
/*    R_OSAL_RCFG_CondSignal => Stub */
int AMSTB_R_OSAL_RCFG_CondSignal(struct st_osal_cond_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqReset:AMSTB_R_OSAL_RCFG_MqReset:inout:::] */
/*    R_OSAL_RCFG_MqReset => Stub */
int AMSTB_R_OSAL_RCFG_MqReset(struct st_osal_mq_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqDelete:AMSTB_R_OSAL_RCFG_MqDelete:inout:::] */
/*    R_OSAL_RCFG_MqDelete => Stub */
int AMSTB_R_OSAL_RCFG_MqDelete(struct st_osal_mq_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSetInitStatus:AMSTB_R_OSAL_RCFG_ThreadSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThreadSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_ThreadSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondStaticCreate:AMSTB_R_OSAL_OS_CondStaticCreate:inout:::] */
/*    R_OSAL_OS_CondStaticCreate => Stub */
int AMSTB_R_OSAL_OS_CondStaticCreate(struct st_osal_os_condattr *cond_attr,struct st_osal_os_cond **p_cond)
{
    #define AMCALLMAX_CondStaticCreate 30
    static int volatile AMIN_return[AMCALLMAX_CondStaticCreate];
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    static struct st_osal_os_cond **volatile AMOUT_p_cond;
    static int volatile AM_count;
    AMOUT_cond_attr = cond_attr;
    AMOUT_p_cond = p_cond;
    
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexUnlock:AMSTB_R_OSAL_OS_MutexUnlock:inout:::] */
/*    R_OSAL_OS_MutexUnlock => Stub */
int AMSTB_R_OSAL_OS_MutexUnlock(struct st_osal_os_mutex *mutex)
{
#define AMCALLMAX_R_OSAL_OS_MutexUnlock 30
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MutexUnlock];
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static int volatile AM_count;

    AMOUT_mutex = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Strnlen:AMSTB_R_OSAL_OS_Strnlen:inout:::] */
/*    R_OSAL_OS_Strnlen => Stub */
unsigned long long AMSTB_R_OSAL_OS_Strnlen(unsigned char *p_str,unsigned long long maxlen)
{
#define AMCALLMAX_R_OSAL_OS_Strnlen 30
    static unsigned long long volatile AMIN_return[AMCALLMAX_R_OSAL_OS_Strnlen];
    static unsigned char *volatile AMOUT_p_str;
    static unsigned long long volatile AMOUT_maxlen;
    static int volatile AM_count;

    AMOUT_p_str = p_str;
    AMOUT_maxlen = maxlen;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoStaticDeinit:AMSTB_R_OSAL_OS_IoStaticDeinit:inout:::] */
/*    R_OSAL_OS_IoStaticDeinit => Stub */
int AMSTB_R_OSAL_OS_IoStaticDeinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_timeclock.c:R_OSAL_OS_ClockTimeGetHighResoTimeStamp:AMSTB_R_OSAL_OS_ClockTimeGetHighResoTimeStamp:inout:::] */
/*    R_OSAL_OS_ClockTimeGetHighResoTimeStamp => Stub */
int AMSTB_R_OSAL_OS_ClockTimeGetHighResoTimeStamp(struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_time volatile AMOUT_p_time_stamp;
    p_time_stamp->tv_sec = AMOUT_p_time_stamp.tv_sec;
    p_time_stamp->tv_nsec = AMOUT_p_time_stamp.tv_nsec;
    *p_time_stamp = AMOUT_p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetThreadInfo:AMSTB_R_OSAL_RCFG_GetThreadInfo:inout:::] */
/*    R_OSAL_RCFG_GetThreadInfo => Stub */
int AMSTB_R_OSAL_RCFG_GetThreadInfo(unsigned int num,struct st_osal_thread_control **pp_info)
{
    static int volatile AMIN_return;
    static unsigned int volatile AMOUT_num;
    static struct st_osal_thread_control **volatile AMOUT_pp_info;
    AMOUT_num = num;
    AMOUT_pp_info = pp_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:r_osal_recover_init_state:AMSTB_r_osal_recover_init_state:inout:::] */
/*    r_osal_recover_init_state => Stub */
void AMSTB_r_osal_recover_init_state(unsigned short init_flg)
{
    static unsigned short volatile AMOUT_init_flg;
    AMOUT_init_flg = init_flg;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_device_name_connect:AMSTB_r_osal_dcfg_io_device_name_connect:inout:::] */
/*    r_osal_dcfg_io_device_name_connect => Stub */
int AMSTB_r_osal_dcfg_io_device_name_connect(unsigned char *p_device_name,unsigned long long num,unsigned long long add_dev_name_len)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long volatile AMOUT_num;
    static unsigned long long volatile AMOUT_add_dev_name_len;
    AMOUT_p_device_name = p_device_name;
    AMOUT_num = num;
    AMOUT_add_dev_name_len = add_dev_name_len;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_use_countup:AMSTB_r_osal_rcfg_cond_use_countup:inout:::] */
/*    r_osal_rcfg_cond_use_countup => Stub */
int AMSTB_r_osal_rcfg_cond_use_countup(struct st_osal_cond_control *cond_handle,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_cond_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_cond_handle = cond_handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_init_table:AMSTB_r_osal_rcfg_cond_init_table:inout:::] */
/*    r_osal_rcfg_cond_init_table => Stub */
int AMSTB_r_osal_rcfg_cond_init_table(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_condattr *cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    AMOUT_mutex_attr = mutex_attr;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_control_deinit:AMSTB_r_osal_rcfg_cond_control_deinit:inout:::] */
/*    r_osal_rcfg_cond_control_deinit => Stub */
int AMSTB_r_osal_rcfg_cond_control_deinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_registermonitor:AMSTB_r_osal_rcfg_mmngr_registermonitor:inout:::] */
/*    r_osal_rcfg_mmngr_registermonitor => Stub */
int AMSTB_r_osal_rcfg_mmngr_registermonitor(void *p_ptr_self,int monitored_event,void *monitor_handler,void *p_monitor_handler_user_arg)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static int volatile AMOUT_monitored_event;
    static void *volatile AMOUT_monitor_handler;
    static void *volatile AMOUT_p_monitor_handler_user_arg;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_monitored_event = monitored_event;
    AMOUT_monitor_handler = monitor_handler;
    AMOUT_p_monitor_handler_user_arg = p_monitor_handler_user_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_invalidate:AMSTB_r_osal_rcfg_mmngr_invalidate:inout:::] */
/*    r_osal_rcfg_mmngr_invalidate => Stub */
int AMSTB_r_osal_rcfg_mmngr_invalidate(void *p_ptr_self,unsigned long long offset,unsigned long long size)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_offset = offset;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_checkbuffer:AMSTB_r_osal_rcfg_mmngr_checkbuffer:inout:::] */
/*    r_osal_rcfg_mmngr_checkbuffer => Stub */
int AMSTB_r_osal_rcfg_mmngr_checkbuffer(void *p_ptr_self)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    AMOUT_p_ptr_self = p_ptr_self;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_timedlock_mutex_cmn:AMSTB_r_osal_os_interrupt_timedlock_mutex_cmn:inout:::] */
/*    r_osal_os_interrupt_timedlock_mutex_cmn => Stub */
int AMSTB_r_osal_os_interrupt_timedlock_mutex_cmn(pthread_mutex_t *p_mutex,osal_milli_sec_t timeout)
{
    static int volatile AMIN_return;
    static pthread_mutex_t *volatile AMOUT_p_mutex;
    static osal_milli_sec_t volatile AMOUT_timeout;
    AMOUT_p_mutex = p_mutex;
    AMOUT_timeout = timeout;
    return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_Alloc:AMSTB_R_MEML_Alloc:inout:::] */
/*    R_MEML_Alloc => Stub */
unsigned long long AMSTB_R_MEML_Alloc(struct st_meml_manage *p_manage,unsigned long long size,unsigned long long alignment)
{
    static unsigned long long volatile AMIN_return;
    static struct st_meml_manage *volatile AMOUT_p_manage;
    static unsigned long long volatile AMOUT_size;
    static unsigned long long volatile AMOUT_alignment;
    AMOUT_p_manage = p_manage;
    AMOUT_size = size;
    AMOUT_alignment = alignment;
    return AMIN_return;
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_GetVersion:AMSTB_R_MEML_GetVersion:inout:::] */
/*    R_MEML_GetVersion => Stub */
st_meml_version_t *AMSTB_R_MEML_GetVersion(void)
{
    static st_meml_version_t *volatile AMIN_return;
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

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC:AMSTB_R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeHPtoCGforTypeABC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmWaitForState:AMSTB_R_OSAL_DCFG_PmWaitForState:inout:::] */
/*    R_OSAL_DCFG_PmWaitForState => Stub */
int AMSTB_R_OSAL_DCFG_PmWaitForState(struct st_osal_device_control *handle,int state,long long time_period)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_state;
    static long long volatile AMOUT_time_period;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_state = state;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_cond.c:R_OSAL_HIF_CondWaitUntilTimeStamp:AMSTB_R_OSAL_HIF_CondWaitUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_CondWaitUntilTimeStamp => Stub */
int AMSTB_R_OSAL_HIF_CondWaitUntilTimeStamp(struct st_osal_os_cond *cond,struct st_osal_os_mutex *mutex,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_os_cond *volatile AMOUT_cond;
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

int AMSTB_R_OSAL_HIF_CondWaitUntilTimeStamp_multiple(struct st_osal_os_cond *cond,struct st_osal_os_mutex *mutex,struct st_osal_time *p_time_stamp)
{
#define AMCALLMAX_CondWait_multiple 30
    static int volatile AMIN_return[AMCALLMAX_CondWait_multiple];
    static struct st_osal_os_cond *volatile AMOUT_cond;
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;

    static int volatile AM_val;
    static int volatile AM_count;

    if (AM_count != 0)
    {
        g_st_osal_os_mq[0].numofmsg = AM_val;
    }

    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetConfig:AMSTB_R_OSAL_RCFG_MmngrGetConfig:inout:::] */
/*    R_OSAL_RCFG_MmngrGetConfig => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetConfig(struct st_osal_memory_manager_obj *hndl_mmngr,struct st_osal_mmngr_config *p_config)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrAlloc:AMSTB_R_OSAL_RCFG_MmngrAlloc:inout:::] */
/*    R_OSAL_RCFG_MmngrAlloc => Stub */
int AMSTB_R_OSAL_RCFG_MmngrAlloc(struct st_osal_memory_manager_obj *hndl_mmngr,unsigned long long size,unsigned long long alignment,struct st_osal_memory_buffer_obj **p_buffer_obj)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static unsigned long long volatile AMOUT_size;
    static unsigned long long volatile AMOUT_alignment;
    static struct st_osal_memory_buffer_obj **volatile AMOUT_p_buffer_obj;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_size = size;
    AMOUT_alignment = alignment;
    AMOUT_p_buffer_obj = p_buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexDestroy:AMSTB_R_OSAL_OS_MutexDestroy:inout:::] */
/*    R_OSAL_OS_MutexDestroy => Stub */
int AMSTB_R_OSAL_OS_MutexDestroy(struct st_osal_os_mutex *mutex)
{
#define AMCALLMAX_R_OSAL_OS_MutexDestroy 600
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MutexDestroy];
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static int volatile AM_count;

    AMOUT_mutex = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSleep:AMSTB_R_OSAL_OS_ThreadSleep:inout:::] */
/*    R_OSAL_OS_ThreadSleep => Stub */
int AMSTB_R_OSAL_OS_ThreadSleep(struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_compare:AMSTB_r_osal_rcfg_cond_compare:inout:::] */
/*    r_osal_rcfg_cond_compare => Stub */
int AMSTB_r_osal_rcfg_cond_compare(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_init_mmngr_self:AMSTB_r_osal_rcfg_mmngr_init_mmngr_self:inout:::] */
/*    r_osal_rcfg_mmngr_init_mmngr_self => Stub */
int AMSTB_r_osal_rcfg_mmngr_init_mmngr_self(struct st_osal_mem_mmngr_ptr_self *p_mmngr_self,unsigned long long num_of_regions)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mmngr_self;
    static unsigned long long volatile AMOUT_num_of_regions;
    AMOUT_p_mmngr_self = p_mmngr_self;
    AMOUT_num_of_regions = num_of_regions;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_debugbufferprint:AMSTB_r_osal_rcfg_mmngr_debugbufferprint:inout:::] */
/*    r_osal_rcfg_mmngr_debugbufferprint => Stub */
int AMSTB_r_osal_rcfg_mmngr_debugbufferprint(void *p_ptr_self,struct _Filet *p_output)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static struct _Filet *volatile AMOUT_p_output;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_p_output = p_output;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_notify_join:AMSTB_r_osal_rcfg_thread_notify_join:inout:::] */
/*    r_osal_rcfg_thread_notify_join => Stub */
int AMSTB_r_osal_rcfg_thread_notify_join(struct st_osal_thread_control *p_control)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_p_control;
    AMOUT_p_control = p_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_internal_thread:AMSTB_r_osal_rcfg_internal_thread:inout:::] */
/*    r_osal_rcfg_internal_thread => Stub */
void *AMSTB_r_osal_rcfg_internal_thread(void *p_arg)
{
    static void *volatile AMIN_return;
    static void *volatile AMOUT_p_arg;
    AMOUT_p_arg = p_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_wait_for_thread_active:AMSTB_r_osal_os_interrupt_wait_for_thread_active:inout:::] */
/*    r_osal_os_interrupt_wait_for_thread_active => Stub */
int AMSTB_r_osal_os_interrupt_wait_for_thread_active(struct st_osal_interrupt_thread_control *p_interrupt_control)
{
    static int volatile AMIN_return;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_interrupt_control;
    AMOUT_p_interrupt_control = p_interrupt_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptSetIsrPriority:AMSTB_R_OSAL_DCFG_InterruptSetIsrPriority:inout:::] */
/*    R_OSAL_DCFG_InterruptSetIsrPriority => Stub */
int AMSTB_R_OSAL_DCFG_InterruptSetIsrPriority(struct st_osal_device_control *handle,unsigned long long irq_channel,int priority_value)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static int volatile AMOUT_priority_value;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName:AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName => Stub */
int AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName(unsigned char *p_axi_bus_name,unsigned long long *p_num_of_axi_ids)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_axi_bus_name;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetDeviceList:AMSTB_R_OSAL_DCFG_IoGetDeviceList:inout:::] */
/*    R_OSAL_DCFG_IoGetDeviceList => Stub */
int AMSTB_R_OSAL_DCFG_IoGetDeviceList(unsigned char *p_device_type_name,unsigned char *p_buffer,unsigned long long buffer_size,unsigned long long *p_num_of_byte)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static unsigned char *volatile AMOUT_p_device_type_name;
    static unsigned char *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    static unsigned long long *volatile AMOUT_p_num_of_byte;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_p_device_type_name = p_device_type_name;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_mutex.c:R_OSAL_HIF_MutexLockForTimePeriod:AMSTB_R_OSAL_HIF_MutexLockForTimePeriod:inout:::] */
/*    R_OSAL_HIF_MutexLockForTimePeriod => Stub */
int AMSTB_R_OSAL_HIF_MutexLockForTimePeriod(struct st_osal_os_mutex *mutex,long long time_period)
{
#define AMCALLMAX_MutexLockForTimePeriod 30
    static int volatile AMIN_return[AMCALLMAX_MutexLockForTimePeriod];
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static long long volatile AMOUT_time_period;
    static int volatile AM_control;
    static int volatile AM_count;
    
    AMOUT_mutex = mutex;
    AMOUT_time_period = time_period;

    int i;

    if (AM_control == 1) /* Special case. Buffer fill for check buffer */
    {
        for (i = 0; i < OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE; i++)
        {
            mmngr_buffer[i] = OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN;
            mmngr_buffer[OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE + 64 + i] = OSAL_RES_CFG_MMNGR_GUARD_PAGE_PATTERN;
        }
    }
    else if (AM_control == 2) /* Special case. Buffer clear for check buffer */
    {
        for (i = 0; i < OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE; i++)
        {
            mmngr_buffer[i] = 0;
            mmngr_buffer[OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE + 64 + i] = 0;
        }
    }
    AM_control = 0;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_HIF_MutexLockForTimePeriod:AMSTB_3R_PMA_MNG_Task_R_OSAL_HIF_MutexLockForTimePeriod:inout:csv_local:array:counter<AM_count>] */
/*    R_PMA_MNG_Task -- R_OSAL_Internal_MutexTimedLock => Stub */
int AMSTB_3R_PMA_MNG_Task_R_OSAL_HIF_MutexLockForTimePeriod(struct st_osal_inner_mutex_control *mutex,long long time_period)
{
#define AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MutexLockForTimePeriod 2
    static int volatile AMIN_return[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MutexLockForTimePeriod];
    static struct st_osal_inner_mutex_control *volatile AMOUT_mutex[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MutexLockForTimePeriod];
    static long long volatile AMOUT_time_period[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MutexLockForTimePeriod];
    static int volatile AM_count;
    AMOUT_mutex[AM_count] = mutex;
    AMOUT_time_period[AM_count] = time_period;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetConstCpuPtr:AMSTB_R_OSAL_RCFG_MmngrGetConstCpuPtr:inout:::] */
/*    R_OSAL_RCFG_MmngrGetConstCpuPtr => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetConstCpuPtr(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,void **pp_const_cpu_ptr)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static void **volatile AMOUT_pp_const_cpu_ptr;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_pp_const_cpu_ptr = pp_const_cpu_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSelf:AMSTB_R_OSAL_RCFG_ThreadSelf:inout:::] */
/*    R_OSAL_RCFG_ThreadSelf => Stub */
int AMSTB_R_OSAL_RCFG_ThreadSelf(struct st_osal_thread_control **p_handle)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control **volatile AMOUT_p_handle;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondTimedWait:AMSTB_R_OSAL_OS_CondTimedWait:inout:::] */
/*    R_OSAL_OS_CondTimedWait => Stub */
int AMSTB_R_OSAL_OS_CondTimedWait(struct st_osal_os_cond *cond,struct st_osal_os_mutex *mutex,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_os_cond *volatile AMOUT_cond;
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondBroadcast:AMSTB_R_OSAL_OS_CondBroadcast:inout:::] */
/*    R_OSAL_OS_CondBroadcast => Stub */
int AMSTB_R_OSAL_OS_CondBroadcast(struct st_osal_os_cond *cond)
{
    static int volatile AMIN_return;
    static struct st_osal_os_cond *volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexInitAttr:AMSTB_R_OSAL_OS_MutexInitAttr:inout:::] */
/*    R_OSAL_OS_MutexInitAttr => Stub */
int AMSTB_R_OSAL_OS_MutexInitAttr(struct st_osal_os_mutexattr **p_mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMIN_p_mutex_attr;
    static struct st_osal_os_mutexattr **volatile AMOUT_p_mutex_attr;

    *p_mutex_attr      = AMIN_p_mutex_attr;
    AMOUT_p_mutex_attr = p_mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadStaticInit:AMSTB_R_OSAL_OS_ThreadStaticInit:inout:::] */
/*    R_OSAL_OS_ThreadStaticInit => Stub */
int AMSTB_R_OSAL_OS_ThreadStaticInit(unsigned long long thread_cfg_num)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_thread_cfg_num;
    AMOUT_thread_cfg_num = thread_cfg_num;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_deinit_device_information:AMSTB_r_osal_dcfg_io_deinit_device_information:inout:::] */
/*    r_osal_dcfg_io_deinit_device_information => Stub */
int AMSTB_r_osal_dcfg_io_deinit_device_information()
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_create_axibus_namelist:AMSTB_r_osal_dcfg_io_create_axibus_namelist:inout:::] */
/*    r_osal_dcfg_io_create_axibus_namelist => Stub */
void AMSTB_r_osal_dcfg_io_create_axibus_namelist(unsigned long long num,unsigned long long *p_axi_bus_name_length,unsigned char *p_axi_bus_name)
{
    static unsigned long long volatile AMOUT_num;
    static unsigned long long *volatile AMOUT_p_axi_bus_name_length;
    static unsigned char *volatile AMOUT_p_axi_bus_name;
    AMOUT_num = num;
    AMOUT_p_axi_bus_name_length = p_axi_bus_name_length;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_getsize:AMSTB_r_osal_rcfg_mmngr_getsize:inout:::] */
/*    r_osal_rcfg_mmngr_getsize => Stub */
int AMSTB_r_osal_rcfg_mmngr_getsize(void *p_ptr_self,unsigned long long *p_size)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static unsigned long long *volatile AMOUT_p_size;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_p_size = p_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_flush:AMSTB_r_osal_rcfg_mmngr_flush:inout:::] */
/*    r_osal_rcfg_mmngr_flush => Stub */
int AMSTB_r_osal_rcfg_mmngr_flush(void *p_ptr_self,unsigned long long offset,unsigned long long size)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_offset = offset;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_deinit_mmngr_self:AMSTB_r_osal_rcfg_mmngr_deinit_mmngr_self:inout:::] */
/*    r_osal_rcfg_mmngr_deinit_mmngr_self => Stub */
int AMSTB_r_osal_rcfg_mmngr_deinit_mmngr_self()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_close_system_resources:AMSTB_r_osal_rcfg_mmngr_close_system_resources:inout:::] */
/*    r_osal_rcfg_mmngr_close_system_resources => Stub */
int AMSTB_r_osal_rcfg_mmngr_close_system_resources(struct st_osal_mem_mmngr_ptr_self *p_mngr_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    AMOUT_p_mngr_control = p_mngr_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_close:AMSTB_r_osal_rcfg_mmngr_close:inout:::] */
/*    r_osal_rcfg_mmngr_close => Stub */
int AMSTB_r_osal_rcfg_mmngr_close(struct st_osal_memory_manager_obj *hndl_mmngr)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    AMOUT_hndl_mmngr = hndl_mmngr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_pre_config_check:AMSTB_r_osal_rcfg_mmngr_alloc_pre_config_check:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_pre_config_check => Stub */
int AMSTB_r_osal_rcfg_mmngr_alloc_pre_config_check(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,unsigned long long bytes,unsigned long long alignment)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static unsigned long long volatile AMOUT_bytes;
    static unsigned long long volatile AMOUT_alignment;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_bytes = bytes;
    AMOUT_alignment = alignment;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetHwaState:AMSTB_R_PMA_GetHwaState:inout:::] */
/*    R_PMA_GetHwaState => Stub */
e_pma_return_t AMSTB_R_PMA_GetHwaState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,st_pma_hwa_state_t *pHwaState)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t volatile AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static st_pma_hwa_state_t *volatile AMOUT_pHwaState;
    static e_pma_hwa_req_t volatile input_enReqState;
    static e_pma_onoff_t   volatile input_enActualPwrStat;
    static e_pma_onoff_t   volatile input_enActualClkStat;

    pHwaState->enReqState      = input_enReqState;
    pHwaState->enActualPwrStat = input_enActualPwrStat;
    pHwaState->enActualClkStat = input_enActualClkStat;

    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pHwaState = pHwaState;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGwithoutCPG(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG:AMSTB_R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToHPwithoutCPG(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_UpdateLowestPowerState:AMSTB_R_PMA_SM_UpdateLowestPowerState:inout:::] */
/*    R_PMA_SM_UpdateLowestPowerState => Stub */
e_result_t AMSTB_R_PMA_SM_UpdateLowestPowerState(e_pma_hwa_id_t hwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiIdListForAxiBusName:AMSTB_R_OSAL_DCFG_IoGetAxiIdListForAxiBusName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiIdListForAxiBusName => Stub */
int AMSTB_R_OSAL_DCFG_IoGetAxiIdListForAxiBusName(unsigned char *p_axi_bus_name,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_written_axi_ids)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_axi_bus_name;
    static unsigned long long *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_size;
    static unsigned long long *volatile AMOUT_p_num_of_written_axi_ids;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_p_array = p_array;
    AMOUT_array_size = array_size;
    AMOUT_p_num_of_written_axi_ids = p_num_of_written_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetInitStatus:AMSTB_R_OSAL_DCFG_PmSetInitStatus:inout:::] */
/*    R_OSAL_DCFG_PmSetInitStatus => Stub */
void AMSTB_R_OSAL_DCFG_PmSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexUnlock:AMSTB_R_OSAL_RCFG_MutexUnlock:inout:::] */
/*    R_OSAL_RCFG_MutexUnlock => Stub */
int AMSTB_R_OSAL_RCFG_MutexUnlock(struct st_osal_mutex_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrOpen:AMSTB_R_OSAL_RCFG_MmngrOpen:inout:::] */
/*    R_OSAL_RCFG_MmngrOpen => Stub */
int AMSTB_R_OSAL_RCFG_MmngrOpen(struct st_osal_mmngr_config *p_config,unsigned long long region_idx,struct st_osal_memory_manager_obj **p_hndl_mmngr)
{
    static int volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_region_idx;
    static struct st_osal_memory_manager_obj **volatile AMOUT_p_hndl_mmngr;
    AMOUT_p_config = p_config;
    AMOUT_region_idx = region_idx;
    AMOUT_p_hndl_mmngr = p_hndl_mmngr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetCpuPtr:AMSTB_R_OSAL_RCFG_MmngrGetCpuPtr:inout:::] */
/*    R_OSAL_RCFG_MmngrGetCpuPtr => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetCpuPtr(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,void **pp_cpu_ptr)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static void **volatile AMOUT_pp_cpu_ptr;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_pp_cpu_ptr = pp_cpu_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqGetConfig:AMSTB_R_OSAL_RCFG_MqGetConfig:inout:::] */
/*    R_OSAL_RCFG_MqGetConfig => Stub */
int AMSTB_R_OSAL_RCFG_MqGetConfig(struct st_osal_mq_control *handle,struct st_osal_mq_config *p_config)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static struct st_osal_mq_config *volatile AMOUT_p_config;
    AMOUT_handle = handle;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSleepUntilTimeStamp:AMSTB_R_OSAL_RCFG_ThreadSleepUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_ThreadSleepUntilTimeStamp => Stub */
int AMSTB_R_OSAL_RCFG_ThreadSleepUntilTimeStamp(struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadDeinitAttr:AMSTB_R_OSAL_OS_ThreadDeinitAttr:inout:::] */
/*    R_OSAL_OS_ThreadDeinitAttr => Stub */
int AMSTB_R_OSAL_OS_ThreadDeinitAttr(struct st_osal_os_threadattr *thread_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_threadattr *volatile AMOUT_thread_attr;
    AMOUT_thread_attr = thread_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_calculate_device_list_length:AMSTB_r_osal_dcfg_io_calculate_device_list_length:inout:::] */
/*    r_osal_dcfg_io_calculate_device_list_length => Stub */
int AMSTB_r_osal_dcfg_io_calculate_device_list_length(struct st_osal_device_control *p_device_control,unsigned char *p_device_type_name,unsigned long long *p_total_length)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_p_device_control;
    static unsigned char *volatile AMOUT_p_device_type_name;
    static unsigned long long *volatile AMOUT_p_total_length;
    AMOUT_p_device_control = p_device_control;
    AMOUT_p_device_type_name = p_device_type_name;
    AMOUT_p_total_length = p_total_length;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_invalidate_proc:AMSTB_r_osal_rcfg_mmngr_invalidate_proc:inout:::] */
/*    r_osal_rcfg_mmngr_invalidate_proc => Stub */
int AMSTB_r_osal_rcfg_mmngr_invalidate_proc(unsigned long long offset,unsigned long long size,struct st_osal_mem_buf_ptr_self *p_buffer)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static struct st_osal_mem_buf_ptr_self *volatile AMOUT_p_buffer;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_buffer = p_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_search:AMSTB_r_osal_rcfg_mq_search:inout:::] */
/*    r_osal_rcfg_mq_search => Stub */
int AMSTB_r_osal_rcfg_mq_search(void *p_key,void *p_search_info)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_key;
    static void *volatile AMOUT_p_search_info;
    AMOUT_p_key = p_key;
    AMOUT_p_search_info = p_search_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_active_check_cmn:AMSTB_r_osal_rcfg_mq_active_check_cmn:inout:::] */
/*    r_osal_rcfg_mq_active_check_cmn => Stub */
int AMSTB_r_osal_rcfg_mq_active_check_cmn(struct st_osal_mq_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetHwAddr:AMSTB_R_OSAL_RCFG_MmngrGetHwAddr:inout:::] */
/*    R_OSAL_RCFG_MmngrGetHwAddr => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetHwAddr(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long axi_id,unsigned long long *p_hw_addr)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_axi_id;
    static unsigned long long *volatile AMOUT_p_hw_addr;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_axi_id = axi_id;
    AMOUT_p_hw_addr = p_hw_addr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetDefaultIdx:AMSTB_R_OSAL_RCFG_MmngrGetDefaultIdx:inout:::] */
/*    R_OSAL_RCFG_MmngrGetDefaultIdx => Stub */
unsigned long long AMSTB_R_OSAL_RCFG_MmngrGetDefaultIdx()
{
    static unsigned long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqSendUntilTimeStamp:AMSTB_R_OSAL_RCFG_MqSendUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_MqSendUntilTimeStamp => Stub */
int AMSTB_R_OSAL_RCFG_MqSendUntilTimeStamp(struct st_osal_mq_control *handle,struct st_osal_time *p_time_stamp,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqReceiveUntilTimeStamp:AMSTB_R_OSAL_RCFG_MqReceiveUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_MqReceiveUntilTimeStamp => Stub */
int AMSTB_R_OSAL_RCFG_MqReceiveUntilTimeStamp(struct st_osal_mq_control *handle,struct st_osal_time *p_time_stamp,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqReceiveForTimePeriod:AMSTB_R_OSAL_RCFG_MqReceiveForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_MqReceiveForTimePeriod => Stub */
int AMSTB_R_OSAL_RCFG_MqReceiveForTimePeriod(struct st_osal_mq_control *handle,long long time_period,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static long long volatile AMOUT_time_period;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_time_period = time_period;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_RCFG_MqReceiveForTimePeriod:AMSTB_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod:inout:csv_local:array:counter<AM_count>] */
/*    R_PMA_MNG_Task -- R_OSAL_RCFG_MqReceiveForTimePeriod => Stub */
int AMSTB_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod(struct st_osal_mq_control *handle,long long time_period,void *p_buffer,unsigned long long buffer_size)
{
#define AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod 2
    static int volatile AMIN_return[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod];
    static struct st_osal_mq_control *volatile AMOUT_handle[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod];
    static long long volatile AMOUT_time_period[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod];
    static void *volatile AMOUT_p_buffer[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod];
    static unsigned long long volatile AMOUT_buffer_size[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_RCFG_MqReceiveForTimePeriod];
    static int volatile AM_count;
    AMOUT_handle[AM_count] = handle;
    AMOUT_time_period[AM_count] = time_period;
    AMOUT_p_buffer[AM_count] = p_buffer;
    AMOUT_buffer_size[AM_count] = buffer_size;

    st_msg_to_task_t * pMsgTotask = p_buffer;
    pMsgTotask->enProcId = dummy_MsgToTask[AM_count].enProcId;
    pMsgTotask->enHwaId = dummy_MsgToTask[AM_count].enHwaId;
    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptIsIsrHandler:AMSTB_R_OSAL_OS_InterruptIsIsrHandler:inout:::] */
/*    R_OSAL_OS_InterruptIsIsrHandler => Stub */
int AMSTB_R_OSAL_OS_InterruptIsIsrHandler(unsigned int num,unsigned char *p_result)
{
    static int volatile AMIN_return;
    static unsigned int volatile AMOUT_num;
    static unsigned char *volatile AMOUT_p_result;
    static unsigned char volatile  AM_result;

    AMOUT_num = num;
    AMOUT_p_result = p_result;

    *p_result = AM_result;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptCheckBusy:AMSTB_R_OSAL_OS_InterruptCheckBusy:inout:::] */
/*    R_OSAL_OS_InterruptCheckBusy => Stub */
int AMSTB_R_OSAL_OS_InterruptCheckBusy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_timeclock.c:R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp:AMSTB_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp:inout:::] */
/*    R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp => Stub */
int AMSTB_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_wait_for_state:AMSTB_r_osal_dcfg_pm_wait_for_state:inout:::] */
/*    r_osal_dcfg_pm_wait_for_state => Stub */
int AMSTB_r_osal_dcfg_pm_wait_for_state(struct st_osal_device_control *handle,const st_pma_hwa_state_t *p_wait_hwa_state,e_pma_reset_req_t wait_reset_req,long long time_period)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static const st_pma_hwa_state_t *volatile AMOUT_p_wait_hwa_state;
    static e_pma_reset_req_t volatile AMOUT_wait_reset_req;
    static long long volatile AMOUT_time_period;
    AMOUT_handle = handle;
    AMOUT_p_wait_hwa_state = p_wait_hwa_state;
    AMOUT_wait_reset_req = wait_reset_req;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_compare:AMSTB_r_osal_rcfg_mutex_compare:inout:::] */
/*    r_osal_rcfg_mutex_compare => Stub */
int AMSTB_r_osal_rcfg_mutex_compare(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open_map:AMSTB_r_osal_rcfg_mmngr_open_map:inout:::] */
/*    r_osal_rcfg_mmngr_open_map => Stub */
int AMSTB_r_osal_rcfg_mmngr_open_map(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mmngr_config *p_usr_config,unsigned long long region_idx)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_mmngr_config *volatile AMOUT_p_usr_config;
    static unsigned long long volatile AMOUT_region_idx;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_p_usr_config = p_usr_config;
    AMOUT_region_idx = region_idx;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open_malloc:AMSTB_r_osal_rcfg_mmngr_open_malloc:inout:::] */
/*    r_osal_rcfg_mmngr_open_malloc => Stub */
int AMSTB_r_osal_rcfg_mmngr_open_malloc(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mmngr_config *p_usr_config)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_mmngr_config *volatile AMOUT_p_usr_config;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_p_usr_config = p_usr_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_deinit_attr:AMSTB_r_osal_rcfg_mq_deinit_attr:inout:::] */
/*    r_osal_rcfg_mq_deinit_attr => Stub */
int AMSTB_r_osal_rcfg_mq_deinit_attr(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_condattr *cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    AMOUT_mutex_attr = mutex_attr;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_unlock_mutex:AMSTB_r_osal_os_interrupt_unlock_mutex:inout:::] */
/*    r_osal_os_interrupt_unlock_mutex => Stub */
int AMSTB_r_osal_os_interrupt_unlock_mutex(struct st_osal_interrupt_thread_control *p_info)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_info;
    AMOUT_g_context_key = g_context_key;
    AMOUT_p_info = p_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_unlock_mutex:AMSTB_r_osal_os_interrupt_unlock_mutex_multiple:inout::array:counter<AM_count>] */
/*    r_osal_os_interrupt_unlock_mutex => Stub */
int AMSTB_r_osal_os_interrupt_unlock_mutex_multiple(struct st_osal_interrupt_thread_control *p_info)
{
#define AMCALLMAX_r_osal_os_interrupt_unlock_mutex 600
    static int volatile AMIN_return[AMCALLMAX_r_osal_os_interrupt_unlock_mutex];
    static int volatile AMOUT_g_context_key;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_info;
    static int volatile AM_count;
    AMOUT_g_context_key = g_context_key;
    AMOUT_p_info = p_info;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_ApplyResetHwa:AMSTB_R_PMA_ApplyResetHwa:inout:::] */
/*    R_PMA_ApplyResetHwa => Stub */
e_pma_return_t AMSTB_R_PMA_ApplyResetHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3:AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3 => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforTypeACofGen3(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H:AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangePGtoCGforRGXofV4H(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName:AMSTB_R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName => Stub */
int AMSTB_R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName(unsigned char *p_device_name,unsigned long long *p_axi_id)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long *volatile AMOUT_p_axi_id;
    AMOUT_p_device_name = p_device_name;
    AMOUT_p_axi_id = p_axi_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrInvalidate:AMSTB_R_OSAL_RCFG_MmngrInvalidate:inout:::] */
/*    R_OSAL_RCFG_MmngrInvalidate => Stub */
int AMSTB_R_OSAL_RCFG_MmngrInvalidate(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long offset,unsigned long long size)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_offset = offset;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqIsFull:AMSTB_R_OSAL_RCFG_MqIsFull:inout:::] */
/*    R_OSAL_RCFG_MqIsFull => Stub */
int AMSTB_R_OSAL_RCFG_MqIsFull(struct st_osal_mq_control *handle,unsigned char *p_result)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_handle = handle;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadEqual:AMSTB_R_OSAL_RCFG_ThreadEqual:inout:::] */
/*    R_OSAL_RCFG_ThreadEqual => Stub */
int AMSTB_R_OSAL_RCFG_ThreadEqual(struct st_osal_thread_control *handle1,struct st_osal_thread_control *handle2,unsigned char *p_result)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_handle1;
    static struct st_osal_thread_control *volatile AMOUT_handle2;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_handle1 = handle1;
    AMOUT_handle2 = handle2;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadCreate:AMSTB_R_OSAL_RCFG_ThreadCreate:inout:::] */
/*    R_OSAL_RCFG_ThreadCreate => Stub */
int AMSTB_R_OSAL_RCFG_ThreadCreate(struct st_osal_thread_config *p_config,unsigned long long thread_id,struct st_osal_thread_control **p_handle)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_thread_id;
    static struct st_osal_thread_control **volatile AMOUT_p_handle;
    AMOUT_p_config = p_config;
    AMOUT_thread_id = thread_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptStaticDeinit:AMSTB_R_OSAL_OS_InterruptStaticDeinit:inout:::] */
/*    R_OSAL_OS_InterruptStaticDeinit => Stub */
int AMSTB_R_OSAL_OS_InterruptStaticDeinit(struct st_osal_device_control *device_control_table,unsigned long long device_table_num)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_device_control *volatile AMOUT_device_control_table;
    static unsigned long long volatile AMOUT_device_table_num;
    AMOUT_g_context_key = g_context_key;
    AMOUT_device_control_table = device_control_table;
    AMOUT_device_table_num = device_table_num;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptDisableIsr:AMSTB_R_OSAL_OS_InterruptDisableIsr:inout:::] */
/*    R_OSAL_OS_InterruptDisableIsr => Stub */
int AMSTB_R_OSAL_OS_InterruptDisableIsr(struct st_osal_device_control *handle,unsigned long long irq_channel)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    AMOUT_g_context_key = g_context_key;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_registermonitor_proc:AMSTB_r_osal_rcfg_mmngr_registermonitor_proc:inout:::] */
/*    r_osal_rcfg_mmngr_registermonitor_proc => Stub */
int AMSTB_r_osal_rcfg_mmngr_registermonitor_proc(int monitored_event,void *monitor_handler,void *p_monitor_handler_user_arg,struct st_osal_mem_mmngr_ptr_self *p_mngr_control)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_monitored_event;
    static void *volatile AMOUT_monitor_handler;
    static void *volatile AMOUT_p_monitor_handler_user_arg;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    AMOUT_monitored_event = monitored_event;
    AMOUT_monitor_handler = monitor_handler;
    AMOUT_p_monitor_handler_user_arg = p_monitor_handler_user_arg;
    AMOUT_p_mngr_control = p_mngr_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_registermonitor_pre_check:AMSTB_r_osal_rcfg_mmngr_registermonitor_pre_check:inout:::] */
/*    r_osal_rcfg_mmngr_registermonitor_pre_check => Stub */
int AMSTB_r_osal_rcfg_mmngr_registermonitor_pre_check(void *p_ptr_self)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    AMOUT_p_ptr_self = p_ptr_self;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open_config_check:AMSTB_r_osal_rcfg_mmngr_open_config_check:inout:::] */
/*    r_osal_rcfg_mmngr_open_config_check => Stub */
int AMSTB_r_osal_rcfg_mmngr_open_config_check(struct st_osal_mmngr_config *p_config,unsigned long long region_idx)
{
    static int volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_region_idx;
    AMOUT_p_config = p_config;
    AMOUT_region_idx = region_idx;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_debugmmngrprint:AMSTB_r_osal_rcfg_mmngr_debugmmngrprint:inout:::] */
/*    r_osal_rcfg_mmngr_debugmmngrprint => Stub */
int AMSTB_r_osal_rcfg_mmngr_debugmmngrprint(void *p_ptr_self,struct _Filet *p_output)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static struct _Filet *volatile AMOUT_p_output;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_p_output = p_output;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc:AMSTB_r_osal_rcfg_mmngr_alloc:inout:::] */
/*    r_osal_rcfg_mmngr_alloc => Stub */
int AMSTB_r_osal_rcfg_mmngr_alloc(void *p_ptr_self,unsigned long long bytes,unsigned long long alignment,struct st_osal_memory_buffer_obj **p_buffer_obj)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static unsigned long long volatile AMOUT_bytes;
    static unsigned long long volatile AMOUT_alignment;
    static struct st_osal_memory_buffer_obj **volatile AMOUT_p_buffer_obj;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_bytes = bytes;
    AMOUT_alignment = alignment;
    AMOUT_p_buffer_obj = p_buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_deallocate_msg_queue:AMSTB_r_osal_rcfg_mq_deallocate_msg_queue:inout:::] */
/*    r_osal_rcfg_mq_deallocate_msg_queue => Stub */
void AMSTB_r_osal_rcfg_mq_deallocate_msg_queue(struct st_osal_mq_control *p_mq_control)
{
    static struct st_osal_mq_control *volatile AMOUT_p_mq_control;
    AMOUT_p_mq_control = p_mq_control;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_search:AMSTB_r_osal_rcfg_thread_search:inout:::] */
/*    r_osal_rcfg_thread_search => Stub */
int AMSTB_r_osal_rcfg_thread_search(void *p_key,void *p_search_info)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_key;
    static void *volatile AMOUT_p_search_info;
    AMOUT_p_key = p_key;
    AMOUT_p_search_info = p_search_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_mmap_device:AMSTB_r_osal_os_io_mmap_device:inout:::] */
/*    r_osal_os_io_mmap_device => Stub */
int AMSTB_r_osal_os_io_mmap_device(struct st_osal_device_info *p_osal_device_config)
{
    static int volatile AMIN_return;
    static struct st_osal_device_info *volatile AMOUT_p_osal_device_config;
    AMOUT_p_osal_device_config = p_osal_device_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetPolicy:AMSTB_R_OSAL_DCFG_PmSetPolicy:inout:::] */
/*    R_OSAL_DCFG_PmSetPolicy => Stub */
int AMSTB_R_OSAL_DCFG_PmSetPolicy(struct st_osal_device_control *handle,int policy,unsigned char applyImmediate)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_policy;
    static unsigned char volatile AMOUT_applyImmediate;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_policy = policy;
    AMOUT_applyImmediate = applyImmediate;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetPolicy:AMSTB_R_OSAL_DCFG_PmGetPolicy:inout:::] */
/*    R_OSAL_DCFG_PmGetPolicy => Stub */
int AMSTB_R_OSAL_DCFG_PmGetPolicy(struct st_osal_device_control *handle,int *p_policy)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_policy;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_p_policy = p_policy;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexLockForTimePeriod:AMSTB_R_OSAL_RCFG_MutexLockForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_MutexLockForTimePeriod => Stub */
int AMSTB_R_OSAL_RCFG_MutexLockForTimePeriod(struct st_osal_mutex_control *handle,long long time_period)
{
    static int volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    static long long volatile AMOUT_time_period;
    AMOUT_handle = handle;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexCreate:AMSTB_R_OSAL_RCFG_MutexCreate:inout:::] */
/*    R_OSAL_RCFG_MutexCreate => Stub */
int AMSTB_R_OSAL_RCFG_MutexCreate(unsigned long long mutex_id,struct st_osal_mutex_control **p_handle)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_mutex_id;
    static struct st_osal_mutex_control **volatile AMOUT_p_handle;
    AMOUT_mutex_id = mutex_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetOsalMaxConfig:AMSTB_R_OSAL_RCFG_MmngrGetOsalMaxConfig:inout:::] */
/*    R_OSAL_RCFG_MmngrGetOsalMaxConfig => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetOsalMaxConfig(unsigned long long region_idx,struct st_osal_mmngr_config *p_config)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_region_idx;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_region_idx = region_idx;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_MmngrGetOsalMaxConfig:AMSTB_R_OSAL_MmngrGetOsalMaxConfig:inout:::] */
/*    R_OSAL_MmngrGetOsalMaxConfig => Stub */
int AMSTB_R_OSAL_MmngrGetOsalMaxConfig(st_osal_mmngr_config_t *const p_config)
{
    static int volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrDebugBufferPrint:AMSTB_R_OSAL_RCFG_MmngrDebugBufferPrint:inout:::] */
/*    R_OSAL_RCFG_MmngrDebugBufferPrint => Stub */
int AMSTB_R_OSAL_RCFG_MmngrDebugBufferPrint(struct st_osal_memory_buffer_obj *hndl_buffer,struct _Filet *p_output)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_hndl_buffer;
    static struct _Filet *volatile AMOUT_p_output;
    AMOUT_hndl_buffer = hndl_buffer;
    AMOUT_p_output = p_output;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqIsEmpty:AMSTB_R_OSAL_RCFG_MqIsEmpty:inout:::] */
/*    R_OSAL_RCFG_MqIsEmpty => Stub */
int AMSTB_R_OSAL_RCFG_MqIsEmpty(struct st_osal_mq_control *handle,unsigned char *p_result)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_handle = handle;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqCreate:AMSTB_R_OSAL_RCFG_MqCreate:inout:::] */
/*    R_OSAL_RCFG_MqCreate => Stub */
int AMSTB_R_OSAL_RCFG_MqCreate(struct st_osal_mq_config *p_config,unsigned long long mq_Id,struct st_osal_mq_control **p_handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_mq_Id;
    static struct st_osal_mq_control **volatile AMOUT_p_handle;
    AMOUT_p_config = p_config;
    AMOUT_mq_Id = mq_Id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_GetSysPageSize:AMSTB_R_OSAL_OS_GetSysPageSize:inout:::] */
/*    R_OSAL_OS_GetSysPageSize => Stub */
long long AMSTB_R_OSAL_OS_GetSysPageSize()
{
    static long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_usr_cfg.c:R_OSAL_RCFG_GetNumOfMq:AMSTB_R_OSAL_RCFG_GetNumOfMq:inout:::] */
/*    R_OSAL_RCFG_GetNumOfMq => Stub */
unsigned long long AMSTB_R_OSAL_RCFG_GetNumOfMq()
{
    static unsigned long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_create_device_list:AMSTB_r_osal_dcfg_io_create_device_list:inout:::] */
/*    r_osal_dcfg_io_create_device_list => Stub */
int AMSTB_r_osal_dcfg_io_create_device_list(struct st_osal_device_control *p_device_control,unsigned char *p_device_type_name,unsigned long long buffer_size,unsigned char *p_buffer,unsigned long long *p_total_length)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_p_device_control;
    static unsigned char *volatile AMOUT_p_device_type_name;
    static unsigned long long volatile AMOUT_buffer_size;
    static unsigned char *volatile AMOUT_p_buffer;
    static unsigned long long *volatile AMOUT_p_total_length;
    AMOUT_p_device_control = p_device_control;
    AMOUT_p_device_type_name = p_device_type_name;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_buffer = p_buffer;
    AMOUT_p_total_length = p_total_length;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_compare_for_axiname:AMSTB_r_osal_dcfg_io_compare_for_axiname:inout:::] */
/*    r_osal_dcfg_io_compare_for_axiname => Stub */
int AMSTB_r_osal_dcfg_io_compare_for_axiname(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_compare_for_axibus:AMSTB_r_osal_dcfg_io_compare_for_axibus:inout:::] */
/*    r_osal_dcfg_io_compare_for_axibus => Stub */
int AMSTB_r_osal_dcfg_io_compare_for_axibus(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_init_table:AMSTB_r_osal_rcfg_mutex_init_table:inout:::] */
/*    r_osal_rcfg_mutex_init_table => Stub */
int AMSTB_r_osal_rcfg_mutex_init_table(struct st_osal_os_mutexattr *mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    AMOUT_mutex_attr = mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_control_deinit:AMSTB_r_osal_rcfg_mutex_control_deinit:inout:::] */
/*    r_osal_rcfg_mutex_control_deinit => Stub */
int AMSTB_r_osal_rcfg_mutex_control_deinit()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_control_init:AMSTB_r_osal_rcfg_cond_control_init:inout:::] */
/*    r_osal_rcfg_cond_control_init => Stub */
int AMSTB_r_osal_rcfg_cond_control_init()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open:AMSTB_r_osal_rcfg_mmngr_open:inout:::] */
/*    r_osal_rcfg_mmngr_open => Stub */
int AMSTB_r_osal_rcfg_mmngr_open(struct st_osal_mmngr_config *p_usr_config,unsigned long long region_idx)
{
    static int volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_usr_config;
    static unsigned long long volatile AMOUT_region_idx;
    AMOUT_p_usr_config = p_usr_config;
    AMOUT_region_idx = region_idx;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_flush_proc:AMSTB_r_osal_rcfg_mmngr_flush_proc:inout:::] */
/*    r_osal_rcfg_mmngr_flush_proc => Stub */
int AMSTB_r_osal_rcfg_mmngr_flush_proc(unsigned long long offset,unsigned long long size,struct st_osal_mem_buf_ptr_self *p_buffer)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static struct st_osal_mem_buf_ptr_self *volatile AMOUT_p_buffer;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_buffer = p_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_cmn_check_buffer_handle:AMSTB_r_osal_rcfg_mmngr_cmn_check_buffer_handle:inout:::] */
/*    r_osal_rcfg_mmngr_cmn_check_buffer_handle => Stub */
int AMSTB_r_osal_rcfg_mmngr_cmn_check_buffer_handle(void* p_ptr_self)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    AMOUT_p_ptr_self = p_ptr_self;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_rcv_not_isr:AMSTB_r_osal_rcfg_mq_rcv_not_isr:inout:::] */
/*    r_osal_rcfg_mq_rcv_not_isr => Stub */
int AMSTB_r_osal_rcfg_mq_rcv_not_isr(struct st_osal_mq_control *handle,struct st_osal_time *p_time_stamp,void *p_buffer)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    static void *volatile AMOUT_p_buffer;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_destroy_cond:AMSTB_r_osal_rcfg_mq_destroy_cond:inout:::] */
/*    r_osal_rcfg_mq_destroy_cond => Stub */
int AMSTB_r_osal_rcfg_mq_destroy_cond(struct st_osal_mq_control *p_mq_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_p_mq_control;
    AMOUT_p_mq_control = p_mq_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_notify_finish:AMSTB_r_osal_rcfg_thread_notify_finish:inout:::] */
/*    r_osal_rcfg_thread_notify_finish => Stub */
int AMSTB_r_osal_rcfg_thread_notify_finish(struct st_osal_thread_control *p_thread_control)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_p_thread_control;
    AMOUT_p_thread_control = p_thread_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_ReleaseResetHwa:AMSTB_R_PMA_ReleaseResetHwa:inout:::] */
/*    R_PMA_ReleaseResetHwa => Stub */
e_pma_return_t AMSTB_R_PMA_ReleaseResetHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoRead:AMSTB_R_OSAL_DCFG_IoRead:inout:::] */
/*    R_OSAL_DCFG_IoRead => Stub */
int AMSTB_R_OSAL_DCFG_IoRead(struct st_osal_device_control *handle,unsigned long long offset,unsigned long long size,unsigned long long unit_size,void *p_data)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static unsigned long long volatile AMOUT_unit_size;
    static void *volatile AMOUT_p_data;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_unit_size = unit_size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrInvalidate:AMSTB_R_OSAL_OS_MmngrInvalidate:inout:::] */
/*    R_OSAL_OS_MmngrInvalidate => Stub */
int AMSTB_R_OSAL_OS_MmngrInvalidate(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long addr,unsigned long long offset,unsigned long long size)
{
#define AMCALLMAX_R_OSAL_OS_MmngrInvalidate 30
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MmngrInvalidate];
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_addr;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static int volatile AM_count;

    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_addr = addr;
    AMOUT_offset = offset;
    AMOUT_size = size;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexTryLock:AMSTB_R_OSAL_OS_MutexTryLock:inout:::] */
/*    R_OSAL_OS_MutexTryLock => Stub */
int AMSTB_R_OSAL_OS_MutexTryLock(struct st_osal_os_mutex *mutex)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_BinarySearch:AMSTB_R_OSAL_OS_BinarySearch:inout:::] */
/*    R_OSAL_OS_BinarySearch => Stub */
void *AMSTB_R_OSAL_OS_BinarySearch(void *p_key,void *p_array,unsigned long long array_num,unsigned long long array_size,void *compare_function)
{
    static void *volatile AMIN_return;
    static void *volatile AMOUT_p_key;
    static void *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_num;
    static unsigned long long volatile AMOUT_array_size;
    static void *volatile AMOUT_compare_function;
    AMOUT_p_key = p_key;
    AMOUT_p_array = p_array;
    AMOUT_array_num = array_num;
    AMOUT_array_size = array_size;
    AMOUT_compare_function = compare_function;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoStaticInit:AMSTB_R_OSAL_OS_IoStaticInit:inout:::] */
/*    R_OSAL_OS_IoStaticInit => Stub */
int AMSTB_R_OSAL_OS_IoStaticInit(unsigned long long *p_num_of_device_config,struct st_osal_device_config_addr **p_device_config_addr)
{
    static int volatile AMIN_return;
    static unsigned long long *volatile AMOUT_p_num_of_device_config;
    static struct st_osal_device_config_addr **volatile AMOUT_p_device_config_addr;
    static int volatile AM_count;

    *p_num_of_device_config = AM_count;
    *p_device_config_addr   = g_osal_device_config_addr_array;

    AMOUT_p_num_of_device_config = p_num_of_device_config;
    AMOUT_p_device_config_addr = p_device_config_addr;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memorybarrier.c:R_OSAL_OS_MemoryBarrier:AMSTB_R_OSAL_OS_MemoryBarrier:inout:::] */
/*    R_OSAL_OS_MemoryBarrier => Stub */
int AMSTB_R_OSAL_OS_MemoryBarrier()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_init_axibus_namelist:AMSTB_r_osal_dcfg_io_init_axibus_namelist:inout:::] */
/*    r_osal_dcfg_io_init_axibus_namelist => Stub */
int AMSTB_r_osal_dcfg_io_init_axibus_namelist()
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_dealloc_proc:AMSTB_r_osal_rcfg_mmngr_dealloc_proc:inout:::] */
/*    r_osal_rcfg_mmngr_dealloc_proc => Stub */
int AMSTB_r_osal_rcfg_mmngr_dealloc_proc(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_memory_buffer_obj *buffer_obj)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_buffer_obj = buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_receive:AMSTB_r_osal_rcfg_mq_receive:inout:::] */
/*    r_osal_rcfg_mq_receive => Stub */
int AMSTB_r_osal_rcfg_mq_receive(struct st_osal_mq_control *handle,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_allocate_msg_queue:AMSTB_r_osal_rcfg_mq_allocate_msg_queue:inout:::] */
/*    r_osal_rcfg_mq_allocate_msg_queue => Stub */
int AMSTB_r_osal_rcfg_mq_allocate_msg_queue(struct st_osal_mq_control *p_mq_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_p_mq_control;
    AMOUT_p_mq_control = p_mq_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3:AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3 => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforTypeACofGen3(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H:AMSTB_R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeHPtoCGforRGXofV4H(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptRegisterIsr:AMSTB_R_OSAL_DCFG_InterruptRegisterIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptRegisterIsr => Stub */
int AMSTB_R_OSAL_DCFG_InterruptRegisterIsr(struct st_osal_device_control *handle,unsigned long long irq_channel,int priority_value,void *irqHandlerFunction,void *p_irq_arg)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static int volatile AMOUT_priority_value;
    static void *volatile AMOUT_irqHandlerFunction;
    static void *volatile AMOUT_p_irq_arg;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    AMOUT_p_irq_arg = p_irq_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiIdListForDeviceName:AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiIdListForDeviceName => Stub */
int AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceName(unsigned char *p_device_name,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_written_axi_ids)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_size;
    static unsigned long long *volatile AMOUT_p_num_of_written_axi_ids;
    AMOUT_p_device_name = p_device_name;
    AMOUT_p_array = p_array;
    AMOUT_array_size = array_size;
    AMOUT_p_num_of_written_axi_ids = p_num_of_written_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl:AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl => Stub */
int AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl(struct st_osal_device_control *device_hdl,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_written_axi_ids)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_device_hdl;
    static unsigned long long *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_size;
    static unsigned long long *volatile AMOUT_p_num_of_written_axi_ids;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_device_hdl = device_hdl;
    AMOUT_p_array = p_array;
    AMOUT_array_size = array_size;
    AMOUT_p_num_of_written_axi_ids = p_num_of_written_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_thread.c:R_OSAL_HIF_ThreadSleepUntilTimeStamp:AMSTB_R_OSAL_HIF_ThreadSleepUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_ThreadSleepUntilTimeStamp => Stub */
int AMSTB_R_OSAL_HIF_ThreadSleepUntilTimeStamp(struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexDestroy:AMSTB_R_OSAL_RCFG_MutexDestroy:inout:::] */
/*    R_OSAL_RCFG_MutexDestroy => Stub */
int AMSTB_R_OSAL_RCFG_MutexDestroy(struct st_osal_mutex_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrSetInitStatus:AMSTB_R_OSAL_RCFG_MmngrSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MmngrSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_MmngrSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrRegisterMonitor:AMSTB_R_OSAL_RCFG_MmngrRegisterMonitor:inout:::] */
/*    R_OSAL_RCFG_MmngrRegisterMonitor => Stub */
int AMSTB_R_OSAL_RCFG_MmngrRegisterMonitor(struct st_osal_memory_manager_obj *hndl_mmngr,int monitored_event,void *monitor_handler,void *p_user_arg)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static int volatile AMOUT_monitored_event;
    static void *volatile AMOUT_monitor_handler;
    static void *volatile AMOUT_p_user_arg;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_monitored_event = monitored_event;
    AMOUT_monitor_handler = monitor_handler;
    AMOUT_p_user_arg = p_user_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrDebugMmngrPrint:AMSTB_R_OSAL_RCFG_MmngrDebugMmngrPrint:inout:::] */
/*    R_OSAL_RCFG_MmngrDebugMmngrPrint => Stub */
int AMSTB_R_OSAL_RCFG_MmngrDebugMmngrPrint(struct st_osal_memory_manager_obj *hndl_mmngr,struct _Filet *p_output)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static struct _Filet *volatile AMOUT_p_output;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_p_output = p_output;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrDealloc:AMSTB_R_OSAL_RCFG_MmngrDealloc:inout:::] */
/*    R_OSAL_RCFG_MmngrDealloc => Stub */
int AMSTB_R_OSAL_RCFG_MmngrDealloc(struct st_osal_memory_manager_obj *hndl_mmngr,struct st_osal_memory_buffer_obj *buffer_obj)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_buffer_obj = buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexDeinitAttr:AMSTB_R_OSAL_OS_MutexDeinitAttr:inout:::] */
/*    R_OSAL_OS_MutexDeinitAttr => Stub */
int AMSTB_R_OSAL_OS_MutexDeinitAttr(struct st_osal_os_mutexattr *mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;

    AMOUT_mutex_attr = mutex_attr;
    
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptSetIsrPriority:AMSTB_R_OSAL_OS_InterruptSetIsrPriority:inout:::] */
/*    R_OSAL_OS_InterruptSetIsrPriority => Stub */
int AMSTB_R_OSAL_OS_InterruptSetIsrPriority(struct st_osal_device_control *handle,unsigned long long irq_channel,int priority_value)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static int volatile AMOUT_priority_value;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptEnableIsr:AMSTB_R_OSAL_OS_InterruptEnableIsr:inout:::] */
/*    R_OSAL_OS_InterruptEnableIsr => Stub */
int AMSTB_R_OSAL_OS_InterruptEnableIsr(struct st_osal_device_control *handle,unsigned long long irq_channel)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    AMOUT_g_context_key = g_context_key;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Free:AMSTB_R_OSAL_OS_Free:inout:::] */
/*    R_OSAL_OS_Free => Stub */
void AMSTB_R_OSAL_OS_Free(void *p_ptr)
{
    static void *volatile AMOUT_p_ptr;
    AMOUT_p_ptr = p_ptr;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_register_axibus_namelist:AMSTB_r_osal_dcfg_io_register_axibus_namelist:inout:::] */
/*    r_osal_dcfg_io_register_axibus_namelist => Stub */
int AMSTB_r_osal_dcfg_io_register_axibus_namelist(struct st_osal_device_control *p_device_control)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_p_device_control;
    AMOUT_p_device_control = p_device_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_getcpuptr:AMSTB_r_osal_rcfg_mmngr_getcpuptr:inout:::] */
/*    r_osal_rcfg_mmngr_getcpuptr => Stub */
int AMSTB_r_osal_rcfg_mmngr_getcpuptr(void *p_ptr_self,void **pp_cpu_ptr)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static void **volatile AMOUT_pp_cpu_ptr;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_pp_cpu_ptr = pp_cpu_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_dealloc:AMSTB_r_osal_rcfg_mmngr_dealloc:inout:::] */
/*    r_osal_rcfg_mmngr_dealloc => Stub */
int AMSTB_r_osal_rcfg_mmngr_dealloc(void *p_ptr_self,struct st_osal_memory_buffer_obj *buffer_obj)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_buffer_obj = buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_snd_is_isr:AMSTB_r_osal_rcfg_mq_snd_is_isr:inout:::] */
/*    r_osal_rcfg_mq_snd_is_isr => Stub */
int AMSTB_r_osal_rcfg_mq_snd_is_isr(struct st_osal_mq_control *handle,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_create_mutex_cond:AMSTB_r_osal_rcfg_mq_create_mutex_cond:inout:::] */
/*    r_osal_rcfg_mq_create_mutex_cond => Stub */
int AMSTB_r_osal_rcfg_mq_create_mutex_cond(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_condattr *cond_attr,struct st_osal_mq_control *p_mq_control)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    static struct st_osal_mq_control *volatile AMOUT_p_mq_control;
    AMOUT_mutex_attr = mutex_attr;
    AMOUT_cond_attr = cond_attr;
    AMOUT_p_mq_control = p_mq_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_init_attr:AMSTB_r_osal_rcfg_thread_init_attr:inout:::] */
/*    r_osal_rcfg_thread_init_attr => Stub */
int AMSTB_r_osal_rcfg_thread_init_attr(struct st_osal_os_mutexattr **p_mutex_attr,struct st_osal_os_condattr **p_cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr **volatile AMOUT_p_mutex_attr;
    static struct st_osal_os_condattr **volatile AMOUT_p_cond_attr;
    AMOUT_p_mutex_attr = p_mutex_attr;
    AMOUT_p_cond_attr = p_cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_compare:AMSTB_r_osal_rcfg_thread_compare:inout:::] */
/*    r_osal_rcfg_thread_compare => Stub */
int AMSTB_r_osal_rcfg_thread_compare(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_WaitForState:AMSTB_R_PMA_WaitForState:inout:::] */
/*    R_PMA_WaitForState => Stub */
e_pma_return_t AMSTB_R_PMA_WaitForState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,const st_pma_hwa_state_t *pHwaState,e_pma_reset_req_t rstStat,long long sleepTimePeriod,long long timeoutMsPeriod,long long timeoutNsPeriod)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static const st_pma_hwa_state_t *volatile AMOUT_pHwaState;
    static e_pma_reset_req_t volatile AMOUT_rstStat;
    static long long volatile AMOUT_sleepTimePeriod;
    static long long volatile AMOUT_timeoutMsPeriod;
    static long long volatile AMOUT_timeoutNsPeriod;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pHwaState = pHwaState;
    AMOUT_rstStat = rstStat;
    AMOUT_sleepTimePeriod = sleepTimePeriod;
    AMOUT_timeoutMsPeriod = timeoutMsPeriod;
    AMOUT_timeoutNsPeriod = timeoutNsPeriod;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetReset:AMSTB_R_PMA_GetReset:inout:::] */
/*    R_PMA_GetReset => Stub */
e_pma_return_t AMSTB_R_PMA_GetReset(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstStat)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_reset_req_t *volatile AMOUT_pRstStat;
    static e_pma_reset_req_t volatile AMIN_RstStat;

    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pRstStat = pRstStat;
    *pRstStat = AMIN_RstStat;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptUnregisterIsr:AMSTB_R_OSAL_DCFG_InterruptUnregisterIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptUnregisterIsr => Stub */
int AMSTB_R_OSAL_DCFG_InterruptUnregisterIsr(struct st_osal_device_control *handle,unsigned long long irq_channel,void *irqHandlerFunction)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static void *volatile AMOUT_irqHandlerFunction;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    return AMIN_return;
}

int AMSTB_R_OSAL_DCFG_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfDevices:AMSTB_R_OSAL_DCFG_IoGetNumOfDevices:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfDevices => Stub */
int AMSTB_R_OSAL_DCFG_IoGetNumOfDevices(unsigned char *p_deviceType,unsigned long long *p_numOfDevices)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static unsigned char *volatile AMOUT_p_deviceType;
    static unsigned long long *volatile AMOUT_p_numOfDevices;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_p_deviceType = p_deviceType;
    AMOUT_p_numOfDevices = p_numOfDevices;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName:AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName => Stub */
int AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName(unsigned char *p_device_name,unsigned long long *p_num_of_axi_ids)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_p_device_name = p_device_name;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl:AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl => Stub */
int AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl(struct st_osal_device_control *device_hdl,unsigned long long *p_num_of_axi_ids)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_device_hdl;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_device_hdl = device_hdl;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetRequiredState:AMSTB_R_OSAL_DCFG_PmSetRequiredState:inout:::] */
/*    R_OSAL_DCFG_PmSetRequiredState => Stub */
int AMSTB_R_OSAL_DCFG_PmSetRequiredState(struct st_osal_device_control *handle,int state,unsigned char applyImmediate)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_state;
    static unsigned char volatile AMOUT_applyImmediate;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_state = state;
    AMOUT_applyImmediate = applyImmediate;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetRequiredState:AMSTB_R_OSAL_DCFG_PmGetRequiredState:inout:::] */
/*    R_OSAL_DCFG_PmGetRequiredState => Stub */
int AMSTB_R_OSAL_DCFG_PmGetRequiredState(struct st_osal_device_control *handle,int *p_state)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_cond.c:R_OSAL_HIF_CondWaitForTimePeriod:AMSTB_R_OSAL_HIF_CondWaitForTimePeriod:inout:::] */
/*    R_OSAL_HIF_CondWaitForTimePeriod => Stub */
int AMSTB_R_OSAL_HIF_CondWaitForTimePeriod(struct st_osal_os_cond *cond,struct st_osal_os_mutex *mutex,long long time_period)
{
    static int volatile AMIN_return;
    static struct st_osal_os_cond *volatile AMOUT_cond;
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static long long volatile AMOUT_time_period;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondWaitForTimePeriod:AMSTB_R_OSAL_RCFG_CondWaitForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_CondWaitForTimePeriod => Stub */
int AMSTB_R_OSAL_RCFG_CondWaitForTimePeriod(struct st_osal_cond_control *cond_handle,struct st_osal_mutex_control *mutex_handle,long long time_period)
{
    static int volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_cond_handle;
    static struct st_osal_mutex_control *volatile AMOUT_mutex_handle;
    static long long volatile AMOUT_time_period;
    AMOUT_cond_handle = cond_handle;
    AMOUT_mutex_handle = mutex_handle;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_forpma.c:R_OSAL_Internal_GetThreadSpecific:AMSTB_R_OSAL_Internal_GetThreadSpecific:inout:::] */
/*    R_OSAL_Internal_GetThreadSpecific => Stub */
void *AMSTB_R_OSAL_Internal_GetThreadSpecific()
{
    static void *volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrUnmap:AMSTB_R_OSAL_OS_MmngrUnmap:inout:::] */
/*    R_OSAL_OS_MmngrUnmap => Stub */
int AMSTB_R_OSAL_OS_MmngrUnmap(struct st_osal_mem_mmngr_ptr_self *mngr_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_mngr_control;
    AMOUT_mngr_control = mngr_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_timeclock.c:R_OSAL_OS_ClockTimeInit:AMSTB_R_OSAL_OS_ClockTimeInit:inout:::] */
/*    R_OSAL_OS_ClockTimeInit => Stub */
int AMSTB_R_OSAL_OS_ClockTimeInit()
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_qtime;
    AMOUT_qtime = qtime;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_get_domain_name:AMSTB_r_osal_dcfg_io_get_domain_name:inout:::] */
/*    r_osal_dcfg_io_get_domain_name => Stub */
int AMSTB_r_osal_dcfg_io_get_domain_name(unsigned char *p_axi_bus_name,unsigned char *p_domain_name)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_axi_bus_name;
    static unsigned char *volatile AMOUT_p_domain_name;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_p_domain_name = p_domain_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_get_state:AMSTB_r_osal_dcfg_pm_get_state:inout:::] */
/*    r_osal_dcfg_pm_get_state => Stub */
int AMSTB_r_osal_dcfg_pm_get_state(st_pma_handle_t pma_handle,e_pma_hwa_id_t pma_hwaId,int *p_state)
{
    static int volatile AMIN_return;
    static st_pma_handle_t AMOUT_pma_handle;
    static e_pma_hwa_id_t volatile AMOUT_pma_hwaId;
    static int *volatile AMOUT_p_state;
    static e_osal_pm_state_t volatile AMIN_state;

    AMOUT_pma_handle = pma_handle;
    AMOUT_pma_hwaId = pma_hwaId;
    AMOUT_p_state = p_state;
    *p_state = AMIN_state;

    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_SetPowerPolicy:AMSTB_R_PMA_SetPowerPolicy:inout:::] */
/*    R_PMA_SetPowerPolicy => Stub */
e_pma_return_t AMSTB_R_PMA_SetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t volatile AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_policy_t volatile AMOUT_policy;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_policy = policy;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_search:AMSTB_r_osal_rcfg_mutex_search:inout:::] */
/*    r_osal_rcfg_mutex_search => Stub */
int AMSTB_r_osal_rcfg_mutex_search(void *p_key,void *p_search_info)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_key;
    static void *volatile AMOUT_p_search_info;
    AMOUT_p_key = p_key;
    AMOUT_p_search_info = p_search_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_dealloc_pre_check:AMSTB_r_osal_rcfg_mmngr_dealloc_pre_check:inout:::] */
/*    r_osal_rcfg_mmngr_dealloc_pre_check => Stub */
int AMSTB_r_osal_rcfg_mmngr_dealloc_pre_check(void *p_ptr_self,struct st_osal_memory_buffer_obj *buffer_obj)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_buffer_obj = buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_send:AMSTB_r_osal_rcfg_mq_send:inout:::] */
/*    r_osal_rcfg_mq_send => Stub */
int AMSTB_r_osal_rcfg_mq_send(struct st_osal_mq_control *handle,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_destroy_mutex:AMSTB_r_osal_rcfg_mq_destroy_mutex:inout:::] */
/*    r_osal_rcfg_mq_destroy_mutex => Stub */
int AMSTB_r_osal_rcfg_mq_destroy_mutex(struct st_osal_mq_control *p_mq_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_p_mq_control;
    AMOUT_p_mq_control = p_mq_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_set_thread_attr:AMSTB_r_osal_os_interrupt_set_thread_attr:inout:::] */
/*    r_osal_os_interrupt_set_thread_attr => Stub */
int AMSTB_r_osal_os_interrupt_set_thread_attr(struct _thread_attr *p_thread_attr,struct st_osal_interrupt_thread_config *p_usr_config)
{
    static int volatile AMIN_return;
    static struct _thread_attr *volatile AMOUT_p_thread_attr;
    static struct st_osal_interrupt_thread_config *volatile AMOUT_p_usr_config;
    AMOUT_p_thread_attr = p_thread_attr;
    AMOUT_p_usr_config = p_usr_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_SetPostClock:AMSTB_R_PMA_SetPostClock:inout:::] */
/*    R_PMA_SetPostClock => Stub */
e_pma_return_t AMSTB_R_PMA_SetPostClock(st_pma_handle_t handle,e_pma_clk_req_t clockEnable,unsigned int chanIdx,unsigned int *pClockDivider)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_clk_req_t volatile AMOUT_clockEnable;
    static unsigned int volatile AMOUT_chanIdx;
    static unsigned int *volatile AMOUT_pClockDivider;
    AMOUT_handle = handle;
    AMOUT_clockEnable = clockEnable;
    AMOUT_chanIdx = chanIdx;
    AMOUT_pClockDivider = pClockDivider;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoSetInitStatus:AMSTB_R_OSAL_DCFG_IoSetInitStatus:inout:::] */
/*    R_OSAL_DCFG_IoSetInitStatus => Stub */
void AMSTB_R_OSAL_DCFG_IoSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoDeviceClose:AMSTB_R_OSAL_DCFG_IoDeviceClose:inout:::] */
/*    R_OSAL_DCFG_IoDeviceClose => Stub */
int AMSTB_R_OSAL_DCFG_IoDeviceClose(struct st_osal_device_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrCheckBuffer:AMSTB_R_OSAL_RCFG_MmngrCheckBuffer:inout:::] */
/*    R_OSAL_RCFG_MmngrCheckBuffer => Stub */
int AMSTB_R_OSAL_RCFG_MmngrCheckBuffer(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadJoin:AMSTB_R_OSAL_RCFG_ThreadJoin:inout:::] */
/*    R_OSAL_RCFG_ThreadJoin => Stub */
int AMSTB_R_OSAL_RCFG_ThreadJoin(struct st_osal_thread_control *handle,long long *p_return_value)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_handle;
    static long long *volatile AMOUT_p_return_value;
    AMOUT_handle = handle;
    AMOUT_p_return_value = p_return_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondInitAttr:AMSTB_R_OSAL_OS_CondInitAttr:inout:::] */
/*    R_OSAL_OS_CondInitAttr => Stub */
int AMSTB_R_OSAL_OS_CondInitAttr(struct st_osal_os_condattr **p_cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_condattr **volatile AMOUT_p_cond_attr;
    AMOUT_p_cond_attr = p_cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_register_axibus_info_proc:AMSTB_r_osal_dcfg_io_register_axibus_info_proc:inout:::] */
/*    r_osal_dcfg_io_register_axibus_info_proc => Stub */
int AMSTB_r_osal_dcfg_io_register_axibus_info_proc(unsigned char *p_bus_domain,unsigned char *p_device_name,int utlb)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_bus_domain;
    static unsigned char *volatile AMOUT_p_device_name;
    static int volatile AMOUT_utlb;
    AMOUT_p_bus_domain = p_bus_domain;
    AMOUT_p_device_name = p_device_name;
    AMOUT_utlb = utlb;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_destroy_mutex:AMSTB_r_osal_rcfg_thread_destroy_mutex:inout:::] */
/*    r_osal_rcfg_thread_destroy_mutex => Stub */
int AMSTB_r_osal_rcfg_thread_destroy_mutex(struct st_osal_thread_control *p_thread_control)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_p_thread_control;
    AMOUT_p_thread_control = p_thread_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3 => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeCofGen3(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToHPforTypeAC:AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforTypeAC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToHPforTypeAC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforTypeAC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_mutex.c:R_OSAL_HIF_MutexLockUntilTimeStamp:AMSTB_R_OSAL_HIF_MutexLockUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_MutexLockUntilTimeStamp => Stub */
int AMSTB_R_OSAL_HIF_MutexLockUntilTimeStamp(struct st_osal_os_mutex *mutex,struct st_osal_time *p_time_stamp)
{
#define AMCALLMAX_R_OSAL_HIF_MutexLockUntilTimeStamp 30
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_HIF_MutexLockUntilTimeStamp];
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    static int volatile AM_count;

    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondCreate:AMSTB_R_OSAL_RCFG_CondCreate:inout:::] */
/*    R_OSAL_RCFG_CondCreate => Stub */
int AMSTB_R_OSAL_RCFG_CondCreate(unsigned long long cond_id,struct st_osal_cond_control **p_handle)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_cond_id;
    static struct st_osal_cond_control **volatile AMOUT_p_handle;
    AMOUT_cond_id = cond_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetSize:AMSTB_R_OSAL_RCFG_MmngrGetSize:inout:::] */
/*    R_OSAL_RCFG_MmngrGetSize => Stub */
int AMSTB_R_OSAL_RCFG_MmngrGetSize(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long *p_size)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long *volatile AMOUT_p_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_p_size = p_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetNumOfRegions:AMSTB_R_OSAL_RCFG_MmngrGetNumOfRegions:inout:::] */
/*    R_OSAL_RCFG_MmngrGetNumOfRegions => Stub */
unsigned long long AMSTB_R_OSAL_RCFG_MmngrGetNumOfRegions()
{
    static unsigned long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqSendForTimePeriod:AMSTB_R_OSAL_RCFG_MqSendForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_MqSendForTimePeriod => Stub */
int AMSTB_R_OSAL_RCFG_MqSendForTimePeriod(struct st_osal_mq_control *handle,long long time_period,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static long long volatile AMOUT_time_period;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_time_period = time_period;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSleepForTimePeriod:AMSTB_R_OSAL_RCFG_ThreadSleepForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_ThreadSleepForTimePeriod => Stub */
int AMSTB_R_OSAL_RCFG_ThreadSleepForTimePeriod(long long time_period)
{
    static int volatile AMIN_return;
    static long long volatile AMOUT_time_period;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_QuickSort:AMSTB_R_OSAL_OS_QuickSort:inout:::] */
/*    R_OSAL_OS_QuickSort => Stub */
void AMSTB_R_OSAL_OS_QuickSort(void *p_array,unsigned long long array_num,unsigned long long array_size,void *compare_function)
{
    static void *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_num;
    static unsigned long long volatile AMOUT_array_size;
    static void *volatile AMOUT_compare_function;
    AMOUT_p_array = p_array;
    AMOUT_array_num = array_num;
    AMOUT_array_size = array_size;
    AMOUT_compare_function = compare_function;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadStaticSelf:AMSTB_R_OSAL_OS_ThreadStaticSelf:inout:::] */
/*    R_OSAL_OS_ThreadSelf => Stub */
int AMSTB_R_OSAL_OS_ThreadStaticSelf(void **pp_arg)
{
    static int volatile AMIN_return;
	static void ** AMIN_pp_arg;
	*pp_arg = AMIN_pp_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSetSelf:AMSTB_R_OSAL_OS_ThreadSetSelf:inout:::] */
/*    R_OSAL_OS_ThreadSetSelf => Stub */
int AMSTB_R_OSAL_OS_ThreadSetSelf(const osal_os_thread_t thread, void *p_arg)
{
    static int volatile AMIN_return;
	static struct st_osal_os_thread * AMOUT_thread;
	static void * AMOUT_p_arg;
	AMOUT_thread = thread;
	AMOUT_p_arg  = p_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_init_axibus_info:AMSTB_r_osal_dcfg_io_init_axibus_info:inout:::] */
/*    r_osal_dcfg_io_init_axibus_info => Stub */
int AMSTB_r_osal_dcfg_io_init_axibus_info()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_check_interrupt_handler:AMSTB_r_osal_dcfg_io_check_interrupt_handler:inout:::] */
/*    r_osal_dcfg_io_check_interrupt_handler => Stub */
int AMSTB_r_osal_dcfg_io_check_interrupt_handler(struct st_osal_device_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_wait_for_policy:AMSTB_r_osal_dcfg_pm_wait_for_policy:inout:::] */
/*    r_osal_dcfg_pm_wait_for_policy => Stub */
int AMSTB_r_osal_dcfg_pm_wait_for_policy(struct st_osal_device_control *handle,e_pma_power_policy_t pma_policy,unsigned char applyImmediate)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static e_pma_power_policy_t volatile AMOUT_pma_policy;
    static unsigned char volatile AMOUT_applyImmediate;
    AMOUT_handle = handle;
    AMOUT_pma_policy = pma_policy;
    AMOUT_applyImmediate = applyImmediate;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_send_monitor_event:AMSTB_r_osal_rcfg_mmngr_send_monitor_event:inout:::] */
/*    r_osal_rcfg_mmngr_send_monitor_event => Stub */
int AMSTB_r_osal_rcfg_mmngr_send_monitor_event(struct st_osal_os_mutex *mutex_id,int event_id,int error_code,struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_memory_buffer_obj *buffer_hndl)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutex *volatile AMOUT_mutex_id;
    static int volatile AMOUT_event_id;
    static int volatile AMOUT_error_code;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_hndl;
    AMOUT_mutex_id = mutex_id;
    AMOUT_event_id = event_id;
    AMOUT_error_code = error_code;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_buffer_hndl = buffer_hndl;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_close_dealloc_buffers:AMSTB_r_osal_rcfg_mmngr_close_dealloc_buffers:inout:::] */
/*    r_osal_rcfg_mmngr_close_dealloc_buffers => Stub */
int AMSTB_r_osal_rcfg_mmngr_close_dealloc_buffers(struct st_osal_mem_mmngr_ptr_self *p_mngr_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    AMOUT_p_mngr_control = p_mngr_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_lib:AMSTB_r_osal_rcfg_mmngr_alloc_lib:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_lib => Stub */
int AMSTB_r_osal_rcfg_mmngr_alloc_lib(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mem_buf_ptr_self *p_buffer_info,unsigned long long bytes,unsigned long long alignment)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_mem_buf_ptr_self *volatile AMOUT_p_buffer_info;
    static unsigned long long volatile AMOUT_bytes;
    static unsigned long long volatile AMOUT_alignment;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_p_buffer_info = p_buffer_info;
    AMOUT_bytes = bytes;
    AMOUT_alignment = alignment;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_reset_proc:AMSTB_r_osal_rcfg_mq_reset_proc:inout:::] */
/*    r_osal_rcfg_mq_reset_proc => Stub */
int AMSTB_r_osal_rcfg_mq_reset_proc(struct st_osal_mq_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_rcv_is_isr:AMSTB_r_osal_rcfg_mq_rcv_is_isr:inout:::] */
/*    r_osal_rcfg_mq_rcv_is_isr => Stub */
int AMSTB_r_osal_rcfg_mq_rcv_is_isr(struct st_osal_mq_control *handle,void *p_buffer)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static void *volatile AMOUT_p_buffer;
    AMOUT_handle = handle;
    AMOUT_p_buffer = p_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_init_attr:AMSTB_r_osal_rcfg_mq_init_attr:inout:::] */
/*    r_osal_rcfg_mq_init_attr => Stub */
int AMSTB_r_osal_rcfg_mq_init_attr(struct st_osal_os_mutexattr **p_mutex_attr,struct st_osal_os_condattr **p_cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr **volatile AMOUT_p_mutex_attr;
    static struct st_osal_os_condattr **volatile AMOUT_p_cond_attr;
    AMOUT_p_mutex_attr = p_mutex_attr;
    AMOUT_p_cond_attr = p_cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_deinit_system_resources:AMSTB_r_osal_rcfg_thread_deinit_system_resources:inout:::] */
/*    r_osal_rcfg_thread_deinit_system_resources => Stub */
int AMSTB_r_osal_rcfg_thread_deinit_system_resources(struct st_osal_thread_control *p_thread_control)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_p_thread_control;
    AMOUT_p_thread_control = p_thread_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_timedjoin:AMSTB_r_osal_os_interrupt_timedjoin:inout:::] */
/*    r_osal_os_interrupt_timedjoin => Stub */
int AMSTB_r_osal_os_interrupt_timedjoin()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_init_proc:AMSTB_r_osal_os_interrupt_init_proc:inout:::] */
/*    r_osal_os_interrupt_init_proc => Stub */
int AMSTB_r_osal_os_interrupt_init_proc(struct _sync_attr *p_mutex_attr,unsigned long long device_table_num,struct st_osal_device_control *device_control_table)
{
    static int volatile AMIN_return;
    static struct _sync_attr *volatile AMOUT_p_mutex_attr;
    static unsigned long long volatile AMOUT_device_table_num;
    static struct st_osal_device_control *volatile AMOUT_device_control_table;
    AMOUT_p_mutex_attr = p_mutex_attr;
    AMOUT_device_table_num = device_table_num;
    AMOUT_device_control_table = device_control_table;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_UnlockHwa:AMSTB_R_PMA_UnlockHwa:inout:::] */
/*    R_PMA_UnlockHwa => Stub */
e_pma_return_t AMSTB_R_PMA_UnlockHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_RequestHwa:AMSTB_R_PMA_RequestHwa:inout:::] */
/*    R_PMA_RequestHwa => Stub */
e_pma_return_t AMSTB_R_PMA_RequestHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetPostClock:AMSTB_R_OSAL_DCFG_PmSetPostClock:inout:::] */
/*    R_OSAL_DCFG_PmSetPostClock => Stub */
int AMSTB_R_OSAL_DCFG_PmSetPostClock(struct st_osal_device_control *device,unsigned char clock_enable,unsigned int clock_divider)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_device;
    static unsigned char volatile AMOUT_clock_enable;
    static unsigned int volatile AMOUT_clock_divider;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_device = device;
    AMOUT_clock_enable = clock_enable;
    AMOUT_clock_divider = clock_divider;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_timeclock.c:R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod:AMSTB_R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod:inout:::] */
/*    R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod => Stub */
int AMSTB_R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(long long time_period,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static long long volatile AMOUT_time_period;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_time_period = time_period;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncSetInitStatus:AMSTB_R_OSAL_RCFG_ThsyncSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThsyncSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_ThsyncSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondBroadcast:AMSTB_R_OSAL_RCFG_CondBroadcast:inout:::] */
/*    R_OSAL_RCFG_CondBroadcast => Stub */
int AMSTB_R_OSAL_RCFG_CondBroadcast(struct st_osal_cond_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrFlush:AMSTB_R_OSAL_RCFG_MmngrFlush:inout:::] */
/*    R_OSAL_RCFG_MmngrFlush => Stub */
int AMSTB_R_OSAL_RCFG_MmngrFlush(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long offset,unsigned long long size)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_offset = offset;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondSignal:AMSTB_R_OSAL_OS_CondSignal:inout:::] */
/*    R_OSAL_OS_CondSignal => Stub */
int AMSTB_R_OSAL_OS_CondSignal(struct st_osal_os_cond *cond)
{
    static int volatile AMIN_return;
    static struct st_osal_os_cond *volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_GetThreadSpecific:AMSTB_R_OSAL_OS_GetThreadSpecific:inout:::] */
/*    R_OSAL_OS_GetThreadSpecific => Stub */
void *AMSTB_R_OSAL_OS_GetThreadSpecific()
{
    static void *volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    AMOUT_g_context_key = g_context_key;
    return AMIN_return;
}

int AMSTB_R_OSAL_OS_IsISRContext(unsigned char *p_is_isr)
{
    static int volatile   AMIN_return;
    static bool *volatile  AMOUT_p_is_isr;
    static unsigned char volatile  AM_result;

    AMOUT_p_is_isr = p_is_isr;

    *p_is_isr = AM_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadStaticCreate:AMSTB_R_OSAL_OS_ThreadStaticCreate:inout:::] */
/*    R_OSAL_OS_ThreadStaticCreate => Stub */
int AMSTB_R_OSAL_OS_ThreadStaticCreate(struct st_osal_thread_config *p_config,void *func,void *p_arg,struct st_osal_os_threadattr *thread_attr,struct st_osal_os_thread **p_thread)
{
    static int volatile AMIN_return;
    static struct st_osal_thread_config *volatile AMOUT_p_config;
    static void *volatile AMOUT_func;
    static void *volatile AMOUT_p_arg;
    static struct st_osal_os_threadattr *volatile AMOUT_thread_attr;
    static struct st_osal_os_thread **volatile AMOUT_p_thread;
    AMOUT_p_config = p_config;
    AMOUT_func = func;
    AMOUT_p_arg = p_arg;
    AMOUT_thread_attr = thread_attr;
    AMOUT_p_thread = p_thread;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSelfFinish:AMSTB_R_OSAL_OS_ThreadSelfFinish:inout:::] */
/*    R_OSAL_OS_ThreadSelfFinish => Stub */
int AMSTB_R_OSAL_OS_ThreadSelfFinish(struct st_osal_os_thread *self_id)
{
    static int volatile AMIN_return;
    static struct st_osal_os_thread *volatile AMOUT_self_id;
    AMOUT_self_id = self_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadInitAttr:AMSTB_R_OSAL_OS_ThreadInitAttr:inout:::] */
/*    R_OSAL_OS_ThreadInitAttr => Stub */
int AMSTB_R_OSAL_OS_ThreadInitAttr(int priority,unsigned long long stack_size,struct st_osal_os_threadattr **p_thread_attr)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_priority;
    static unsigned long long volatile AMOUT_stack_size;
    static struct st_osal_os_threadattr **volatile AMOUT_p_thread_attr;
    AMOUT_priority = priority;
    AMOUT_stack_size = stack_size;
    AMOUT_p_thread_attr = p_thread_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_check_busy:AMSTB_r_osal_rcfg_mutex_check_busy:inout:::] */
/*    r_osal_rcfg_mutex_check_busy => Stub */
int AMSTB_r_osal_rcfg_mutex_check_busy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_thread_proc:AMSTB_r_osal_os_interrupt_thread_proc:inout:::] */
/*    r_osal_os_interrupt_thread_proc => Stub */
void AMSTB_r_osal_os_interrupt_thread_proc(struct st_osal_interrupt_thread_control *p_interrupt_control,int local_result)
{
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_interrupt_control;
    static int volatile AMOUT_local_result;
    AMOUT_p_interrupt_control = p_interrupt_control;
    AMOUT_local_result = local_result;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_thread_init:AMSTB_r_osal_os_interrupt_thread_init:inout:::] */
/*    r_osal_os_interrupt_thread_init => Stub */
int AMSTB_r_osal_os_interrupt_thread_init(struct st_osal_interrupt_thread_control *p_info,struct st_osal_device_control *p_device_control,struct _sync_attr *p_mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_info;
    static struct st_osal_device_control *volatile AMOUT_p_device_control;
    static struct _sync_attr *volatile AMOUT_p_mutex_attr;
    AMOUT_p_info = p_info;
    AMOUT_p_device_control = p_device_control;
    AMOUT_p_mutex_attr = p_mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_thread:AMSTB_r_osal_os_interrupt_thread:inout:::] */
/*    r_osal_os_interrupt_thread => Stub */
void *AMSTB_r_osal_os_interrupt_thread(void *p_arg)
{
    static void *volatile AMIN_return;
    static void *volatile AMOUT_p_arg;
    AMOUT_p_arg = p_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_compare:AMSTB_r_osal_os_io_compare:inout:::] */
/*    r_osal_os_io_compare => Stub */
int AMSTB_r_osal_os_io_compare(void *p_cmp_info1,void *p_cmp_info2)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_cmp_info1;
    static void *volatile AMOUT_p_cmp_info2;
    AMOUT_p_cmp_info1 = p_cmp_info1;
    AMOUT_p_cmp_info2 = p_cmp_info2;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGwithoutSYSC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforRGXofV4H(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC:AMSTB_R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToHPwithoutSYSC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H:AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforRGXofV4H(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC:AMSTB_R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToCGwithoutSYSC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_Task:AMSTB_R_PMA_MNG_Task:inout:::] */
/*    R_PMA_MNG_Task => Stub */
void *AMSTB_R_PMA_MNG_Task(void *pvParameters)
{
    static void *volatile AMIN_return;
    static void *volatile AMOUT_pvParameters;
    AMOUT_pvParameters = pvParameters;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetResetState:AMSTB_R_OSAL_DCFG_PmSetResetState:inout:::] */
/*    R_OSAL_DCFG_PmSetResetState => Stub */
int AMSTB_R_OSAL_DCFG_PmSetResetState(struct st_osal_device_control *handle,int state)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_state;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_state = state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetResetState:AMSTB_R_OSAL_DCFG_PmGetResetState:inout:::] */
/*    R_OSAL_DCFG_PmGetResetState => Stub */
int AMSTB_R_OSAL_DCFG_PmGetResetState(struct st_osal_device_control *handle,int *p_state)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetLowestIntermediatePowerState:AMSTB_R_OSAL_DCFG_PmGetLowestIntermediatePowerState:inout:::] */
/*    R_OSAL_DCFG_PmGetLowestIntermediatePowerState => Stub */
int AMSTB_R_OSAL_DCFG_PmGetLowestIntermediatePowerState(struct st_osal_device_control *handle,int *p_state)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_timeclock.c:R_OSAL_HIF_ClockTimeRoundupByTimeStamp:AMSTB_R_OSAL_HIF_ClockTimeRoundupByTimeStamp:inout:::] */
/*    R_OSAL_HIF_ClockTimeRoundupByTimeStamp => Stub */
void AMSTB_R_OSAL_HIF_ClockTimeRoundupByTimeStamp(struct st_osal_time *p_time_stamp_in,struct st_osal_time *p_time_stamp_out)
{
    static struct st_osal_time *volatile AMOUT_p_time_stamp_in;
    static struct st_osal_time *volatile AMOUT_p_time_stamp_out;
    AMOUT_p_time_stamp_in = p_time_stamp_in;
    AMOUT_p_time_stamp_out = p_time_stamp_out;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondDestroy:AMSTB_R_OSAL_RCFG_CondDestroy:inout:::] */
/*    R_OSAL_RCFG_CondDestroy => Stub */
int AMSTB_R_OSAL_RCFG_CondDestroy(struct st_osal_cond_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrMap:AMSTB_R_OSAL_OS_MmngrMap:inout:::] */
/*    R_OSAL_OS_MmngrMap => Stub */
int AMSTB_R_OSAL_OS_MmngrMap(struct st_osal_mem_mmngr_ptr_self *mngr_control,struct st_osal_mmngr_config *p_usr_config,unsigned long long region_idx)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_mngr_control;
    static struct st_osal_mmngr_config *volatile AMOUT_p_usr_config;
    static unsigned long long volatile AMOUT_region_idx;
    AMOUT_mngr_control = mngr_control;
    AMOUT_p_usr_config = p_usr_config;
    AMOUT_region_idx = region_idx;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptUnregisterIsr:AMSTB_R_OSAL_OS_InterruptUnregisterIsr:inout:::] */
/*    R_OSAL_OS_InterruptUnregisterIsr => Stub */
int AMSTB_R_OSAL_OS_InterruptUnregisterIsr(struct st_osal_device_control *handle,unsigned long long irq_channel,void *irqHandlerFunction)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static void *volatile AMOUT_irqHandlerFunction;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptRegisterIsr:AMSTB_R_OSAL_OS_InterruptRegisterIsr:inout:::] */
/*    R_OSAL_OS_InterruptRegisterIsr => Stub */
int AMSTB_R_OSAL_OS_InterruptRegisterIsr(struct st_osal_device_control *handle,unsigned long long irq_channel,int priority_value,void *irqHandlerFunction,void *p_irq_arg)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static int volatile AMOUT_priority_value;
    static void *volatile AMOUT_irqHandlerFunction;
    static void *volatile AMOUT_p_irq_arg;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    AMOUT_p_irq_arg = p_irq_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadEqual:AMSTB_R_OSAL_OS_ThreadEqual:inout:::] */
/*    R_OSAL_OS_ThreadEqual => Stub */
int AMSTB_R_OSAL_OS_ThreadEqual(struct st_osal_os_thread *thread1,struct st_osal_os_thread *thread2,unsigned char *p_result)
{
    static int volatile AMIN_return;
    static unsigned char volatile AMIN_result;
    static struct st_osal_os_thread *volatile AMOUT_thread1;
    static struct st_osal_os_thread *volatile AMOUT_thread2;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_thread1 = thread1;
    AMOUT_thread2 = thread2;
    AMOUT_p_result = p_result;
    *p_result = AMIN_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_init_device_information:AMSTB_r_osal_dcfg_io_init_device_information:inout:::] */
/*    r_osal_dcfg_io_init_device_information => Stub */
int AMSTB_r_osal_dcfg_io_init_device_information(struct st_osal_os_mutexattr *mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_mutex_attr = mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_control_init:AMSTB_r_osal_rcfg_mutex_control_init:inout:::] */
/*    r_osal_rcfg_mutex_control_init => Stub */
int AMSTB_r_osal_rcfg_mutex_control_init()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_deinit_attr:AMSTB_r_osal_rcfg_cond_deinit_attr:inout:::] */
/*    r_osal_rcfg_cond_deinit_attr => Stub */
int AMSTB_r_osal_rcfg_cond_deinit_attr(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_condattr *cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    AMOUT_mutex_attr = mutex_attr;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_check_busy:AMSTB_r_osal_rcfg_cond_check_busy:inout:::] */
/*    r_osal_rcfg_cond_check_busy => Stub */
int AMSTB_r_osal_rcfg_cond_check_busy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_gethwaddr:AMSTB_r_osal_rcfg_mmngr_gethwaddr:inout:::] */
/*    r_osal_rcfg_mmngr_gethwaddr => Stub */
int AMSTB_r_osal_rcfg_mmngr_gethwaddr(void *p_ptr_self,unsigned long long axi_id,unsigned long long *p_hw_addr)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static unsigned long long volatile AMOUT_axi_id;
    static unsigned long long *volatile AMOUT_p_hw_addr;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_axi_id = axi_id;
    AMOUT_p_hw_addr = p_hw_addr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_find_free_buffer_info:AMSTB_r_osal_rcfg_mmngr_find_free_buffer_info:inout:::] */
/*    r_osal_rcfg_mmngr_find_free_buffer_info => Stub */
int AMSTB_r_osal_rcfg_mmngr_find_free_buffer_info(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mem_buf_ptr_self **pp_buffer_info)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_mem_buf_ptr_self **volatile AMOUT_pp_buffer_info;
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_fill_guard_area:AMSTB_r_osal_rcfg_mmngr_alloc_fill_guard_area:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_fill_guard_area => Stub */
int AMSTB_r_osal_rcfg_mmngr_alloc_fill_guard_area(int mode,struct st_osal_mem_buf_ptr_self *p_buffer_info)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_mode;
    static struct st_osal_mem_buf_ptr_self *volatile AMOUT_p_buffer_info;
    AMOUT_mode = mode;
    AMOUT_p_buffer_info = p_buffer_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_find_control_info:AMSTB_r_osal_os_interrupt_find_control_info:inout:::] */
/*    r_osal_os_interrupt_find_control_info => Stub */
int AMSTB_r_osal_os_interrupt_find_control_info(struct st_osal_device_control *p_device_control,unsigned long long channel_count,struct _sync_attr *p_mutex_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_p_device_control;
    static unsigned long long volatile AMOUT_channel_count;
    static struct _sync_attr *volatile AMOUT_p_mutex_attr;
    AMOUT_p_device_control = p_device_control;
    AMOUT_channel_count = channel_count;
    AMOUT_p_mutex_attr = p_mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:r_osal_os_thread_init_attr:AMSTB_r_osal_os_thread_init_attr:inout:::] */
/*    r_osal_os_thread_init_attr => Stub */
int AMSTB_r_osal_os_thread_init_attr(int priority,unsigned long long stack_size,struct st_osal_os_threadattr *p_local_attr)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_priority;
    static unsigned long long volatile AMOUT_stack_size;
    static struct st_osal_os_threadattr *volatile AMOUT_p_local_attr;
    AMOUT_priority = priority;
    AMOUT_stack_size = stack_size;
    AMOUT_p_local_attr = p_local_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetLowestPowerState:AMSTB_R_PMA_GetLowestPowerState:inout:::] */
/*    R_PMA_GetLowestPowerState => Stub */
e_pma_return_t AMSTB_R_PMA_GetLowestPowerState(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_power_state_t *pLowestPower)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    static e_pma_power_state_t *volatile AMOUT_pLowestPower;
    static e_pma_power_state_t volatile AMIN_LowestPower;

    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    AMOUT_pLowestPower = pLowestPower;
    
    *pLowestPower = AMIN_LowestPower;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetState:AMSTB_R_OSAL_DCFG_PmGetState:inout:::] */
/*    R_OSAL_DCFG_PmGetState => Stub */
int AMSTB_R_OSAL_DCFG_PmGetState(struct st_osal_device_control *handle,int *p_state)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondDestroy:AMSTB_R_OSAL_OS_CondDestroy:inout:::] */
/*    R_OSAL_OS_CondDestroy => Stub */
int AMSTB_R_OSAL_OS_CondDestroy(struct st_osal_os_cond *cond)
{
#define AMCALLMAX_R_OSAL_OS_CondDestroy 30
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_CondDestroy];
    static struct st_osal_os_cond *volatile AMOUT_cond;
    static int volatile AM_count;

    AMOUT_cond = cond;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexTimedLock:AMSTB_R_OSAL_OS_MutexTimedLock:inout:::] */
/*    R_OSAL_OS_MutexTimedLock => Stub */
int AMSTB_R_OSAL_OS_MutexTimedLock(struct st_osal_os_mutex *mutex,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutex *volatile AMOUT_mutex;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptStaticInit:AMSTB_R_OSAL_OS_InterruptStaticInit:inout:::] */
/*    R_OSAL_OS_InterruptStaticInit => Stub */
int AMSTB_R_OSAL_OS_InterruptStaticInit(struct st_osal_device_control *device_control_table,unsigned long long device_table_num)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_device_control *volatile AMOUT_device_control_table;
    static unsigned long long volatile AMOUT_device_table_num;
    AMOUT_g_context_key = g_context_key;
    AMOUT_device_control_table = device_control_table;
    AMOUT_device_table_num = device_table_num;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_register_axibus_info:AMSTB_r_osal_dcfg_io_register_axibus_info:inout:::] */
/*    r_osal_dcfg_io_register_axibus_info => Stub */
int AMSTB_r_osal_dcfg_io_register_axibus_info(struct st_osal_device_control *p_device_control)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_p_device_control;
    AMOUT_p_device_control = p_device_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_init_attr:AMSTB_r_osal_rcfg_cond_init_attr:inout:::] */
/*    r_osal_rcfg_cond_init_attr => Stub */
int AMSTB_r_osal_rcfg_cond_init_attr(struct st_osal_os_mutexattr **p_mutex_attr,struct st_osal_os_condattr **p_cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr **volatile AMOUT_p_mutex_attr;
    static struct st_osal_os_condattr **volatile AMOUT_p_cond_attr;
    AMOUT_p_mutex_attr = p_mutex_attr;
    AMOUT_p_cond_attr = p_cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_checkbuffer_proc:AMSTB_r_osal_rcfg_mmngr_checkbuffer_proc:inout:::] */
/*    r_osal_rcfg_mmngr_checkbuffer_proc => Stub */
int AMSTB_r_osal_rcfg_mmngr_checkbuffer_proc(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_memory_buffer_obj *buffer_obj,int *p_check_ret)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    static int *volatile AMOUT_p_check_ret;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_buffer_obj = buffer_obj;
    AMOUT_p_check_ret = p_check_ret;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_snd_not_isr:AMSTB_r_osal_rcfg_mq_snd_not_isr:inout:::] */
/*    r_osal_rcfg_mq_snd_not_isr => Stub */
int AMSTB_r_osal_rcfg_mq_snd_not_isr(struct st_osal_mq_control *handle,struct st_osal_time *p_time_stamp,void *p_buffer,unsigned long long buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    static void *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_deinit_ctrl_table:AMSTB_r_osal_rcfg_mq_deinit_ctrl_table:inout:::] */
/*    r_osal_rcfg_mq_deinit_ctrl_table => Stub */
int AMSTB_r_osal_rcfg_mq_deinit_ctrl_table()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_init_table:AMSTB_r_osal_rcfg_thread_init_table:inout:::] */
/*    r_osal_rcfg_thread_init_table => Stub */
int AMSTB_r_osal_rcfg_thread_init_table(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_condattr *cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    AMOUT_mutex_attr = mutex_attr;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_deinit_attr:AMSTB_r_osal_rcfg_thread_deinit_attr:inout:::] */
/*    r_osal_rcfg_thread_deinit_attr => Stub */
int AMSTB_r_osal_rcfg_thread_deinit_attr(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_condattr *cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    AMOUT_mutex_attr = mutex_attr;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_Quit:AMSTB_R_PMA_Quit:inout:::] */
/*    R_PMA_Quit => Stub */
e_pma_return_t AMSTB_R_PMA_Quit(st_pma_handle_t handle)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_if.c:R_PMA_GetLockHwa:AMSTB_R_PMA_GetLockHwa:inout:::] */
/*    R_PMA_GetLockHwa => Stub */
e_pma_return_t AMSTB_R_PMA_GetLockHwa(st_pma_handle_t handle,e_pma_hwa_id_t hwaId)
{
    static e_pma_return_t volatile AMIN_return;
    static st_pma_handle_t AMOUT_handle;
    static e_pma_hwa_id_t volatile AMOUT_hwaId;
    AMOUT_handle = handle;
    AMOUT_hwaId = hwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGforTypeC:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeC:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGforTypeC => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeC(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGforTypeB:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeB:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGforTypeB => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeB(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToPGforTypeA:AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeA:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToPGforTypeA => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToPGforTypeA(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_NoticeChangeToHPforTypeB:AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforTypeB:inout:::] */
/*    R_PMA_HW_DEP_NoticeChangeToHPforTypeB => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_NoticeChangeToHPforTypeB(e_pma_hwa_id_t refHwaId,e_pma_hwa_id_t targetHwaId)
{
    static e_result_t volatile AMIN_return;
    static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
    static e_pma_hwa_id_t volatile AMOUT_refHwaId;
    static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
    AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
    AMOUT_refHwaId = refHwaId;
    AMOUT_targetHwaId = targetHwaId;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptIsISRContext:AMSTB_R_OSAL_DCFG_InterruptIsISRContext:inout:::] */
/*    R_OSAL_DCFG_InterruptIsISRContext => Stub */
int AMSTB_R_OSAL_DCFG_InterruptIsISRContext(unsigned char *p_is_isr)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_is_isr;
    AMOUT_p_is_isr = p_is_isr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptDisableIsr:AMSTB_R_OSAL_DCFG_InterruptDisableIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptDisableIsr => Stub */
int AMSTB_R_OSAL_DCFG_InterruptDisableIsr(struct st_osal_device_control *handle,unsigned long long irq_channel)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiBusName:AMSTB_R_OSAL_DCFG_IoGetAxiBusName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiBusName => Stub */
int AMSTB_R_OSAL_DCFG_IoGetAxiBusName(unsigned long long axi_id,unsigned char *p_buffer,unsigned long long buffer_size,unsigned long long *p_num_of_byte)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_axi_id;
    static unsigned char *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    static unsigned long long *volatile AMOUT_p_num_of_byte;
    AMOUT_axi_id = axi_id;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoDeviceOpen:AMSTB_R_OSAL_DCFG_IoDeviceOpen:inout:::] */
/*    R_OSAL_DCFG_IoDeviceOpen => Stub */
int AMSTB_R_OSAL_DCFG_IoDeviceOpen(unsigned char *p_id,struct st_osal_device_control **p_handle)
{
    static int volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_gp_osal_device_control;
    static unsigned long long volatile AMOUT_g_osal_num_of_device_table;
    static unsigned char *volatile AMOUT_p_id;
    static struct st_osal_device_control **volatile AMOUT_p_handle;
    AMOUT_gp_osal_device_control = gp_osal_device_control;
    AMOUT_g_osal_num_of_device_table = g_osal_num_of_device_table;
    AMOUT_p_id = p_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_thread.c:R_OSAL_HIF_ThreadSleepForTimePeriod:AMSTB_R_OSAL_HIF_ThreadSleepForTimePeriod:inout:::] */
/*    R_OSAL_HIF_ThreadSleepForTimePeriod => Stub */
int AMSTB_R_OSAL_HIF_ThreadSleepForTimePeriod(long long time_period)
{
    static int volatile AMIN_return;
    static long long volatile AMOUT_time_period;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_MqSendUntilTimeStamp:AMSTB_R_OSAL_HIF_MqSendUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_MqSendUntilTimeStamp => Stub */
int AMSTB_R_OSAL_HIF_MqSendUntilTimeStamp(st_osal_os_mq_t * mq, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mq_t * volatile AMOUT_mq;
    static st_osal_time_t * volatile AMOUT_p_time_stamp;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;

    AMOUT_mq           = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer     = p_buffer;
    AMOUT_buffer_size  = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_MqSendForTimePeriod:AMSTB_R_OSAL_HIF_MqSendForTimePeriod:inout:::] */
/*    R_OSAL_HIF_MqSendForTimePeriod => Stub */
int AMSTB_R_OSAL_HIF_MqSendForTimePeriod(st_osal_os_mq_t * mq, osal_milli_sec_t time_period, const void* p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mq_t * volatile AMOUT_mq;
    static osal_milli_sec_t volatile AMOUT_time_period;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;

    AMOUT_mq           = mq;
    AMOUT_time_period  = time_period;
    AMOUT_p_buffer     = p_buffer;
    AMOUT_buffer_size  = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_MqReceiveUntilTimeStamp:AMSTB_R_OSAL_HIF_MqReceiveUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_MqReceiveUntilTimeStamp => Stub */
int AMSTB_R_OSAL_HIF_MqReceiveUntilTimeStamp(st_osal_os_mq_t * mq, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mq_t * volatile AMOUT_mq;
    static st_osal_time_t * volatile AMOUT_p_time_stamp;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;

    AMOUT_mq           = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer     = p_buffer;
    AMOUT_buffer_size  = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_MqReceiveForTimePeriod:AMSTB_R_OSAL_HIF_MqReceiveForTimePeriod:inout:::] */
/*    R_OSAL_HIF_MqReceiveForTimePeriod => Stub */
int AMSTB_R_OSAL_HIF_MqReceiveForTimePeriod(st_osal_os_mq_t * mq, osal_milli_sec_t time_period, const void* p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mq_t * volatile AMOUT_mq;
    static osal_milli_sec_t volatile AMOUT_time_period;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;

    AMOUT_mq           = mq;
    AMOUT_time_period  = time_period;
    AMOUT_p_buffer     = p_buffer;
    AMOUT_buffer_size  = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_common.c:R_OSAL_HIF_MqReceiveForTimePeriod:AMSTB_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod:inout:csv_local:array:counter<AM_count>] */
/*    R_PMA_MNG_Task -- R_OSAL_HIF_MqReceiveForTimePeriod => Stub */
int AMSTB_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod(st_osal_os_mq_t *mq,long long time_period, const void *p_buffer, size_t buffer_size)
{
#define AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod 2
    static int volatile AMIN_return[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod];
    static struct st_osal_mq_control *volatile AMOUT_mq[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod];
    static long long volatile AMOUT_time_period[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod];
    static void *volatile AMOUT_p_buffer[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod];
    static unsigned long long volatile AMOUT_buffer_size[AMCALLMAX_3R_PMA_MNG_Task_R_OSAL_HIF_MqReceiveForTimePeriod];
    static int volatile AM_count;
    AMOUT_mq[AM_count] = mq;
    AMOUT_time_period[AM_count] = time_period;
    AMOUT_p_buffer[AM_count] = p_buffer;
    AMOUT_buffer_size[AM_count] = buffer_size;

    st_msg_to_task_t * pMsgTotask = p_buffer;
    pMsgTotask->enProcId = dummy_MsgToTask[AM_count].enProcId;
    pMsgTotask->enHwaId = dummy_MsgToTask[AM_count].enHwaId;
    AM_count++;

    return AMIN_return[AM_count - 1];
}
/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexTryLock:AMSTB_R_OSAL_RCFG_MutexTryLock:inout:::] */
/*    R_OSAL_RCFG_MutexTryLock => Stub */
int AMSTB_R_OSAL_RCFG_MutexTryLock(struct st_osal_mutex_control *handle)
{
    static int volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MemoryBarrier:AMSTB_R_OSAL_RCFG_MemoryBarrier:inout:::] */
/*    R_OSAL_RCFG_MemoryBarrier => Stub */
int AMSTB_R_OSAL_RCFG_MemoryBarrier()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondWaitUntilTimeStamp:AMSTB_R_OSAL_RCFG_CondWaitUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_CondWaitUntilTimeStamp => Stub */
int AMSTB_R_OSAL_RCFG_CondWaitUntilTimeStamp(struct st_osal_cond_control *cond_handle,struct st_osal_mutex_control *mutex_handle,struct st_osal_time *p_time_stamp)
{
    static int volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_cond_handle;
    static struct st_osal_mutex_control *volatile AMOUT_mutex_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_cond_handle = cond_handle;
    AMOUT_mutex_handle = mutex_handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeSetInitStatus:AMSTB_R_OSAL_RCFG_ClockTimeSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ClockTimeSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_ClockTimeSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondDeinitAttr:AMSTB_R_OSAL_OS_CondDeinitAttr:inout:::] */
/*    R_OSAL_OS_CondDeinitAttr => Stub */
int AMSTB_R_OSAL_OS_CondDeinitAttr(struct st_osal_os_condattr *cond_attr)
{
    static int volatile AMIN_return;
    static struct st_osal_os_condattr *volatile AMOUT_cond_attr;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexCreate:AMSTB_R_OSAL_OS_MutexCreate:inout:::] */
/*    R_OSAL_OS_MutexCreate => Stub */
int AMSTB_R_OSAL_OS_MutexCreate(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_mutex **p_mutex)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_mutex **volatile AMOUT_p_mutex;
    AMOUT_mutex_attr = mutex_attr;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Malloc:AMSTB_R_OSAL_OS_Malloc:inout:::] */
/*    R_OSAL_OS_Malloc => Stub */
void *AMSTB_R_OSAL_OS_Malloc(unsigned long long size)
{
#define AMCALLMAX_R_OSAL_OS_Malloc 30
    static void *volatile AMIN_return[AMCALLMAX_R_OSAL_OS_Malloc];
    static unsigned long long volatile AMOUT_size;
    static int volatile AM_count;

    AMOUT_size = size;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_GetWrapperVersion:AMSTB_R_OSAL_OS_GetWrapperVersion:inout:::] */
/*    R_OSAL_OS_GetWrapperVersion => Stub */
struct st_osal_sub_version *AMSTB_R_OSAL_OS_GetWrapperVersion()
{
    static struct st_osal_sub_version *volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadWaitFinish:AMSTB_R_OSAL_OS_ThreadWaitFinish:inout:::] */
/*    R_OSAL_OS_ThreadWaitFinish => Stub */
int AMSTB_R_OSAL_OS_ThreadWaitFinish(struct st_osal_os_thread *thread)
{
    static int volatile AMIN_return;
    static struct st_osal_os_thread *volatile AMOUT_thread;
    AMOUT_thread = thread;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqCreate:AMSTB_R_OSAL_OS_MqCreate:inout:::] */
/*    R_OSAL_OS_MqCreate => Stub */
int AMSTB_R_OSAL_OS_MqCreate(const st_osal_mq_config_t* const p_config, const osal_os_mqattr_t mq_attr, st_osal_os_mq_t ** p_mq)
{
    static int volatile AMIN_return;
    static st_osal_mq_config_t *volatile AMOUT_p_config;
    static osal_os_mqattr_t volatile AMOUT_mq_attr;
    static st_osal_os_mq_t ** volatile AMOUT_p_mq;
    
    AMOUT_p_config = p_config;
    AMOUT_mq_attr = mq_attr;
    AMOUT_p_mq = p_mq;
    
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqDelete:AMSTB_R_OSAL_OS_MqDelete:inout:::] */
/*    R_OSAL_OS_MqDelete => Stub */
int AMSTB_R_OSAL_OS_MqDelete(st_osal_os_mq_t * mq)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    
    AMOUT_mq = mq;
    
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqInitAttr:AMSTB_R_OSAL_OS_MqInitAttr:inout:::] */
/*    R_OSAL_OS_MqInitAttr => Stub */
int AMSTB_R_OSAL_OS_MqInitAttr(osal_os_mqattr_t * p_mq_attr)
{
    static int volatile AMIN_return;
    static osal_os_mqattr_t volatile AMIN_mq_attr;
    static osal_os_mqattr_t *volatile AMOUT_p_mq_attr;

    *p_mq_attr = AMIN_mq_attr;
    AMOUT_p_mq_attr = p_mq_attr;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqDeinitAttr:AMSTB_R_OSAL_OS_MqDeinitAttr:inout:::] */
/*    R_OSAL_OS_MqDeinitAttr => Stub */
int AMSTB_R_OSAL_OS_MqDeinitAttr(osal_os_mqattr_t mq_attr)
{
    static int volatile AMIN_return;
    static osal_os_mqattr_t volatile AMOUT_mq_attr;

    AMOUT_mq_attr = mq_attr;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqSend:AMSTB_R_OSAL_OS_MqSend:inout:::] */
/*    R_OSAL_OS_MqSend => Stub */
int AMSTB_R_OSAL_OS_MqSend(st_osal_os_mq_t * mq, const void * p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;

    AMOUT_mq          = mq;
    AMOUT_p_buffer    = p_buffer;
    AMOUT_buffer_size = buffer_size;
    
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqTimedSend:AMSTB_R_OSAL_OS_MqTimedSend:inout:::] */
/*    R_OSAL_OS_MqTimedSend => Stub */
int AMSTB_R_OSAL_OS_MqTimedSend(st_osal_os_mq_t * mq, const st_osal_time_t* const p_time_stamp, const void * p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    static st_osal_time_t * volatile AMOUT_p_time_stamp;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;

    AMOUT_mq           = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer     = p_buffer;
    AMOUT_buffer_size  = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqReceive:AMSTB_R_OSAL_OS_MqReceive:inout:::] */
/*    R_OSAL_OS_MqReceive => Stub */
int AMSTB_R_OSAL_OS_MqReceive(st_osal_os_mq_t * mq, const void *p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;
    
    AMOUT_mq          = mq;
    AMOUT_p_buffer    = p_buffer;
    AMOUT_buffer_size = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqTimedReceive:AMSTB_R_OSAL_OS_MqTimedReceive:inout:::] */
/*    R_OSAL_OS_MqTimedReceive => Stub */
int AMSTB_R_OSAL_OS_MqTimedReceive(st_osal_os_mq_t * mq, const st_osal_time_t* const p_time_stamp, const void *p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    static st_osal_time_t * volatile AMOUT_p_time_stamp;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;
    
    AMOUT_mq           = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer     = p_buffer;
    AMOUT_buffer_size  = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqIsEmpty:AMSTB_R_OSAL_OS_MqIsEmpty:inout:::] */
/*    R_OSAL_OS_MqIsEmpty => Stub */
int AMSTB_R_OSAL_OS_MqIsEmpty(st_osal_os_mq_t * mq, bool* const p_result)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    static unsigned char * volatile AMOUT_p_result;
    
    AMOUT_mq       = mq;
    AMOUT_p_result = p_result;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqIsFull:AMSTB_R_OSAL_OS_MqIsFull:inout:::] */
/*    R_OSAL_OS_MqIsFull => Stub */
int AMSTB_R_OSAL_OS_MqIsFull(st_osal_os_mq_t * mq, bool* const p_result)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t * volatile AMOUT_mq;
    static unsigned char * volatile AMOUT_p_result;
    
    AMOUT_mq       = mq;
    AMOUT_p_result = p_result;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqReset:AMSTB_R_OSAL_OS_MqReset:inout:::] */
/*    R_OSAL_OS_MqReset => Stub */
int AMSTB_R_OSAL_OS_MqReset(st_osal_os_mq_t * mq)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mq_t * volatile AMOUT_mq;
    AMOUT_mq = mq;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqCheckBusy:AMSTB_R_OSAL_OS_MqCheckBusy:inout:::] */
/*    R_OSAL_OS_MqCheckBusy => Stub */
int AMSTB_R_OSAL_OS_MqCheckBusy(st_osal_os_mq_t * mq)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mq_t *volatile AMOUT_mq;
    AMOUT_mq = mq;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_convert_wait_state:AMSTB_r_osal_dcfg_pm_convert_wait_state:inout:::] */
/*    r_osal_dcfg_pm_convert_wait_state => Stub */
int AMSTB_r_osal_dcfg_pm_convert_wait_state(int osal_pm_state,st_pma_hwa_state_t *p_pma_hwa_state,e_pma_reset_req_t *p_pma_reset_req)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_osal_pm_state;
    static st_pma_hwa_state_t *volatile AMOUT_p_pma_hwa_state;
    static e_pma_reset_req_t *volatile AMOUT_p_pma_reset_req;
    AMOUT_osal_pm_state = osal_pm_state;
    AMOUT_p_pma_hwa_state = p_pma_hwa_state;
    AMOUT_p_pma_reset_req = p_pma_reset_req;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_getconstcpuptr:AMSTB_r_osal_rcfg_mmngr_getconstcpuptr:inout:::] */
/*    r_osal_rcfg_mmngr_getconstcpuptr => Stub */
int AMSTB_r_osal_rcfg_mmngr_getconstcpuptr(void *p_ptr_self,void **pp_const_cpu_ptr)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static void **volatile AMOUT_pp_const_cpu_ptr;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_pp_const_cpu_ptr = pp_const_cpu_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_getconfig:AMSTB_r_osal_rcfg_mmngr_getconfig:inout:::] */
/*    r_osal_rcfg_mmngr_getconfig => Stub */
int AMSTB_r_osal_rcfg_mmngr_getconfig(void *p_ptr_self,struct st_osal_mmngr_config *p_config)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_pre_check:AMSTB_r_osal_rcfg_mmngr_alloc_pre_check:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_pre_check => Stub */
int AMSTB_r_osal_rcfg_mmngr_alloc_pre_check(void *p_ptr_self,unsigned long long bytes,unsigned long long alignment)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    static unsigned long long volatile AMOUT_bytes;
    static unsigned long long volatile AMOUT_alignment;
    AMOUT_p_ptr_self = p_ptr_self;
    AMOUT_bytes = bytes;
    AMOUT_alignment = alignment;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_timedlock_mutex:AMSTB_r_osal_os_interrupt_timedlock_mutex:inout:::] */
/*    r_osal_os_interrupt_timedlock_mutex => Stub */
int AMSTB_r_osal_os_interrupt_timedlock_mutex(struct st_osal_interrupt_thread_control *p_info)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_info;
    AMOUT_g_context_key = g_context_key;
    AMOUT_p_info = p_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_timedlock_mutex:AMSTB_r_osal_os_interrupt_timedlock_mutex_multiple:inout::array:counter<AM_count>] */
/*    r_osal_os_interrupt_timedlock_mutex => Stub */
int AMSTB_r_osal_os_interrupt_timedlock_mutex_multiple(struct st_osal_interrupt_thread_control *p_info)
{
#define AMCALLMAX_r_osal_os_interrupt_timedlock_mutex 600
    static int volatile AMIN_return[AMCALLMAX_r_osal_os_interrupt_timedlock_mutex];
    static int volatile AMOUT_g_context_key;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_info;
    static int volatile AM_count;
    AMOUT_g_context_key = g_context_key;
    AMOUT_p_info = p_info;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_call_handler:AMSTB_r_osal_os_interrupt_call_handler:inout:::] */
/*    r_osal_os_interrupt_call_handler => Stub */
int AMSTB_r_osal_os_interrupt_call_handler(struct st_osal_interrupt_thread_control *p_interrupt_control)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_g_context_key;
    static struct st_osal_interrupt_thread_control *volatile AMOUT_p_interrupt_control;
    AMOUT_g_context_key = g_context_key;
    AMOUT_p_interrupt_control = p_interrupt_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_send:AMSTB_r_osal_os_mq_send:inout:::] */
/*    r_osal_os_mq_send => Stub */
int AMSTB_r_osal_os_mq_send(osal_os_mq_t mq, const void *p_buffer, size_t buffer_size)
{
    static int volatile          AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;
    
    AMOUT_mq          = mq;
    AMOUT_p_buffer    = p_buffer;
    AMOUT_buffer_size = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_receive:AMSTB_r_osal_os_mq_receive:inout:::] */
/*    r_osal_os_mq_receive => Stub */
int AMSTB_r_osal_os_mq_receive(osal_os_mq_t mq, const void *p_buffer, size_t buffer_size)
{
    static int volatile          AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static const void volatile * AMOUT_p_buffer;
    static size_t volatile       AMOUT_buffer_size;
    
    AMOUT_mq          = mq;
    AMOUT_p_buffer    = p_buffer;
    AMOUT_buffer_size = buffer_size;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_timedlock_mutex:AMSTB_r_osal_os_mq_timedlock_mutex:inout:::] */
/*    r_osal_os_mq_timedlock_mutex => Stub */
int AMSTB_r_osal_os_mq_timedlock_mutex(pthread_mutex_t *p_mutex, int64_t timeout)
{
    static int volatile              AMIN_return;
    static pthread_mutex_t *volatile AMOUT_p_mutex;
    static int64_t volatile          AMOUT_timeout;
    
    AMOUT_p_mutex     = p_mutex;
    AMOUT_timeout     = timeout;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_destroy_cond:AMSTB_r_osal_os_mq_destroy_cond:inout:::] */
/*    r_osal_os_mq_destroy_cond => Stub */
int AMSTB_r_osal_os_mq_destroy_cond(st_osal_os_mq_t *mq)
{
    static int volatile          AMIN_return;
    static st_osal_os_mq_t *volatile AMOUT_mq;
    
    AMOUT_mq = mq;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_free_queue_buff:AMSTB_r_osal_os_mq_free_queue_buff:inout:::] */
/*    r_osal_os_mq_free_queue_buff => Stub */
void AMSTB_r_osal_os_mq_free_queue_buff(const st_osal_mq_config_t* const p_config, st_osal_os_mq_t *mq)
{
    static int volatile                  AMIN_return;
    static st_osal_mq_config_t *volatile AMOUT_p_config;
    static st_osal_os_mq_t *volatile     AMOUT_mq;
    
    AMOUT_p_config = p_config;
    AMOUT_mq       = mq;

    return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptCheckBusy:AMSTB_R_OSAL_DCFG_InterruptCheckBusy:inout:::] */
/*    R_OSAL_DCFG_InterruptCheckBusy => Stub */
int AMSTB_R_OSAL_DCFG_InterruptCheckBusy()
{
    static int volatile AMIN_return;
    return AMIN_return;
}

void AMSTB_osal_internal_interrupt_execute_userfunction_irq_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    static osal_device_handle_t volatile AMOUT_device_handle;
    static uint64_t volatile AMOUT_irq_channel;
    static void* volatile AMOUT_irq_arg;
    return;
}

/* use R_OSAL_DCFG_InterruptRegisterIsr, R_OSAL_DCFG_InterruptUnregisterIsr */
int AMSTB_r_osal_dcfg_io_add_utlb_list(const char* p_domain_name, const char *p_num_str, size_t list_size,
                                        osal_axi_bus_id_t* p_list_buffer, st_osal_utlb_list_control_t *p_utlb_control)
{
    static int volatile AMIN_return;
    static volatile size_t AMIN_match_count;
    static volatile osal_axi_bus_id_t* AMOUT_p_list_buffer;
    static volatile st_osal_utlb_list_control_t* AMOUT_p_utlb_control;

    AMOUT_p_list_buffer  = p_list_buffer;
    AMOUT_p_utlb_control = p_utlb_control;

    p_utlb_control->match_count = AMIN_match_count;

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_sort_axi_bus_list:AMSTB_r_osal_dcfg_io_sort_axi_bus_list:inout:::] */
/*    r_osal_dcfg_io_sort_axi_bus_list => Stub */
void AMSTB_r_osal_dcfg_io_sort_axi_bus_list(size_t array_num, st_osal_axi_bus_list_t *p_axi_bus_list)
{
    static size_t volatile AMOUT_array_num;
    static st_osal_axi_bus_list_t* volatile AMOUT_p_axi_bus_list;
    AMOUT_array_num = array_num;
    AMOUT_p_axi_bus_list = p_axi_bus_list;
    return ;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_sort_axibus_name_info:AMSTB_r_osal_dcfg_io_sort_axibus_name_info:inout:::] */
/*    r_osal_dcfg_io_sort_axibus_name_info => Stub */
void AMSTB_r_osal_dcfg_io_sort_axibus_name_info(size_t array_num, st_osal_axi_bus_name_info_t *p_axi_bus_name_info)
{
    static size_t volatile AMOUT_array_num;
    static st_osal_axi_bus_name_info_t* volatile AMOUT_p_axi_bus_name_info;
    AMOUT_array_num = array_num;
    AMOUT_p_axi_bus_name_info = p_axi_bus_name_info;
    return ;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_search_strings:AMSTB_r_osal_dcfg_io_search_strings:inout:::] */
/*    r_osal_dcfg_io_search_strings => Stub */
char * AMSTB_r_osal_dcfg_io_search_strings(const char * p_src_string, const char * p_search_string)
{
#define AMCALLMAX_r_osal_dcfg_io_search_strings 30
    static char *volatile AMIN_return[AMCALLMAX_r_osal_dcfg_io_search_strings];
    static const char *volatile AMOUT_p_src_string;
    static const char *volatile AMOUT_p_search_string;
    AMOUT_p_src_string = p_src_string;
    AMOUT_p_search_string = p_search_string;
    static int volatile AM_count;

    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:strncmp:AMSTB_strncmp:inout:::] */
/*    strncmp => Stub */
int AMSTB_strncmp(const char *s1, const char *s2, unsigned long n)
{
#define AMCALLMAX_strncmp 300
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

/* WINAMS_STUB[:snprintf:AMSTB_snprintf:inout:::] */
int AMSTB_snprintf()
{
#define AMCALLMAX_snprintf 30
    static int volatile AMIN_return[AMCALLMAX_snprintf];
    static int volatile AM_count;
    
    AM_count++;
    return AMIN_return[AM_count -1];
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

/* WINAMS_STUB[stub.c:bsearch:AMSTB_bsearch:inout:::] */
/*    bsearch => Stub */
void *AMSTB_bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *pkey, const void *pbase))
{
    static void *volatile AMIN_return;
    static void *volatile AMOUT_key;
    static void *volatile AMOUT_base;
    static size_t volatile AMOUT_nmemb;
    static size_t volatile AMOUT_size;
    AMOUT_key = key;
    AMOUT_base = base;
    AMOUT_nmemb = nmemb;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:calloc:AMSTB_calloc:inout::array:counter<AM_count>] */
/*    calloc => Stub */
void *AMSTB_calloc(unsigned long n, unsigned long size)
{
#define AMCALLMAX_calloc 30
    static void *volatile AMIN_return[AMCALLMAX_calloc];
    static unsigned long volatile AMOUT_n[AMCALLMAX_calloc];
    static unsigned long volatile AMOUT_size[AMCALLMAX_calloc];
    static int volatile AM_count;
    AMOUT_n[AM_count] = n;
    AMOUT_size[AM_count] = size;
    AM_count++;
    return AMIN_return[AM_count - 1];
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

/* WINAMS_STUB[stub.c:munmap:AMSTB_munmap:inout:::] */
/*    munmap => Stub */
int AMSTB_munmap(void *addr, unsigned long long length)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_addr;
    static unsigned long long volatile AMOUT_length;
    AMOUT_addr = addr;
    AMOUT_length = length;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:strnlen:AMSTB_strnlen:inout:::] */
/*    strnlen => Stub */
size_t AMSTB_strnlen(const char *s, size_t maxlen)
{
    static size_t volatile AMIN_return;
    static char *volatile AMOUT_s;
    static size_t volatile AMOUT_maxlen;
    AMOUT_s = s;
    AMOUT_maxlen = maxlen;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_init:AMSTB_pthread_cond_init:inout:::] */
/*    pthread_cond_init => Stub */
int AMSTB_pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_cond_t *volatile AMOUT_cond;
    static const pthread_condattr_t *volatile AMOUT_attr;

    AMOUT_cond = cond;
    AMOUT_attr = attr;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_init:AMSTB_pthread_cond_init_mqstb:inout:::] */
/*    pthread_cond_init_mqstb => Stub */
int AMSTB_pthread_cond_init_mqstb(pthread_cond_t *cond, const pthread_condattr_t *attr)
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

/* WINAMS_STUB[stub.c:pthread_cond_destroy:AMSTB_pthread_cond_destroy:inout:::] */
/*    pthread_cond_destroy => Stub */
int AMSTB_pthread_cond_destroy(pthread_cond_t *cond)
{
    static int volatile AMIN_return;
    static pthread_cond_t *volatile AMOUT_cond;

    AMOUT_cond = cond;

    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_cond_destroy:AMSTB_pthread_cond_destroy_mqstb:inout:::] */
/*    pthread_cond_destroy_mqstb => Stub */
int AMSTB_pthread_cond_destroy_mqstb(pthread_cond_t *cond)
{
#define AMCALLMAX_pthread_cond_destroy 30
    static int volatile AMIN_return[AMCALLMAX_pthread_cond_destroy];
    static pthread_cond_t *volatile AMOUT_cond;
    static int volatile AM_count;

    AMOUT_cond = cond;

    AM_count++;

    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_cond_timedwait:AMSTB_pthread_cond_timedwait:inout:::] */
/*    pthread_cond_timedwait => Stub */
int AMSTB_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    static int volatile AMIN_return;
    static pthread_cond_t *volatile AMOUT_cond;
    static pthread_mutex_t *volatile AMOUT_mutex;
    static const struct timespec *volatile AMOUT_abstime;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_abstime = abstime;
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
        g_st_osal_os_mq[0].numofmsg = AM_val;
    }

    AM_count++;

    return AMIN_return[AM_count - 1];

}
/* WINAMS_STUB[stub.c:pthread_cond_wait:AMSTB_pthread_cond_wait:inout:::] */
/*    pthread_cond_wait => Stub */
int AMSTB_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static bool volatile AMIN_flag;
    static pthread_cond_t *volatile AMOUT_cond;
    static pthread_mutex_t *volatile AMOUT_mutex;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    *gp_osal_os_interrupt_init_flag = AMIN_flag;
    return AMIN_return;
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

/* WINAMS_STUB[stub.c:pthread_cond_broadcast:AMSTB_pthread_cond_broadcast:inout:::] */
/*    pthread_cond_broadcast => Stub */
int AMSTB_pthread_cond_broadcast(pthread_cond_t *cond)
{
    static int volatile AMIN_return;
    static pthread_cond_t *volatile AMOUT_cond;
    AMOUT_cond = cond;
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

/* WINAMS_STUB[stub.c:pthread_condattr_setclock:AMSTB_pthread_condattr_setclock:inout:::] */
/*    pthread_condattr_setclock => Stub */
int AMSTB_pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t id)
{
    static int volatile AMIN_return;
    static pthread_condattr_t *volatile AMOUT_attr;
    static clockid_t volatile AMOUT_id;
    AMOUT_attr = attr;
    AMOUT_id = id;
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

/* WINAMS_STUB[stub.c:pthread_mutex_init:AMSTB_pthread_mutex_init_multiple:inout:::] */
/*    AMSTB_pthread_mutex_init_multiple => Stub */
int AMSTB_pthread_mutex_init_multiple(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
#define AMCALLMAX_pthread_mutex_init 30
    static int volatile AMIN_return[AMCALLMAX_pthread_mutex_init];
    static pthread_mutex_t *volatile AMOUT_mutex;
    static const pthread_mutexattr_t *volatile AMOUT_attr;
    static int volatile AM_count;

    AMOUT_mutex = mutex;
    AMOUT_attr = attr;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_mutex_destroy:AMSTB_pthread_mutex_destroy:inout:::] */
/*    pthread_mutex_destroy => Stub */
int AMSTB_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_mutex_t *volatile AMOUT_mutex;

    AMOUT_mutex = mutex;

    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_destroy:AMSTB_pthread_mutex_destroy_mqstb:inout:::] */
/*    pthread_mutex_destroy_mqstb => Stub */
int AMSTB_pthread_mutex_destroy_mqstb(pthread_mutex_t *mutex)
{
#define AMCALLMAX_pthread_mutex_destroy 600
    static int volatile AMIN_return[AMCALLMAX_pthread_mutex_destroy];
    static pthread_mutex_t *volatile AMOUT_mutex;
    static int volatile AM_count;

    AMOUT_mutex = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_mutex_destroy:AMSTB_pthread_mutex_destroy_multiple:inout:::] */
/*    pthread_mutex_destroy => Stub */
int AMSTB_pthread_mutex_destroy_multiple(pthread_mutex_t *mutex)
{
#define AMCALLMAX_pthread_mutex_destroy_multiple 600
    static int volatile AMIN_return[AMCALLMAX_pthread_mutex_destroy_multiple];
    static pthread_mutex_t *volatile AMOUT_mutex;
    static int volatile AM_count;

    AMOUT_mutex = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_mutex_timedlock_monotonic:AMSTB_pthread_mutex_timedlock_monotonic:inout:::] */
/*    pthread_mutex_timedlock_monotonic => Stub */
int AMSTB_pthread_mutex_timedlock_monotonic(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
    static int volatile AMIN_return;
    static pthread_mutex_t *volatile AMOUT_mutex;
    static const struct timespec *volatile AMOUT_abs_timeout;
    AMOUT_mutex = mutex;
    AMOUT_abs_timeout = abs_timeout;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_timedlock_monotonic:AMSTB_pthread_mutex_timedlock_monotonic_multiple:inout::array:counter<AM_count>] */
/*    pthread_mutex_timedlock_monotonic => Stub */
int AMSTB_pthread_mutex_timedlock_monotonic_multiple(pthread_mutex_t *mutex, const struct timespec *abs_timeout)
{
#define AMCALLMAX_pthread_mutex_timedlock_monotonic 300
    static int volatile AMIN_return[AMCALLMAX_pthread_mutex_timedlock_monotonic];
    static pthread_mutex_t *volatile AMOUT_mutex;
    static const struct timespec *volatile AMOUT_abs_timeout;
    static int volatile AM_count;
    AMOUT_mutex = mutex;
    AMOUT_abs_timeout = abs_timeout;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:pthread_mutex_trylock:AMSTB_pthread_mutex_trylock:inout:::] */
/*    pthread_mutex_trylock => Stub */
int AMSTB_pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_mutex_t *volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_unlock:AMSTB_pthread_mutex_unlock:inout:::] */
/*    pthread_mutex_unlock => Stub */
int AMSTB_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_mutex_t *volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_mutex_unlock:AMSTB_pthread_mutex_unlock_multiple:inout::array:counter<AM_count>] */
/*    pthread_mutex_unlock => Stub */
int AMSTB_pthread_mutex_unlock_multiple(pthread_mutex_t *mutex)
{
#define AMCALLMAX_pthread_mutex_unlock 300
    static int volatile AMIN_return[AMCALLMAX_pthread_mutex_unlock];
    static pthread_mutex_t *volatile AMOUT_mutex;
    static int volatile AM_count;
    AMOUT_mutex = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
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

/* WINAMS_STUB[stub.c:pthread_mutexattr_destroy:AMSTB_pthread_mutexattr_destroy:inout:::] */
/*    pthread_mutexattr_destroy => Stub */
int AMSTB_pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_mutexattr_t *volatile AMOUT_attr;
    AMOUT_attr = attr;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:clock_gettime:AMSTB_clock_gettime:inout::array:counter<AM_count>] */
/*    clock_gettime => Stub */
int AMSTB_clock_gettime(clockid_t clock_id, struct timespec *tp)
{
#define AMCALLMAX_clock_gettime 600
    static int volatile AMIN_return[AMCALLMAX_clock_gettime];
    static int volatile AMOUT_clock_id;
    static struct timespec *volatile AMOUT_tp;
    static int volatile AM_count;
    AMOUT_clock_id = clock_id;
    AMOUT_tp = tp;
    tp->tv_sec  = g_os_time.tv_sec;
    tp->tv_nsec = g_os_time.tv_nsec;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stub.c:clock_gettime:AMSTB_clock_gettime:inout:::] */
/*    clock_gettime => Stub */
int AMSTB_clock_gettime_multiple(clockid_t clock_id, struct timespec *tp)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_clock_id;
    static struct timespec *volatile AMOUT_tp;
    AMOUT_clock_id = clock_id;
    AMOUT_tp = tp;
    tp->tv_sec  = g_os_time.tv_sec;
    tp->tv_nsec = g_os_time.tv_nsec;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:clock_getres:AMSTB_clock_getres:inout:::] */
/*    clock_getres => Stub */
int AMSTB_clock_getres(clockid_t clockid, struct timespec *res)
{
    static int volatile AMIN_return;
    static clockid_t volatile AMOUT_clockid;
    static struct timespec *volatile AMOUT_res;
    AMOUT_clockid = clockid;
    AMOUT_res = res;
    res->tv_sec  = 0;
    res->tv_nsec = g_os_restime.tv_nsec;
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

/* WINAMS_STUB[stub.c:ClockCycles:AMSTB_ClockCycles:inout:::] */
/*    ClockCycles => Stub */
uint64_t AMSTB_ClockCycles(void)
{
    static uint64_t volatile AMIN_return;
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

/* WINAMS_STUB[stub.c:pthread_equal:AMSTB_pthread_equal:inout:::] */
/*    pthread_equal => Stub */
int AMSTB_pthread_equal(pthread_t t1, pthread_t t2)
{
#define AMCALLMAX_pthread_equal 30
    static int volatile AMIN_return[AMCALLMAX_pthread_equal];
    static pthread_t volatile AMOUT_t1;
    static pthread_t volatile AMOUT_t2;
    static int volatile AM_count;
    AMOUT_t1 = t1;
    AMOUT_t2 = t2;

    AM_count++;
    return AMIN_return[AM_count - 1];
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

/* WINAMS_STUB[stub.c:pthread_attr_init:AMSTB_pthread_attr_init:inout:::] */
/*    pthread_attr_init => Stub */
int AMSTB_pthread_attr_init(pthread_attr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_attr_t *volatile AMOUT_attr;
    AMOUT_attr = attr;
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

/* WINAMS_STUB[stub.c:pthread_attr_destroy:AMSTB_pthread_attr_destroy:inout:::] */
/*    pthread_attr_destroy => Stub */
int AMSTB_pthread_attr_destroy(pthread_attr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_attr_t *volatile AMOUT_attr;
    AMOUT_attr = attr;
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

/* WINAMS_STUB[stub.c:mmap64:AMSTB_mmap64:inout:::] */
/*    mmap64 => Stub */
void *AMSTB_mmap64(void *addr, size_t len, int prot, int flags, int fildes, off_t off)
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

/* WINAMS_STUB[stub.c:mem_offset64:AMSTB_mem_offset64:inout:::] */
/*    mem_offset64 => Stub */
int AMSTB_mem_offset64( const void * addr, int fd, size_t length, off64_t * offset, size_t * contig_len )
{
    static int volatile AMIN_return;
    static const void *volatile AMOUT_addr;
    static int volatile AMOUT_fd;
    static size_t volatile AMOUT_length;
    static off64_t *volatile AMOUT_offset;
    static size_t *volatile AMOUT_contig_len;
    AMOUT_addr = addr;
    AMOUT_fd = fd;
    AMOUT_length = length;
    AMOUT_offset = offset;
    AMOUT_contig_len = contig_len;
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

/* WINAMS_STUB[stub.c:close:AMSTB_close:inout:::] */
/*    close => Stub */
int AMSTB_close(int fildes)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_fildes;
    AMOUT_fildes = fildes;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:msync:AMSTB_msync:inout:::] */
/*    msync => Stub */
int AMSTB_msync(void *addr, size_t len,int flags)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_addr;
    static size_t volatile AMOUT_len;
    static int volatile AMOUT_flags;
    AMOUT_addr = addr;
    AMOUT_len = len;
    AMOUT_flags = flags;
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

/* WINAMS_STUB[stub.c:pthread_key_delete:AMSTB_pthread_key_delete:inout:::] */
/*    pthread_key_delete => Stub */
int AMSTB_pthread_key_delete(pthread_key_t key)
{
    static int volatile AMIN_return;
    static pthread_key_t volatile AMOUT_key;
    AMOUT_key = key;
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

/* WINAMS_STUB[stub.c:InterruptDetach:AMSTB_InterruptDetach:inout:::] */
/*    InterruptDetach => Stub */
int AMSTB_InterruptDetach(int id)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_id;
    AMOUT_id = id;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:InterruptWait_r:AMSTB_InterruptWait_r:inout:::] */
/*    InterruptWait_r => Stub */
int AMSTB_InterruptWait_r(int flags, const uint64_t *timeout)
{
    static int volatile AMIN_return;
    static p_osal_isr_func_t volatile AMIN_irq_handler;
    static bool volatile AMIN_irq_enable;
    static int volatile AMOUT_flags;
    static const uint64_t *volatile AMOUT_timeout;
    AMOUT_flags = flags;
    AMOUT_timeout = timeout;
    g_osal_interrupt_thread_control[0].irq_handler = AMIN_irq_handler;
    g_osal_interrupt_thread_control[0].irq_enable  = AMIN_irq_enable;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:TimerTimeout:AMSTB_TimerTimeout:inout::array:counter<AM_count>] */
/*    TimerTimeout => Stub */
int AMSTB_TimerTimeout(clockid_t id, int flags, const struct sigevent *notify, const uint64_t *ntime, uint64_t *otime)
{
#define AMCALLMAX_TimerTimeout 30
    static int volatile AMIN_return[AMCALLMAX_TimerTimeout];
    static clockid_t volatile AMOUT_id;
    static int volatile AMOUT_flags;
    static const struct sigevent *volatile AMOUT_notify;
    static const uint64_t *volatile AMOUT_ntime;
    static uint64_t *volatile AMOUT_otime;
    static int volatile AM_count;
    AMOUT_id = id;
    AMOUT_flags = flags;
    AMOUT_notify = notify;
    AMOUT_ntime = ntime;
    AMOUT_otime = otime;
    AM_count++;
    return AMIN_return[AM_count - 1];
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

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiBusNameList:AMSTB_R_OSAL_DCFG_IoGetAxiBusNameList:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiBusNameList => Stub */
int AMSTB_R_OSAL_DCFG_IoGetAxiBusNameList(unsigned char *p_buffer,unsigned long long buffer_size,unsigned long long *p_num_of_byte)
{
    static int volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    static unsigned long long *volatile AMOUT_p_num_of_byte;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_GetResetState:AMSTB_R_PMA_HW_DEP_GetResetState:inout:::] */
/*    R_PMA_HW_DEP_GetResetState => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_GetResetState(e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstStat)
{
	static e_result_t volatile AMIN_return;
	static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_reset_req_t *volatile AMOUT_pRstStat;
	AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
	AMOUT_hwaId = hwaId;
	*pRstStat = AMOUT_pRstStat;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_bsp_wrapper.c:R_PMA_BSP_WRAP_GetResetState:AMSTB_R_PMA_BSP_WRAP_GetResetState:inout:::] */
/*    R_PMA_BSP_WRAP_GetResetState => Stub */
e_result_t AMSTB_R_PMA_BSP_WRAP_GetResetState(e_cpg_domain_id_t modId,e_pma_reset_req_t *pRstStat)
{
	static e_result_t volatile AMIN_return;
	static e_cpg_domain_id_t volatile AMOUT_modId;
	static e_pma_reset_req_t *volatile AMOUT_pRstStat;
	AMOUT_modId = modId;
	*pRstStat = AMOUT_pRstStat;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_SetHwaState:AMSTB_R_PMA_HW_DEP_SetHwaState:inout:::] */
/*    R_PMA_HW_DEP_SetHwaState => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_SetHwaState(e_pma_hwa_id_t hwaId)
{
	static e_result_t volatile AMIN_return;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_SetResetState:AMSTB_R_PMA_HW_DEP_SetResetState:inout:::] */
/*    R_PMA_HW_DEP_SetResetState => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_SetResetState(e_pma_hwa_id_t hwaId)
{
	static e_result_t volatile AMIN_return;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	AMOUT_hwaId = hwaId;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_SetupThreadSync:AMSTB_R_PMA_MNG_SetupThreadSync:inout:::] */
/*    R_PMA_MNG_SetupThreadSync => Stub */
e_result_t AMSTB_R_PMA_MNG_SetupThreadSync(struct st_osal_os_mutex **pSetMutex,struct st_osal_os_cond **pSetCond)
{
	static e_result_t volatile AMIN_return;
	static struct st_osal_os_mutex **volatile AMOUT_pSetMutex;
	static struct st_osal_os_cond **volatile AMOUT_pSetCond;
	AMOUT_pSetMutex = pSetMutex;
	AMOUT_pSetCond = pSetCond;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetTimeDiffAndCondWait:AMSTB_R_PMA_MNG_GetTimeDiffAndCondWait:inout:::counter<AM_count>] */
/*    R_PMA_MNG_GetTimeDiffAndCondWait => Stub */
int AMSTB_R_PMA_MNG_GetTimeDiffAndCondWait(struct st_osal_os_cond *osal_os_cond,struct st_osal_os_mutex *osal_os_mutex,struct st_osal_time *LstStartTime,struct st_osal_time *pDiffTime,long long sleepTimePeriod)
{
#define AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait 2
	static int volatile AMIN_return[AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait];
	static struct st_osal_os_cond *volatile AMOUT_osal_os_cond[AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait];
	static struct st_osal_os_mutex *volatile AMOUT_osal_os_mutex[AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait];
	static struct st_osal_time *volatile AMOUT_LstStartTime[AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait];
	static struct st_osal_time volatile AMOUT_pDiffTime[AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait];
	static long long volatile AMOUT_sleepTimePeriod[AMCALLMAX_R_PMA_IF_GetTimeDiffAndCondWait];
	static int volatile AM_count;
	AMOUT_osal_os_cond[AM_count] = osal_os_cond;
	AMOUT_osal_os_mutex[AM_count] = osal_os_mutex;
	AMOUT_LstStartTime[AM_count] = LstStartTime;
//	AMOUT_pDiffTime = pDiffTime;
    pDiffTime->tv_sec = AMOUT_pDiffTime[AM_count].tv_sec;
    pDiffTime->tv_nsec = AMOUT_pDiffTime[AM_count].tv_nsec;
    *pDiffTime = AMOUT_pDiffTime[AM_count];
	AMOUT_sleepTimePeriod[AM_count] = sleepTimePeriod;
	AM_count++;

	return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSetCpuAffinity:AMSTB_R_OSAL_OS_ThreadSetCpuAffinity:inout:::] */
/*    R_OSAL_OS_ThreadSetCpuAffinity => Stub */
int AMSTB_R_OSAL_OS_ThreadSetCpuAffinity(unsigned long long id)
{
    static int volatile AMIN_return;
    static unsigned long long volatile AMOUT_id;
    AMOUT_id = id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSetTaskName:AMSTB_R_OSAL_OS_ThreadSetTaskName:inout:::] */
/*    R_OSAL_OS_ThreadSetTaskName => Stub */
int AMSTB_R_OSAL_OS_ThreadSetTaskName(char* p_task_name)
{
    static int volatile AMIN_return;
    static char *volatile AMOUT_p_task_name;
    AMOUT_p_task_name = p_task_name;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:ThreadCtl:AMSTB_ThreadCtl:inout:::] */
/*    ThreadCtl => Stub */
int AMSTB_ThreadCtl(int cmd, void * data)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_cmd;
    static void *volatile AMOUT_data;
    AMOUT_cmd = cmd;
    AMOUT_data = data;
    return AMIN_return;
}

/* WINAMS_STUB[stub.c:pthread_setname_np:AMSTB_pthread_setname_np:inout:::] */
/*    pthread_setname_np => Stub */
int AMSTB_pthread_setname_np(pthread_t thr, const char *name)
{
    static int volatile AMIN_return;
    static pthread_t volatile AMOUT_thr;
    static const char *volatile AMOUT_name;
    AMOUT_thr = thr;
    AMOUT_name = name;
    return AMIN_return;
}


/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_IsEnableControlPowerClock:AMSTB_R_PMA_MNG_IsEnableControlPowerClock:inout:::] */
/*    R_PMA_MNG_IsEnableControlPowerClock => Stub */
e_result_t AMSTB_R_PMA_MNG_IsEnableControlPowerClock(e_pma_hwa_id_t hwaId,unsigned char *isEnable)
{
	static e_result_t volatile AMIN_return;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static unsigned char volatile AMOUT_isEnable;
	AMOUT_hwaId = hwaId;
	*isEnable = AMOUT_isEnable;

	return AMIN_return;
}

/* WINAMS_STUB[r_pma_statemachine.c:R_PMA_SM_SetLowestPowerState:AMSTB_R_PMA_SM_SetLowestPowerState:inout:::] */
/*    R_PMA_SM_SetLowestPowerState => Stub */
void AMSTB_R_PMA_SM_SetLowestPowerState(e_pma_hwa_id_t hwaId)
{
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	AMOUT_hwaId = hwaId;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetReset:AMSTB_2R_PMA_ApplyResetHwa_R_PMA_MNG_GetReset:inout:csv_local::] */
/*    R_PMA_ApplyResetHwa -- R_PMA_MNG_GetReset => Stub */
e_result_t AMSTB_2R_PMA_ApplyResetHwa_R_PMA_MNG_GetReset(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstState)
{
	static e_result_t volatile AMIN_return;
	static st_pma_handle_t AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_reset_req_t volatile AMOUT_pRstState;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	*pRstState = AMOUT_pRstState;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_manager.c:R_PMA_MNG_GetReset:AMSTB_2R_PMA_ReleaseResetHwa_R_PMA_MNG_GetReset:inout:csv_local::] */
/*    R_PMA_ReleaseResetHwa -- R_PMA_MNG_GetReset => Stub */
e_result_t AMSTB_2R_PMA_ReleaseResetHwa_R_PMA_MNG_GetReset(st_pma_handle_t handle,e_pma_hwa_id_t hwaId,e_pma_reset_req_t *pRstState)
{
	static e_result_t volatile AMIN_return;
	static st_pma_handle_t AMOUT_handle;
	static e_pma_hwa_id_t volatile AMOUT_hwaId;
	static e_pma_reset_req_t volatile AMOUT_pRstState;
	AMOUT_handle = handle;
	AMOUT_hwaId = hwaId;
	*pRstState = AMOUT_pRstState;
	return AMIN_return;
}

/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_IsEnableControlPowerClock:AMSTB_R_PMA_HW_DEP_IsEnableControlPowerClock:inout:::] */
/*    R_PMA_HW_DEP_IsEnableControlPowerClock => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_IsEnableControlPowerClock(e_pma_hwa_id_t targetHwaId,unsigned char *isEnable)
{
	static e_result_t volatile AMIN_return;
	static e_pma_hwa_id_t volatile AMOUT_g_pma_max_hwa_id_num;
	static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
	static unsigned char volatile * AMOUT_isEnable;
	static unsigned char volatile AMIN_isEnable;
	AMOUT_g_pma_max_hwa_id_num = g_pma_max_hwa_id_num;
	AMOUT_targetHwaId = targetHwaId;
    AMOUT_isEnable = isEnable;
	*isEnable = AMIN_isEnable;
	return AMIN_return;
}

#ifdef __cplusplus
}
#endif
#endif /* WINAMS_STUB */
/* WINAMS_STUB[r_pma_hwdepend.c:R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power:AMSTB_R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power:inout:::] */
/*    R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power => Stub */
e_result_t AMSTB_R_PMA_HW_DEP_Change_Share_Domain_Lowest_Power(e_pma_hwa_id_t targetHwaId)
{
	static e_result_t volatile AMIN_return;
	static e_pma_hwa_id_t volatile AMOUT_targetHwaId;
	AMOUT_targetHwaId = targetHwaId;
	return AMIN_return;
}

#ifdef __cplusplus
}
#endif
#endif /* WINAMS_STUB */
