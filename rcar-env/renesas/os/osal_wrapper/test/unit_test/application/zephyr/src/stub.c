#include <stub.h>
#include <sys/types.h>
#include <zephyr/drivers/clock_control.h>

int cache_data_range(void *addr, size_t size, int op)
{
    (void)addr;
    (void)size;
    (void)op;

    return 0;
}

void *calloc(size_t nmemb, size_t size)
{
    (void)nmemb;
    (void)size;

    return 0;
}
#if 0
enum clock_control_status clock_control_get_status(const struct device *dev, clock_control_subsys_t sys)
{
    (void)dev;
    (void)sys;

    return CLOCK_CONTROL_STATUS_STARTING;
}

int clock_control_off(const struct device *dev, clock_control_subsys_t sys)
{
    (void)dev;
    (void)sys;

    return 0;
}

int clock_control_on(const struct device *dev, clock_control_subsys_t sys)
{
    (void)dev;
    (void)sys;

    return 0;
}

int clock_control_set_rate(const struct device *dev, clock_control_subsys_t sys, clock_control_subsys_rate_t rate)
{
    (void)dev;
    (void)sys;
    (void)rate;

    return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec *ts)
{
    (void)clock_id;
    (void)ts;

    return 0;
}
#endif

void free(void *ptr)
{
    (void)ptr;

    return;
}

#if 0
int irq_connect_dynamic(unsigned int irq, unsigned int priority, void (*routine)(const void *parameter), const void *parameter, uint32_t flags)
{
    (void)irq;
    (void)routine;
    (void)parameter;
    (void)flags;

    return 0;
}

unsigned int arch_irq_lock(void)
{
    return 0;
}

void arch_irq_unlock(unsigned int key)
{
    (void)key;

    return;
}

k_spinlock_key_t k_spin_lock(struct k_spinlock *l)
{
    k_spinlock_key_t dummy;

    (void)l;

    return dummy;
}

void k_spin_unlock(struct k_spinlock *l, k_spinlock_key_t key)
{
    (void)l;
    (void)key;
    return;
}

uint32_t k_cycle_get_32(void)
{
    return 0;
}
#endif

bool k_is_in_isr(void)
{
    return true;
}

#if 0
void k_yield(void)
{
    return;
}
#endif

void *memcpy(void *dest, const void *src, size_t n)
{
    (void)dest;
    (void)src;
    (void)n;

    return 0;
}

void *memset(void *s, int c, size_t n)
{
    (void)s;
    (void)c;
    (void)n;

    return 0;
}

int nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    (void)rqtp;
    (void)rmtp;

    return 0;
}

int pm_device_runtime_enable(const struct device *dev)
{
    (void)dev;

    return 0;
}

int pm_device_runtime_get(const struct device *dev)
{
    (void)dev;

    return 0;
}

int pm_device_runtime_put(const struct device *dev)
{
    (void)dev;

    return 0;
}

int pm_device_state_get(const struct device *dev, enum pm_device_state *state)
{
    (void)dev;
    (void)state;

    return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
    (void)attr;

    return 0;
}

int pthread_attr_init(pthread_attr_t *attr)
{
    (void)attr;

    return 0;
}

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
    (void)attr;
    (void)param;

    return 0;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
    (void)attr;
    (void)policy;

    return 0;
}

int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize)
{
    (void)attr;
    (void)stackaddr;
    (void)stacksize;

    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    (void)cond;

    return 0;
}

#if 0
int pthread_cond_destroy(pthread_cond_t *cond)
{
    (void)cond;

    return 0;
}

int pthread_equal (pthread_t __thread1, pthread_t __thread2)
{
    (void)__thread1;
    (void)__thread2;
    return 0;
}
#endif

int pthread_cond_signal(pthread_cond_t *cond)
{
    (void)cond;

    return 0;
}

#if 0
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr)
{
    (void)cond;
    (void)attr;

    return 0;
}
#endif

int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime)
{
    (void)cond;
    (void)mutex;
    (void)abstime;

    return 0;
}

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex)
{
    (void)cond;
    (void)mutex;

    return 0;
}

int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)
{
    (void)thread;
    (void)attr;
    (void)arg;
    (void)start_routine;

    return 0;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    (void)key;
    (void)value;

    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    (void)mutex;

    return 0;
}
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
{
    (void)mutex;
    (void)attr;

    return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout)
{
    (void)mutex;
    (void)abs_timeout;

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    (void)mutex;

    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    (void)mutex;

    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    (void)mutex;

    return 0;
}

#if 0
int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    (void)attr;

    return 0;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    (void)attr;

    return 0;
}
#endif

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    (void)attr;
    (void)type;

    return 0;
}

#if 0
pthread_t pthread_self(void)
{
    return 0;
}
#endif

void *realloc(void *ptr, size_t size)
{
    (void)ptr;
    (void)size;

    return 0;
}

int sem_destroy(sem_t *semaphore)
{
    (void)semaphore;

    return 0;
}

int sem_init(sem_t *semaphore, int pshared, unsigned int value)
{
    (void)semaphore;
    (void)pshared;
    (void)value;
    return 0;
}

int sem_post(sem_t *semaphore)
{
    (void)semaphore;

    return 0;
}

int sem_timedwait(sem_t *semaphore, struct timespec *abstime)
{
    (void)semaphore;
    (void)abstime;
    return 0;
}

int sem_getvalue(sem_t *semaphore, int *value)
{
    (void)semaphore;
    (void)value;
    return 0;
}

int sem_trywait(sem_t *semaphore)
{
    (void)semaphore;

    return 0;
}

void pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    (void)routine;
    (void)arg;

    return;
}

size_t strlen(const char *s)
{
    (void)s;

    return 0;
}

char *strncat(char *dest, const char *src, size_t n)
{
    (void)dest;
    (void)src;
    (void)n;

    return 0;
}

int strcmp(const char *s1, const char *s2)
{
    (void)s1;
    (void)s2;

    return 0;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
    (void)s1;
    (void)s2;
    (void)n;

    return 0;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    (void)dest;
    (void)src;
    (void)n;

    return 0;
}

size_t strnlen(const char *s, size_t maxlen)
{
    (void)s;
    (void)maxlen;

    return 0;
}

char *strstr(const char *s, const char *find)
{
    (void)s;
    (void)find;

    return 0;
}


unsigned long long __udivdi3(unsigned long long a, unsigned long long b)
{
	(void)a;
	(void)b;

	return 0;
}

long long __divdi3(long long a, long long b)
{
	(void)a;
	(void)b;

	return 0;
}

long long __moddi3(long long a, long long b)
{
	(void)a;
	(void)b;

	return 0;
}

k_ticks_t z_timeout_expires(const struct _timeout *timeout)
{
	(void)timeout;

	return 0;
}

k_ticks_t z_timeout_remaining(const struct _timeout *timeout)
{
	(void)timeout;

	return 0;
}


int arch_dcache_all(int op)
{
	(void)op;

	return 0;
}

int arch_dcache_range(void *addr, size_t size, int op)
{
	(void)addr;
	(void)size;
	(void)op;

	return 0;
}

int arch_icache_all(int op)
{
	(void)op;

	return 0;
}

int arch_icache_range(void *addr, size_t size, int op)
{
	(void)addr;
	(void)size;
	(void)op;

	return 0;
}

int arch_irq_connect_dynamic(unsigned int irq, unsigned int priority,
			     void (*routine)(const void *parameter),
			     const void *parameter, uint32_t flags)
{
	(void)irq;
	(void)priority;
	(void)routine;
	(void)parameter;
	(void)flags;

	return 0;
}

void arch_irq_disable(unsigned int irq)
{
	(void)irq;

	return;
}

void arch_irq_enable(unsigned int irq)
{
	(void)irq;

	return;
}

bool z_device_is_ready(const struct device *dev)
{
	(void)dev;

	return true;
}

void z_impl_k_yield(void)
{
	return;
}

int z_impl_clock_gettime(clockid_t clock_id, struct timespec *ts)
{
	(void)clock_id;
	(void)ts;

	return 0;
}

k_tid_t z_impl_z_current_get(void)
{
	return 0;
}

uint32_t sys_clock_cycle_get_32(void)
{
	return 0;
}

void *bsearch(const void *key, const void *array, size_t count, size_t size,
	      int (*cmp)(const void *key, const void *element))
{
	return 0;
}

int fprintf( FILE *stream , const char *format , ... )
{
    /* replace with AMSTB_fprintf */
    (void) stream;
    (void) format;

    return 0;
}

int snprintf(char *str, size_t size, const char *format, ...)
{
    /* replace with AMSTB_snprintf */
    (void) str;
    (void) size;
    (void) format;

    strncpy(str, format, size);

    return strlen(str);
}

int k_thread_cpu_mask_clear(k_tid_t thread)
{
    /* replace with AMSTB_k_thread_cpu_mask_clear */
	(void) thread;

	return 0;
}

int k_thread_cpu_mask_enable(k_tid_t thread, int cpu)
{
    /* replace with AMSTB_k_thread_cpu_mask_enable */
	(void) thread;
	(void) cpu;

	return 0;
}

int pthread_setname_np(pthread_t thread, const char *name)
{
    /* replace with AMSTB_pthread_setname_np */
	(void) thread;
	(void) name;

	return 0;
}

void z_impl_k_thread_resume(struct k_thread *thread)
{
    /* replace with AMSTB_z_impl_k_thread_resume */
	(void) thread;

	return;
}

void z_impl_k_thread_suspend(struct k_thread *thread)
{
    /* replace with AMSTB_z_impl_k_thread_suspend */
	(void) thread;

	return;
}

int pthread_detach(pthread_t thread)
{
    /* replace with AMSTB_pthread_detach */
	(void) thread;

	return 0;
}
