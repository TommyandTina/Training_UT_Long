
#include "stub.h"
#include "rcar-xos/osal/r_osal.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"
#include "target/common/res_cfg/static/r_osal_res_cfg.h"

typedef struct st_osal_interrupt_thread_control
{
    uint64_t              handle_id;        /*!< Fixed value: OSAL_INTERRUPT_HANDLE_ID */
    bool                  register_signal;  /*!< Registration notification flag */
    bool                  thread_active;    /*!< interrupt thread active flag(true: active interrupt thread is active, false: interrupt thread is inactive) */
    pthread_cond_t        inner_cond;       /*!< Internal condition variable object */
    pthread_mutex_t       inner_mtx;        /*!< Internal mutex object */
    osal_milli_sec_t      inner_timeout;    /*!< Waiting time of lock inner_mtx */
    osal_device_handle_t  device_handle;    /*!< Handle of device according to interrupt */
    pthread_t             thread_id;        /*!< Handle of interrupt thread for QNX */
    p_osal_isr_func_t     irq_handler;      /*!< Pointer of Worker function of Interrupt thread */
    void                  *irq_arg;         /*!< Pointer of Worker function argument */
    uint64_t              irq_channel;      /*!< irq number channel */
    bool                  irq_enable;       /*!< interrupt status (true: Enable interrupt, false: Disable interrupt) */
    /* PRQA S 5209 1 # System is requesting an int, so it cannot be changed. */
    int                   event_id;         /*!< Attach event ID */

    const struct st_osal_interrupt_thread_config *usr_config; /*!< User configuration for message queue */
} st_osal_interrupt_thread_control_t;

typedef struct {
    uint64_t        cycles_per_sec;
} syspage_entry_info_dummy;

int g_mutex_trylock_err = 0;

syspage_entry_info_dummy     qtime_dummy = {16666666};
void *qtime = (void *)&qtime_dummy;

/* malloc */
#define STUB_MALLOC_ALIGN   (8)
#define STUB_MALLOC_SIZE    (0x2000000)   /* 32MB */
#define STUB_MALLOC_COUNT   (8000)
typedef struct st_stub_malloc_tbl{
    size_t  size;
    void*   ptr;
} st_stub_malloc_tbl_t;

size_t  stub_malloc_pos = 0;
size_t  stub_malloc_count = 0;
st_stub_malloc_tbl_t stub_malloc_tbl[STUB_MALLOC_COUNT];
char   stub_heap[STUB_MALLOC_SIZE] = {0};

uint64_t g_stub_time;
uint64_t g_pthread_create_count;
uint64_t g_pthread_mutex_init_count;
uint64_t g_pthread_cond_init_count;
uint64_t g_malloc_count;
uint64_t g_realloc_count;
uint64_t g_calloc_count;
uint64_t g_timer_create_count;

void init_stub_value(void)
{
    g_stub_time                 = 0;
    g_pthread_create_count      = 0;
    g_pthread_mutex_init_count  = 0;
    g_pthread_cond_init_count   = 0;
    g_malloc_count              = 0;
    g_realloc_count             = 0;
    g_calloc_count              = 0;
    g_timer_create_count        = 0;

    return;
}

uint64_t get_stub_time(void)
{
    return g_stub_time;
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


#ifndef WINAMS_STUB
#define SEC2NSEC    (1000000000ULL)
#define START_TIMER g_clocktime_before = ClockCycles();
#define STOP_TIMER  g_clocktime_after  = ClockCycles(); g_stub_time += (g_clocktime_after - g_clocktime_before);

uint64_t g_clocktime_before;
uint64_t g_clocktime_after;
#else   /* ifndef WINAMS_STUB */
#define START_TIMER do{}while(0);
#define STOP_TIMER  do{}while(0);
#endif  /* else WINAMS_STUB */

void *malloc(unsigned long size)
{
    size_t size_a;
    void *ptr;

    g_malloc_count++;

    if (size == 0)
    {
        return NULL;
    }
    size_a = ((size + STUB_MALLOC_ALIGN - 1) / STUB_MALLOC_ALIGN) * STUB_MALLOC_ALIGN;
    if ((stub_malloc_pos + size_a) >= STUB_MALLOC_SIZE)
    {
        return NULL;
    }
    if ((stub_malloc_count + 1) >= STUB_MALLOC_COUNT)
    {
        return NULL;
    }
    ptr = (void*)&stub_heap[stub_malloc_pos];
    stub_malloc_tbl[stub_malloc_count].size = size;
    stub_malloc_tbl[stub_malloc_count].ptr = ptr;
    stub_malloc_pos += size_a;
    stub_malloc_count++;

    return ptr;
}

void *calloc(unsigned long num, unsigned long nsize)
{
    size_t size;
    void *p;
START_TIMER
    size = num * nsize;
    p = malloc(size);
    g_malloc_count--;
    g_calloc_count++;
STOP_TIMER
    return p;
}

void free(void *ptr)
{
START_TIMER
    if (ptr == NULL)
    {
STOP_TIMER
        return;
    }
    if (stub_malloc_count == 0)
    {
STOP_TIMER
        return;
    }
    stub_malloc_count--;
    if (stub_malloc_count == 0)
        stub_malloc_pos = 0;   // reset heap
STOP_TIMER
    return;
}

void *realloc(void *ptr, unsigned long size)
{
START_TIMER
    size_t i;
    void *ptr_new;

    g_realloc_count++;
    if (ptr == NULL)
    {
        ptr_new = malloc(size);    // malloc new area
        g_malloc_count--;
STOP_TIMER
    }
    else
    {
        /* search ptr */
        for (i=0; i<stub_malloc_count; i++)
        {
            if(stub_malloc_tbl[stub_malloc_count].ptr == ptr)
                break;
        }
        if (i >= stub_malloc_count)
        {
            ptr_new = malloc(size); // malloc new area
            if (ptr_new != NULL)
            {
                memcpy(ptr_new, ptr, size);
                free(ptr);
            }
            g_malloc_count--;
STOP_TIMER
            return ptr_new;
        }

        if (stub_malloc_tbl[i].size >= size)
        {
STOP_TIMER
            return ptr;
        }
        /* realloc */
        ptr_new = malloc(size);
        g_malloc_count--;
STOP_TIMER
        if (ptr_new != NULL)
        {
            memcpy(ptr_new, ptr, size);
            free(ptr);
        }
    }
    return ptr_new;
}

void *memcpy(void *dest, const void *src, unsigned long n)
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

void *memset(void *s, int c, unsigned long n)
{
    uint8_t *ptr = s;
    size_t i;

    if (s != NULL)
    {
        for (i=0; i<n-1; i++)
        {
            ptr[i] = '\0';
        }
    }

    return s;
}

unsigned long strlen(const char *s)
{
START_TIMER
    size_t len = 0;

    if (s == NULL)
    {
STOP_TIMER
        return 0;
    }
    while (*s++)
        len++;
STOP_TIMER
    return len;
}

unsigned long strnlen(const char *s, unsigned long n)
{
START_TIMER
    size_t len = 0;

    if (s == NULL)
    {
STOP_TIMER
        return 0;
    }
    while ((n > 0) && (*s++))
    {
        n--;
        len++;
    }
STOP_TIMER
    return len;
}

char *strncpy(char *dest, const char *src, unsigned long n)
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

int strcmp(const char *s1, const char *s2)
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

int strncmp(const char *s1, const char *s2, unsigned long n)
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

char* strncat(char* restrict s1, const char* restrict s2, unsigned long n)
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

char *strstr(const char *s1, const char *s2)
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

void qsort(void *__base, size_t __nmemb, size_t __size, int (*__compar)(const void *, const void *))
{

    return ;
}

void *bsearch(const void *__key, const void *__base, size_t __nmemb, size_t __size, int (*__compar)(const void *__pkey, const void *__pbase))
{
    void *p_ret_ptr = NULL;
    char *p_str;
    int  ret;
START_TIMER
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

/* pthread_mutex */
typedef struct {
    void* array[4];
} dummy_thread;
dummy_thread stub_thread_main;
dummy_thread stub_thread[256];
uint32_t stub_pthread_create_count = 0;
st_osal_thread_control_t *g_control = NULL;

void *stub_p_setspecific = NULL;

int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
{
    (void) mutex;
    (void) attr;

    g_pthread_mutex_init_count++;

    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout)
{
    (void) mutex;
    (void) abs_timeout;

    return 0;
}

int pthread_mutex_timedlock_monotonic(pthread_mutex_t *__mutex, const struct timespec *__abs_timeout)
{
    return pthread_mutex_timedlock(__mutex, __abs_timeout);
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    (void) mutex;

    return g_mutex_trylock_err;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    (void) attr;

    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    (void) attr;

    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    (void) attr;
    (void) type;

    return 0;
}

/* pthread_key */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
    (void) key;
    (void) destructor;

    return 0;
}

int pthread_key_delete(pthread_key_t key)
{
    (void) key;

    if (stub_p_setspecific)
    {
        stub_p_setspecific = NULL;
    }
    return 0;
}

void *pthread_getspecific(pthread_key_t key)
{
    (void) key;

    return stub_p_setspecific;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    (void) key;
    (void) value;
    stub_p_setspecific = value;  /* free in pthread_key_delete */
    return 0;
}

/* pthread_cond */
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr)
{
    (void) cond;
    (void) attr;

    g_pthread_cond_init_count++;

    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    (void) cond;

    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    (void) cond;

    return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    (void) cond;

    return 0;
}

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex)
{
START_TIMER
    (void) cond;
    (void) mutex;

    uint32_t num_of_thread = 0;
    uint32_t i;

    if (NULL != g_control)
    {
        while((uint64_t)OSAL_THREAD_INVALID_ID != g_osal_thread_config[num_of_thread].id)
        {
            num_of_thread++;
        }

        for (i = 0; i < num_of_thread; i++)
        {
            g_control[i].active = false;
        }
        g_control = NULL;
    }

STOP_TIMER
    return 0;
}

int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime)
{
    (void) cond;
    (void) mutex;
    (void) abstime;

/*    if (NULL != R_PMA_SM_GxTaskQueue)
    {
        st_osal_stub_mq_control_t *stub_handle = (st_osal_stub_mq_control_t *)R_PMA_SM_GxTaskQueue;
        stub_handle->numofmsg = 0;
    }
*/
    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t *__attr)
{
    (void) __attr;

    return 0;
}

int pthread_condattr_init(pthread_condattr_t *__attr)
{
    (void) __attr;

    return 0;
}

int pthread_condattr_setclock(pthread_condattr_t *__attr, int __id)
{
    (void) __attr;
    (void) __id;

    return 0;
}

/* pthread_attr */
int pthread_attr_init(pthread_attr_t *attr)
{
    (void) attr;

    return 0;
}

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
    (void) attr;
    (void) inheritsched;

    return 0;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
    (void) attr;
    (void) policy;

    return 0;
}

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
    (void) attr;
    (void) param;

    return 0;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    (void) attr;
    (void) stacksize;

    return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
    (void) attr;

    return 0;
}

/* pthread */
#define PMA_THREAD_COUNT (1U)

int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)
{
START_TIMER
    (void) thread;
    (void) attr;
    (void) arg;
    (void) start_routine;

    uint32_t num_of_thread = 0U;
    st_osal_interrupt_thread_control_t *interrupt_control;

    while((uint64_t)OSAL_THREAD_INVALID_ID != g_osal_thread_config[num_of_thread].id)
    {
        num_of_thread++;
    }

    g_pthread_create_count++;
    *thread = &stub_thread[stub_pthread_create_count];    /* dummy of created thread */

//    if (stub_pthread_create_count >= (num_of_thread + PMA_THREAD_COUNT))
    if (stub_pthread_create_count > num_of_thread)
    {
        /* interrupt thread control */
        interrupt_control = (st_osal_interrupt_thread_control_t *)arg;
        interrupt_control->thread_active = true;
    }
    else if (stub_pthread_create_count == num_of_thread)
    {
        /* PMA */
/*        for (uint32_t id = hwa_idmin; id < (uint32_t)hwa_idmax; id++)
        {
            st_hwa_t *HwaState = R_PMA_SM_GetHwa((e_pma_hwa_id_t)id);
            HwaState->enApparentMode = R_PMA_CG_MODE;
        }
*/
    }
    else
    {
        /* thread control */
        if (NULL == g_control)
        {
            g_control = (st_osal_thread_control_t *)arg;
        }
    }

    stub_pthread_create_count++;
STOP_TIMER
    return 0;
}

int pthread_cancel(pthread_t thread)
{
    (void) thread;

    return 0;
}

void pthread_exit(void *retval)
{
    (void) retval;
}

pthread_t pthread_self(void)
{
    return &stub_thread_main;   /* dummy of main thread */
}

int pthread_equal(pthread_t __t1, pthread_t __t2)
{
    int ret;
    if (__t1 != __t2)
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }

    return ret;
}

int pthread_setcancelstate(int state, int *oldstate)
{
    (void)state;
    (void)oldstate;
    return 0;
}

int pthread_detach(pthread_t thread)
{
    (void)thread;
    return 0;
}

int pthread_join(pthread_t thread, void **retval)
{
    (void) thread;

    if (retval != NULL)
    {
        *retval = NULL; 
    }

    return 0;
}

int pthread_timedjoin(pthread_t __thr, void **__value_ptr, const struct timespec *__abstime)
{
START_TIMER
    (void) __thr;
    (void) __abstime;

/*    if (NULL != R_PMA_SM_GxTaskQueue)
    {
        st_osal_stub_mq_control_t *stub_handle = (st_osal_stub_mq_control_t *)R_PMA_SM_GxTaskQueue;
        stub_handle->numofmsg = 0;
    }
*/
    if (__value_ptr != NULL)
    {
        *__value_ptr = NULL; 
    }
STOP_TIMER
    return 0;
}

int pthread_timedjoin_monotonic(pthread_t __thr, void **__value_ptr, const struct timespec *__abstime)
{
    return pthread_timedjoin(__thr, __value_ptr, __abstime);
}

int pthread_setschedprio(pthread_t thread, int prio)
{
    (void) thread;
    (void) prio;

    return 0;
}

int pthread_setname_np(pthread_t __thr, const char *__name)
{
    (void) __thr;
    (void) __name;
    return 0;
}

int __sigsetjmp (struct __jmp_buf_tag *__env, int __savemask)
{
    (void)__env;
    (void)__savemask;
    return 0;
}

int error_stub(void)
{
    return 0;
}

void __cpu_membarrier(void)
{
    return;
}

long int syscall (long int __sysno, ...)
{
    (void)__sysno;
    return 4096;
}

/* Interrupt */
int InterruptAttachEvent(int __intr, const struct sigevent *__event, unsigned __flags)
{
    (void) __intr;
    (void) __event;
    (void) __flags;

    return 0;
}

int InterruptDetach(int id)
{
    (void) id;

    return 0;
}

int InterruptWait(int __flags, const uint64_t *__timeout)
{
    (void) __flags;
    (void) __timeout;

    return 0;
}

int InterruptWait_r(int __flags, const uint64_t *__timeout)
{
    (void) __flags;
    (void) __timeout;

    return 0;
}

int  InterruptMask(int __intr, int __id)
{
    (void) __intr;
    (void) __id;

    return 0;
}

int  InterruptUnmask(int __intr, int __id)
{
    (void) __intr;
    (void) __id;

    return 0;
}

/* Timer */
int TimerTimeout(clockid_t __id, int __flags, const struct sigevent *__notify, const uint64_t *__ntime, uint64_t *__otime)
{
    (void) __id;
    (void) __flags;
    (void) __notify;
    (void) __ntime;
    (void) __otime;

    return 0;
}

/* clock */
int timer_create(clockid_t __clock_id, struct sigevent *__evp, timer_t *__timerid)
{
    (void) __clock_id;
    (void) __evp;
    (void) __timerid;

    g_timer_create_count++;

    return 0;
}

clock_t clock(void)
{
    return 0;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    (void) clk_id;
    (void) tp;
    static int clock_gettime_count = 1000000;

    if (tp)
    {
        tp->tv_sec  = 0;
        tp->tv_nsec = clock_gettime_count++;
    }

    return 0;
}

int clock_getres(clockid_t clk_id, struct timespec *res)
{
    (void) clk_id;
    (void) res;

    if (res)
    {
        res->tv_sec  = 0;
        res->tv_nsec = 1000000; /* 1 us */
    }

    return 0;
}

int clock_settime(clockid_t clk_id, const struct timespec *tp)
{
    (void) clk_id;
    (void) tp;

    return 0;
}

int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain)
{
    (void) clockid;
    (void) flags;
    (void) request;
    (void) remain;

    if (remain)
    {
        remain->tv_sec  = 0;
        remain->tv_nsec = 0;
    }

    return 0;
}

__inline__ uint64_t __attribute__((__unused__))
ClockCycles(void)
{
	uint64_t	cntv;

	/*
	 * We need an isb barrier to ensure our read occurs in program order
	 */
	__asm__ __volatile__(
		"isb	\n"
		"mrs	%0, cntvct_el0"
		: "=r"(cntv)
	);
	return cntv;
}

int ThreadCtl(int cmd, void * data)
{
    (void) cmd;
    (void) data;

    return 0;
}

long sysconf(int __name)
{
    (void) __name;

    return 4096;
}

/* mmap */
#define STUB_MMAP_SIZE    (64*1024*1024) /* 64MB */
char  stub_mmap_area[STUB_MMAP_SIZE] = {0};

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    (void) addr;
    (void) length;
    (void) prot;
    (void) flags;
    (void) fd;
    (void) offset;

    return &stub_mmap_area[0];
}

void *mmap64(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    (void) addr;
    (void) length;
    (void) prot;
    (void) flags;
    (void) fd;
    (void) offset;

    return &stub_mmap_area[0];
}

int munmap(void *addr, size_t length)
{
    (void) addr;
    (void) length;

    return 0;
}

void *mmap_device_memory(void *__addr, size_t __len, int __prot, int __flags, uint64_t __physical)
{
    return &stub_mmap_area[0];
}

int munmap_device_memory(void *__addr, size_t __len)
{
    return munmap(__addr, __len);
}

int mem_offset64(__const void *__addr, int __fd, size_t __len, off64_t *__off, size_t *__contig_len)
{
    (void) __addr;
    (void) __fd;
    (void) __contig_len;

    *__off = 4096U;

    return 0;
}

int mlock(__const void * __addr, size_t __len)
{
    (void) __addr;
    (void) __len;

    return 0;
}

int munlock(__const void *__addr, size_t __len)
{
    (void) __addr;
    (void) __len;

    return 0;
}

int msync(void *__addr, size_t __len, int __flags)
{
    (void) __addr;
    (void) __len;
    (void) __flags;

    return 0;
}


#define STUB_FD_MAX     (100)
#define STUB_FD_CMEM    (0)
#define STUB_FD_ELSE    (100)
int stub_fd[STUB_FD_MAX];
int stub_fd_count = 0;

/* open */
int open(const char *filename, int amode, ...)
{
    /* replace with AMSTB_open */
    (void) filename;
    (void) amode;

    return stub_fd_count++;
}

int close(int fd)
{
    (void) fd;

    return 0;
}

int posix_typed_mem_open(const char *__name, int __oflag, int __tflag)
{
    (void) __tflag;

    return open(__name, __oflag);
}

int fprintf( FILE *stream , const char *format , ... )
{
    /* replace with AMSTB_fprintf */
    (void) stream;
    (void) format;

    return 0;
}

//int snprintf(char *str, size_t size, const char *format, ...)
//{
//    /* replace with AMSTB_snprintf */
//    (void) str;
//    (void) size;
//    (void) format;
//
//    strncpy(str, format, size);
//
//    return strlen(str);
//}

/* CPG */
int cpg_init(int vbose)
{
    (void) vbose;

    return 0;
}

int cpg_deinit(void)
{
    return 0;
}

int cpg_mstp_is_enabled(int mod_id)
{
    (void) mod_id;

    /* 0 - Module is not enabled */
    return 0;
}

int cpg_mstp_enable(int mod_id)
{
    (void) mod_id;

    return 0;
}

int cpg_mstp_disable(int mod_id)
{
    (void) mod_id;

    return 0;
}

int cpg_mstp_reset(int mod_id)
{
    (void) mod_id;

    return 0;
}

int cpg_mstp_assert(int mod_id)
{
    (void) mod_id;

    return 0;
}

int cpg_mstp_deassert(int mod_id)
{
    (void) mod_id;

    return 0;
}

int cpg_mstp_reset_state(int mod_id)
{
    (void) mod_id;

    return 0;
}

int cpg_clk_is_enabled(cpg_clk_id_t id, int idx)
{
    (void) id;
    (void) idx;

    /* 0 - Module is not enabled */
    return 0;
}

int cpg_clk_enable(cpg_clk_id_t id, int idx)
{
    (void) id;
    (void) idx;

    return 0;
}

int cpg_clk_disable(cpg_clk_id_t id, int idx)
{
    (void) id;
    (void) idx;

    return 0;
}

int cpg_clk_rate_get(cpg_clk_id_t id, int idx, uint32_t *rate)
{
    (void) id;
    (void) idx;

    if (rate)
    {
        *rate = 1u;
    }

    return 0;
}

int cpg_clk_rate_set(cpg_clk_id_t id, int idx, uint32_t *rate)
{
    (void) id;
    (void) idx;
    (void) rate;

    return 0;
}

int cpg_clk_divider_get(cpg_clk_id_t id, int idx, uint32_t *div)
{
    (void) id;
    (void) idx;

    if (div)
    {
        *div = 64u;
    }

    return 0;
}

int cpg_clk_divider_set(cpg_clk_id_t id, int idx, uint32_t *div)
{
    (void) id;
    (void) idx;
    (void) div;

    return 0;
}

int cpg_mgr_getfuncs(cpg_mgr_funcs_t *funcs, int tabsize)
{
    (void) tabsize;

    funcs->init             = cpg_init;
    funcs->deinit           = cpg_deinit;
    funcs->mstp_is_enabled  = cpg_mstp_is_enabled;
    funcs->mstp_enable      = cpg_mstp_enable;
    funcs->mstp_disable     = cpg_mstp_disable;
    funcs->mstp_reset       = cpg_mstp_reset;
    funcs->mstp_assert      = cpg_mstp_assert;
    funcs->mstp_deassert    = cpg_mstp_deassert;
    funcs->mstp_reset_state = cpg_mstp_reset_state;
    funcs->clk_is_enabled   = cpg_clk_is_enabled;
    funcs->clk_enable       = cpg_clk_enable;
    funcs->clk_disable      = cpg_clk_disable;
    funcs->clk_rate_get     = cpg_clk_rate_get;
    funcs->clk_rate_set     = cpg_clk_rate_set;
    funcs->clk_divider_get  = cpg_clk_divider_get;
    funcs->clk_divider_set  = cpg_clk_divider_set;

    return 0;
}

/* SYSC */
int sysc_init(void)
{
    return 0;
}

void sysc_deinit(void)
{
    return 0;
}

int sysc_power_is_off(int pdr_number)
{
    (void) pdr_number;

    /* 0 - Power is ON */
    return 0;
}

int sysc_power_off(int pdr_number)
{
    (void) pdr_number;

    return 0;
}

int sysc_power_on(int pdr_number)
{
    (void) pdr_number;

    return 0;
}

int sysc_read_pdmode(void)
{
    return 0;
}

int sysc_mgr_getfuncs(sysc_mgr_funcs_t *funcs, int tabsize)
{
    (void) tabsize;

    funcs->init         = sysc_init;
    funcs->deinit       = sysc_deinit;
    funcs->power_is_off = sysc_power_is_off;
    funcs->power_off    = sysc_power_off;
    funcs->power_on     = sysc_power_on;
    funcs->read_pdmode  = sysc_read_pdmode;

    return 0;
}

