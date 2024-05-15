#ifdef WINAMS_STUB
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _DEFAULT_SOURCE
/* PRQA S 0603,3214,0602 1 # This definition is required to use glibc. */
#define _DEFAULT_SOURCE
#endif
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <poll.h>
#include <dirent.h>
#include <glob.h>
#include <sys/file.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <regex.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <linux/rcar-ipmmu-domains.h>
#include <linux/renesas_uioctl.h>
#include <linux/cmemdrv.h>
#include <linux/membarrier.h>
#include "rcar-xos/osal_configuration/target/linux/r_osal_configuration.h"
#include "rcar-xos/memory_allocator/r_meml.h"
#include "target/common/r_osal_common.h"
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg.h"
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg_share.h"
#include "target/common/res_cfg/r_osal_res_cfg_memory.h"
#include "target/common/res_cfg/dynamic/r_osal_res_cfg.h"
#include "target/common/res_cfg/dynamic/r_osal_res_cfg_share.h"
#include "target/linux/r_osal_os_types.h"

/* r_osal_configuration.c */
const st_osal_mmngr_config_t g_osal_memory_max_configration =
{
    .mode                           = OSAL_MMNGR_ALLOC_MODE_STACK_PROT,
    .memory_limit                   = OSAL_MEMORY_MANAGER_CFG_UNLIMITED,
    .max_allowed_allocations        = (4U * 1024U),
    .max_registered_monitors_cbs    = (4U * 1024U)
};

const st_osal_mmngr_region_info_list_t g_osal_memory_region_info_list =
{
    .num_of_memory_regions = 2,
    .default_memory_region = 0,
    .p_info = (st_osal_mmngr_region_info_t[])
    {
        /* Memory region 0 */
        {
            .name = "/dev/cmem0",
            .offset = 0,
            .range = (512U * 1024U * 1024U)
        },
        /* Memory region 1 */ /* example */
        {
            .name = "/dev/cmem1",
            .offset = 0,
            .range = (16U * 1024U * 1024U)
        }
    }
};

const st_osal_thread_affinity_t g_osal_thread_affinity[10] =
{
    {.id = OSAL_THREAD_AFFINITY_INVALID_ID}
};

const st_osal_interrupt_thread_affinity_t g_osal_interrupt_thread_affinity[10] =
{
    {.spi_num = OSAL_INTERRUPT_AFFINITY_INVALID_ID}
};

/* ----------------------------------------------- */
typedef enum
{
    OSAL_SEARCH_THREAD_CREATE_HANDLE,
    OSAL_SEARCH_THREAD_REFER_HANDLE
} e_osal_rcfg_thread_search_array_t;

typedef enum
{
    OSAL_SEARCH_THSYNC_CREATE_HANDLE,
    OSAL_SEARCH_THSYNC_REFER_HANDLE
} e_osal_rcfg_thsync_search_array_t;

typedef enum
{
    OSAL_SEARCH_MQ_CREATE_HANDLE,
    OSAL_SEARCH_MQ_REFER_HANDLE
} e_osal_rcfg_mq_search_array_t;

/* ----------------------------------------------- */
st_osal_time_t                                      g_st_osal_time_t;

osal_thread_handle_t                                g_osal_thread_handle_t[10];
st_osal_thread_config_t                             g_osal_thread_config_t;
st_osal_thread_control_t                            g_st_osal_thread_control_t[10];
st_osal_thread_control_t*                           g_st_osal_thread_control_t_p[10];
p_osal_thread_func_t                                g_p_osal_thread_func_t;
p_osal_os_thread_func_t                             g_p_osal_os_thread_func_t;
st_osal_os_thread_t                                 g_st_osal_os_thread_t;
osal_os_thread_t                                    g_osal_thread_t;

osal_mutex_handle_t                                 g_osal_mutex_handle_t[10];
st_osal_mutex_control_t                             g_st_osal_mutex_control_t[10];
st_osal_mutex_control_t*                            g_st_osal_mutex_control_t_p[10];
st_osal_os_mutex_t                                  g_st_osal_os_mutex_t[10];
osal_os_mutex_t                                     g_osal_os_mutex_t[10];
osal_os_mutexattr_t                                 g_osal_os_mutexattr_t;
st_osal_os_mutexattr_t                              g_st_osal_os_mutexattr_t;

osal_cond_handle_t                                  g_osal_cond_handle_t[10];
st_osal_cond_control_t                              g_st_osal_cond_control_t[10];
st_osal_cond_control_t*                             g_st_osal_cond_control_t_p[10];
st_osal_os_cond_t                                   g_st_osal_os_cond_t[10];
osal_os_cond_t                                      g_osal_os_cond_t;

osal_mq_handle_t                                    g_osal_mq_handle_t[10];
st_osal_mq_config_t                                 g_st_osal_mq_config_t;
st_osal_mq_control_t                                g_st_osal_mq_control_t[10];
st_osal_mq_control_t*                               g_st_osal_mq_control_t_p[10];
st_osal_os_mq_t                                     g_st_osal_os_mq_t;
osal_os_mq_t                                        g_osal_os_mq_t;
st_osal_os_mqattr_t                                 g_st_osal_os_mqattr_t;
osal_os_mqattr_t                                    g_osal_os_mqattr_t;

struct mem_info                                     g_mem_info;

osal_device_handle_t                                g_osal_device_handle_t;
st_osal_device_control_t                            g_st_osal_device_control_t[10];
st_osal_os_io_get_device_axi_bus_id_t               g_st_osal_os_io_get_device_axi_bus_id_t;
st_osal_os_io_get_axi_bus_id_from_device_name_t     g_st_osal_os_io_get_axi_bus_id_from_device_name_t;
st_osal_os_io_get_axi_bus_name_list_t               g_st_osal_os_io_get_axi_bus_name_list_t;
st_osal_os_io_get_axi_bus_name_t                    g_st_osal_os_io_get_axi_bus_name_t;
st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t g_st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t;
st_osal_os_io_get_axi_id_list_for_axi_bus_name_t    g_st_osal_os_io_get_axi_id_list_for_axi_bus_name_t;
st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t   g_st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t;
st_osal_os_io_get_axi_id_list_for_device_hdl_t      g_st_osal_os_io_get_axi_id_list_for_device_hdl_t;
st_osal_os_io_get_num_of_axi_ids_for_device_name_t  g_st_osal_os_io_get_num_of_axi_ids_for_device_name_t;
st_osal_os_io_get_axi_id_list_for_device_name_t     g_st_osal_os_io_get_axi_id_list_for_device_name_t;
st_osal_interrupt_control_t                         g_st_osal_interrupt_control_t[10];
st_osal_power_control_t                             g_st_osal_power_control_t[10];
osal_axi_bus_id_t*                                  gp_osal_axi_bus_id_t;
osal_axi_bus_id_t                                   g_osal_axi_bus_id_t[10];
st_osal_axi_bus_name_info_t                         g_st_osal_axi_bus_name_info_t[10];
st_osal_io_domain_table_t                           g_st_osal_io_domain_table_t;
st_osal_mq_node_t                                   g_st_osal_mq_node_t[10];
st_osal_device_list_t                               g_st_osal_device_list_t[10];
struct dirent                                       g_st_dirent[10];
glob_t                                              g_glob_t;
regmatch_t                                          g_regmatch_t;

e_osal_pm_required_state_t                          g_e_osal_pm_required_state_t;
e_osal_pm_state_t                                   g_e_osal_pm_state_t;
e_osal_pm_policy_t                                  g_e_osal_pm_policy_t;
e_osal_pm_reset_state_t                             g_e_osal_pm_reset_state_t;

/* ----------------------------------------------- */
int         g_dummy;
int         g_array_dummy[10];
bool        g_bool;
void*       gp_void;
size_t*     gp_size_t;
size_t      g_size_t;
uintptr_t   g_uintptr_t;
FILE        g_FILE;
bool*       p_result_common;
int8_t*     gp_int8_t;
int16_t*    gp_int16_t;
int32_t*    gp_int32_t;
int64_t*    gp_int64_t;
int8_t      g_int8_t;
int16_t     g_int16_t;
int32_t     g_int32_t;
int64_t     g_int64_t;
uint8_t     g_uint8_t;
uint16_t    g_uint16_t;
uint32_t    g_uint32_t;
uint64_t    g_uint64_t;
int8_t      array_int8[10];
int16_t     array_int16[10];
int32_t     array_int32[10];
int64_t     array_int64[10];
uint8_t     array_uint8[10];
uint16_t    array_uint16[10];
uint32_t    array_uint32[10];
uint64_t    array_uint64[10];
size_t      array_sizet[10];
char*       p_char;
char        str[PATH_MAX];

/* For Common Specification */
osal_thread_handle_t* const         g_uts_common_thread_handle;
st_osal_thread_config_t* const      g_uts_common_thread_config;
st_osal_thread_control_t            g_thread_common_control_1;
st_osal_thread_control_t            g_thread_common_control_2;

st_osal_mutex_control_t             g_st_osal_common_mutex_control;
osal_cond_handle_t                  g_osal_common_cond_handle;
st_osal_cond_control_t              g_st_osal_common_cond_control;

st_osal_mq_config_t                 g_st_osal_common_mq_config;
osal_mq_handle_t                    g_osal_common_mq_handle;
st_osal_mq_control_t                g_st_osal_common_mq_control;
st_osal_os_mq_t                     g_st_osal_os_mq[10];

st_osal_time_t                      g_st_osal_time;
osal_nano_sec_t                     g_osal_nano_sec_t;

st_osal_mmngr_config_t              g_st_osal_common_mmngr_config;
osal_memory_manager_handle_t        g_osal_common_memory_manager_handle;
struct st_osal_memory_manager_obj   g_st_osal_common_memory_manager_obj;
osal_memory_buffer_handle_t         g_osal_common_memory_buffer_handle;
struct st_osal_memory_buffer_obj*   g_osal_common_memory_buffer_obj;
struct st_osal_memory_buffer_obj    g_st_osal_common_memory_buffer_obj;
osal_memory_manager_t               g_osal_common_memory_manager;
osal_memory_buffer_t                g_osal_common_memory_buffer;

typedef struct
{
    e_osal_init_status_t            osal_memory_is_opened;
    osal_memory_manager_handle_t    osal_memory_manager;
} st_osal_memory_manager_table_t;
st_osal_memory_manager_table_t      g_st_osal_memory_manager_table;

osal_memory_manager_handle_t        g_memory_manager_handle;
struct st_osal_memory_manager_obj   g_st_osal_memory_manager_obj[30];
st_osal_mem_mmngr_ptr_self_t        g_st_osal_mem_mmngr_ptr_self[30];
st_osal_mem_mmngr_ptr_self_t*       g_st_osal_mem_mmngr_ptr_self_pp[30];
osal_memory_buffer_handle_t         g_memory_buffer_handle;
struct st_osal_memory_buffer_obj    g_st_osal_memory_buffer_obj;
struct st_osal_memory_buffer_obj*   g_osal_memory_buffer_obj;
st_osal_mem_buf_ptr_self_t          g_osal_mem_buf_ptr_self;
st_osal_mem_monitor_desc_t          g_st_osal_mem_monitor_desc;
st_osal_mem_monitor_desc_t          g_st_osal_mem_monitor_desc_array[10];
st_osal_os_mutex_t                  g_osal_os_mutex;
st_osal_mmngr_config_t              g_st_osal_mmngr_config;
uint8_t                             mmngr_buffer[OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE + 64 + OSAL_RES_CFG_MMNGR_GUARD_PAGE_SIZE];

osal_device_handle_t                g_osal_common_device_handle;
st_osal_device_control_t            g_st_osal_common_device_control;
st_osal_device_control_t            g_st_osal_common_device_control_array[10];
osal_axi_bus_id_t                   g_osal_common_axi_bus_id;

st_meml_buffer_t                    meml_buffer_dummy[1024];
st_meml_manage_t                    meml_manager_dummy;
uintptr_t                           dummy_buffer_pa[4096];
uintptr_t                           dummy_buffer_va[4096];

st_osal_os_cond_t                   g_osal_common_cond;
st_osal_os_mutex_t                  g_osal_common_mutex;

int AMSTB_dummy(void *arg)
{
    return 0;
}

/* Common stub for internal memory functions */
e_osal_return_t AMSTB_osal_mem_internal()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

void osal_memory_mngr_cb_dummy(void * user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)event;
    (void)error;
    (void)buffer_hndl;
    return;
}

/* WINAMS_STUB[mman.h:mmap:AMSTB_mmap:inout:::] */
/*    mmap => Stub */
void *AMSTB_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    static void* volatile AMIN_return;
    static void * volatile AMOUT_addr;
    static size_t volatile AMOUT_length;
    static int volatile AMOUT_prot;
    static int volatile AMOUT_flags;
    static int volatile AMOUT_fd;
    static off_t volatile AMOUT_offset;
    AMOUT_addr = addr;
    AMOUT_length = length;
    AMOUT_prot = prot;
    AMOUT_flags = flags;
    AMOUT_fd = fd;
    AMOUT_offset = offset;
    return AMIN_return;
}

/* WINAMS_STUB[mman.h:munmap:AMSTB_munmap:inout:::] */
/*    munmap => Stub */
int AMSTB_munmap(void *addr, size_t length)
{
    static int volatile AMIN_return;
    static void * volatile AMOUT_addr;
    static size_t volatile AMOUT_length;
    AMOUT_addr = addr;
    AMOUT_length = length;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_getspecific:AMSTB_pthread_getspecific:inout:::] */
/*    pthread_getspecific => Stub */
void *AMSTB_pthread_getspecific(pthread_key_t key)
{
    static void* volatile AMIN_return;
    static pthread_key_t volatile AMOUT_key;
    AMOUT_key = key;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_cond_broadcast:AMSTB_pthread_cond_broadcast:inout:::] */
/*    pthread_cond_broadcast => Stub */
int AMSTB_pthread_cond_broadcast(pthread_cond_t *cond)
{
    static int volatile AMIN_return;
    static pthread_cond_t * volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_cond_timedwait:AMSTB_pthread_cond_timedwait:inout:::] */
/*    pthread_cond_timedwait => Stub */
int AMSTB_pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    static int volatile AMIN_return;
    static pthread_cond_t * volatile AMOUT_cond;
    static pthread_mutex_t * volatile AMOUT_mutex;
    static struct timespec * volatile AMOUT_abstime;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_abstime = abstime;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_cond_destroy:AMSTB_pthread_cond_destroy:inout:::] */
/*    pthread_cond_destroy => Stub */
int AMSTB_pthread_cond_destroy(pthread_cond_t *cond)
{
    static int volatile AMIN_return;
    static pthread_cond_t * volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_cond_signal:AMSTB_pthread_cond_signal:inout:::] */
/*    pthread_cond_signal => Stub */
int AMSTB_pthread_cond_signal(pthread_cond_t *cond)
{
    static int volatile AMIN_return;
    static pthread_cond_t * volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_cond_init:AMSTB_pthread_cond_init:inout:::] */
/*    pthread_cond_init => Stub */
int AMSTB_pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr)
{
    static int volatile AMIN_return;
    static pthread_cond_t * volatile AMOUT_cond;
    static pthread_condattr_t * volatile AMOUT_cond_attr;
    AMOUT_cond = cond;
    AMOUT_cond_attr = cond_attr;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutex_unlock:AMSTB_pthread_mutex_unlock:inout:::] */
/*    pthread_mutex_unlock => Stub */
int AMSTB_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_mutex_t * volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutex_trylock:AMSTB_pthread_mutex_trylock:inout:::] */
/*    pthread_mutex_trylock => Stub */
int AMSTB_pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_mutex_t * volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutex_timedlock:AMSTB_pthread_mutex_timedlock:inout:::] */
/*    pthread_mutex_timedlock => Stub */
int AMSTB_pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout)
{
    static int volatile AMIN_return;
    static pthread_mutex_t * volatile AMOUT_mutex;
    static struct timespec * volatile AMOUT_abs_timeout;
    AMOUT_mutex = mutex;
    AMOUT_abs_timeout = abs_timeout;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutex_destroy:AMSTB_pthread_mutex_destroy:inout:::] */
/*    pthread_mutex_destroy => Stub */
int AMSTB_pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_mutex_t * volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutex_init:AMSTB_pthread_mutex_init:inout:::] */
/*    pthread_mutex_init => Stub */
int AMSTB_pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr)
{
    static int volatile AMIN_return;
    static pthread_mutex_t * volatile AMOUT_mutex;
    static pthread_mutexattr_t * volatile AMOUT_mutexattr;
    AMOUT_mutex = mutex;
    AMOUT_mutexattr = mutexattr;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutexattr_destroy:AMSTB_pthread_mutexattr_destroy:inout:::] */
/*    pthread_mutexattr_destroy => Stub */
int AMSTB_pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_mutexattr_t * volatile AMOUT_attr;
    AMOUT_attr = attr;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutexattr_settype:AMSTB_pthread_mutexattr_settype:inout:::] */
/*    pthread_mutexattr_settype => Stub */
int AMSTB_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind)
{
    static int volatile AMIN_return;
    static pthread_mutexattr_t * volatile AMOUT_attr;
    static int volatile AMOUT_kind;
    AMOUT_attr = attr;
    AMOUT_kind = kind;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_mutexattr_init:AMSTB_pthread_mutexattr_init:inout:::] */
/*    pthread_mutexattr_init => Stub */
int AMSTB_pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_mutexattr_t * volatile AMOUT_attr;
    AMOUT_attr = attr;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_equal:AMSTB_pthread_equal:inout:::] */
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

/* WINAMS_STUB[pthread.h:pthread_attr_setstacksize:AMSTB_pthread_attr_setstacksize:inout:::] */
/*    pthread_attr_setstacksize => Stub */
int AMSTB_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    static int volatile AMIN_return;
    static pthread_attr_t * volatile AMOUT_attr;
    static size_t volatile AMOUT_stacksize;
    AMOUT_attr = attr;
    AMOUT_stacksize = stacksize;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_attr_init:AMSTB_pthread_attr_init:inout:::] */
/*    pthread_attr_init => Stub */
int AMSTB_pthread_attr_init(pthread_attr_t* attr)
{
    static int volatile AMIN_return;
    static pthread_attr_t* volatile AMOUT_attr;
    AMOUT_attr = attr;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_attr_setinheritsched:AMSTB_pthread_attr_setinheritsched:inout:::] */
/*    pthread_attr_setinheritsched => Stub */
int AMSTB_pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
    static int volatile AMIN_return;
    static pthread_attr_t* volatile AMOUT_attr;
    static int volatile AMOUT_inheritsched;
    AMOUT_attr = attr;
    AMOUT_inheritsched = inheritsched;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_attr_setschedpolicy:AMSTB_pthread_attr_setschedpolicy:inout:::] */
/*    pthread_attr_setschedpolicy => Stub */
int AMSTB_pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
    static int volatile AMIN_return;
    static pthread_attr_t* volatile AMOUT_attr;
    static int volatile AMOUT_policy;
    AMOUT_attr = attr;
    AMOUT_policy = policy;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_attr_setschedparam:AMSTB_pthread_attr_setschedparam:inout:::] */
/*    pthread_attr_setschedparam => Stub */
int AMSTB_pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
    static int volatile AMIN_return;
    static pthread_attr_t* volatile AMOUT_attr;
    static struct sched_param* volatile AMOUT_param;
    AMOUT_attr = attr;
    AMOUT_param = param;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_attr_destroy:AMSTB_pthread_attr_destroy:inout:::] */
/*    pthread_attr_destroy => Stub */
int AMSTB_pthread_attr_destroy(pthread_attr_t *attr)
{
    static int volatile AMIN_return;
    static pthread_attr_t* volatile AMOUT_attr;
    AMOUT_attr = attr;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_create:AMSTB_pthread_create:inout:::] */
/*    pthread_create => Stub */
int AMSTB_pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    static int volatile AMIN_return;
    static pthread_t* volatile AMOUT_thread;
    static pthread_attr_t* volatile AMOUT_attr;
    static void* volatile AMOUT_start_routine;
    static void* volatile AMOUT_arg;
    AMOUT_thread = thread;
    AMOUT_attr = attr;
    AMOUT_start_routine = start_routine;
    AMOUT_arg = arg;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_join:AMSTB_pthread_join:inout:::] */
/*    pthread_join => Stub */
int AMSTB_pthread_join(pthread_t thread, void **retval)
{
    static int volatile AMIN_return;
    static void* volatile AMIN_retval;
    static pthread_t volatile AMOUT_thread;
    static void** volatile AMOUT_retval;
    AMOUT_thread = thread;
    AMOUT_retval = retval;
    *retval = AMIN_retval;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_detach:AMSTB_pthread_detach:inout:::] */
/*    pthread_detach => Stub */
int AMSTB_pthread_detach(pthread_t thread)
{
    static int volatile AMIN_return;
    static pthread_t volatile AMOUT_thread;
    AMOUT_thread = thread;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_setschedprio:AMSTB_pthread_setschedprio:inout:::] */
/*    pthread_setschedprio => Stub */
int AMSTB_pthread_setschedprio(pthread_t thread, int prio)
{
    static int volatile AMIN_return;
    static pthread_t volatile AMOUT_thread;
    static int volatile AMOUT_prio;
    AMOUT_thread = thread;
    AMOUT_prio = prio;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_cancel:AMSTB_pthread_cancel:inout:::] */
/*    pthread_cancel => Stub */
int AMSTB_pthread_cancel(pthread_t thread)
{
    static int volatile AMIN_return;
    static pthread_t volatile AMOUT_thread;
    AMOUT_thread = thread;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_setcancelstate:AMSTB_pthread_setcancelstate:inout:::] */
/*    pthread_setcancelstate => Stub */
int AMSTB_pthread_setcancelstate(int state, int *oldstate)
{
#define AMCALLMAX_pthread_setcancelstate 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_pthread_setcancelstate];
    static int volatile AMOUT_state[AMCALLMAX_pthread_setcancelstate];
    static int* volatile AMOUT_oldstate[AMCALLMAX_pthread_setcancelstate];
    AMOUT_state[AM_count] = state;
    AMOUT_oldstate[AM_count] = oldstate;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[pthread.h:pthread_cond_wait:AMSTB_pthread_cond_wait:inout:::] */
/*    pthread_cond_wait => Stub */
int AMSTB_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    static int volatile AMIN_return;
    static pthread_cond_t* volatile AMOUT_cond;
    static pthread_mutex_t* volatile AMOUT_mutex;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_key_create:AMSTB_pthread_key_create:inout:::] */
/*    pthread_key_create => Stub */
int AMSTB_pthread_key_create(pthread_key_t *key, void (*destr_function) (void *))
{
    static int volatile AMIN_return;
    static pthread_key_t* volatile AMOUT_key;
    static void* volatile AMOUT_destr_function;
    AMOUT_key = key;
    AMOUT_destr_function = destr_function;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_key_delete:AMSTB_pthread_key_delete:inout:::] */
/*    pthread_key_delete => Stub */
int AMSTB_pthread_key_delete(pthread_key_t key)
{
    static int volatile AMIN_return;
    static pthread_key_t volatile AMOUT_key;
    AMOUT_key = key;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:CPU_SET:AMSTB_CPU_SET:inout:::] */
/*    CPU_SET => Stub */
void AMSTB_CPU_SET(int cpu, cpu_set_t *set)
{
#define AMCALLMAX_CPU_SET 64
    static int volatile AM_count;
    static int volatile AMOUT_cpu[AMCALLMAX_CPU_SET];
    static cpu_set_t* volatile AMOUT_set[AMCALLMAX_CPU_SET];
    AMOUT_cpu[AM_count] = cpu;
    AMOUT_set[AM_count] = set;
    AM_count++;
    return;
}

/* WINAMS_STUB[pthread.h:pthread_setaffinity_np:AMSTB_pthread_setaffinity_np:inout:::] */
/*    pthread_setaffinity_np => Stub */
int AMSTB_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset)
{
    static int volatile AMIN_return;
    static pthread_t volatile AMOUT_thread;
    static size_t volatile AMOUT_cpusetsize;
    static cpu_set_t* volatile AMOUT_cpuset;
    AMOUT_thread = thread;
    AMOUT_cpusetsize = cpusetsize;
    AMOUT_cpuset = cpuset;
    return AMIN_return;
}

/* WINAMS_STUB[pthread.h:pthread_setname_np:AMSTB_pthread_setname_np:inout:::] */
/*    pthread_setname_np => Stub */
int AMSTB_pthread_setname_np(pthread_t thread, const char *name)
{
    static int volatile AMIN_return;
    static pthread_t volatile AMOUT_thread;
    static char* volatile AMOUT_name;
    AMOUT_thread = thread;
    AMOUT_name = name;
    return AMIN_return;
}

/* WINAMS_STUB[time.h:clock_gettime:AMSTB_clock_gettime:inout:::] */
/*    clock_gettime => Stub */
int AMSTB_clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    static int volatile AMIN_return;
    static int volatile AMIN_tv_nsec;
    static clockid_t volatile AMOUT_clk_id;
    static struct timespec * volatile AMOUT_tp;
    tp->tv_nsec = AMIN_tv_nsec;
    AMOUT_clk_id = clk_id;
    AMOUT_tp = tp;
    return AMIN_return;
}

/* WINAMS_STUB[time.h:clock_nanosleep:AMSTB_clock_nanosleep:inout:::] */
/*    clock_nanosleep => Stub */
int AMSTB_clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain)
{
    static int volatile AMIN_return;
    static clockid_t volatile AMOUT_clockid;
    static int volatile AMOUT_flags;
    static struct timespec * volatile AMOUT_request;
    static struct timespec * volatile AMOUT_remain;
    AMOUT_clockid = clockid;
    AMOUT_flags = flags;
    AMOUT_request = request;
    AMOUT_remain = remain;
    return AMIN_return;
}

/* WINAMS_STUB[string.h:strlen:AMSTB_strlen:inout:::] */
/*    strlen => Stub */
size_t AMSTB_strlen(const char *s)
{
#define AMCALLMAX_strlen 10
    static size_t volatile AM_count;
    static size_t volatile AMIN_return[AMCALLMAX_strlen];
    static char* volatile AMOUT_s[AMCALLMAX_strlen];
    AMOUT_s[AM_count] = s;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[string.h:strncmp:AMSTB_strncmp:inout:::] */
/*    strncmp => Stub */
int AMSTB_strcmp(const char *s1, const char *s2)
{
#define AMCALLMAX_strcmp 50
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_strcmp];
    static char * volatile AMOUT_s1[AMCALLMAX_strcmp];
    static char * volatile AMOUT_s2[AMCALLMAX_strcmp];
    AMOUT_s1[AM_count] = s1;
    AMOUT_s2[AM_count] = s2;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[string.h:strncmp:AMSTB_strncmp:inout:::] */
/*    strncmp => Stub */
int AMSTB_strncmp(const char *s1, const char *s2, size_t n)
{
#define AMCALLMAX_strncmp 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_strncmp];
    static char * volatile AMOUT_s1[AMCALLMAX_strncmp];
    static char * volatile AMOUT_s2[AMCALLMAX_strncmp];
    static size_t volatile AMOUT_n[AMCALLMAX_strncmp];
    AMOUT_s1[AM_count] = s1;
    AMOUT_s2[AM_count] = s2;
    AMOUT_n[AM_count] = n;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[string.h:strstr:AMSTB_strstr:inout:::] */
/*    strstr => Stub */
char *AMSTB_strstr(const char *haystack, const char *needle)
{
#define AMCALLMAX_strstr 10
    static int volatile AM_count;
    static char* volatile AMIN_return[AMCALLMAX_strstr];
    static char* volatile AMOUT_haystack[AMCALLMAX_strstr];
    static char* volatile AMOUT_needle[AMCALLMAX_strstr];
    AMOUT_haystack[AM_count] = haystack;
    AMOUT_needle[AM_count] = needle;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdlib.h:bsearch:AMSTB_bsearch:inout:::] */
/*    bsearch => Stub */
void *AMSTB_bsearch(const void *key, const void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *))
{
    static void* volatile AMIN_return;
    static void * volatile AMOUT_key;
    static void * volatile AMOUT_base;
    static size_t volatile AMOUT_nmemb;
    static size_t volatile AMOUT_size;
    static int volatile AMOUT_compar;
    AMOUT_key = key;
    AMOUT_base = base;
    AMOUT_nmemb = nmemb;
    AMOUT_size = size;
    AMOUT_compar = compar;
    return AMIN_return;
}

/* WINAMS_STUB[stdlib.h:free:AMSTB_free:inout:::] */
/*    free => Stub */
void AMSTB_free(void *ptr)
{
    static void* volatile AMIN_return;
    static void * volatile AMOUT_ptr;
    AMOUT_ptr = ptr;
    return;
}

/* WINAMS_STUB[stdlib.h:calloc:AMSTB_calloc:inout:::] */
/*    calloc => Stub */
void *AMSTB_calloc(size_t n, size_t size)
{
#define AMCALLMAX_calloc 50
    static int volatile AM_count;
    static void* volatile AMIN_return[AMCALLMAX_calloc];
    static size_t volatile AMOUT_n[AMCALLMAX_calloc];
    static size_t volatile AMOUT_size[AMCALLMAX_calloc];
    AMOUT_n[AM_count] = n;
    AMOUT_size[AM_count] = size;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdlib.h:realloc:AMSTB_realloc:inout:::] */
/*    realloc => Stub */
void *AMSTB_realloc(void *ptr, size_t size)
{
#define AMCALLMAX_realloc 50
    static int volatile AM_count;
    static void* volatile AMIN_return[AMCALLMAX_realloc];
    static void* volatile AMOUT_ptr[AMCALLMAX_realloc];
    static size_t volatile AMOUT_size[AMCALLMAX_realloc];
    AMOUT_ptr[AM_count] = ptr;
    AMOUT_size[AM_count] = size;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdio.h:fopen:AMSTB_fopen:inout:::] */
/*    fopen => Stub */
FILE* AMSTB_fopen(const char *pathname, const char *mode)
{
#define AMCALLMAX_fopen 10
    static int volatile AM_count;
    static FILE* volatile AMIN_return[AMCALLMAX_fopen];
    static char* volatile AMOUT_pathname[AMCALLMAX_fopen];
    static char* volatile AMOUT_mode[AMCALLMAX_fopen];
    AMOUT_pathname[AM_count] = pathname;
    AMOUT_mode[AM_count] = mode;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdio.h:_fclose_internal:AMSTB__fclose_internal:inout:::] */
/*    _fclose_internal => Stub */
int AMSTB__fclose_internal(FILE *stream)
{
#define AMCALLMAX__fclose_internal 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX__fclose_internal];
    static FILE* volatile AMOUT_stream[AMCALLMAX__fclose_internal];
    AMOUT_stream[AM_count] = stream;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdio.h:fread:AMSTB_fread:inout:::] */
/*    fread => Stub */
size_t AMSTB_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    static size_t volatile AMIN_return;
    static int volatile AMIN_ptr;
    static void* volatile AMOUT_ptr;
    static size_t volatile AMOUT_size;
    static size_t volatile AMOUT_nmemb;
    static FILE* volatile AMOUT_stream;
    AMOUT_ptr = ptr;
    AMOUT_size = size;
    AMOUT_nmemb = nmemb;
    AMOUT_stream = stream;
    *((int *)ptr) = AMIN_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[stdio.h:fscanf:AMSTB_fscanf:inout:::] */
/*    fscanf => Stub */
int AMSTB_fscanf(FILE *stream, const char *format, unsigned long* p_result)
{
#define AMCALLMAX_fscanf 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_fscanf];
    static unsigned long volatile AMIN_p_result[AMCALLMAX_fscanf];
    static FILE* volatile AMOUT_stream[AMCALLMAX_fscanf];
    static char* volatile AMOUT_format[AMCALLMAX_fscanf];
    static unsigned long* volatile AMOUT_p_result[AMCALLMAX_fscanf];
    AMOUT_stream[AM_count] = stream;
    AMOUT_format[AM_count] = format;
    AMOUT_p_result[AM_count] = p_result;
    *p_result = AMIN_p_result[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdio.h:fprintf:AMSTB_fprintf:inout:::] */
/*    fprintf => Stub */
int AMSTB_fprintf()
{
#define AMCALLMAX_fprintf 30
    static int volatile AMIN_return[AMCALLMAX_fprintf];
    static int volatile AM_count;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[stdio.h:snprintf:AMSTB_snprintf:inout:::] */
/*    snprintf => Stub */
int AMSTB_snprintf(char *str, size_t size, const char *format, ...)
{
#define AMCALLMAX_snprintf 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_snprintf];
    static char* volatile AMOUT_str[AMCALLMAX_snprintf];
    static size_t volatile AMOUT_size[AMCALLMAX_snprintf];
    static char* volatile AMOUT_format[AMCALLMAX_snprintf];
    AMOUT_str[AM_count] = str;
    AMOUT_size[AM_count] = size;
    AMOUT_format[AM_count] = format;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[poll.h:poll:AMSTB_poll:inout:::] */
/*    poll => Stub */
int AMSTB_poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
#define AMCALLMAX_poll 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_poll];
    static struct pollfd* volatile AMOUT_fds[AMCALLMAX_poll];
    static nfds_t volatile AMOUT_nfds[AMCALLMAX_poll];
    static int volatile AMOUT_timeout[AMCALLMAX_poll];
    AMOUT_fds[AM_count] = fds;
    AMOUT_nfds[AM_count] = nfds;
    AMOUT_timeout[AM_count] = timeout;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[glob.h:glob:AMSTB_glob:inout:::] */
/*    glob => Stub */
int AMSTB_glob(const char *pattern, int flags, int (*errfunc) (const char *epath, int eerrno), glob_t *pglob)
{
    static int volatile AMIN_return;
    static size_t volatile AMIN_pglob_gl_pathc;
    static char* volatile AMIN_pglob_gl_pathv;
    static char* volatile AMOUT_pattern;
    static int volatile AMOUT_flags;
    static void* volatile AMOUT_errfunc;
    static glob_t* volatile AMOUT_pglob;
    AMOUT_pattern = pattern;
    AMOUT_flags = flags;
    AMOUT_errfunc = errfunc;
    AMOUT_pglob = pglob;
    pglob->gl_pathc = AMIN_pglob_gl_pathc;
    pglob->gl_pathv = &AMIN_pglob_gl_pathv;
    return AMIN_return;
}

/* WINAMS_STUB[sys/file.h:flock:AMSTB_flock:inout:::] */
/*    flock => Stub */
int AMSTB_flock(int fd, int operation)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_fd;
    static int volatile AMOUT_operation;
    AMOUT_fd = fd;
    AMOUT_operation = operation;
    return AMIN_return;
}

/* WINAMS_STUB[dirent.h:opendir:AMSTB_opendir:inout:::] */
/*    opendir => Stub */
DIR* AMSTB_opendir(const char *name)
{
    static DIR* volatile AMIN_return;
    static char* volatile AMOUT_name;
    AMOUT_name = name;
    return AMIN_return;
}

/* WINAMS_STUB[dirent.h:closedir:AMSTB_closedir:inout:::] */
/*    closedir => Stub */
int AMSTB_closedir(DIR *dirp)
{
    static int volatile AMIN_return;
    static DIR* volatile AMOUT_dirp;
    AMOUT_dirp = dirp;
    return AMIN_return;
}

/* WINAMS_STUB[dirent.h:readdir:AMSTB_readdir:inout:::] */
/*    readdir => Stub */
struct dirent* AMSTB_readdir(DIR *dirp)
{
#define AMCALLMAX_readdir 10
    static int volatile AM_count;
    static struct dirent* volatile AMIN_return[AMCALLMAX_readdir];
    static DIR* volatile AMOUT_dirp[AMCALLMAX_readdir];
    AMOUT_dirp[AM_count] = dirp;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[regex.h:regcomp:AMSTB_regcomp:inout:::] */
/*    regcomp => Stub */
int AMSTB_regcomp(regex_t *preg, const char *regex, int cflags)
{
#define AMCALLMAX_regcomp 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_regcomp];
    static size_t volatile AMIN_preg_re_nsub[AMCALLMAX_regcomp];
    static regex_t* volatile AMOUT_preg[AMCALLMAX_regcomp];
    static char* volatile AMOUT_regex[AMCALLMAX_regcomp];
    static int volatile AMOUT_cflags[AMCALLMAX_regcomp];
    AMOUT_preg[AM_count] = preg;
    AMOUT_regex[AM_count] = regex;
    AMOUT_cflags[AM_count] = cflags;
    preg->re_nsub = AMIN_preg_re_nsub[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[regex.h:regexec:AMSTB_regexec:inout:::] */
/*    regexec => Stub */
int AMSTB_regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags)
{
#define AMCALLMAX_regexec 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_regexec];
    static int volatile AMIN_pmatch_index[AMCALLMAX_regexec];
    static regoff_t volatile AMIN_pmatch_rm_so[AMCALLMAX_regexec];
    static regoff_t volatile AMIN_pmatch_rm_eo[AMCALLMAX_regexec];
    static regex_t* volatile AMOUT_preg[AMCALLMAX_regexec];
    static char* volatile AMOUT_string[AMCALLMAX_regexec];
    static size_t volatile AMOUT_nmatch[AMCALLMAX_regexec];
    static regmatch_t* volatile AMOUT_pmatch[AMCALLMAX_regexec];
    static int volatile AMOUT_eflags[AMCALLMAX_regexec];
    AMOUT_preg[AM_count] = preg;
    AMOUT_string[AM_count] = string;
    AMOUT_nmatch[AM_count] = nmatch;
    AMOUT_pmatch[AM_count] = pmatch;
    AMOUT_eflags[AM_count] = eflags;
    pmatch[AMIN_pmatch_index[AM_count]].rm_so = AMIN_pmatch_rm_so[AM_count];
    pmatch[AMIN_pmatch_index[AM_count]].rm_eo = AMIN_pmatch_rm_eo[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[fcntl.h:open:AMSTB_open:inout:::] */
/*    open => Stub */
int AMSTB_open(const char *pathname, int flags)
{
    static int volatile AMIN_return;
    static char * volatile AMOUT_pathname;
    static int volatile AMOUT_flags;
    AMOUT_pathname = pathname;
    AMOUT_flags = flags;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:close:AMSTB_close:inout:::] */
/*    close => Stub */
int AMSTB_close(int fd)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_fd;
    AMOUT_fd = fd;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:read:AMSTB_read:inout:::] */
/*    read => Stub */
int AMSTB_read(int fd, void *buf, size_t count)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_fd;
    static void * volatile AMOUT_buf;
    static size_t volatile AMOUT_count;
    AMOUT_fd = fd;
    AMOUT_buf = buf;
    AMOUT_count = count;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:write:AMSTB_write:inout:::] */
/*    write => Stub */
int AMSTB_write(int fd, const void *buf, size_t count)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_fd;
    static void * volatile AMOUT_buf;
    static size_t volatile AMOUT_count;
    AMOUT_fd = fd;
    AMOUT_buf = buf;
    AMOUT_count = count;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:sysconf:AMSTB_sysconf:inout:::] */
/*    sysconf => Stub */
int AMSTB_sysconf(int name)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_name;
    AMOUT_name = name;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:syscall:AMSTB_syscall:inout:::] */
/*    syscall => Stub */
int AMSTB_syscall(long number, int cmd, unsigned int flags, int cpu_id)
{
    static int volatile AMIN_return;
    static long volatile AMOUT_number;
    static int volatile AMOUT_cmd;
    static unsigned int volatile AMOUT_flags;
    static int volatile AMOUT_cpu_id;
    AMOUT_number = number;
    AMOUT_cmd = cmd;
    AMOUT_flags = flags;
    AMOUT_cpu_id = cpu_id;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:readlink:AMSTB_readlink:inout:::] */
/*    readlink => Stub */
ssize_t AMSTB_readlink(const char *pathname, char *buf, size_t bufsiz)
{
    static ssize_t volatile AMIN_return;
    static char* volatile AMOUT_pathname;
    static char* volatile AMOUT_buf;
    static size_t volatile AMOUT_bufsiz;
    AMOUT_pathname = pathname;
    AMOUT_buf = buf;
    AMOUT_bufsiz = bufsiz;
    return AMIN_return;
}

/* WINAMS_STUB[unistd.h:access:AMSTB_access:inout:::] */
/*    access => Stub */
int AMSTB_access(const char *pathname, int mode)
{
    static int volatile AMIN_return;
    static char* volatile AMOUT_pathname;
    static char* volatile AMOUT_mode;
    AMOUT_pathname = pathname;
    AMOUT_mode = mode;
    return AMIN_return;
}

/* WINAMS_STUB[sys/ioctl.h:ioctl:AMSTB_ioctl:inout:::] */
/*    ioctl => Stub */
int AMSTB_ioctl(int fd, unsigned long request, void* p_param)
{
#define AMCALLMAX_ioctl 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_ioctl];
    static void* volatile AMIN_p_param[AMCALLMAX_ioctl];
    static int volatile AMOUT_fd[AMCALLMAX_ioctl];
    static unsigned long volatile AMOUT_request[AMCALLMAX_ioctl];
    AMOUT_fd[AM_count] = fd;
    AMOUT_request[AM_count] = request;
    if (GET_PHYS_ADDR == request)
    {
        ((struct mem_info *)p_param)->phys_addr = ((struct mem_info *)AMIN_p_param[AM_count])->phys_addr;
    }
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_GetVersion:AMSTB_R_MEML_GetVersion:inout:::] */
/*    R_MEML_GetVersion => Stub */
st_meml_version_t *AMSTB_R_MEML_GetVersion(void)
{
    static st_meml_version_t *volatile AMIN_return;
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

/* WINAMS_STUB[r_meml_allocator.c:R_MEML_Deinitialize:AMSTB_R_MEML_Deinitialize:inout:::] */
/*    R_MEML_Deinitialize => Stub */
e_meml_return_t AMSTB_R_MEML_Deinitialize(st_meml_manage_t* const p_manage)
{
    static e_meml_return_t volatile AMIN_return;
    static st_meml_manage_t* const volatile AMOUT_p_manage;
    *AMOUT_p_manage = *p_manage;
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

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Open:AMSTB_R_OSAL_OS_Open:inout:::] */
/*    R_OSAL_OS_Open => Stub */
int AMSTB_R_OSAL_OS_Open(const char* p_pathname, int flags)
{
#define AMCALLMAX_R_OSAL_OS_Open 10
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_Open];
    static char* volatile AMOUT_p_pathname[AMCALLMAX_R_OSAL_OS_Open];
    static int volatile AMOUT_flags[AMCALLMAX_R_OSAL_OS_Open];
    AMOUT_p_pathname[AM_count] = p_pathname;
    AMOUT_flags[AM_count] = flags;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Close:AMSTB_R_OSAL_OS_Close:inout:::] */
/*    R_OSAL_OS_Close => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_Close(int fd)
{
#define AMCALLMAX_R_OSAL_OS_Close 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_Close];
    static int volatile AMOUT_fd[AMCALLMAX_R_OSAL_OS_Close];
    AMOUT_fd[AM_count] = fd;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Read:AMSTB_R_OSAL_OS_Read:inout:::] */
/*    R_OSAL_OS_Read => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_Read(int fd, void* p_buf, size_t count)
{
    static e_osal_return_t volatile AMIN_return;
    static int volatile AMOUT_fd;
    static void* volatile AMOUT_p_buf;
    static size_t volatile AMOUT_count;
    AMOUT_fd = fd;
    AMOUT_p_buf = p_buf;
    AMOUT_count = count;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Write:AMSTB_R_OSAL_OS_Write:inout:::] */
/*    R_OSAL_OS_Write => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_Write(int fd, const void* p_buf, size_t count)
{
    static e_osal_return_t volatile AMIN_return;
    static int volatile AMOUT_fd;
    static void* volatile AMOUT_p_buf;
    static size_t volatile AMOUT_count;
    AMOUT_fd = fd;
    AMOUT_p_buf = p_buf;
    AMOUT_count = count;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Malloc:AMSTB_R_OSAL_OS_Malloc:inout:::] */
/*    R_OSAL_OS_Malloc => Stub */
void* AMSTB_R_OSAL_OS_Malloc(size_t size)
{
#define AMCALLMAX_R_OSAL_OS_Malloc 50
    static int volatile AM_count;
    static void* volatile AMIN_return[AMCALLMAX_R_OSAL_OS_Malloc];
    static size_t volatile AMOUT_size[AMCALLMAX_R_OSAL_OS_Malloc];
    AMOUT_size[AM_count] = size;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_Free:AMSTB_R_OSAL_OS_Free:inout:::] */
/*    R_OSAL_OS_Free => Stub */
void AMSTB_R_OSAL_OS_Free(void* p_ptr)
{
    static int volatile AM_count;
    static void* volatile AMOUT_p_ptr;
    AMOUT_p_ptr = p_ptr;
    AM_count++;
    return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_BinarySearch:AMSTB_R_OSAL_OS_BinarySearch:inout:::] */
/*    R_OSAL_OS_BinarySearch => Stub */
void* AMSTB_R_OSAL_OS_BinarySearch(const void* p_key, const void* p_array, size_t array_num, size_t array_size, p_osal_compare_func_t compare_function)
{
    static void* volatile AMIN_return;
    static int volatile AMIN_type;
    static void** volatile AMIN_pp_array1;
    static void* volatile AMIN_p_array1;
    static uint64_t volatile AMIN_id1;
    static void** volatile AMIN_pp_array2;
    static void* volatile AMIN_p_array2;
    static uint64_t volatile AMIN_id2;
    static void* volatile AMOUT_p_key;
    static void* volatile AMOUT_p_array;
    static size_t volatile AMOUT_array_num;
    static size_t volatile AMOUT_array_size;
    static p_osal_compare_func_t volatile AMOUT_compare_function;
    AMOUT_p_key = p_key;
    AMOUT_p_array = p_array;
    AMOUT_array_num = array_num;
    AMOUT_array_size = array_size;
    AMOUT_compare_function = compare_function;

    if (NULL != AMIN_pp_array1)
    {
        *AMIN_pp_array1 = AMIN_p_array1;
    }

    if (NULL != AMIN_pp_array2)
    {
        *AMIN_pp_array2 = AMIN_p_array2;
    }

    switch(AMIN_type)
    {
        case 0 :    // Thread
            if (NULL != AMIN_p_array1)
            {
                ((st_osal_thread_control_t *)AMIN_p_array1)->thread_id = AMIN_id1;
            }

            if (NULL != AMIN_p_array2)
            {
                ((st_osal_thread_control_t *)AMIN_p_array2)->thread_id = AMIN_id2;
            }
            break;

        case 1 :    // Threadsync (Mutex)
            if (NULL != AMIN_p_array1)
            {
                ((st_osal_mutex_control_t *)AMIN_p_array1)->mutex_id = AMIN_id1;
            }

            if (NULL != AMIN_p_array2)
            {
                ((st_osal_mutex_control_t *)AMIN_p_array2)->mutex_id = AMIN_id2;
            }
            break;

        case 2 :    // Threadsync (Condition variable)
            if (NULL != AMIN_p_array1)
            {
                ((st_osal_cond_control_t *)AMIN_p_array1)->cond_id = AMIN_id1;
            }

            if (NULL != AMIN_p_array2)
            {
                ((st_osal_cond_control_t *)AMIN_p_array2)->cond_id = AMIN_id2;
            }
            break;

        case 3 :    // Message
        default :
            if (NULL != AMIN_p_array1)
            {
                ((st_osal_mq_control_t *)AMIN_p_array1)->mq_id = AMIN_id1;
            }

            if (NULL != AMIN_p_array2)
            {
                ((st_osal_mq_control_t *)AMIN_p_array2)->mq_id = AMIN_id2;
            }
            break;
    }

    (void)compare_function((void *)AMIN_pp_array1, (void *)AMIN_pp_array2);
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_IsISRContext:AMSTB_R_OSAL_OS_IsISRContext:inout:::] */
/*    R_OSAL_OS_IsISRContext => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IsISRContext(bool *const p_is_isr)
{
    static e_osal_return_t volatile AMIN_return;
    static bool volatile AM_result;
    static bool* volatile AMOUT_p_is_isr;
    AMOUT_p_is_isr = p_is_isr;
    *p_is_isr = AM_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_common.c:R_OSAL_OS_GetSysPageSize:AMSTB_R_OSAL_OS_GetSysPageSize:inout:::] */
/*    R_OSAL_OS_GetSysPageSize => Stub */
long AMSTB_R_OSAL_OS_GetSysPageSize()
{
    static long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexCreate:AMSTB_R_OSAL_OS_MutexCreate:inout:::] */
/*    R_OSAL_OS_MutexCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexCreate(struct st_osal_os_mutexattr *mutex_attr,struct st_osal_os_mutex **p_mutex)
{
    static int volatile AMIN_return;
    static struct st_osal_os_mutexattr *volatile AMOUT_mutex_attr;
    static struct st_osal_os_mutex **volatile AMOUT_p_mutex;
    AMOUT_mutex_attr = mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexCreate:AMSTB_R_OSAL_OS_MutexCreate_multiple:inout:::] */
/*    R_OSAL_OS_MutexCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexCreate_multiple(const osal_os_mutexattr_t mutex_attr, osal_os_mutex_t* p_mutex)
{
#define AMCALLMAX_R_OSAL_OS_MutexCreate 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MutexCreate];
    static osal_os_mutexattr_t volatile AMOUT_mutex_attr[AMCALLMAX_R_OSAL_OS_MutexCreate];
    static osal_os_mutex_t* volatile AMOUT_p_mutex[AMCALLMAX_R_OSAL_OS_MutexCreate];
    AMOUT_mutex_attr[AM_count] = mutex_attr;
    AMOUT_p_mutex[AM_count] = p_mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexDestroy:AMSTB_R_OSAL_OS_MutexDestroy:inout:::] */
/*    R_OSAL_OS_MutexDestroy => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexDestroy(osal_os_mutex_t mutex)
{
#define AMCALLMAX_R_OSAL_OS_MutexDestroy 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MutexDestroy];
    static osal_os_mutex_t volatile AMOUT_mutex[AMCALLMAX_R_OSAL_OS_MutexDestroy];
    AMOUT_mutex[AM_count] = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexTimedLock:AMSTB_R_OSAL_OS_MutexTimedLock:inout:::] */
/*    R_OSAL_OS_MutexTimedLock => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexTimedLock(osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mutex_t volatile AMOUT_mutex;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexTryLock:AMSTB_R_OSAL_OS_MutexTryLock:inout:::] */
/*    R_OSAL_OS_MutexTryLock => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexTryLock(osal_os_mutex_t mutex)
{
#define AMCALLMAX_R_OSAL_OS_MutexTryLock 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MutexTryLock];
    static osal_os_mutex_t volatile AMOUT_mutex[AMCALLMAX_R_OSAL_OS_MutexTryLock];
    AMOUT_mutex[AM_count] = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexUnlock:AMSTB_R_OSAL_OS_MutexUnlock:inout:::] */
/*    R_OSAL_OS_MutexUnlock => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexUnlock(osal_os_mutex_t mutex)
{
#define AMCALLMAX_R_OSAL_OS_MutexUnlock 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MutexUnlock];
    static osal_os_mutex_t volatile AMOUT_mutex[AMCALLMAX_R_OSAL_OS_MutexUnlock];
    AMOUT_mutex[AM_count] = mutex;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexInitAttr:AMSTB_R_OSAL_OS_MutexInitAttr:inout:::] */
/*    R_OSAL_OS_MutexInitAttr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexInitAttr(osal_os_mutexattr_t* p_mutex_attr)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mutexattr_t volatile AMIN_p_mutex_attr;
    static osal_os_mutexattr_t* volatile AMOUT_p_mutex_attr;
    AMOUT_p_mutex_attr = p_mutex_attr;
    *p_mutex_attr = AMIN_p_mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_mutex.c:R_OSAL_OS_MutexDeinitAttr:AMSTB_R_OSAL_OS_MutexDeinitAttr:inout:::] */
/*    R_OSAL_OS_MutexDeinitAttr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MutexDeinitAttr(osal_os_mutexattr_t mutex_attr)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mutexattr_t volatile AMOUT_mutex_attr;
    AMOUT_mutex_attr = mutex_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondCreate:AMSTB_R_OSAL_OS_CondCreate:inout:::] */
/*    R_OSAL_OS_CondCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_CondCreate(osal_os_cond_t* p_cond)
{
#define AMCALLMAX_R_OSAL_OS_CondCreate 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_CondCreate];
    static osal_os_cond_t* volatile AMOUT_p_cond[AMCALLMAX_R_OSAL_OS_CondCreate];
    AMOUT_p_cond[AM_count] = p_cond;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondDestroy:AMSTB_R_OSAL_OS_CondDestroy:inout:::] */
/*    R_OSAL_OS_CondDestroy => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_CondDestroy(osal_os_cond_t cond)
{
#define AMCALLMAX_R_OSAL_OS_CondDestroy 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_CondDestroy];
    static osal_os_cond_t volatile AMOUT_cond[AMCALLMAX_R_OSAL_OS_CondDestroy];
    AMOUT_cond[AM_count] = cond;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondTimedWait:AMSTB_R_OSAL_OS_CondTimedWait:inout:::] */
/*    R_OSAL_OS_CondTimedWait => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_CondTimedWait(osal_os_cond_t cond, osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_cond_t AMOUT_cond;
    static osal_os_mutex_t AMOUT_mutex;
    static st_osal_time_t* AMOUT_p_time_stamp;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondBroadcast:AMSTB_R_OSAL_OS_CondBroadcast:inout:::] */
/*    R_OSAL_OS_CondBroadcast => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_CondBroadcast(osal_os_cond_t cond)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_cond_t volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_cond.c:R_OSAL_OS_CondSignal:AMSTB_R_OSAL_OS_CondSignal:inout:::] */
/*    R_OSAL_OS_CondSignal => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_CondSignal(osal_os_cond_t cond)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_cond_t volatile AMOUT_cond;
    AMOUT_cond = cond;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqCheckBusy:AMSTB_R_OSAL_OS_MqCheckBusy:inout:::] */
/*    R_OSAL_OS_MqCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqCheckBusy(osal_os_mq_t mq)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    AMOUT_mq = mq;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqCreate:AMSTB_R_OSAL_OS_MqCreate:inout:::] */
/*    R_OSAL_OS_MqCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqCreate(const st_osal_mq_config_t* const p_config, const osal_os_mqattr_t mq_attr, osal_os_mq_t* p_mq)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMIN_p_mq;
    static st_osal_mq_config_t* volatile AMOUT_p_config;
    static osal_os_mqattr_t volatile AMOUT_mq_attr;
    static osal_os_mq_t* volatile AMOUT_p_mq;
    AMOUT_p_config = p_config;
    AMOUT_mq_attr = mq_attr;
    AMOUT_p_mq = p_mq;
    *p_mq = AMIN_p_mq;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqDelete:AMSTB_R_OSAL_OS_MqDelete:inout:::] */
/*    R_OSAL_OS_MqDelete => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqDelete(osal_os_mq_t mq)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    AMOUT_mq = mq;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqSend:AMSTB_R_OSAL_OS_MqSend:inout:::] */
/*    R_OSAL_OS_MqSend => Stub */
int AMSTB_R_OSAL_OS_MqSend(st_osal_os_mq_t * mq, const void * p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t* volatile AMOUT_mq;
    static const void volatile* AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqTimedSend:AMSTB_R_OSAL_OS_MqTimedSend:inout:::] */
/*    R_OSAL_OS_MqTimedSend => Stub */
int AMSTB_R_OSAL_OS_MqTimedSend(st_osal_os_mq_t * mq, const st_osal_time_t* const p_time_stamp, const void * p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t* volatile AMOUT_mq;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    static const void volatile* AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqReceive:AMSTB_R_OSAL_OS_MqReceive:inout:::] */
/*    R_OSAL_OS_MqReceive => Stub */
int AMSTB_R_OSAL_OS_MqReceive(st_osal_os_mq_t * mq, const void *p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t* volatile AMOUT_mq;
    static const void volatile* AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqTimedReceive:AMSTB_R_OSAL_OS_MqTimedReceive:inout:::] */
/*    R_OSAL_OS_MqTimedReceive => Stub */
int AMSTB_R_OSAL_OS_MqTimedReceive(st_osal_os_mq_t * mq, const st_osal_time_t* const p_time_stamp, const void *p_buffer, size_t buffer_size)
{
    static int volatile AMIN_return;
    static st_osal_os_mq_t* volatile AMOUT_mq;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    static const void volatile* AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqIsFull:AMSTB_R_OSAL_OS_MqIsFull:inout:::] */
/*    R_OSAL_OS_MqIsFull => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqIsFull(osal_os_mq_t mq, bool* const p_result)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static bool* volatile AMOUT_p_result;
    AMOUT_mq = mq;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqIsEmpty:AMSTB_R_OSAL_OS_MqIsEmpty:inout:::] */
/*    R_OSAL_OS_MqIsEmpty => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqIsEmpty(osal_os_mq_t mq, bool* const p_result)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static bool* volatile AMOUT_p_result;
    AMOUT_mq = mq;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqReset:AMSTB_R_OSAL_OS_MqReset:inout:::] */
/*    R_OSAL_OS_MqReset => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqReset(osal_os_mq_t mq)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    AMOUT_mq = mq;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqInitAttr:AMSTB_R_OSAL_OS_MqInitAttr:inout:::] */
/*    R_OSAL_OS_MqInitAttr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqInitAttr(osal_os_mqattr_t* p_mq_attr)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mqattr_t volatile AMIN_p_mq_attr;
    static osal_os_mqattr_t* volatile AMOUT_p_mq_attr;
    AMOUT_p_mq_attr = p_mq_attr;
    *p_mq_attr = AMIN_p_mq_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:R_OSAL_OS_MqDeinitAttr:AMSTB_R_OSAL_OS_MqDeinitAttr:inout:::] */
/*    R_OSAL_OS_MqDeinitAttr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MqDeinitAttr(osal_os_mqattr_t mq_attr)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mqattr_t volatile AMOUT_mq_attr;
    AMOUT_mq_attr = mq_attr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_send:AMSTB_r_osal_os_mq_send:inout:::] */
/*    r_osal_os_mq_send => Stub */
e_osal_return_t AMSTB_r_osal_os_mq_send(osal_os_mq_t mq, const void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_receive:AMSTB_r_osal_os_mq_receive:inout:::] */
/*    r_osal_os_mq_receive => Stub */
void AMSTB_r_osal_os_mq_receive(osal_os_mq_t mq, void* p_buffer, size_t buffer_size)
{
    static int volatile AM_count;
    static uint64_t volatile AMIN_mq_numofmsg;
    static osal_os_mq_t volatile AMOUT_mq;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    mq->numofmsg = AMIN_mq_numofmsg;
    return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_mutex_destroy:AMSTB_r_osal_os_mq_mutex_destroy:inout:::] */
/*    r_osal_os_mq_mutex_destroy => Stub */
e_osal_return_t AMSTB_r_osal_os_mq_mutex_destroy(osal_os_mutex_t* p_mutex)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mutex_t* volatile AMOUT_p_mutex;
    AMOUT_p_mutex = p_mutex;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_mutex_lock_for_time_period:AMSTB_r_osal_os_mq_mutex_lock_for_time_period:inout:::] */
/*    r_osal_os_mq_mutex_lock_for_time_period => Stub */
e_osal_return_t AMSTB_r_osal_os_mq_mutex_lock_for_time_period(osal_os_mutex_t mutex)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mutex_t volatile AMOUT_mutex;
    AMOUT_mutex = mutex;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_mutex_until_timestamp:AMSTB_r_osal_os_mq_mutex_until_timestamp:inout:::] */
/*    r_osal_os_mq_mutex_until_timestamp => Stub */
e_osal_return_t AMSTB_r_osal_os_mq_mutex_until_timestamp(osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mutex_t volatile AMOUT_mutex;
    static osal_os_mutex_t volatile AMOUT_p_time_stamp;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_message.c:r_osal_os_mq_cond_destroy:AMSTB_r_osal_os_mq_cond_destroy:inout:::] */
/*    r_osal_os_mq_cond_destroy => Stub */
e_osal_return_t AMSTB_r_osal_os_mq_cond_destroy(osal_os_cond_t* p_cond)
{
#define AMCALLMAX_r_osal_os_mq_cond_destroy 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_mq_cond_destroy];
    static osal_os_cond_t* volatile AMOUT_p_cond[AMCALLMAX_r_osal_os_mq_cond_destroy];
    AMOUT_p_cond[AM_count] = p_cond;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrMap:AMSTB_R_OSAL_OS_MmngrMap:inout:::] */
/*    R_OSAL_OS_MmngrMap => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MmngrMap(struct st_osal_mem_mmngr_ptr_self *mngr_control,struct st_osal_mmngr_config *p_usr_config,unsigned long long region_idx)
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

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrUnmap:AMSTB_R_OSAL_OS_MmngrUnmap:inout:::] */
/*    R_OSAL_OS_MmngrUnmap => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MmngrUnmap(struct st_osal_mem_mmngr_ptr_self *mngr_control)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_mngr_control;
    AMOUT_mngr_control = mngr_control;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrFlush:AMSTB_R_OSAL_OS_MmngrFlush:inout:::] */
/*    R_OSAL_OS_MmngrFlush => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MmngrFlush(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long addr,unsigned long long offset,unsigned long long size)
{
#define AMCALLMAX_R_OSAL_OS_MmngrFlush 30
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MmngrFlush];
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_addr;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_addr = addr;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_memory.c:R_OSAL_OS_MmngrInvalidate:AMSTB_R_OSAL_OS_MmngrInvalidate:inout:::] */
/*    R_OSAL_OS_MmngrInvalidate => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MmngrInvalidate(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long addr,unsigned long long offset,unsigned long long size)
{
#define AMCALLMAX_R_OSAL_OS_MmngrInvalidate 30
    static int volatile AM_count;
    static int volatile AMIN_return[AMCALLMAX_R_OSAL_OS_MmngrInvalidate];
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_addr;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_addr = addr;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmGetResetState:AMSTB_R_OSAL_OS_PmGetResetState:inout:::] */
/*    R_OSAL_OS_PmGetResetState => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmGetResetState(osal_device_handle_t const handle, int* p_value)
{
    static int volatile AMIN_value;
    static int volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static int* volatile AMOUT_p_value;
    *p_value = AMIN_value;
    AMOUT_handle = handle;
    AMOUT_p_value = p_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_get_power_clock_state:AMSTB_r_osal_dcfg_pm_get_power_clock_state:inout:::] */
/*    r_osal_dcfg_pm_get_power_clock_state => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_pm_get_power_clock_state(osal_device_handle_t handle, int* p_pwr_status, int* p_clk_status)
{
    static e_osal_return_t volatile AMIN_return;
    static int volatile AMIN_pwr_status;
    static int volatile AMIN_clk_status;
    static osal_device_handle_t volatile AMOUT_handle;
    static int* volatile AMOUT_p_pwr_status;
    static int* volatile AMOUT_p_clk_status;
    AMOUT_handle = handle;
    AMOUT_p_pwr_status = p_pwr_status;
    AMOUT_p_clk_status = p_clk_status;
    *p_pwr_status = AMIN_pwr_status;
    *p_clk_status = AMIN_clk_status;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmSetClockState:AMSTB_R_OSAL_OS_PmSetClockState:inout:::] */
/*    R_OSAL_OS_PmSetClockState => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmSetClockState(osal_device_handle_t const handle, int value)
{
    static int volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static int volatile AMOUT_value;
    AMOUT_handle = handle;
    AMOUT_value = value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmSetPowerState:AMSTB_R_OSAL_OS_PmSetPowerState:inout:::] */
/*    R_OSAL_OS_PmSetPowerState => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmSetPowerState(osal_device_handle_t const handle, int value)
{
    static int volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static int volatile AMOUT_value;
    AMOUT_handle = handle;
    AMOUT_value = value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmGetClockState:AMSTB_R_OSAL_OS_PmGetClockState:inout:::] */
/*    R_OSAL_OS_PmGetClockState => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmGetClockState(osal_device_handle_t const handle, int* p_value)
{
    static int volatile AM_status;
    static int volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static int* volatile AMOUT_p_value;
    *p_value = AM_status;
    AMOUT_handle = handle;
    AMOUT_p_value = p_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmGetPowerState:AMSTB_R_OSAL_OS_PmGetPowerState:inout:::] */
/*    R_OSAL_OS_PmGetPowerState => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmGetPowerState(osal_device_handle_t const handle, int* p_value)
{
    static int volatile AM_status;
    static int volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static int* volatile AMOUT_p_value;
    *p_value = AM_status;
    AMOUT_handle = handle;
    AMOUT_p_value = p_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmSetPostClock:AMSTB_R_OSAL_OS_PmSetPostClock:inout:::] */
/*    R_OSAL_OS_PmSetPostClock => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmSetPostClock(osal_device_handle_t const handle, bool clock_enable, uint32_t clock_divider)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static bool volatile AMOUT_clock_enable;
    static uint32_t volatile AMOUT_clock_divider;
    AMOUT_handle = handle;
    AMOUT_clock_enable = clock_enable;
    AMOUT_clock_divider = clock_divider;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:R_OSAL_OS_PmSetResetState:AMSTB_R_OSAL_OS_PmSetResetState:inout:::] */
/*    R_OSAL_OS_PmSetResetState => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_PmSetResetState(osal_device_handle_t const handle, int value)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static int volatile AMOUT_value;
    AMOUT_handle = handle;
    AMOUT_value = value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:r_osal_os_pm_get_ioctl:AMSTB_r_osal_os_pm_get_ioctl:inout:::] */
/*    r_osal_os_pm_get_ioctl => Stub */
e_osal_return_t AMSTB_r_osal_os_pm_get_ioctl(int device_fd, unsigned long int req, int* p_value)
{
    static e_osal_return_t volatile AMIN_return;
    static int volatile AMIN_p_value;
    static int volatile AMOUT_device_fd;
    static unsigned long int volatile AMOUT_req;
    static int* volatile AMOUT_p_value;
    AMOUT_device_fd = device_fd;
    AMOUT_req = req;
    AMOUT_p_value = p_value;
    *p_value = AMIN_p_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_power.c:r_osal_os_pm_set_ioctl:AMSTB_r_osal_os_pm_set_ioctl:inout:::] */
/*    r_osal_os_pm_set_ioctl => Stub */
e_osal_return_t AMSTB_r_osal_os_pm_set_ioctl(int device_fd, unsigned long int req, int value)
{
    static e_osal_return_t volatile AMIN_return;
    static int volatile AMOUT_device_fd;
    static unsigned long int volatile AMOUT_req;
    static int volatile AMOUT_value;
    AMOUT_device_fd = device_fd;
    AMOUT_req = req;
    AMOUT_value = value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_get_reset_state:AMSTB_r_osal_dcfg_pm_get_reset_state:inout:::] */
/*    r_osal_dcfg_pm_get_reset_state => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_pm_get_reset_state(osal_device_handle_t handle, e_osal_pm_reset_state_t* const p_reset_state)
{
    static e_osal_return_t volatile AMIN_return;
    static e_osal_pm_reset_state_t volatile AM_state;
    static osal_device_handle_t volatile AMOUT_handle;
    static e_osal_pm_reset_state_t* volatile AMOUT_p_reset_state;
    AMOUT_handle = handle;
    AMOUT_p_reset_state = p_reset_state;
    *p_reset_state = AM_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_wait_for_required_state:AMSTB_r_osal_dcfg_pm_wait_for_required_state:inout:::] */
/*    r_osal_dcfg_pm_wait_for_required_state => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_pm_wait_for_required_state(osal_device_handle_t handle, e_osal_pm_state_t state, st_osal_time_t const* p_abs_timeout)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static e_osal_pm_state_t volatile AMOUT_state;
    static st_osal_time_t* volatile AMOUT_p_abs_timeout;
    AMOUT_handle = handle;
    AMOUT_state = state;
    AMOUT_p_abs_timeout = p_abs_timeout;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_set_power_clock_state:AMSTB_r_osal_dcfg_pm_set_power_clock_state:inout:::] */
/*    r_osal_dcfg_pm_set_power_clock_state => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_pm_set_power_clock_state(osal_device_handle_t handle, e_osal_pm_policy_t policy)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static e_osal_pm_policy_t volatile AMOUT_policy;
    AMOUT_handle = handle;
    AMOUT_policy = policy;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSleep:AMSTB_R_OSAL_OS_ThreadSleep:inout:::] */
/*    R_OSAL_OS_ThreadSleep => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadSleep(const st_osal_time_t* const p_time_stamp)
{
#define AMCALLMAX_R_OSAL_OS_ThreadSleep 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_ThreadSleep];
    static st_osal_time_t* volatile AMOUT_p_time_stamp[AMCALLMAX_R_OSAL_OS_ThreadSleep];
    AMOUT_p_time_stamp[AM_count] = p_time_stamp;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:r_osal_dcfg_pm_get_state:AMSTB_r_osal_dcfg_pm_get_state:inout:::] */
/*    r_osal_dcfg_pm_get_state => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_pm_get_state(osal_device_handle_t handle, e_osal_pm_state_t* const p_state)
{
#define AMCALLMAX_r_osal_dcfg_pm_get_state 10
    static int volatile AM_count;
    static e_osal_pm_state_t volatile AM_state[AMCALLMAX_r_osal_dcfg_pm_get_state];
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_dcfg_pm_get_state];
    static osal_device_handle_t volatile AMOUT_handle[AMCALLMAX_r_osal_dcfg_pm_get_state];
    static e_osal_pm_state_t* volatile AMOUT_p_state[AMCALLMAX_r_osal_dcfg_pm_get_state];
    *p_state = AM_state[AM_count];
    AMOUT_handle[AM_count] = handle;
    AMOUT_p_state[AM_count] = p_state;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_hif_mutex.c:R_OSAL_HIF_MutexLockUntilTimeStamp:AMSTB_R_OSAL_HIF_MutexLockUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_MutexLockUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_MutexLockUntilTimeStamp(osal_os_mutex_t mutex, const st_osal_time_t* const p_time_stamp)
{
#define AMCALLMAX_R_OSAL_HIF_MutexLockUntilTimeStamp 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_HIF_MutexLockUntilTimeStamp];
    static osal_os_mutex_t volatile AMOUT_mutex[AMCALLMAX_R_OSAL_HIF_MutexLockUntilTimeStamp];
    static st_osal_time_t* volatile AMOUT_p_time_stamp[AMCALLMAX_R_OSAL_HIF_MutexLockUntilTimeStamp];
    AMOUT_mutex[AM_count] = mutex;
    AMOUT_p_time_stamp[AM_count] = p_time_stamp;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadCreate:AMSTB_R_OSAL_OS_ThreadCreate:inout:::] */
/*    R_OSAL_OS_ThreadCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadCreate(const st_osal_thread_config_t* const p_config, p_osal_os_thread_func_t func, void* p_arg, osal_os_thread_t* p_thread)
{
#define AMCALLMAX_R_OSAL_OS_ThreadCreate 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_ThreadCreate];
    static st_osal_thread_config_t* volatile AMOUT_p_config[AMCALLMAX_R_OSAL_OS_ThreadCreate];
    static p_osal_os_thread_func_t volatile AMOUT_func[AMCALLMAX_R_OSAL_OS_ThreadCreate];
    static void* volatile AMOUT_p_arg[AMCALLMAX_R_OSAL_OS_ThreadCreate];
    static osal_os_thread_t* volatile AMOUT_p_thread[AMCALLMAX_R_OSAL_OS_ThreadCreate];
    AMOUT_p_config[AM_count] = p_config;
    AMOUT_func[AM_count] = func;
    AMOUT_p_arg[AM_count] = p_arg;
    AMOUT_p_thread[AM_count] = p_thread;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_hif_mutex.c:R_OSAL_HIF_MutexLockForTimePeriod:AMSTB_R_OSAL_HIF_MutexLockForTimePeriod:inout:::] */
/*    R_OSAL_HIF_MutexLockForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_MutexLockForTimePeriod(osal_os_mutex_t mutex, osal_milli_sec_t time_period)
{
#define AMCALLMAX_R_OSAL_HIF_MutexLockForTimePeriod 10
    static int i;
    static int volatile AM_count;
    static int volatile AM_control;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_HIF_MutexLockForTimePeriod];
    static osal_os_mutex_t volatile AMOUT_mutex[AMCALLMAX_R_OSAL_HIF_MutexLockForTimePeriod];
    static osal_milli_sec_t volatile AMOUT_time_period[AMCALLMAX_R_OSAL_HIF_MutexLockForTimePeriod];
    AMOUT_mutex[AM_count] = mutex;
    AMOUT_time_period[AM_count] = time_period;

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

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_CondWaitUntilTimeStamp:AMSTB_R_OSAL_HIF_CondWaitUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_CondWaitUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_CondWaitUntilTimeStamp(osal_os_cond_t cond, osal_os_mutex_t mutex, const st_osal_time_t *const p_time_stamp)
{
    static int volatile AMIN_return;
    static osal_os_cond_t volatile AMOUT_cond;
    static osal_os_mutex_t volatile AMOUT_mutex;
    static st_osal_time_t * volatile AMOUT_p_time_stamp;
    AMOUT_cond = cond;
    AMOUT_mutex = mutex;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_receive_isr:AMSTB_r_osal_rcfg_mq_receive_isr:inout:::] */
/*    r_osal_rcfg_mq_receive_isr => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mq_receive_isr(osal_mq_handle_t handle, void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_mq_handle_t volatile AMOUT_handle;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_receive_not_isr:AMSTB_r_osal_rcfg_mq_receive_not_isr:inout:::] */
/*    r_osal_rcfg_mq_receive_not_isr => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mq_receive_not_isr(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_mq_handle_t volatile AMOUT_handle;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_send_isr:AMSTB_r_osal_rcfg_mq_send_isr:inout:::] */
/*    r_osal_rcfg_mq_send_isr => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mq_send_isr(osal_mq_handle_t handle, const void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_mq_handle_t volatile AMOUT_handle;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_send_not_isr:AMSTB_r_osal_rcfg_mq_send_not_isr:inout:::] */
/*    r_osal_rcfg_mq_send_not_isr => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mq_send_not_isr(osal_mq_handle_t handle, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_mq_handle_t volatile AMOUT_handle;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:r_osal_rcfg_mq_search_array:AMSTB_r_osal_rcfg_mq_search_array:inout:::] */
/*    r_osal_rcfg_mq_search_array => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mq_search_array(e_osal_rcfg_mq_search_array_t search_type, osal_mq_id_t check_id, size_t* p_index)
{
    static e_osal_return_t volatile AMIN_return;
    static size_t volatile AMIN_p_index;
    static e_osal_rcfg_mq_search_array_t volatile AMOUT_search_type;
    static osal_mq_id_t volatile AMOUT_check_id;
    static size_t* volatile AMOUT_p_index;
    AMOUT_search_type = search_type;
    AMOUT_check_id = check_id;
    AMOUT_p_index = p_index;
    *p_index = AMIN_p_index;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memorybarrier.c:R_OSAL_OS_MemoryBarrier:AMSTB_R_OSAL_OS_MemoryBarrier:inout:::] */
/*    R_OSAL_OS_MemoryBarrier => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MemoryBarrier(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_wait:AMSTB_r_osal_rcfg_cond_wait:inout:::] */
/*    r_osal_rcfg_cond_wait => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_cond_wait(const osal_cond_handle_t cond_handle, const osal_mutex_handle_t mutex_handle, const st_osal_time_t* const p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_cond_handle_t volatile AMOUT_cond_handle;
    static osal_mutex_handle_t volatile AMOUT_mutex_handle;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    AMOUT_cond_handle = cond_handle;
    AMOUT_mutex_handle = mutex_handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_cond_search_array:AMSTB_r_osal_rcfg_cond_search_array:inout:::] */
/*    r_osal_rcfg_cond_search_array => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_cond_search_array(e_osal_rcfg_thsync_search_array_t search_type, osal_cond_id_t check_id, size_t* p_index)
{
    static e_osal_return_t volatile AMIN_return;
    static size_t volatile AMIN_index;
    static e_osal_rcfg_thsync_search_array_t volatile AMOUT_search_type;
    static osal_cond_id_t volatile AMOUT_check_id;
    static size_t* volatile AMOUT_p_index;
    AMOUT_search_type = search_type;
    AMOUT_check_id = check_id;
    AMOUT_p_index = p_index;
    *p_index = AMIN_index;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_lock:AMSTB_r_osal_rcfg_mutex_lock:inout:::] */
/*    r_osal_rcfg_mutex_lock => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mutex_lock(const osal_mutex_handle_t handle, const st_osal_time_t* const p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_mutex_handle_t volatile AMOUT_handle;
    static const st_osal_time_t* volatile AMOUT_p_time_stamp;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:r_osal_rcfg_mutex_search_array:AMSTB_r_osal_rcfg_mutex_search_array:inout:::] */
/*    r_osal_rcfg_mutex_search_array => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mutex_search_array(e_osal_rcfg_thsync_search_array_t search_type, osal_mutex_id_t check_id, size_t* p_index)
{
    static e_osal_return_t volatile AMIN_return;
    static size_t volatile AMIN_index;
    static e_osal_rcfg_thsync_search_array_t volatile AMOUT_search_type;
    static osal_mutex_id_t volatile AMOUT_check_id;
    static size_t* volatile AMOUT_p_index;
    AMOUT_search_type = search_type;
    AMOUT_check_id = check_id;
    AMOUT_p_index = p_index;
    *p_index = AMIN_index;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_memorybarrier.c:R_OSAL_OS_MemoryBarrierInit:AMSTB_R_OSAL_OS_MemoryBarrierInit:inout:::] */
/*    R_OSAL_OS_MemoryBarrierInit => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_MemoryBarrierInit(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_thread.c:R_OSAL_HIF_ThreadSleepUntilTimeStamp:AMSTB_R_OSAL_HIF_ThreadSleepUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_ThreadSleepUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_ThreadSleepUntilTimeStamp(const st_osal_time_t* const p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_thread.c:R_OSAL_HIF_ThreadSleepForTimePeriod:AMSTB_R_OSAL_HIF_ThreadSleepForTimePeriod:inout:::] */
/*    R_OSAL_HIF_ThreadSleepForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_ThreadSleepForTimePeriod(osal_milli_sec_t time_period)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_milli_sec_t volatile AMOUT_time_period;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadWaitFinish:AMSTB_R_OSAL_OS_ThreadWaitFinish:inout:::] */
/*    R_OSAL_OS_ThreadWaitFinish => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadWaitFinish(osal_os_thread_t thread)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_thread_t volatile AMOUT_thread;
    AMOUT_thread = thread;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSelf:AMSTB_R_OSAL_OS_ThreadSelf:inout:::] */
/*    R_OSAL_OS_ThreadSelf => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadSelf(osal_os_thread_t * p_self_id)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_thread_t* volatile AMOUT_p_self_id;
    AMOUT_p_self_id = p_self_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:r_osal_rcfg_thread_search_array:AMSTB_r_osal_rcfg_thread_search_array:inout:::] */
/*    r_osal_rcfg_thread_search_array => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_thread_search_array(e_osal_rcfg_thread_search_array_t search_type, osal_thread_id_t check_id, size_t* p_index)
{
#define AMCALLMAX_r_osal_rcfg_thread_search_array 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_rcfg_thread_search_array];
    static size_t volatile AMIN_p_index[AMCALLMAX_r_osal_rcfg_thread_search_array];
    static e_osal_rcfg_thread_search_array_t volatile AMOUT_search_type[AMCALLMAX_r_osal_rcfg_thread_search_array];
    static osal_thread_id_t volatile AMOUT_check_id[AMCALLMAX_r_osal_rcfg_thread_search_array];
    static size_t* volatile AMOUT_p_index[AMCALLMAX_r_osal_rcfg_thread_search_array];
    AMOUT_search_type[AM_count] = search_type;
    AMOUT_check_id[AM_count] = check_id;
    AMOUT_p_index[AM_count] = p_index;
    *p_index = AMIN_p_index[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadEqual:AMSTB_R_OSAL_OS_ThreadEqual:inout:::] */
/*    R_OSAL_OS_ThreadEqual => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadEqual(osal_os_thread_t thread1, osal_os_thread_t thread2, bool* const p_result)
{
    static e_osal_return_t volatile AMIN_return;
    static bool volatile AMIN_result;
    static osal_os_thread_t volatile AMOUT_thread1;
    static osal_os_thread_t volatile AMOUT_thread2;
    static bool* volatile AMOUT_p_result;
    AMOUT_thread1 = thread1;
    AMOUT_thread2 = thread2;
    AMOUT_p_result = p_result;
    *p_result = AMIN_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSetCpuAffinity:AMSTB_R_OSAL_OS_ThreadSetCpuAffinity:inout:::] */
/*    R_OSAL_OS_ThreadSetCpuAffinity => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadSetCpuAffinity(osal_thread_id_t id)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_thread_id_t volatile AMOUT_id;
    AMOUT_id = id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_thread.c:R_OSAL_OS_ThreadSetTaskName:AMSTB_R_OSAL_OS_ThreadSetTaskName:inout:::] */
/*    R_OSAL_OS_ThreadSetTaskName => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ThreadSetTaskName(const char* p_task_name)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_task_name;
    AMOUT_p_task_name = p_task_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeGetInitStatus:AMSTB_R_OSAL_RCFG_ClockTimeGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ClockTimeGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ClockTimeGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeSetInitStatus:AMSTB_R_OSAL_RCFG_ClockTimeSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ClockTimeSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_ClockTimeSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeControlInit:AMSTB_R_OSAL_RCFG_ClockTimeControlInit:inout:::] */
/*    R_OSAL_RCFG_ClockTimeControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ClockTimeControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadGetInitStatus:AMSTB_R_OSAL_RCFG_ThreadGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThreadGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSetInitStatus:AMSTB_R_OSAL_RCFG_ThreadSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThreadSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_ThreadSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadControlInit:AMSTB_R_OSAL_RCFG_ThreadControlInit:inout:::] */
/*    R_OSAL_RCFG_ThreadControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadControlDeinit:AMSTB_R_OSAL_RCFG_ThreadControlDeinit:inout:::] */
/*    R_OSAL_RCFG_ThreadControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncGetInitStatus:AMSTB_R_OSAL_RCFG_ThsyncGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThsyncGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThsyncGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncSetInitStatus:AMSTB_R_OSAL_RCFG_ThsyncSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_ThsyncSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_ThsyncSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncControlInit:AMSTB_R_OSAL_RCFG_ThsyncControlInit:inout:::] */
/*    R_OSAL_RCFG_ThsyncControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThsyncControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncControlDeinit:AMSTB_R_OSAL_RCFG_ThsyncControlDeinit:inout:::] */
/*    R_OSAL_RCFG_ThsyncControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThsyncControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqGetInitStatus:AMSTB_R_OSAL_RCFG_MqGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MqGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqSetInitStatus:AMSTB_R_OSAL_RCFG_MqSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MqSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_MqSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqControlInit:AMSTB_R_OSAL_RCFG_MqControlInit:inout:::] */
/*    R_OSAL_RCFG_MqControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqControlDeinit:AMSTB_R_OSAL_RCFG_MqControlDeinit:inout:::] */
/*    R_OSAL_RCFG_MqControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetInitStatus:AMSTB_R_OSAL_DCFG_IoGetInitStatus:inout:::] */
/*    R_OSAL_DCFG_IoGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoSetInitStatus:AMSTB_R_OSAL_DCFG_IoSetInitStatus:inout:::] */
/*    R_OSAL_DCFG_IoSetInitStatus => Stub */
void AMSTB_R_OSAL_DCFG_IoSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoControlInit:AMSTB_R_OSAL_DCFG_IoControlInit:inout:::] */
/*    R_OSAL_DCFG_IoControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoControlDeinit:AMSTB_R_OSAL_DCFG_IoControlDeinit:inout:::] */
/*    R_OSAL_DCFG_IoControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetInitStatus:AMSTB_R_OSAL_DCFG_PmGetInitStatus:inout:::] */
/*    R_OSAL_DCFG_PmGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetInitStatus:AMSTB_R_OSAL_DCFG_PmSetInitStatus:inout:::] */
/*    R_OSAL_DCFG_PmSetInitStatus => Stub */
void AMSTB_R_OSAL_DCFG_PmSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmControlInit:AMSTB_R_OSAL_DCFG_PmControlInit:inout:::] */
/*    R_OSAL_DCFG_PmControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmControlDeinit:AMSTB_R_OSAL_DCFG_PmControlDeinit:inout:::] */
/*    R_OSAL_DCFG_PmControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptGetInitStatus:AMSTB_R_OSAL_DCFG_InterruptGetInitStatus:inout:::] */
/*    R_OSAL_DCFG_InterruptGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptSetInitStatus:AMSTB_R_OSAL_DCFG_InterruptSetInitStatus:inout:::] */
/*    R_OSAL_DCFG_InterruptSetInitStatus => Stub */
void AMSTB_R_OSAL_DCFG_InterruptSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptControlInit:AMSTB_R_OSAL_DCFG_InterruptControlInit:inout:::] */
/*    R_OSAL_DCFG_InterruptControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptControlDeinit:AMSTB_R_OSAL_DCFG_InterruptControlDeinit:inout:::] */
/*    R_OSAL_DCFG_InterruptControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetInitStatus:AMSTB_R_OSAL_RCFG_MmngrGetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MmngrGetInitStatus => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetInitStatus()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrSetInitStatus:AMSTB_R_OSAL_RCFG_MmngrSetInitStatus:inout:::] */
/*    R_OSAL_RCFG_MmngrSetInitStatus => Stub */
void AMSTB_R_OSAL_RCFG_MmngrSetInitStatus(int status)
{
    static int volatile AMOUT_status;
    AMOUT_status = status;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrControlInit:AMSTB_R_OSAL_RCFG_MmngrControlInit:inout:::] */
/*    R_OSAL_RCFG_MmngrControlInit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrControlInit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrControlDeinit:AMSTB_R_OSAL_RCFG_MmngrControlDeinit:inout:::] */
/*    R_OSAL_RCFG_MmngrControlDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrControlDeinit()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqCheckBusy:AMSTB_R_OSAL_RCFG_MqCheckBusy:inout:::] */
/*    R_OSAL_RCFG_MqCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqCheckBusy()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_ThsyncCheckBusy:AMSTB_R_OSAL_RCFG_ThsyncCheckBusy:inout:::] */
/*    R_OSAL_RCFG_ThsyncCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThsyncCheckBusy()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptCheckBusy:AMSTB_R_OSAL_DCFG_InterruptCheckBusy:inout:::] */
/*    R_OSAL_DCFG_InterruptCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptCheckBusy()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmCheckBusy:AMSTB_R_OSAL_DCFG_PmCheckBusy:inout:::] */
/*    R_OSAL_DCFG_PmCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmCheckBusy()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoCheckBusy:AMSTB_R_OSAL_DCFG_IoCheckBusy:inout:::] */
/*    R_OSAL_DCFG_IoCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoCheckBusy()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadCheckBusy:AMSTB_R_OSAL_RCFG_ThreadCheckBusy:inout:::] */
/*    R_OSAL_RCFG_ThreadCheckBusy => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadCheckBusy()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_timeclock.c:R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod:AMSTB_R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod:inout:::] */
/*    R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_ClockTimeGetTimeoutByTimePeriod(long long time_period, struct st_osal_time *p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_time volatile AMIN_p_time_stamp;
    static long long volatile AMOUT_time_period;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_time_period = time_period;
    AMOUT_p_time_stamp = p_time_stamp;
    *p_time_stamp = AMIN_p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_timeclock.c:R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp:AMSTB_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp:inout:::] */
/*    R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp(struct st_osal_time *p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_time volatile AMIN_p_time_stamp;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    *p_time_stamp = AMIN_p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_timeclock.c:R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp:AMSTB_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp_multiple:inout:::] */
/*    R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp_multiple(struct st_osal_time *p_time_stamp)
{
#define AMCALLMAX_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp 10
    static int volatile AM_count = 0;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp];
    static struct st_osal_time volatile AMIN_p_time_stamp[AMCALLMAX_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp];
    static struct st_osal_time *volatile AMOUT_p_time_stamp[AMCALLMAX_R_OSAL_OS_ClockTimeGetHighPrecisTimeStamp];
    AMOUT_p_time_stamp[AM_count] = p_time_stamp;
    *p_time_stamp = AMIN_p_time_stamp[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptGetNumOfIrqChannels:AMSTB_R_OSAL_DCFG_InterruptGetNumOfIrqChannels:inout:::] */
/*    R_OSAL_DCFG_InterruptGetNumOfIrqChannels => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptGetNumOfIrqChannels(struct st_osal_device_control *handle,unsigned long long *p_numOfChannels)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long *volatile AMOUT_p_numOfChannels;
    AMOUT_handle = handle;
    AMOUT_p_numOfChannels = p_numOfChannels;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptRegisterIsr:AMSTB_R_OSAL_DCFG_InterruptRegisterIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptRegisterIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptRegisterIsr(struct st_osal_device_control *handle,unsigned long long irq_channel,int priority_value,void *irqHandlerFunction,void *p_irq_arg)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptUnregisterIsr:AMSTB_R_OSAL_DCFG_InterruptUnregisterIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptUnregisterIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptUnregisterIsr(struct st_osal_device_control *handle,unsigned long long irq_channel,void *irqHandlerFunction)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static void *volatile AMOUT_irqHandlerFunction;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptEnableIsr:AMSTB_R_OSAL_DCFG_InterruptEnableIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptEnableIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptDisableIsr:AMSTB_R_OSAL_DCFG_InterruptDisableIsr:inout:::] */
/*    R_OSAL_DCFG_InterruptDisableIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptDisableIsr(struct st_osal_device_control *handle,unsigned long long irq_channel)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptIsISRContext:AMSTB_R_OSAL_DCFG_InterruptIsISRContext:inout:::] */
/*    R_OSAL_DCFG_InterruptIsISRContext => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptIsISRContext(unsigned char *p_is_isr)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_is_isr;
    AMOUT_p_is_isr = p_is_isr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_interrupt.c:R_OSAL_DCFG_InterruptSetIsrPriority:AMSTB_R_OSAL_DCFG_InterruptSetIsrPriority:inout:::] */
/*    R_OSAL_DCFG_InterruptSetIsrPriority => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_InterruptSetIsrPriority(struct st_osal_device_control *handle,unsigned long long irq_channel,int priority_value)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_irq_channel;
    static int volatile AMOUT_priority_value;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    return AMIN_return;
}

void AMSTB_osal_internal_interrupt_execute_userfunction_irq_func(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    static osal_device_handle_t volatile AMOUT_device_handle;
    static uint64_t volatile AMOUT_irq_channel;
    static void* volatile AMOUT_irq_arg;
    AMOUT_device_handle = device_handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_irq_arg = irq_arg;
    return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfDevices:AMSTB_R_OSAL_DCFG_IoGetNumOfDevices:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfDevices => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetNumOfDevices(unsigned char *p_deviceType,unsigned long long *p_numOfDevices)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_deviceType;
    static unsigned long long *volatile AMOUT_p_numOfDevices;
    AMOUT_p_deviceType = p_deviceType;
    AMOUT_p_numOfDevices = p_numOfDevices;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetDeviceList:AMSTB_R_OSAL_DCFG_IoGetDeviceList:inout:::] */
/*    R_OSAL_DCFG_IoGetDeviceList => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetDeviceList(unsigned char *p_device_type_name,unsigned char *p_buffer,unsigned long long buffer_size,unsigned long long *p_num_of_byte)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_device_type_name;
    static unsigned char *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    static unsigned long long *volatile AMOUT_p_num_of_byte;
    AMOUT_p_device_type_name = p_device_type_name;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoDeviceOpen:AMSTB_R_OSAL_DCFG_IoDeviceOpen:inout:::] */
/*    R_OSAL_DCFG_IoDeviceOpen => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoDeviceOpen(unsigned char *p_id,struct st_osal_device_control **p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_id;
    static struct st_osal_device_control **volatile AMOUT_p_handle;
    AMOUT_p_id = p_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoDeviceClose:AMSTB_R_OSAL_DCFG_IoDeviceClose:inout:::] */
/*    R_OSAL_DCFG_IoDeviceClose => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoDeviceClose(struct st_osal_device_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockRead8:AMSTB_R_OSAL_IoBlockRead8:inout:::] */
/*    R_OSAL_IoBlockRead8 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockRead8(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockRead16:AMSTB_R_OSAL_IoBlockRead16:inout:::] */
/*    R_OSAL_IoBlockRead16 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockRead16(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockRead32:AMSTB_R_OSAL_IoBlockRead32:inout:::] */
/*    R_OSAL_IoBlockRead32 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockRead32(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockRead64:AMSTB_R_OSAL_IoBlockRead64:inout:::] */
/*    R_OSAL_IoBlockRead64 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockRead64(osal_device_handle_t handle, uintptr_t offset, uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockWrite8:AMSTB_R_OSAL_IoBlockWrite8:inout:::] */
/*    R_OSAL_IoBlockWrite8 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockWrite8(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockWrite16:AMSTB_R_OSAL_IoBlockWrite16:inout:::] */
/*    R_OSAL_IoBlockWrite16 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockWrite16(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockWrite32:AMSTB_R_OSAL_IoBlockWrite32:inout:::] */
/*    R_OSAL_IoBlockWrite32 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockWrite32(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_io.c:R_OSAL_IoBlockWrite64:AMSTB_R_OSAL_IoBlockWrite64:inout:::] */
/*    R_OSAL_IoBlockWrite64 => Stub */
e_osal_return_t AMSTB_R_OSAL_IoBlockWrite64(osal_device_handle_t handle, uintptr_t offset, const uint8_t * p_data, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    static void *volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoRead:AMSTB_R_OSAL_DCFG_IoRead:inout:::] */
/*    R_OSAL_DCFG_IoRead => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoRead(struct st_osal_device_control *handle,unsigned long long offset,unsigned long long size,unsigned long long unit_size,void *p_data)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoWrite:AMSTB_R_OSAL_DCFG_IoWrite:inout:::] */
/*    R_OSAL_DCFG_IoWrite => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoWrite(struct st_osal_device_control *handle,unsigned long long offset,unsigned long long size,unsigned long long unit_size,void *p_data)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetDeviceAxiBusId:AMSTB_R_OSAL_DCFG_IoGetDeviceAxiBusId:inout:::] */
/*    R_OSAL_DCFG_IoGetDeviceAxiBusId => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetDeviceAxiBusId(struct st_osal_device_control *device,unsigned long long *p_axi_id)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_device;
    static unsigned long long *volatile AMOUT_p_axi_id;
    AMOUT_device = device;
    AMOUT_p_axi_id = p_axi_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName:AMSTB_R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetAxiBusIdFromDeviceName(unsigned char *p_device_name,unsigned long long *p_axi_id)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long *volatile AMOUT_p_axi_id;
    AMOUT_p_device_name = p_device_name;
    AMOUT_p_axi_id = p_axi_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiBusNameList:AMSTB_R_OSAL_DCFG_IoGetAxiBusNameList:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiBusNameList => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetAxiBusNameList(unsigned char *p_buffer,unsigned long long buffer_size,unsigned long long *p_num_of_byte)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_buffer;
    static unsigned long long volatile AMOUT_buffer_size;
    static unsigned long long *volatile AMOUT_p_num_of_byte;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiBusName:AMSTB_R_OSAL_DCFG_IoGetAxiBusName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiBusName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetAxiBusName(unsigned long long axi_id,unsigned char *p_buffer,unsigned long long buffer_size,unsigned long long *p_num_of_byte)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName:AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForAxiBusName(unsigned char *p_axi_bus_name,unsigned long long *p_num_of_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_axi_bus_name;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiIdListForAxiBusName:AMSTB_R_OSAL_DCFG_IoGetAxiIdListForAxiBusName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiIdListForAxiBusName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetAxiIdListForAxiBusName(unsigned char *p_axi_bus_name,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_written_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl:AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceHdl(struct st_osal_device_control *device_hdl,unsigned long long *p_num_of_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_device_hdl;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    AMOUT_device_hdl = device_hdl;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl:AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceHdl(struct st_osal_device_control *device_hdl,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_written_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_device_hdl;
    static unsigned long long *volatile AMOUT_p_array;
    static unsigned long long volatile AMOUT_array_size;
    static unsigned long long *volatile AMOUT_p_num_of_written_axi_ids;
    AMOUT_device_hdl = device_hdl;
    AMOUT_p_array = p_array;
    AMOUT_array_size = array_size;
    AMOUT_p_num_of_written_axi_ids = p_num_of_written_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName:AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName:inout:::] */
/*    R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetNumOfAxiIdsForDeviceName(unsigned char *p_device_name,unsigned long long *p_num_of_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned char *volatile AMOUT_p_device_name;
    static unsigned long long *volatile AMOUT_p_num_of_axi_ids;
    AMOUT_p_device_name = p_device_name;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:R_OSAL_DCFG_IoGetAxiIdListForDeviceName:AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceName:inout:::] */
/*    R_OSAL_DCFG_IoGetAxiIdListForDeviceName => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_IoGetAxiIdListForDeviceName(unsigned char *p_device_name,unsigned long long *p_array,unsigned long long array_size,unsigned long long *p_num_of_written_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
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

uintptr_t meml_alloc_dummy(st_meml_manage_t* p_manage, size_t size, size_t alignment)
{
    (void)p_manage;
    (void)size;
    (void)alignment;
    return 1;
}

e_meml_return_t meml_dealloc_dummy(st_meml_manage_t* p_manage, uintptr_t address)
{
    (void)p_manage;
    (void)address;
    return 0;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetDefaultIdx:AMSTB_R_OSAL_RCFG_MmngrGetDefaultIdx:inout:::] */
/*    R_OSAL_RCFG_MmngrGetDefaultIdx => Stub */
unsigned long long AMSTB_R_OSAL_RCFG_MmngrGetDefaultIdx()
{
    static unsigned long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetNumOfRegions:AMSTB_R_OSAL_RCFG_MmngrGetNumOfRegions:inout:::] */
/*    R_OSAL_RCFG_MmngrGetNumOfRegions => Stub */
unsigned long long AMSTB_R_OSAL_RCFG_MmngrGetNumOfRegions()
{
    static unsigned long long volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrOpen:AMSTB_R_OSAL_RCFG_MmngrOpen:inout:::] */
/*    R_OSAL_RCFG_MmngrOpen => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrOpen(struct st_osal_mmngr_config *p_config,unsigned long long region_idx,struct st_osal_memory_manager_obj **p_hndl_mmngr)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_region_idx;
    static struct st_osal_memory_manager_obj **volatile AMOUT_p_hndl_mmngr;
    AMOUT_p_config = p_config;
    AMOUT_region_idx = region_idx;
    AMOUT_p_hndl_mmngr = p_hndl_mmngr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open:AMSTB_r_osal_rcfg_mmngr_open:inout:::] */
/*    r_osal_rcfg_mmngr_open => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_open(struct st_osal_mmngr_config *p_usr_config,unsigned long long region_idx)
{
    static int volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_usr_config;
    static unsigned long long volatile AMOUT_region_idx;
    AMOUT_p_usr_config = p_usr_config;
    AMOUT_region_idx = region_idx;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open_malloc:AMSTB_r_osal_rcfg_mmngr_open_malloc:inout:::] */
/*    r_osal_rcfg_mmngr_open_malloc => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_open_malloc(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mmngr_config *p_usr_config)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_mmngr_config *volatile AMOUT_p_usr_config;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_p_usr_config = p_usr_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_open_map:AMSTB_r_osal_rcfg_mmngr_open_map:inout:::] */
/*    r_osal_rcfg_mmngr_open_map => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_open_map(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mmngr_config *p_usr_config,unsigned long long region_idx)
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrClose:AMSTB_R_OSAL_RCFG_MmngrClose:inout:::] */
/*    R_OSAL_RCFG_MmngrClose => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrClose(struct st_osal_memory_manager_obj *hndl_mmngr)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    AMOUT_hndl_mmngr = hndl_mmngr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_close:AMSTB_r_osal_rcfg_mmngr_close:inout:::] */
/*    r_osal_rcfg_mmngr_close => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_close(struct st_osal_memory_manager_obj *hndl_mmngr)
{
    static int volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    AMOUT_hndl_mmngr = hndl_mmngr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrAlloc:AMSTB_R_OSAL_RCFG_MmngrAlloc:inout:::] */
/*    R_OSAL_RCFG_MmngrAlloc => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrAlloc(struct st_osal_memory_manager_obj *hndl_mmngr,unsigned long long size,unsigned long long alignment,struct st_osal_memory_buffer_obj **p_buffer_obj)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_fill_guard_area:AMSTB_r_osal_rcfg_mmngr_alloc_fill_guard_area:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_fill_guard_area => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_alloc_fill_guard_area(int mode,struct st_osal_mem_buf_ptr_self *p_buffer_info)
{
    static int volatile AMIN_return;
    static int volatile AMOUT_mode;
    static struct st_osal_mem_buf_ptr_self *volatile AMOUT_p_buffer_info;
    AMOUT_mode = mode;
    AMOUT_p_buffer_info = p_buffer_info;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_alloc_lib:AMSTB_r_osal_rcfg_mmngr_alloc_lib:inout:::] */
/*    r_osal_rcfg_mmngr_alloc_lib => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_alloc_lib(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mem_buf_ptr_self *p_buffer_info,unsigned long long bytes,unsigned long long alignment)
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrDealloc:AMSTB_R_OSAL_RCFG_MmngrDealloc:inout:::] */
/*    R_OSAL_RCFG_MmngrDealloc => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrDealloc(struct st_osal_memory_manager_obj *hndl_mmngr,struct st_osal_memory_buffer_obj *buffer_obj)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_buffer_obj = buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_dealloc_proc:AMSTB_r_osal_rcfg_mmngr_dealloc_proc:inout:::] */
/*    r_osal_rcfg_mmngr_dealloc_proc => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_dealloc_proc(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_memory_buffer_obj *buffer_obj)
{
    static int volatile AMIN_return;
    static struct st_osal_mem_mmngr_ptr_self *volatile AMOUT_p_mngr_control;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_buffer_obj;
    AMOUT_p_mngr_control = p_mngr_control;
    AMOUT_buffer_obj = buffer_obj;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_find_free_buffer_info:AMSTB_r_osal_rcfg_mmngr_find_free_buffer_info:inout:::] */
/*    r_osal_rcfg_mmngr_find_free_buffer_info => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_find_free_buffer_info(struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_mem_buf_ptr_self **pp_buffer_info)
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_MmngrGetOsalMaxConfig:AMSTB_R_OSAL_MmngrGetOsalMaxConfig:inout:::] */
/*    R_OSAL_MmngrGetOsalMaxConfig => Stub */
e_osal_return_t AMSTB_R_OSAL_MmngrGetOsalMaxConfig(st_osal_mmngr_config_t *const p_config)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetConfig:AMSTB_R_OSAL_RCFG_MmngrGetConfig:inout:::] */
/*    R_OSAL_RCFG_MmngrGetConfig => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetConfig(struct st_osal_memory_manager_obj *hndl_mmngr,struct st_osal_mmngr_config *p_config)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetOsalMaxConfig:AMSTB_R_OSAL_RCFG_MmngrGetOsalMaxConfig:inout:::] */
/*    R_OSAL_RCFG_MmngrGetOsalMaxConfig => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetOsalMaxConfig(unsigned long long region_idx,struct st_osal_mmngr_config *p_config)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned long long volatile AMOUT_region_idx;
    static struct st_osal_mmngr_config *volatile AMOUT_p_config;
    AMOUT_region_idx = region_idx;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrRegisterMonitor:AMSTB_R_OSAL_RCFG_MmngrRegisterMonitor:inout:::] */
/*    R_OSAL_RCFG_MmngrRegisterMonitor => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrRegisterMonitor(struct st_osal_memory_manager_obj *hndl_mmngr,int monitored_event,void *monitor_handler,void *p_user_arg)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_send_monitor_event:AMSTB_r_osal_rcfg_mmngr_send_monitor_event:inout:::] */
/*    r_osal_rcfg_mmngr_send_monitor_event => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_send_monitor_event(struct st_osal_os_mutex *mutex_id,int event_id,int error_code,struct st_osal_mem_mmngr_ptr_self *p_mngr_control,struct st_osal_memory_buffer_obj *buffer_hndl)
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetCpuPtr:AMSTB_R_OSAL_RCFG_MmngrGetCpuPtr:inout:::] */
/*    R_OSAL_RCFG_MmngrGetCpuPtr => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetCpuPtr(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,void **pp_cpu_ptr)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static void **volatile AMOUT_pp_cpu_ptr;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_pp_cpu_ptr = pp_cpu_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetConstCpuPtr:AMSTB_R_OSAL_RCFG_MmngrGetConstCpuPtr:inout:::] */
/*    R_OSAL_RCFG_MmngrGetConstCpuPtr => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetConstCpuPtr(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,void **pp_const_cpu_ptr)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static void **volatile AMOUT_pp_const_cpu_ptr;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_pp_const_cpu_ptr = pp_const_cpu_ptr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrFlush:AMSTB_R_OSAL_RCFG_MmngrFlush:inout:::] */
/*    R_OSAL_RCFG_MmngrFlush => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrFlush(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long offset,unsigned long long size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_offset = offset;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrInvalidate:AMSTB_R_OSAL_RCFG_MmngrInvalidate:inout:::] */
/*    R_OSAL_RCFG_MmngrInvalidate => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrInvalidate(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long offset,unsigned long long size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_offset;
    static unsigned long long volatile AMOUT_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_offset = offset;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetHwAddr:AMSTB_R_OSAL_RCFG_MmngrGetHwAddr:inout:::] */
/*    R_OSAL_RCFG_MmngrGetHwAddr => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetHwAddr(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long axi_id,unsigned long long *p_hw_addr)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long volatile AMOUT_axi_id;
    static unsigned long long *volatile AMOUT_p_hw_addr;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_axi_id = axi_id;
    AMOUT_p_hw_addr = p_hw_addr;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrGetSize:AMSTB_R_OSAL_RCFG_MmngrGetSize:inout:::] */
/*    R_OSAL_RCFG_MmngrGetSize => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrGetSize(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl,unsigned long long *p_size)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    static unsigned long long *volatile AMOUT_p_size;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    AMOUT_p_size = p_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrCheckBuffer:AMSTB_R_OSAL_RCFG_MmngrCheckBuffer:inout:::] */
/*    R_OSAL_RCFG_MmngrCheckBuffer => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrCheckBuffer(struct st_osal_memory_buffer_obj *memory_buffer_obj_hndl)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_memory_buffer_obj_hndl;
    AMOUT_memory_buffer_obj_hndl = memory_buffer_obj_hndl;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrDebugMmngrPrint:AMSTB_R_OSAL_RCFG_MmngrDebugMmngrPrint:inout:::] */
/*    R_OSAL_RCFG_MmngrDebugMmngrPrint => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrDebugMmngrPrint(struct st_osal_memory_manager_obj *hndl_mmngr, FILE *p_output)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static FILE *volatile AMOUT_p_output;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_p_output = p_output;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:R_OSAL_RCFG_MmngrDebugBufferPrint:AMSTB_R_OSAL_RCFG_MmngrDebugBufferPrint:inout:::] */
/*    R_OSAL_RCFG_MmngrDebugBufferPrint => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MmngrDebugBufferPrint(struct st_osal_memory_buffer_obj *hndl_buffer, FILE *p_output)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_memory_buffer_obj *volatile AMOUT_hndl_buffer;
    static FILE *volatile AMOUT_p_output;
    AMOUT_hndl_buffer = hndl_buffer;
    AMOUT_p_output = p_output;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_convert_unit:AMSTB_r_osal_rcfg_mmngr_convert_unit:inout:::] */
/*    r_osal_rcfg_mmngr_convert_unit => Stub */
e_osal_return_t AMSTB_r_osal_rcfg_mmngr_convert_unit(unsigned long long size,unsigned char *p_output,unsigned long long array_size)
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

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_cmn_check_buffer_handle:AMSTB_r_osal_rcfg_mmngr_cmn_check_buffer_handle:inout:::] */
/*    r_osal_rcfg_mmngr_cmn_check_buffer_handle => Stub */
int AMSTB_r_osal_rcfg_mmngr_cmn_check_buffer_handle(void *p_ptr_self)
{
    static int volatile AMIN_return;
    static void *volatile AMOUT_p_ptr_self;
    AMOUT_p_ptr_self = p_ptr_self;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_memory.c:r_osal_rcfg_mmngr_cmn_get_handle_state:AMSTB_r_osal_rcfg_mmngr_cmn_get_handle_state:inout:::] */
/*    r_osal_rcfg_mmngr_cmn_get_handle_state => Stub */
int AMSTB_r_osal_rcfg_mmngr_cmn_get_handle_state(struct st_osal_memory_manager_obj *hndl_mmngr,int *p_state)
{
    static int volatile AMIN_return;
    static int volatile AMIN_p_state;
    static struct st_osal_memory_manager_obj *volatile AMOUT_hndl_mmngr;
    static int *volatile AMOUT_p_state;
    AMOUT_hndl_mmngr = hndl_mmngr;
    AMOUT_p_state = p_state;
    *p_state = AMIN_p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_MqSendUntilTimeStamp:AMSTB_R_OSAL_HIF_MqSendUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_MqSendUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_MqSendUntilTimeStamp(osal_os_mq_t mq, const st_osal_time_t* const p_time_stamp, const void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_hif_message.c:R_OSAL_HIF_MqReceiveUntilTimeStamp:AMSTB_R_OSAL_HIF_MqReceiveUntilTimeStamp:inout:::] */
/*    R_OSAL_HIF_MqReceiveUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_HIF_MqReceiveUntilTimeStamp(osal_os_mq_t mq, const st_osal_time_t* const p_time_stamp, void* p_buffer, size_t buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_os_mq_t volatile AMOUT_mq;
    static st_osal_time_t* volatile AMOUT_p_time_stamp;
    static void* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    AMOUT_mq = mq;
    AMOUT_p_time_stamp = p_time_stamp;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqCreate:AMSTB_R_OSAL_RCFG_MqCreate:inout:::] */
/*    R_OSAL_RCFG_MqCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqCreate(struct st_osal_mq_config *p_config,unsigned long long mq_Id,struct st_osal_mq_control **p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mq_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_mq_Id;
    static struct st_osal_mq_control **volatile AMOUT_p_handle;
    AMOUT_p_config = p_config;
    AMOUT_mq_Id = mq_Id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqDelete:AMSTB_R_OSAL_RCFG_MqDelete:inout:::] */
/*    R_OSAL_RCFG_MqDelete => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqDelete(struct st_osal_mq_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqGetConfig:AMSTB_R_OSAL_RCFG_MqGetConfig:inout:::] */
/*    R_OSAL_RCFG_MqGetConfig => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqGetConfig(struct st_osal_mq_control *handle,struct st_osal_mq_config *p_config)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static struct st_osal_mq_config *volatile AMOUT_p_config;
    AMOUT_handle = handle;
    AMOUT_p_config = p_config;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqIsEmpty:AMSTB_R_OSAL_RCFG_MqIsEmpty:inout:::] */
/*    R_OSAL_RCFG_MqIsEmpty => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqIsEmpty(struct st_osal_mq_control *handle,unsigned char *p_result)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_handle = handle;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqIsFull:AMSTB_R_OSAL_RCFG_MqIsFull:inout:::] */
/*    R_OSAL_RCFG_MqIsFull => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqIsFull(struct st_osal_mq_control *handle,unsigned char *p_result)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_handle = handle;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqReceiveForTimePeriod:AMSTB_R_OSAL_RCFG_MqReceiveForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_MqReceiveForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqReceiveForTimePeriod(struct st_osal_mq_control *handle,long long time_period,void *p_buffer,unsigned long long buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqReceiveUntilTimeStamp:AMSTB_R_OSAL_RCFG_MqReceiveUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_MqReceiveUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqReceiveUntilTimeStamp(struct st_osal_mq_control *handle,struct st_osal_time *p_time_stamp,void *p_buffer,unsigned long long buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqReset:AMSTB_R_OSAL_RCFG_MqReset:inout:::] */
/*    R_OSAL_RCFG_MqReset => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqReset(struct st_osal_mq_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mq_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqSendForTimePeriod:AMSTB_R_OSAL_RCFG_MqSendForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_MqSendForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqSendForTimePeriod(struct st_osal_mq_control *handle,long long time_period,void *p_buffer,unsigned long long buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_res_cfg_message.c:R_OSAL_RCFG_MqSendUntilTimeStamp:AMSTB_R_OSAL_RCFG_MqSendUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_MqSendUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MqSendUntilTimeStamp(struct st_osal_mq_control *handle,struct st_osal_time *p_time_stamp,void *p_buffer,unsigned long long buffer_size)
{
    static e_osal_return_t volatile AMIN_return;
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

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetRequiredState:AMSTB_R_OSAL_DCFG_PmSetRequiredState:inout:::] */
/*    R_OSAL_DCFG_PmSetRequiredState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmSetRequiredState(struct st_osal_device_control *handle,int state,unsigned char applyImmediate)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_state;
    static unsigned char volatile AMOUT_applyImmediate;
    AMOUT_handle = handle;
    AMOUT_state = state;
    AMOUT_applyImmediate = applyImmediate;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetRequiredState:AMSTB_R_OSAL_DCFG_PmGetRequiredState:inout:::] */
/*    R_OSAL_DCFG_PmGetRequiredState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmGetRequiredState(struct st_osal_device_control *handle,int *p_state)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmWaitForState:AMSTB_R_OSAL_DCFG_PmWaitForState:inout:::] */
/*    R_OSAL_DCFG_PmWaitForState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmWaitForState(struct st_osal_device_control *handle,int state,long long time_period)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_state;
    static long long volatile AMOUT_time_period;
    AMOUT_handle = handle;
    AMOUT_state = state;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetState:AMSTB_R_OSAL_DCFG_PmGetState:inout:::] */
/*    R_OSAL_DCFG_PmGetState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmGetState(struct st_osal_device_control *handle,int *p_state)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetLowestIntermediatePowerState:AMSTB_R_OSAL_DCFG_PmGetLowestIntermediatePowerState:inout:::] */
/*    R_OSAL_DCFG_PmGetLowestIntermediatePowerState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmGetLowestIntermediatePowerState(struct st_osal_device_control *handle,int *p_state)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetPolicy:AMSTB_R_OSAL_DCFG_PmSetPolicy:inout:::] */
/*    R_OSAL_DCFG_PmSetPolicy => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmSetPolicy(struct st_osal_device_control *handle,int policy,unsigned char applyImmediate)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_policy;
    static unsigned char volatile AMOUT_applyImmediate;
    AMOUT_handle = handle;
    AMOUT_policy = policy;
    AMOUT_applyImmediate = applyImmediate;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetPolicy:AMSTB_R_OSAL_DCFG_PmGetPolicy:inout:::] */
/*    R_OSAL_DCFG_PmGetPolicy => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmGetPolicy(struct st_osal_device_control *handle,int *p_policy)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_policy;
    AMOUT_handle = handle;
    AMOUT_p_policy = p_policy;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetResetState:AMSTB_R_OSAL_DCFG_PmSetResetState:inout:::] */
/*    R_OSAL_DCFG_PmSetResetState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmSetResetState(struct st_osal_device_control *handle,int state)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int volatile AMOUT_state;
    AMOUT_handle = handle;
    AMOUT_state = state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmGetResetState:AMSTB_R_OSAL_DCFG_PmGetResetState:inout:::] */
/*    R_OSAL_DCFG_PmGetResetState => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmGetResetState(struct st_osal_device_control *handle,int *p_state)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_handle;
    static int *volatile AMOUT_p_state;
    AMOUT_handle = handle;
    AMOUT_p_state = p_state;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_power.c:R_OSAL_DCFG_PmSetPostClock:AMSTB_R_OSAL_DCFG_PmSetPostClock:inout:::] */
/*    R_OSAL_DCFG_PmSetPostClock => Stub */
e_osal_return_t AMSTB_R_OSAL_DCFG_PmSetPostClock(struct st_osal_device_control *device,unsigned char clock_enable,unsigned int clock_divider)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_device_control *volatile AMOUT_device;
    static unsigned char volatile AMOUT_clock_enable;
    static unsigned int volatile AMOUT_clock_divider;
    AMOUT_device = device;
    AMOUT_clock_enable = clock_enable;
    AMOUT_clock_divider = clock_divider;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadCreate:AMSTB_R_OSAL_RCFG_ThreadCreate:inout:::] */
/*    R_OSAL_RCFG_ThreadCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadCreate(struct st_osal_thread_config *p_config,unsigned long long thread_id,struct st_osal_thread_control **p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_thread_config *volatile AMOUT_p_config;
    static unsigned long long volatile AMOUT_thread_id;
    static struct st_osal_thread_control **volatile AMOUT_p_handle;
    AMOUT_p_config = p_config;
    AMOUT_thread_id = thread_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadEqual:AMSTB_R_OSAL_RCFG_ThreadEqual:inout:::] */
/*    R_OSAL_RCFG_ThreadEqual => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadEqual(struct st_osal_thread_control *handle1,struct st_osal_thread_control *handle2,unsigned char *p_result)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_handle1;
    static struct st_osal_thread_control *volatile AMOUT_handle2;
    static unsigned char *volatile AMOUT_p_result;
    AMOUT_handle1 = handle1;
    AMOUT_handle2 = handle2;
    AMOUT_p_result = p_result;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadJoin:AMSTB_R_OSAL_RCFG_ThreadJoin:inout:::] */
/*    R_OSAL_RCFG_ThreadJoin => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadJoin(struct st_osal_thread_control *handle,long long *p_return_value)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_thread_control *volatile AMOUT_handle;
    static long long *volatile AMOUT_p_return_value;
    AMOUT_handle = handle;
    AMOUT_p_return_value = p_return_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSelf:AMSTB_R_OSAL_RCFG_ThreadSelf:inout:::] */
/*    R_OSAL_RCFG_ThreadSelf => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadSelf(struct st_osal_thread_control **p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_thread_control **volatile AMOUT_p_handle;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSleepForTimePeriod:AMSTB_R_OSAL_RCFG_ThreadSleepForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_ThreadSleepForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadSleepForTimePeriod(long long time_period)
{
    static e_osal_return_t volatile AMIN_return;
    static long long volatile AMOUT_time_period;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_thread.c:R_OSAL_RCFG_ThreadSleepUntilTimeStamp:AMSTB_R_OSAL_RCFG_ThreadSleepUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_ThreadSleepUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ThreadSleepUntilTimeStamp(struct st_osal_time *p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondBroadcast:AMSTB_R_OSAL_RCFG_CondBroadcast:inout:::] */
/*    R_OSAL_RCFG_CondBroadcast => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_CondBroadcast(struct st_osal_cond_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondCreate:AMSTB_R_OSAL_RCFG_CondCreate:inout:::] */
/*    R_OSAL_RCFG_CondCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_CondCreate(unsigned long long cond_id,struct st_osal_cond_control **p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned long long volatile AMOUT_cond_id;
    static struct st_osal_cond_control **volatile AMOUT_p_handle;
    AMOUT_cond_id = cond_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondDestroy:AMSTB_R_OSAL_RCFG_CondDestroy:inout:::] */
/*    R_OSAL_RCFG_CondDestroy => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_CondDestroy(struct st_osal_cond_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondWaitForTimePeriod:AMSTB_R_OSAL_RCFG_CondWaitForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_CondWaitForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_CondWaitForTimePeriod(struct st_osal_cond_control *cond_handle,struct st_osal_mutex_control *mutex_handle,long long time_period)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_cond_handle;
    static struct st_osal_mutex_control *volatile AMOUT_mutex_handle;
    static long long volatile AMOUT_time_period;
    AMOUT_cond_handle = cond_handle;
    AMOUT_mutex_handle = mutex_handle;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondWaitUntilTimeStamp:AMSTB_R_OSAL_RCFG_CondWaitUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_CondWaitUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_CondWaitUntilTimeStamp(struct st_osal_cond_control *cond_handle,struct st_osal_mutex_control *mutex_handle,struct st_osal_time *p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_cond_handle;
    static struct st_osal_mutex_control *volatile AMOUT_mutex_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_cond_handle = cond_handle;
    AMOUT_mutex_handle = mutex_handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_CondSignal:AMSTB_R_OSAL_RCFG_CondSignal:inout:::] */
/*    R_OSAL_RCFG_CondSignal => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_CondSignal(struct st_osal_cond_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_cond_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexCreate:AMSTB_R_OSAL_RCFG_MutexCreate:inout:::] */
/*    R_OSAL_RCFG_MutexCreate => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MutexCreate(unsigned long long mutex_id,struct st_osal_mutex_control **p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static unsigned long long volatile AMOUT_mutex_id;
    static struct st_osal_mutex_control **volatile AMOUT_p_handle;
    AMOUT_mutex_id = mutex_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexDestroy:AMSTB_R_OSAL_RCFG_MutexDestroy:inout:::] */
/*    R_OSAL_RCFG_MutexDestroy => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MutexDestroy(struct st_osal_mutex_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexLockForTimePeriod:AMSTB_R_OSAL_RCFG_MutexLockForTimePeriod:inout:::] */
/*    R_OSAL_RCFG_MutexLockForTimePeriod => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MutexLockForTimePeriod(struct st_osal_mutex_control *handle,long long time_period)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    static long long volatile AMOUT_time_period;
    AMOUT_handle = handle;
    AMOUT_time_period = time_period;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexLockUntilTimeStamp:AMSTB_R_OSAL_RCFG_MutexLockUntilTimeStamp:inout:::] */
/*    R_OSAL_RCFG_MutexLockUntilTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MutexLockUntilTimeStamp(struct st_osal_mutex_control *handle,struct st_osal_time *p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_handle = handle;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexTryLock:AMSTB_R_OSAL_RCFG_MutexTryLock:inout:::] */
/*    R_OSAL_RCFG_MutexTryLock => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MutexTryLock(struct st_osal_mutex_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MutexUnlock:AMSTB_R_OSAL_RCFG_MutexUnlock:inout:::] */
/*    R_OSAL_RCFG_MutexUnlock => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MutexUnlock(struct st_osal_mutex_control *handle)
{
    static e_osal_return_t volatile AMIN_return;
    static struct st_osal_mutex_control *volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_threadsync.c:R_OSAL_RCFG_MemoryBarrier:AMSTB_R_OSAL_RCFG_MemoryBarrier:inout:::] */
/*    R_OSAL_RCFG_MemoryBarrier => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_MemoryBarrier()
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_res_cfg_timeclock.c:R_OSAL_RCFG_ClockTimeGetTimeStamp:AMSTB_R_OSAL_RCFG_ClockTimeGetTimeStamp:inout:::] */
/*    R_OSAL_RCFG_ClockTimeGetTimeStamp => Stub */
e_osal_return_t AMSTB_R_OSAL_RCFG_ClockTimeGetTimeStamp(int clock_type,struct st_osal_time *p_time_stamp)
{
    static e_osal_return_t volatile AMIN_return;
    static int volatile AMOUT_clock_type;
    static struct st_osal_time *volatile AMOUT_p_time_stamp;
    AMOUT_clock_type = clock_type;
    AMOUT_p_time_stamp = p_time_stamp;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoInit:AMSTB_R_OSAL_OS_IoInit:inout:::] */
/*    R_OSAL_OS_IoInit => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoInit(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoDeinit:AMSTB_R_OSAL_OS_IoDeinit:inout:::] */
/*    R_OSAL_OS_IoDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoDeinit(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoGetNumOfDevices:AMSTB_R_OSAL_OS_IoGetNumOfDevices:inout:::] */
/*    R_OSAL_OS_IoGetNumOfDevices => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoGetNumOfDevices(const char* p_deviceType, size_t* const p_numOfDevices)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_deviceType;
    static size_t* volatile AMOUT_p_numOfDevices;
    AMOUT_p_deviceType = p_deviceType;
    AMOUT_p_numOfDevices = p_numOfDevices;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoGetDeviceList:AMSTB_R_OSAL_OS_IoGetDeviceList:inout:::] */
/*    R_OSAL_OS_IoGetDeviceList => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoGetDeviceList(const char* p_device_type_name, char* p_buffer, size_t buffer_size, size_t* const p_num_of_byte)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_device_type_name;
    static char* volatile AMOUT_p_buffer;
    static size_t volatile AMOUT_buffer_size;
    static size_t* volatile AMOUT_p_num_of_byte;
    AMOUT_p_device_type_name = p_device_type_name;
    AMOUT_p_buffer = p_buffer;
    AMOUT_buffer_size = buffer_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoDeviceOpen:AMSTB_R_OSAL_OS_IoDeviceOpen:inout:::] */
/*    R_OSAL_OS_IoDeviceOpen => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoDeviceOpen(const char* p_id, osal_device_handle_t* p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_id;
    static osal_device_handle_t* volatile AMOUT_p_handle;
    AMOUT_p_id = p_id;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoDeviceClose:AMSTB_R_OSAL_OS_IoDeviceClose:inout:::] */
/*    R_OSAL_OS_IoDeviceClose => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoDeviceClose(osal_device_handle_t handle)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoRead:AMSTB_R_OSAL_OS_IoRead:inout:::] */
/*    R_OSAL_OS_IoRead => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoRead(osal_device_handle_t const handle, uintptr_t offset, size_t size, size_t unit_size, void* p_data)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uintptr_t volatile AMOUT_offset;
    static size_t volatile AMOUT_size;
    static size_t volatile AMOUT_unit_size;
    static void* volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_unit_size = unit_size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoWrite:AMSTB_R_OSAL_OS_IoWrite:inout:::] */
/*    R_OSAL_OS_IoWrite => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoWrite(osal_device_handle_t const handle, uintptr_t offset, size_t size, size_t unit_size, const void* p_data)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uintptr_t volatile AMOUT_offset;
    static size_t volatile AMOUT_size;
    static size_t volatile AMOUT_unit_size;
    static void* volatile AMOUT_p_data;
    AMOUT_handle = handle;
    AMOUT_offset = offset;
    AMOUT_size = size;
    AMOUT_unit_size = unit_size;
    AMOUT_p_data = p_data;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:R_OSAL_OS_IoGetAxiBusInformation:AMSTB_R_OSAL_OS_IoGetAxiBusInformation:inout:::] */
/*    R_OSAL_OS_IoGetAxiBusInformation => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_IoGetAxiBusInformation(e_osal_get_axi_bus_type_t get_axi_bus_type, void* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static void* volatile AMIN_p_param;
    static e_osal_get_axi_bus_type_t volatile AMOUT_get_axi_bus_type;
    static void* volatile AMOUT_p_param;
    AMOUT_get_axi_bus_type = get_axi_bus_type;
    AMOUT_p_param = p_param;

    switch (get_axi_bus_type)
    {
        case OSAL_OS_IO_GET_DEVICE_AXI_BUS_ID :
            ((st_osal_os_io_get_device_axi_bus_id_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_device_axi_bus_id_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_AXI_BUS_ID_FROM_DEVICE_NAME :
            ((st_osal_os_io_get_axi_bus_id_from_device_name_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_axi_bus_id_from_device_name_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_AXI_BUS_NAME_LIST :
            ((st_osal_os_io_get_axi_bus_name_list_t *)p_param)->axi_bus_name_list_len = ((st_osal_os_io_get_axi_bus_name_list_t *)AMIN_p_param)->axi_bus_name_list_len;
            ((st_osal_os_io_get_axi_bus_name_list_t *)p_param)->p_axi_bus_name_list = ((st_osal_os_io_get_axi_bus_name_list_t *)AMIN_p_param)->p_axi_bus_name_list;
            break;
        case OSAL_OS_IO_GET_AXI_BUS_NAME :
            ((st_osal_os_io_get_axi_bus_name_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_axi_bus_name_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_AXI_BUS_NAME :
            ((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)p_param)->p_axi_bus_name = ((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)AMIN_p_param)->p_axi_bus_name;
            ((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)p_param)->pst_domain_table = ((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)AMIN_p_param)->pst_domain_table;
            ((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_AXI_ID_LIST_FOR_AXI_BUS_NAME :
            ((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)p_param)->p_axi_bus_name = ((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)AMIN_p_param)->p_axi_bus_name;
            ((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)p_param)->pst_domain_table = ((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)AMIN_p_param)->pst_domain_table;
            ((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_axi_id_list_for_axi_bus_name_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_HDL :
            ((st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_HDL :
            ((st_osal_os_io_get_axi_id_list_for_device_hdl_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_axi_id_list_for_device_hdl_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_NUM_OF_AXI_IDS_FOR_DEVICE_NAME :
            ((st_osal_os_io_get_num_of_axi_ids_for_device_name_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_num_of_axi_ids_for_device_name_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
        case OSAL_OS_IO_GET_AXI_ID_LIST_FOR_DEVICE_NAME :
        default :
            ((st_osal_os_io_get_axi_id_list_for_device_name_t *)p_param)->pst_axi_bus_name_info = ((st_osal_os_io_get_axi_id_list_for_device_name_t *)AMIN_p_param)->pst_axi_bus_name_info;
            break;
    }

    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_check_axiidlist_for_devicename:AMSTB_r_osal_dcfg_io_check_axiidlist_for_devicename:inout:::] */
/*    r_osal_dcfg_io_check_axiidlist_for_devicename => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_io_check_axiidlist_for_devicename(const char* p_device_name, const st_osal_axi_bus_name_info_t* pst_axi_bus_name_info, osal_axi_bus_id_t** pp_list_buffer, size_t* const p_num_of_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_axi_bus_id_t* volatile AMIN_pp_list_buffer;
    static size_t volatile AMIN_p_num_of_axi_ids;
    static char* volatile AMOUT_p_device_name;
    static st_osal_axi_bus_name_info_t* volatile AMOUT_pst_axi_bus_name_info;
    static osal_axi_bus_id_t** volatile AMOUT_pp_list_buffer;
    static size_t* volatile AMOUT_p_num_of_axi_ids;
    AMOUT_p_device_name = p_device_name;
    AMOUT_pst_axi_bus_name_info = pst_axi_bus_name_info;
    AMOUT_pp_list_buffer = pp_list_buffer;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    *pp_list_buffer = AMIN_pp_list_buffer;
    *p_num_of_axi_ids = AMIN_p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_check_axiidlist_for_domainname:AMSTB_r_osal_dcfg_io_check_axiidlist_for_domainname:inout:::] */
/*    r_osal_dcfg_io_check_axiidlist_for_domainname => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_io_check_axiidlist_for_domainname(const st_osal_io_domain_table_t* p_domain, const st_osal_axi_bus_name_info_t* pst_axi_bus_name_info, osal_axi_bus_id_t** pp_list_buffer, size_t* const p_num_of_axi_ids)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_axi_bus_id_t* volatile AMIN_pp_list_buffer;
    static size_t volatile AMIN_p_num_of_axi_ids;
    static st_osal_io_domain_table_t* volatile AMOUT_p_domain;
    static st_osal_axi_bus_name_info_t* volatile AMOUT_pst_axi_bus_name_info;
    static osal_axi_bus_id_t** volatile AMOUT_pp_list_buffer;
    static size_t* volatile AMOUT_p_num_of_axi_ids;
    AMOUT_p_domain = p_domain;
    AMOUT_pst_axi_bus_name_info = pst_axi_bus_name_info;
    AMOUT_pp_list_buffer = pp_list_buffer;
    AMOUT_p_num_of_axi_ids = p_num_of_axi_ids;
    *pp_list_buffer = AMIN_pp_list_buffer;
    *p_num_of_axi_ids = AMIN_p_num_of_axi_ids;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_dev_cfg_io.c:r_osal_dcfg_io_add_axiid_to_list:AMSTB_r_osal_dcfg_io_add_axiid_to_list:inout:::] */
/*    r_osal_dcfg_io_add_axiid_to_list => Stub */
e_osal_return_t AMSTB_r_osal_dcfg_io_add_axiid_to_list(osal_axi_bus_id_t** pp_list_buffer, size_t* p_dst_index, size_t count)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_axi_bus_id_t** volatile AMOUT_pp_list_buffer;
    static size_t* volatile AMOUT_p_dst_index;
    static size_t volatile AMOUT_count;
    AMOUT_pp_list_buffer = pp_list_buffer;
    AMOUT_p_dst_index = p_dst_index;
    AMOUT_count = count;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptInit:AMSTB_R_OSAL_OS_InterruptInit:inout:::] */
/*    R_OSAL_OS_InterruptInit => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptInit(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptDeinit:AMSTB_R_OSAL_OS_InterruptDeinit:inout:::] */
/*    R_OSAL_OS_InterruptDeinit => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptDeinit(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptRegisterIsr:AMSTB_R_OSAL_OS_InterruptRegisterIsr:inout:::] */
/*    R_OSAL_OS_InterruptRegisterIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptRegisterIsr(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value, p_osal_isr_func_t irqHandlerFunction, void* p_irq_arg)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uint64_t volatile AMOUT_irq_channel;
    static e_osal_interrupt_priority_t volatile AMOUT_priority_value;
    static p_osal_isr_func_t volatile AMOUT_irqHandlerFunction;
    static void* volatile AMOUT_p_irq_arg;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    AMOUT_p_irq_arg = p_irq_arg;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptUnregisterIsr:AMSTB_R_OSAL_OS_InterruptUnregisterIsr:inout:::] */
/*    R_OSAL_OS_InterruptUnregisterIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptUnregisterIsr(osal_device_handle_t handle, uint64_t irq_channel, p_osal_isr_func_t irqHandlerFunction)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uint64_t volatile AMOUT_irq_channel;
    static p_osal_isr_func_t volatile AMOUT_irqHandlerFunction;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_irqHandlerFunction = irqHandlerFunction;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptEnableIsr:AMSTB_R_OSAL_OS_InterruptEnableIsr:inout:::] */
/*    R_OSAL_OS_InterruptEnableIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptEnableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uint64_t volatile AMOUT_irq_channel;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptDisableIsr:AMSTB_R_OSAL_OS_InterruptDisableIsr:inout:::] */
/*    R_OSAL_OS_InterruptDisableIsr => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptDisableIsr(osal_device_handle_t handle, uint64_t irq_channel)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uint64_t volatile AMOUT_irq_channel;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:R_OSAL_OS_InterruptSetIsrPriority:AMSTB_R_OSAL_OS_InterruptSetIsrPriority:inout:::] */
/*    R_OSAL_OS_InterruptSetIsrPriority => Stub */
e_osal_return_t AMSTB_R_OSAL_OS_InterruptSetIsrPriority(osal_device_handle_t handle, uint64_t irq_channel, e_osal_interrupt_priority_t priority_value)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static uint64_t volatile AMOUT_irq_channel;
    static e_osal_interrupt_priority_t volatile AMOUT_priority_value;
    AMOUT_handle = handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_priority_value = priority_value;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_thread_start:AMSTB_r_osal_os_interrupt_thread_start:inout:::] */
/*    r_osal_os_interrupt_thread_start => Stub */
e_osal_return_t AMSTB_r_osal_os_interrupt_thread_start(st_osal_interrupt_control_t* p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_interrupt_control_t* volatile AMOUT_p_handle;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:r_osal_os_interrupt_execute_userfunction:AMSTB_r_osal_os_interrupt_execute_userfunction:inout:::] */
/*    r_osal_os_interrupt_execute_userfunction => Stub */
e_osal_return_t AMSTB_r_osal_os_interrupt_execute_userfunction(const st_osal_interrupt_control_t* p_handle)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_interrupt_control_t* volatile AMOUT_p_handle;
    AMOUT_p_handle = p_handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_interrupt.c:p_osal_isr_func_t:AMSTB_p_osal_isr_func_t:inout:::] */
/*    p_osal_isr_func_t => Stub */
void AMSTB_p_osal_isr_func_t(osal_device_handle_t device_handle, uint64_t irq_channel, void* irq_arg)
{
    static osal_device_handle_t volatile AMOUT_device_handle;
    static uint64_t volatile AMOUT_irq_channel;
    static void* volatile AMOUT_irq_arg;
    AMOUT_device_handle = device_handle;
    AMOUT_irq_channel = irq_channel;
    AMOUT_irq_arg = irq_arg;
    return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_read_uio_dir:AMSTB_r_osal_os_io_read_uio_dir:inout:::] */
/*    r_osal_os_io_read_uio_dir => Stub */
e_osal_return_t AMSTB_r_osal_os_io_read_uio_dir(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_initialize_axi_bus_info:AMSTB_r_osal_os_io_initialize_axi_bus_info:inout:::] */
/*    r_osal_os_io_initialize_axi_bus_info => Stub */
e_osal_return_t AMSTB_r_osal_os_io_initialize_axi_bus_info(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_initialize_device:AMSTB_r_osal_os_io_initialize_device:inout:::] */
/*    r_osal_os_io_initialize_device => Stub */
e_osal_return_t AMSTB_r_osal_os_io_initialize_device(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_num_of_device:AMSTB_r_osal_os_io_get_num_of_device:inout:::] */
/*    r_osal_os_io_get_num_of_device => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_num_of_device(const char* p_deviceType, size_t* const p_numOfDevices, glob_t const* p_glob_buffer)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_deviceType;
    static size_t* volatile AMOUT_p_numOfDevices;
    static glob_t* volatile AMOUT_p_glob_buffer;
    AMOUT_p_deviceType = p_deviceType;
    AMOUT_p_numOfDevices = p_numOfDevices;
    AMOUT_p_glob_buffer = p_glob_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_devicelist:AMSTB_r_osal_os_io_get_devicelist:inout:::] */
/*    r_osal_os_io_get_devicelist => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_devicelist(const char* p_device_type_name, char* p_array, size_t array_size, size_t* const p_num_of_byte, glob_t const* p_glob_buffer)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_device_type_name;
    static char* volatile AMOUT_p_array;
    static size_t volatile AMOUT_array_size;
    static size_t* volatile AMOUT_p_num_of_byte;
    static glob_t* volatile AMOUT_p_glob_buffer;
    AMOUT_p_device_type_name = p_device_type_name;
    AMOUT_p_array = p_array;
    AMOUT_array_size = array_size;
    AMOUT_p_num_of_byte = p_num_of_byte;
    AMOUT_p_glob_buffer = p_glob_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_open:AMSTB_r_osal_os_io_open:inout:::] */
/*    r_osal_os_io_open => Stub */
e_osal_return_t AMSTB_r_osal_os_io_open(osal_device_handle_t handle, const char* p_id)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static char* volatile AMOUT_p_id;
    AMOUT_handle = handle;
    AMOUT_p_id = p_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_open_error:AMSTB_r_osal_os_io_open_error:inout:::] */
/*    r_osal_os_io_open_error => Stub */
void AMSTB_r_osal_os_io_open_error(osal_device_handle_t handle)
{
    static int volatile AM_count;
    static osal_device_handle_t volatile AMOUT_handle;
    AMOUT_handle = handle;
    AM_count++;
    return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_unset_irq_information:AMSTB_r_osal_os_io_unset_irq_information:inout:::] */
/*    r_osal_os_io_unset_irq_information => Stub */
e_osal_return_t AMSTB_r_osal_os_io_unset_irq_information(osal_device_handle_t handle)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_close_post_fd:AMSTB_r_osal_os_io_close_post_fd:inout:::] */
/*    r_osal_os_io_close_post_fd => Stub */
e_osal_return_t AMSTB_r_osal_os_io_close_post_fd(osal_device_handle_t handle)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    AMOUT_handle = handle;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_device_axi_bus_id:AMSTB_r_osal_os_io_get_device_axi_bus_id:inout:::] */
/*    r_osal_os_io_get_device_axi_bus_id => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_device_axi_bus_id(st_osal_os_io_get_device_axi_bus_id_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_device_axi_bus_id_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_axi_bus_id_from_device_name:AMSTB_r_osal_os_io_get_axi_bus_id_from_device_name:inout:::] */
/*    r_osal_os_io_get_axi_bus_id_from_device_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_axi_bus_id_from_device_name(st_osal_os_io_get_axi_bus_id_from_device_name_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_axi_bus_id_from_device_name_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_axi_bus_name_list:AMSTB_r_osal_os_io_get_axi_bus_name_list:inout:::] */
/*    r_osal_os_io_get_axi_bus_name_list => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_axi_bus_name_list(st_osal_os_io_get_axi_bus_name_list_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_axi_bus_name_list_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_axi_bus_name:AMSTB_r_osal_os_io_get_axi_bus_name:inout:::] */
/*    r_osal_os_io_get_axi_bus_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_axi_bus_name(st_osal_os_io_get_axi_bus_name_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_axi_bus_name_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name:AMSTB_r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name:inout:::] */
/*    r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_num_of_axi_ids_for_axi_bus_name(st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_num_of_axi_ids_for_axi_bus_name_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_axi_id_list_for_axi_bus_name:AMSTB_r_osal_os_io_get_axi_id_list_for_axi_bus_name:inout:::] */
/*    r_osal_os_io_get_axi_id_list_for_axi_bus_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_axi_id_list_for_axi_bus_name(st_osal_os_io_get_axi_id_list_for_axi_bus_name_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_axi_id_list_for_axi_bus_name_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_num_of_axi_ids_for_device_hdl:AMSTB_r_osal_os_io_get_num_of_axi_ids_for_device_hdl:inout:::] */
/*    r_osal_os_io_get_num_of_axi_ids_for_device_hdl => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_num_of_axi_ids_for_device_hdl(st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_num_of_axi_ids_for_device_hdl_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_axi_id_list_for_device_hdl:AMSTB_r_osal_os_io_get_axi_id_list_for_device_hdl:inout:::] */
/*    r_osal_os_io_get_axi_id_list_for_device_hdl => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_axi_id_list_for_device_hdl(st_osal_os_io_get_axi_id_list_for_device_hdl_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_axi_id_list_for_device_hdl_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_num_of_axi_ids_for_device_name:AMSTB_r_osal_os_io_get_num_of_axi_ids_for_device_name:inout:::] */
/*    r_osal_os_io_get_num_of_axi_ids_for_device_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_num_of_axi_ids_for_device_name(st_osal_os_io_get_num_of_axi_ids_for_device_name_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_num_of_axi_ids_for_device_name_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_axi_id_list_for_device_name:AMSTB_r_osal_os_io_get_axi_id_list_for_device_name:inout:::] */
/*    r_osal_os_io_get_axi_id_list_for_device_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_axi_id_list_for_device_name(st_osal_os_io_get_axi_id_list_for_device_name_t* p_param)
{
    static e_osal_return_t volatile AMIN_return;
    static st_osal_os_io_get_axi_id_list_for_device_name_t* volatile AMOUT_p_param;
    AMOUT_p_param = p_param;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_set_always_on:AMSTB_r_osal_os_io_set_always_on:inout:::] */
/*    r_osal_os_io_set_always_on => Stub */
e_osal_return_t AMSTB_r_osal_os_io_set_always_on(DIR* p_dir)
{
    static e_osal_return_t volatile AMIN_return;
    static DIR* volatile AMOUT_p_dir;
    AMOUT_p_dir = p_dir;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_power_domain:AMSTB_r_osal_os_io_get_power_domain:inout:::] */
/*    r_osal_os_io_get_power_domain => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_power_domain(const char* p_uio_symbolic_name, uint64_t* p_pm_domain)
{
    static e_osal_return_t volatile AMIN_return;
    static uint64_t volatile AMIN_p_pm_domain;
    static char* volatile AMOUT_p_uio_symbolic_name;
    static uint64_t* volatile AMOUT_p_pm_domain;
    AMOUT_p_uio_symbolic_name = p_uio_symbolic_name;
    AMOUT_p_pm_domain = p_pm_domain;
    *p_pm_domain = AMIN_p_pm_domain;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_initialize_bus_information:AMSTB_r_osal_os_io_initialize_bus_information:inout:::] */
/*    r_osal_os_io_initialize_bus_information => Stub */
e_osal_return_t AMSTB_r_osal_os_io_initialize_bus_information(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_initialize_axi_bus_name_table:AMSTB_r_osal_os_io_initialize_axi_bus_name_table:inout:::] */
/*    r_osal_os_io_initialize_axi_bus_name_table => Stub */
e_osal_return_t AMSTB_r_osal_os_io_initialize_axi_bus_name_table(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_create_bus_name_list:AMSTB_r_osal_os_io_create_bus_name_list:inout:::] */
/*    r_osal_os_io_create_bus_name_list => Stub */
e_osal_return_t AMSTB_r_osal_os_io_create_bus_name_list(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_ipmmu_id:AMSTB_r_osal_os_io_get_ipmmu_id:inout:::] */
/*    r_osal_os_io_get_ipmmu_id => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_ipmmu_id(const char* const p_uio_name, osal_axi_bus_id_t* p_ipmmu_id)
{
#define AMCALLMAX_r_osal_os_io_get_ipmmu_id 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_get_ipmmu_id];
    static osal_axi_bus_id_t volatile AMIN_p_ipmmu_id[AMCALLMAX_r_osal_os_io_get_ipmmu_id];
    static char* volatile AMOUT_p_uio_name[AMCALLMAX_r_osal_os_io_get_ipmmu_id];
    static osal_axi_bus_id_t* volatile AMOUT_p_ipmmu_id[AMCALLMAX_r_osal_os_io_get_ipmmu_id];
    AMOUT_p_uio_name[AM_count] = p_uio_name;
    AMOUT_p_ipmmu_id[AM_count] = p_ipmmu_id;
    *p_ipmmu_id = AMIN_p_ipmmu_id[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_bus_domain:AMSTB_r_osal_os_io_get_bus_domain:inout:::] */
/*    r_osal_os_io_get_bus_domain => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_bus_domain(const char* const p_uio_symbolic_name, char* p_bus_domain)
{
#define AMCALLMAX_r_osal_os_io_get_bus_domain 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_get_bus_domain];
    static char* volatile AMOUT_p_uio_symbolic_name[AMCALLMAX_r_osal_os_io_get_bus_domain];
    static char* volatile AMOUT_p_bus_domain[AMCALLMAX_r_osal_os_io_get_bus_domain];
    AMOUT_p_uio_symbolic_name[AM_count] = p_uio_symbolic_name;
    AMOUT_p_bus_domain[AM_count] = p_bus_domain;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_create_axi_bus_name_table:AMSTB_r_osal_os_io_create_axi_bus_name_table:inout:::] */
/*    r_osal_os_io_create_axi_bus_name_table => Stub */
e_osal_return_t AMSTB_r_osal_os_io_create_axi_bus_name_table(const char* const p_uio_name)
{
#define AMCALLMAX_r_osal_os_io_create_axi_bus_name_table 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_create_axi_bus_name_table];
    static char* volatile AMOUT_p_uio_name[AMCALLMAX_r_osal_os_io_create_axi_bus_name_table];
    AMOUT_p_uio_name[AM_count] = p_uio_name;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_register_axi_bus_info:AMSTB_r_osal_os_io_register_axi_bus_info:inout:::] */
/*    r_osal_os_io_register_axi_bus_info => Stub */
e_osal_return_t AMSTB_r_osal_os_io_register_axi_bus_info(const char* const p_bus_domain, const char* const p_device_name, int32_t utlb)
{
#define AMCALLMAX_r_osal_os_io_register_axi_bus_info 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_register_axi_bus_info];
    static char* volatile AMOUT_p_bus_domain[AMCALLMAX_r_osal_os_io_register_axi_bus_info];
    static char* volatile AMOUT_p_device_name[AMCALLMAX_r_osal_os_io_register_axi_bus_info];
    static int32_t volatile AMOUT_utlb[AMCALLMAX_r_osal_os_io_register_axi_bus_info];
    AMOUT_p_bus_domain[AM_count] = p_bus_domain;
    AMOUT_p_device_name[AM_count] = p_device_name;
    AMOUT_utlb[AM_count] = utlb;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_register_axi_bus_info_last_data:AMSTB_r_osal_os_io_register_axi_bus_info_last_data:inout:::] */
/*    r_osal_os_io_register_axi_bus_info_last_data => Stub */
e_osal_return_t AMSTB_r_osal_os_io_register_axi_bus_info_last_data(void)
{
    static e_osal_return_t volatile AMIN_return;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_device_name_from_uio:AMSTB_r_osal_os_io_get_device_name_from_uio:inout:::] */
/*    r_osal_os_io_get_device_name_from_uio => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_device_name_from_uio(const char* const p_uio_name, char* p_device_name)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_uio_name;
    static char* volatile AMOUT_p_device_name;
    AMOUT_p_uio_name = p_uio_name;
    AMOUT_p_device_name = p_device_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_info_of_axi_bus_name:AMSTB_r_osal_os_io_get_info_of_axi_bus_name:inout:::] */
/*    r_osal_os_io_get_info_of_axi_bus_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_info_of_axi_bus_name(const char* const p_uio_name, const char* const device_name, uint32_t* utlbs)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_uio_name;
    static char* volatile AMOUT_device_name;
    static uint32_t* volatile AMOUT_utlbs;
    AMOUT_p_uio_name = p_uio_name;
    AMOUT_device_name = device_name;
    AMOUT_utlbs = utlbs;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_utlbs_from_uio:AMSTB_r_osal_os_io_get_utlbs_from_uio:inout:::] */
/*    r_osal_os_io_get_utlbs_from_uio => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_utlbs_from_uio(uint32_t* p_utlbs, const char* const p_uio_name, size_t* p_utlb_num)
{
    static e_osal_return_t volatile AMIN_return;
    static size_t volatile AMIN_p_utlb_num;
    static uint32_t* volatile AMOUT_p_utlbs;
    static char* volatile AMOUT_p_uio_name;
    static size_t* volatile AMOUT_p_utlb_num;
    AMOUT_p_utlbs = p_utlbs;
    AMOUT_p_uio_name = p_uio_name;
    AMOUT_p_utlb_num = p_utlb_num;
    *p_utlb_num = AMIN_p_utlb_num;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_device_name_connect:AMSTB_r_osal_os_io_device_name_connect:inout:::] */
/*    r_osal_os_io_device_name_connect => Stub */
e_osal_return_t AMSTB_r_osal_os_io_device_name_connect(const char* const p_device_name, size_t connect_index)
{
#define AMCALLMAX_r_osal_os_io_device_name_connect 10
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_device_name_connect];
    static char* volatile AMOUT_p_device_name[AMCALLMAX_r_osal_os_io_device_name_connect];
    static size_t volatile AMOUT_connect_index[AMCALLMAX_r_osal_os_io_device_name_connect];
    AMOUT_p_device_name[AM_count] = p_device_name;
    AMOUT_connect_index[AM_count] = connect_index;
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_create_axi_bus_name:AMSTB_r_osal_os_io_create_axi_bus_name:inout:::] */
/*    r_osal_os_io_create_axi_bus_name => Stub */
void AMSTB_r_osal_os_io_create_axi_bus_name(const char* p_axi_bus_name, char* p_buffer, size_t buffer_size)
{
#define AMCALLMAX_r_osal_os_io_create_axi_bus_name 50
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_create_axi_bus_name];
    static char* volatile AMOUT_p_axi_bus_name[AMCALLMAX_r_osal_os_io_create_axi_bus_name];
    static char* volatile AMOUT_p_buffer[AMCALLMAX_r_osal_os_io_create_axi_bus_name];
    static size_t volatile AMOUT_buffer_size[AMCALLMAX_r_osal_os_io_create_axi_bus_name];
    AMOUT_p_axi_bus_name[AM_count] = p_axi_bus_name;
    AMOUT_p_buffer[AM_count] = p_buffer;
    AMOUT_buffer_size[AM_count] = buffer_size;
    AM_count++;
    return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_create_regex_pattern:AMSTB_r_osal_os_io_create_regex_pattern:inout:::] */
/*    r_osal_os_io_create_regex_pattern => Stub */
e_osal_return_t AMSTB_r_osal_os_io_create_regex_pattern(const char* p_type, char* p_pattern, size_t size)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_type;
    static char* volatile AMOUT_p_pattern;
    static size_t volatile AMOUT_size;
    AMOUT_p_type = p_type;
    AMOUT_p_pattern = p_pattern;
    AMOUT_size = size;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_count_device:AMSTB_r_osal_os_io_count_device:inout:::] */
/*    r_osal_os_io_count_device => Stub */
e_osal_return_t AMSTB_r_osal_os_io_count_device(glob_t const* p_glob_buffer, char const* p_dev_pattern, size_t* p_match_count)
{
    static e_osal_return_t volatile AMIN_return;
    static glob_t* volatile AMOUT_p_glob_buffer;
    static char* volatile AMOUT_p_dev_pattern;
    static size_t* volatile AMOUT_p_match_count;
    AMOUT_p_glob_buffer = p_glob_buffer;
    AMOUT_p_dev_pattern = p_dev_pattern;
    AMOUT_p_match_count = p_match_count;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_generate_list:AMSTB_r_osal_os_io_generate_list:inout:::] */
/*    r_osal_os_io_generate_list => Stub */
e_osal_return_t AMSTB_r_osal_os_io_generate_list(glob_t const* p_glob_buffer, char const* p_dev_pattern, char** pp_list_buffer)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMIN_pp_list_buffer;
    static glob_t* volatile AMOUT_p_glob_buffer;
    static char* volatile AMOUT_p_dev_pattern;
    static char** volatile AMOUT_pp_list_buffer;
    AMOUT_p_glob_buffer = p_glob_buffer;
    AMOUT_p_dev_pattern = p_dev_pattern;
    AMOUT_pp_list_buffer = pp_list_buffer;
    *pp_list_buffer = AMIN_pp_list_buffer;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_add_item_to_list:AMSTB_r_osal_os_io_add_item_to_list:inout:::] */
/*    r_osal_os_io_add_item_to_list => Stub */
e_osal_return_t AMSTB_r_osal_os_io_add_item_to_list(glob_t const* p_glob_buffer, char** pp_list_buffer, size_t count, size_t* p_dst_offset)
{
#define AMCALLMAX_r_osal_os_io_add_item_to_list 50
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_add_item_to_list];
    static size_t* volatile AMIN_p_dst_offset[AMCALLMAX_r_osal_os_io_add_item_to_list];
    static glob_t* volatile AMOUT_p_glob_buffer[AMCALLMAX_r_osal_os_io_add_item_to_list];
    static size_t volatile AMOUT_count[AMCALLMAX_r_osal_os_io_add_item_to_list];
    static char** volatile AMOUT_pp_list_buffer[AMCALLMAX_r_osal_os_io_add_item_to_list];
    static size_t* volatile AMOUT_p_dst_offset[AMCALLMAX_r_osal_os_io_add_item_to_list];
    AMOUT_p_glob_buffer[AM_count] = p_glob_buffer;
    AMOUT_pp_list_buffer[AM_count] = pp_list_buffer;
    AMOUT_count[AM_count] = count;
    AMOUT_p_dst_offset[AM_count] = p_dst_offset;
    *p_dst_offset = AMIN_p_dst_offset[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_lock_device:AMSTB_r_osal_os_io_lock_device:inout:::] */
/*    r_osal_os_io_lock_device => Stub */
e_osal_return_t AMSTB_r_osal_os_io_lock_device(osal_device_handle_t handle, const char* p_id)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static char* volatile AMOUT_p_id;
    AMOUT_handle = handle;
    AMOUT_p_id = p_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_uio_link:AMSTB_r_osal_os_io_get_uio_link:inout:::] */
/*    r_osal_os_io_get_uio_link => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_uio_link(char const* p_uio_dev_name, char* p_uio_link_name)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_uio_dev_name;
    static char* volatile AMOUT_p_uio_link_name;
    AMOUT_p_uio_dev_name = p_uio_dev_name;
    AMOUT_p_uio_link_name = p_uio_link_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_map_information:AMSTB_r_osal_os_io_get_map_information:inout:::] */
/*    r_osal_os_io_get_map_information => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_map_information(osal_device_handle_t handle, const char* p_uio_symbolic_name)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static char* volatile AMOUT_p_uio_symbolic_name;
    AMOUT_handle = handle;
    AMOUT_p_uio_symbolic_name = p_uio_symbolic_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_set_irq_information:AMSTB_r_osal_os_io_set_irq_information:inout:::] */
/*    r_osal_os_io_set_irq_information => Stub */
e_osal_return_t AMSTB_r_osal_os_io_set_irq_information(osal_device_handle_t handle, char* p_uio_symbolic_name, const char* p_id)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static char* volatile AMOUT_p_uio_symbolic_name;
    static char* volatile AMOUT_p_id;
    AMOUT_handle = handle;
    AMOUT_p_uio_symbolic_name = p_uio_symbolic_name;
    AMOUT_p_id = p_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_initialize_for_pm:AMSTB_r_osal_os_io_initialize_for_pm:inout:::] */
/*    r_osal_os_io_initialize_for_pm => Stub */
void AMSTB_r_osal_os_io_initialize_for_pm(st_osal_device_list_t* p_device)
{
    static int volatile AM_count;
    static st_osal_device_list_t* volatile AMOUT_p_device;
    AMOUT_p_device = p_device;
    AM_count++;
    return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_map_size:AMSTB_r_osal_os_io_get_map_size:inout:::] */
/*    r_osal_os_io_get_map_size => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_map_size(osal_device_handle_t handle, const char* p_uio_symbolic_name)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static char* volatile AMOUT_p_uio_symbolic_name;
    AMOUT_handle = handle;
    AMOUT_p_uio_symbolic_name = p_uio_symbolic_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_irq_num:AMSTB_r_osal_os_io_get_irq_num:inout:::] */
/*    r_osal_os_io_get_irq_num => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_irq_num(osal_device_handle_t handle, char* p_uio_symbolic_name, const char* p_id)
{
    static e_osal_return_t volatile AMIN_return;
    static osal_device_handle_t volatile AMOUT_handle;
    static char* volatile AMOUT_p_uio_symbolic_name;
    static char* volatile AMOUT_p_id;
    AMOUT_handle = handle;
    AMOUT_p_uio_symbolic_name = p_uio_symbolic_name;
    AMOUT_p_id = p_id;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_pm_state:AMSTB_r_osal_os_io_get_pm_state:inout:::] */
/*    r_osal_os_io_get_pm_state => Stub */
void AMSTB_r_osal_os_io_get_pm_state(st_osal_device_list_t* p_device, int* p_pw_state, int* p_clock_state)
{
    static int volatile AMIN_p_pw_state;
    static int volatile AMIN_p_clock_state;
    static bool volatile AMIN_is_power_support;
    static bool volatile AMIN_is_clock_support;
    static bool volatile AMIN_is_reset_support;
    static int volatile AMOUT_p_device;
    static int* volatile AMOUT_p_pw_state;
    static int* volatile AMOUT_p_clock_state;
    AMOUT_p_device = p_device;
    AMOUT_p_pw_state = p_pw_state;
    AMOUT_p_clock_state = p_clock_state;
    *p_pw_state = AMIN_p_pw_state;
    *p_clock_state = AMIN_p_clock_state;
    p_device->pm_control.is_power_support = AMIN_is_power_support;
    p_device->pm_control.is_clock_support = AMIN_is_clock_support;
    p_device->pm_control.is_reset_support = AMIN_is_reset_support;
    return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_domain_name:AMSTB_r_osal_os_io_get_domain_name:inout:::] */
/*    r_osal_os_io_get_domain_name => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_domain_name(const char* p_axi_bus_name, char* p_domain_name)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_axi_bus_name;
    static char* volatile AMOUT_p_domain_name;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_p_domain_name = p_domain_name;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_utlb_parser:AMSTB_r_osal_os_io_utlb_parser:inout:::] */
/*    r_osal_os_io_utlb_parser => Stub */
e_osal_return_t AMSTB_r_osal_os_io_utlb_parser(const char* p_axi_bus_name, size_t len, int32_t** pp_list, size_t* const p_numOfutlb)
{
    static e_osal_return_t volatile AMIN_return;
    static int32_t* volatile AMIN_pp_list;
    static size_t volatile AMIN_p_numOfutlb;
    static char* volatile AMOUT_p_axi_bus_name;
    static size_t volatile AMOUT_len;
    static int32_t** volatile AMOUT_pp_list;
    static size_t* volatile AMOUT_p_numOfutlb;
    AMOUT_p_axi_bus_name = p_axi_bus_name;
    AMOUT_len = len;
    AMOUT_pp_list = pp_list;
    AMOUT_p_numOfutlb = p_numOfutlb;
    *pp_list = AMIN_pp_list;
    *p_numOfutlb = AMIN_p_numOfutlb;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_utlb_parser_sub:AMSTB_r_osal_os_io_utlb_parser_sub:inout:::] */
/*    r_osal_os_io_utlb_parser_sub => Stub */
e_osal_return_t AMSTB_r_osal_os_io_utlb_parser_sub(int32_t** pp_list, size_t* p_index, const char* p_num_str)
{
#define AMCALLMAX_r_osal_os_io_utlb_parser_sub 50
    static int volatile AM_count;
    static e_osal_return_t volatile AMIN_return[AMCALLMAX_r_osal_os_io_utlb_parser_sub];
    static size_t volatile AMIN_p_index[AMCALLMAX_r_osal_os_io_utlb_parser_sub];
    static int32_t** volatile AMOUT_pp_list[AMCALLMAX_r_osal_os_io_utlb_parser_sub];
    static size_t* volatile AMOUT_p_index[AMCALLMAX_r_osal_os_io_utlb_parser_sub];
    static char* volatile AMOUT_p_num_str[AMCALLMAX_r_osal_os_io_utlb_parser_sub];
    AMOUT_pp_list[AM_count] = pp_list;
    AMOUT_p_index[AM_count] = p_index;
    AMOUT_p_num_str[AM_count] = p_num_str;
    *p_index = AMIN_p_index[AM_count];
    AM_count++;
    return AMIN_return[AM_count - 1];
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_utlb_parser_sub1:AMSTB_r_osal_os_io_utlb_parser_sub1:inout:::] */
/*    r_osal_os_io_utlb_parser_sub1 => Stub */
e_osal_return_t AMSTB_r_osal_os_io_utlb_parser_sub1(int32_t** pp_list, size_t* p_index, int32_t num)
{
    static e_osal_return_t volatile AMIN_return;
    static int32_t** volatile AMOUT_pp_list;
    static size_t* volatile AMOUT_p_index;
    static int32_t volatile AMOUT_num;
    AMOUT_pp_list = pp_list;
    AMOUT_p_index = p_index;
    AMOUT_num = num;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_utlb_parser_sub2:AMSTB_r_osal_os_io_utlb_parser_sub2:inout:::] */
/*    r_osal_os_io_utlb_parser_sub2 => Stub */
e_osal_return_t AMSTB_r_osal_os_io_utlb_parser_sub2(int32_t** pp_list, size_t* p_index, int32_t num)
{
    static e_osal_return_t volatile AMIN_return;
    static int32_t** volatile AMOUT_pp_list;
    static size_t* volatile AMOUT_p_index;
    static int32_t volatile AMOUT_num;
    AMOUT_pp_list = pp_list;
    AMOUT_p_index = p_index;
    AMOUT_num = num;
    return AMIN_return;
}

/* WINAMS_STUB[r_osal_os_io.c:r_osal_os_io_get_spi_num:AMSTB_r_osal_os_io_get_spi_num:inout:::] */
/*    r_osal_os_io_get_spi_num => Stub */
e_osal_return_t AMSTB_r_osal_os_io_get_spi_num(char* p_irq_uio_name, uint64_t* p_spi_num)
{
    static e_osal_return_t volatile AMIN_return;
    static char* volatile AMOUT_p_irq_uio_name;
    static uint64_t* volatile AMOUT_p_spi_num;
    AMOUT_p_irq_uio_name = p_irq_uio_name;
    AMOUT_p_spi_num = p_spi_num;
    return AMIN_return;
}

#ifdef __cplusplus
}
#endif
#endif /* WINAMS_STUB */
