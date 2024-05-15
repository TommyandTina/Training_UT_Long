#include "stub.h"
#include "rcar-xos/osal/r_osal.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"
#include "target/common/res_cfg/static/r_osal_res_cfg.h"
#include "target/zephyr/r_osal_os_type.h"

/* malloc */
#define STUB_MALLOC_COUNT   (8000)
typedef struct st_stub_malloc_tbl
{
    size_t  size;
    void*   ptr;
} st_stub_malloc_tbl_t;
st_stub_malloc_tbl_t    g_stub_malloc_tbl[STUB_MALLOC_COUNT];
size_t                  g_stub_malloc_count = 0;

uint64_t    g_pthread_create_count;
uint64_t    g_pthread_mutex_init_count;
uint64_t    g_pthread_cond_init_count;
uint64_t    g_malloc_count;
uint64_t    g_realloc_count;
uint64_t    g_calloc_count;
uint64_t    g_timer_create_count;

enum clock_control_status   g_clock_control_status = CLOCK_CONTROL_STATUS_OFF;
int                         g_mutex_trylock_err = 0;

struct posix_thread         stub_thread_main;
struct posix_thread         stub_thread[256];
uint32_t                    stub_pthread_create_count = 0;

uint32_t    g_stub_cycle;
uint32_t    g_stub_cycle_before;
uint32_t    g_stub_cycle_after;
#define START_TIMER __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (g_stub_cycle_before));
#define STOP_TIMER  __asm__ volatile("mrc   p15, 0, %0, c9, c13, 0" : "=r" (g_stub_cycle_after)); g_stub_cycle += (g_stub_cycle_after - g_stub_cycle_before);

void init_stub_value(void)
{
    g_stub_cycle                = 0;
    g_pthread_create_count      = 0;
    g_pthread_mutex_init_count  = 0;
    g_pthread_cond_init_count   = 0;
    g_malloc_count              = 0;
    g_realloc_count             = 0;
    g_calloc_count              = 0;
    g_timer_create_count        = 0;

    g_stub_malloc_count         = 0;
    memset(g_stub_malloc_tbl, 0x00, sizeof(g_stub_malloc_tbl));

    return;
}

uint32_t get_stub_cycle()
{
    return g_stub_cycle;
}

uint64_t get_pthread_create_count(void)
{
    return g_pthread_create_count;
}

uint64_t get_pthread_mutex_init_count(void)
{
    return g_pthread_mutex_init_count;
}

uint64_t get_pthread_cond_init_count(void)
{
    return g_pthread_cond_init_count;
}

uint64_t get_malloc_count(void)
{
    return g_malloc_count;
}

uint64_t get_realloc_count(void)
{
    return g_realloc_count;
}

uint64_t get_calloc_count(void)
{
    return g_calloc_count;
}

uint64_t get_timer_create_count(void)
{
    return g_timer_create_count;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include/stdlib.h
void* calloc_stub(size_t num, size_t nsize)
{
START_TIMER
    size_t  size;
    void*   ptr;

    g_calloc_count++;

    if (STUB_MALLOC_COUNT <= (g_stub_malloc_count + 1))
    {
        printf("[calloc_stub]: Error malloc count %zu\n", g_stub_malloc_count);
STOP_TIMER
        return NULL;
    }

    size = num * nsize;
    ptr = malloc(size);
    if (NULL != ptr)
    {
        memset(ptr, 0x00, size);

        g_stub_malloc_tbl[g_stub_malloc_count].size = size;
        g_stub_malloc_tbl[g_stub_malloc_count].ptr  = ptr;
        g_stub_malloc_count++;
    }

STOP_TIMER
    return ptr;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include/stdlib.h
void* realloc_stub(void* ptr, size_t size)
{
START_TIMER
    size_t  cnt;
    void*   ptr_new;

    g_realloc_count++;

    if (NULL == ptr)
    {
        // malloc
        if (STUB_MALLOC_COUNT <= (g_stub_malloc_count + 1))
        {
            printf("[realloc_stub]: Error malloc count %zu\n", g_stub_malloc_count);
STOP_TIMER
            return NULL;
        }

        ptr_new = malloc(size);
        if (NULL != ptr_new)
        {
            g_stub_malloc_tbl[g_stub_malloc_count].size = size;
            g_stub_malloc_tbl[g_stub_malloc_count].ptr  = ptr_new;
            g_stub_malloc_count++;
        }
    }
    else
    {
        // search ptr
        for (cnt = 0; cnt < g_stub_malloc_count; cnt++)
        {
            if (ptr == g_stub_malloc_tbl[cnt].ptr)
            {
                break;
            }
        }

        if (g_stub_malloc_count <= cnt)
        {
            printf("[realloc_stub]: Error not found ptr %p\n", ptr);
STOP_TIMER
            return NULL;
        }

        if (size <= g_stub_malloc_tbl[cnt].size)
        {
            ptr_new = ptr;
            g_stub_malloc_tbl[cnt].size = size;
        }
        else
        {
            // realloc
            ptr_new = malloc(size);
            if (NULL != ptr_new)
            {
                memcpy(ptr_new, ptr, g_stub_malloc_tbl[cnt].size);
                free(ptr);
                g_stub_malloc_tbl[cnt].size = size;
                g_stub_malloc_tbl[cnt].ptr  = ptr_new;
            }
        }
    }
STOP_TIMER
    return ptr_new;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include/stdlib.h
// W.A. Use the free stub for avoid sysdown.
//      For the above, memory leak occur in this TP.
//      But, UT performance can be measurement.
void free_stub(void* ptr)
{
    return;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include/stdlib.h
void* bsearch_stub(const void *__key, const void *__base, size_t __nmemb, size_t __size, __compar_fn_t _compar)
{
START_TIMER
    void *p_ret_ptr = NULL;
    char *p_str;
    int  ret;

    if (__base == gp_osal_device_control)
    {
        p_str = (char*)__key;
        for (size_t i = 0; i < g_osal_num_of_device_table; i++)
        {
            ret = strncmp(p_str, gp_osal_device_control[i].dev_info->id, OSAL_DEVICE_NAME_MAX_LENGTH);
            if (0 == ret)
            {
                p_ret_ptr = (void*)&gp_osal_device_control[i];
                break;
            }
            else
            {
                /* Do nothing */
            }
        }
    }
    else
    {
        p_ret_ptr = (void*)__base;
    }
STOP_TIMER
    return p_ret_ptr;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/include/stdio.h
int fprintf_stub(FILE *, const char *, ...)
{
    return 0;
}

// include/zephyrproject/zephyr/include/generated/syscalls/time.h
int clock_gettime_stub(clockid_t clock_id, struct timespec * ts)
{
    static int clock_gettime_count = 1000000;

    (void)clock_id;

    if (ts)
    {
        ts->tv_sec  = 0;
        ts->tv_nsec = clock_gettime_count++;
    }

    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/time.h
int nanosleep_stub(const struct timespec *rqtp, struct timespec *rmtp)
{
    (void)rqtp;
    (void)rmtp;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/cache.h
int cache_data_range_stub(void *addr, size_t size, int op)
{
    (void)addr;
    (void)size;
    (void)op;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/drivers/clock_control.h
int clock_control_on_stub(const struct device *dev, clock_control_subsys_t sys)
{
    (void)dev;
    (void)sys;
    g_clock_control_status = CLOCK_CONTROL_STATUS_ON;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/drivers/clock_control.h
int clock_control_off_stub(const struct device *dev, clock_control_subsys_t sys)
{
    (void)dev;
    (void)sys;
    g_clock_control_status = CLOCK_CONTROL_STATUS_OFF;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/drivers/clock_control.h
enum clock_control_status clock_control_get_status_stub(const struct device *dev, clock_control_subsys_t sys)
{
    (void)dev;
    (void)sys;
    return g_clock_control_status;
}

// include/zephyrproject/zephyr/include/zephyr/drivers/clock_control.h
int clock_control_set_rate_stub(const struct device *dev, clock_control_subsys_t sys, clock_control_subsys_rate_t rate)
{
    (void)dev;
    (void)sys;
    (void)rate;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/irq.h
int irq_connect_dynamic_stub(unsigned int irq, unsigned int priority, void (*routine)(const void *parameter), const void *parameter, uint32_t flags)
{
    (void)irq;
    (void)priority;
    (void)routine;
    (void)parameter;
    (void)flags;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/irq.h
unsigned int irq_lock_stub(void)
{
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/irq.h
void irq_unlock_stub(unsigned int key)
{
    (void)key;
    return;
}

// include/zephyrproject/zephyr/include/zephyr/arch/arm/aarch32/irq.h
void arch_irq_enable_stub(unsigned int irq)
{
    (void)irq;
    return;
}

// include/zephyrproject/zephyr/include/zephyr/arch/arm/aarch32/irq.h
void arch_irq_disable_stub(unsigned int irq)
{
    (void)irq;
    return;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
uint32_t k_cycle_get_32_stub(void)
{
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
bool k_is_in_isr_stub(void)
{
    return false;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
int k_thread_cpu_mask_clear_stub(k_tid_t thread)
{
    (void)thread;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
int k_thread_cpu_mask_enable_stub(k_tid_t thread, int cpu)
{
    (void)thread;
    (void)cpu;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
void k_yield_stub(void)
{
    return;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
void k_thread_suspend_stub(k_tid_t thread)
{
    (void)thread;
    return;
}

// include/zephyrproject/zephyr/include/zephyr/kernel.h
void k_thread_resume_stub(k_tid_t thread)
{
    (void)thread;
    return;
}

// include/zephyrproject/zephyr/include/zephyr/spinlock.h
k_spinlock_key_t k_spin_lock_stub(struct k_spinlock *l)
{
    static k_spinlock_key_t key;
    (void)l;
    return key;
}

// include/zephyrproject/zephyr/include/zephyr/spinlock.h
void k_spin_unlock_stub(struct k_spinlock *l, k_spinlock_key_t key)
{
    (void)l;
    (void)key;
    return;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
void* memcpy_stub(void* dest, const void* src, size_t n)
{
START_TIMER
    uint8_t *p_dest = dest;
    uint8_t *p_src = src;
    size_t i;

    if ((dest != NULL) && (src != NULL))
    {
        for (i=0; i<n; i++)
        {
            p_dest[i] = p_src[i];
        }
    }
STOP_TIMER
    return dest;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
void* memset_stub(void* s, int c, size_t n)
{
START_TIMER
    uint8_t *ptr = s;
    size_t i;

    if (s != NULL)
    {
        for (i=0; i<n-1; i++)
        {
            ptr[i] = '\0';
        }
    }
STOP_TIMER
    return s;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
int strcmp_stub(const char *s1, const char *s2)
{
START_TIMER
    while ((*s1 != '\0') && (*s2 != '\0'))
    {
        if(*s1 != *s2)
            break;
        s1++;
        s2++;
    }
STOP_TIMER
    return ((uint8_t)*s1 - (uint8_t)*s2);
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
char* strncat_stub(char* restrict s1, const char* restrict s2, size_t n)
{
START_TIMER
    char *ptr = s1;

    if ((s1 == NULL) || (s2 == NULL))
    {
STOP_TIMER
        return ptr;
    }
    while ((*s2 != '\0') && (n > 0))
    {
        *s1 = *s2;
        n--;
        s1++;
        s2++;
    }
STOP_TIMER
    return ptr;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
int strncmp_stub(const char *s1, const char *s2, size_t n)
{
START_TIMER
    int i = 0;

    while ((s1[i] != '\0') && (s2[i] != '\0') && (n > i))
    {
        if(s1[i] != s2[i])
            break;
        i++;
    }

    if (n == i)
    {
        i--;
    }
STOP_TIMER
    return ((int)s1[i] - (int)s2[i]);
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
char* strncpy_stub(char *dest, const char *src, size_t n)
{
START_TIMER
    char *ptr = dest;

    if ((dest == NULL) || (src == NULL))
    {
STOP_TIMER
        return ptr;
    }
    while (n > 0)
    {
        n--;
        if((*dest = *src) == '\0')
            break;
        dest++;
        src++;
    }
    while (n--)
        *dest++ = '\0';
STOP_TIMER
    return ptr;
}

// include/zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
char* strstr_stub(const char *s1, const char *s2)
{
START_TIMER
    int i, j;

    if ((s1 == NULL) || (s2 == NULL))
    {
        return 0;
    }

    for (i = 0; s1[i] != '\0'; i++)
    {
        for (j = 0; s2[j] != '\0'; j++)
        {
            if (s1[i + j] != s2[j])
            {
                break;
            }
        }
        if (s2[j] == '\0')
        {
STOP_TIMER
            return s1;
        }
    }

STOP_TIMER
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/pm/device_runtime.h
int pm_device_runtime_get_stub(const struct device *dev)
{
    (void)dev;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/pm/device_runtime.h
int pm_device_runtime_put_stub(const struct device *dev)
{
    (void)dev;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/pm/device_runtime.h
int pm_device_runtime_enable_stub(const struct device *dev)
{
    (void)dev;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/pm/device.h
int pm_device_state_get_stub(const struct device *dev, enum pm_device_state *state)
{
    (void)dev;
    (void)state;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_attr_init_stub(pthread_attr_t *attr)
{
    (void)attr;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_attr_destroy_stub(pthread_attr_t *attr)
{
    (void)attr;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_attr_setschedparam_stub(pthread_attr_t *attr, const struct sched_param *schedparam)
{
    (void)attr;
    (void)schedparam;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_attr_setschedpolicy_stub(pthread_attr_t *attr, int policy)
{
    (void)attr;
    (void)policy;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_attr_setstack_stub(pthread_attr_t *attr, void *stackaddr, size_t stacksize)
{
    (void)attr;
    (void)stackaddr;
    (void)stacksize;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_create_stub(pthread_t *newthread, const pthread_attr_t *attr, void *(*threadroutine)(void *), void *arg)
{
    uint32_t num_of_thread = 0U;
    st_osal_thread_control_t* p_control = (st_osal_thread_control_t *)arg;

START_TIMER
    (void)newthread;
    (void)attr;
    (void)threadroutine;
    (void)arg;

    g_pthread_create_count++;

    stub_thread[stub_pthread_create_count].state = PTHREAD_EXITED;
    *newthread = &stub_thread[stub_pthread_create_count];    /* dummy of created thread */

    stub_pthread_create_count++;

STOP_TIMER
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
pthread_t pthread_self_stub(void)
{
    return &stub_thread_main;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_equal_stub(pthread_t pt1, pthread_t pt2)
{
    int ret;

    if (pt1 != pt2)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_detach_stub(pthread_t thread)
{
    (void)thread;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutexattr_init_stub(pthread_mutexattr_t *m)
{
    (void)m;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutexattr_destroy_stub(pthread_mutexattr_t *m)
{
    (void)m;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutexattr_settype_stub(pthread_mutexattr_t *attr, int type)
{
    (void)attr;
    (void)type;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutex_init_stub(pthread_mutex_t *m, const pthread_mutexattr_t *att)
{
    (void)m;
    (void)att;
    g_pthread_mutex_init_count++;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutex_destroy_stub(pthread_mutex_t *m)
{
    (void)m;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutex_lock_stub(pthread_mutex_t *m)
{
    (void)m;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutex_trylock_stub(pthread_mutex_t *m)
{
    (void)m;

    if (EBUSY == g_mutex_trylock_err)
    {
        g_mutex_trylock_err = 0;
        return EBUSY;
    }
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutex_timedlock_stub(pthread_mutex_t *m, const struct timespec *abstime)
{
    (void)m;
    (void)abstime;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_mutex_unlock_stub(pthread_mutex_t *m)
{
    (void)m;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_cond_init_stub(pthread_cond_t *cv, const pthread_condattr_t *att)
{
    (void)cv;
    (void)att;
    g_pthread_cond_init_count++;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_cond_destroy_stub(pthread_cond_t *cv)
{
    (void)cv;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_cond_wait_stub(pthread_cond_t *cv, pthread_mutex_t *mut)
{
    (void)cv;
    (void)mut;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_cond_timedwait_stub(pthread_cond_t *cv, pthread_mutex_t *mut, const struct timespec *abstime)
{
    (void)cv;
    (void)mut;
    (void)abstime;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_cond_signal_stub(pthread_cond_t *cv)
{
    (void)cv;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_cond_broadcast_stub(pthread_cond_t *cv)
{
    (void)cv;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/pthread.h
int pthread_setname_np_stub(pthread_t thread, const char *name)
{
    (void)thread;
    (void)name;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/semaphore.h
int sem_destroy_stub(sem_t *semaphore)
{
    (void)semaphore;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/semaphore.h
int sem_getvalue_stub(sem_t *ZRESTRICT semaphore, int *ZRESTRICT value)
{
    (void)semaphore;
    (void)value;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/semaphore.h
int sem_init_stub(sem_t *semaphore, int pshared, unsigned int value)
{
    (void)semaphore;
    (void)pshared;
    (void)value;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/semaphore.h
int sem_post_stub(sem_t *semaphore)
{
    (void)semaphore;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/semaphore.h
int sem_timedwait_stub(sem_t *ZRESTRICT semaphore, struct timespec *ZRESTRICT abstime)
{
    (void)semaphore;
    (void)abstime;
    return 0;
}

// include/zephyrproject/zephyr/include/zephyr/posix/semaphore.h
int sem_trywait_stub(sem_t *semaphore)
{
    (void)semaphore;
    return 0;
}
