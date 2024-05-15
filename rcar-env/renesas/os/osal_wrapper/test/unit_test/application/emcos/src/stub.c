#include <stub.h>
#include <sys/types.h>

e_pma_return_t R_PMA_Init(st_pma_handle_t *pHandle, const st_pma_config_t *pConfig)
{
    return 0;
}
e_pma_return_t R_PMA_Quit(st_pma_handle_t handle)
{
    return 0;
}
e_pma_return_t R_PMA_GetLockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    return 0;
}
e_pma_return_t R_PMA_UnlockHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    return 0;
}
e_pma_return_t R_PMA_RequestHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    return 0;
}
e_pma_return_t R_PMA_ReleaseHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    return 0;
}
e_pma_return_t R_PMA_GetHwaState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t *pHwaState)
{
    return 0;
}
e_pma_return_t R_PMA_SetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t policy)
{
    return 0;
}
e_pma_return_t R_PMA_GetPowerPolicy(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_policy_t *pPolicy)
{
    return 0;
}
e_pma_return_t R_PMA_ApplyResetHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    return 0;
}
e_pma_return_t R_PMA_ReleaseResetHwa(st_pma_handle_t handle, e_pma_hwa_id_t hwaId)
{
    return 0;
}
e_pma_return_t R_PMA_GetReset(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_reset_req_t *pRstStat)
{
    return 0;
}
e_pma_return_t R_PMA_GetLowestPowerState(st_pma_handle_t handle, e_pma_hwa_id_t hwaId, e_pma_power_state_t *pLowestPower)
{
    return 0;
}
e_pma_return_t R_PMA_WaitForState(st_pma_handle_t const handle, e_pma_hwa_id_t hwaId, st_pma_hwa_state_t const * pHwaState, e_pma_reset_req_t rstStat, osal_milli_sec_t sleepTimePeriod, osal_milli_sec_t timeoutMsPeriod, osal_nano_sec_t timeoutNsPeriod)
{
    return 0;
}
e_pma_return_t R_PMA_SetPostClock(st_pma_handle_t const handle, e_pma_clk_req_t  clockEnable, uint32_t chanIdx, uint32_t const * pClockDivider)
{
    return 0;
}

int32_t uring_interrupt_create(uring_t *r, int32_t interrupts_count)
{
    return 0;
}

int32_t uring_destroy(uring_t r)
{
    return 0;
}

int32_t uring_interrupt_notify(uring_t r, const void *data, size_t data_size)
{
    return 0;
}

int32_t uring_interrupt_receive(uring_t r, void *data, size_t data_size)
{
    return 0;
}

void bios_cache_invalidate(uintptr_t addr, size_t size){}
void bios_cache_purge(uintptr_t addr, size_t size){}
void bios_cache_writeback(uintptr_t addr, size_t size){}

int posix_devctl(int fno, int dcmd, void* data, size_t size, int* info)
{
    return 0;
}

int open(const char *pathname, int flags)
{
    return 0;
}

int open64(const char * __file, int __oflag)
{
    return open(__file, __oflag);
}

int _DCTLOUT(uint32_t val)
{
    return (int)val;
}
int _DCTL(uint32_t val)
{
    return (int)val;
}
int _DCTLIN(uint32_t val)
{
    return (int)val;
}


typedef struct {
    uint64_t        cycles_per_sec;
} syspage_entry_info_dummy;

syspage_entry_info_dummy     qtime_dummy;
void *qtime = (void *)&qtime_dummy;

int clock_getres(clockid_t clockid, struct timespec *res)
{
    (void)clockid;
    (void)res;

    return 0;
}

int clock_gettime(clockid_t clock_id, struct timespec * tp)
{
    (void) clock_id;
    (void) tp;

    return 0;
}

// unistd.h
int close(int fildes)
{
    (void) fildes;

    return 0;
}

// sys/mman.h
/*int mem_offset64( const void * addr, int fd, size_t length, off64_t * offset, size_t * contig_len )
{
    (void) addr;
    (void) fd;
    (void) length;
    (void) offset;
    (void) contig_len;

    return 0;
}*/

void *mmap64(void * __addr, size_t __len, int __prot, int __flags, int __fd, __off64_t __offset)
{
    (void) __addr;
    (void) __len;
    (void) __prot;
    (void) __flags;
    (void) __fd;
    (void) __offset;

    return 0;

} 

void * mmap( void * addr, size_t len, int prot, int flags, int fildes, off_t off )
{
    (void) addr;
    (void) len;
    (void) prot;
    (void) flags;
    (void) fildes;
    (void) off;

    return 0;
}

int msync(void *addr, size_t len, int flags)
{
    (void) addr;
    (void) len;
    (void) flags;

    return 0;
}

int munmap(void *addr, size_t len)
{
    (void) addr;
    (void) len;

    return 0;
}

int posix_typed_mem_open(const char *name, int oflag, int tflag)
{
    (void) name;
    (void) oflag;
    (void) tflag;

    return 0;
}

// sys/neutrino.h
int InterruptAttachEvent(int intr, const struct sigevent* event, unsigned flags)
{
    (void) intr;
    (void) event;
    (void) flags;

    return 0;
}

int InterruptMask(int intr, int id)
{
    (void) intr;
    (void) id;

    return 0;
}

int InterruptUnmask(int intr, int id)
{
    (void) intr;
    (void) id;

    return 0;
}

int InterruptWait( int flags, const uint64_t * timeout )
{
    (void) flags;
    (void) timeout;

    return 0;
}

int ThreadCtl(int cmd, void * data)
{
    (void) cmd;
    (void) data;

    return 0;
}

uint64_t ClockCycles( void )
{
    return 0;
}

int TimerTimeout(clockid_t id, int flags, const struct sigevent * notify, const uint64_t * ntime, uint64_t * otime )
{
    (void) id;
    (void) flags;
    (void) notify;
    (void) ntime;
    (void) otime;
    return 0;
}

// pthread.h
int pthread_cond_broadcast(pthread_cond_t *cond)
{
    (void) cond;

    return 0;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime)
{
    (void) cond;
    (void) mutex;
    (void) abstime;

    return 0;
}

int pthread_mutex_lock( pthread_mutex_t* mutex )
{
    (void) mutex;

    return 0;
}

int pthread_mutex_unlock( pthread_mutex_t* mutex )
{
    (void) mutex;

    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr )
{
    (void) mutex;
    (void) attr;

    return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
    (void) attr;

    return 0;
}

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

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
    (void) attr;
    (void) param;

    return 0;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
    (void) attr;
    (void) policy;

    return 0;
}

int pthread_cancel(pthread_t thr)
{
    (void) thr;

    return 0;
}

int pthread_create( pthread_t* thread, const pthread_attr_t* attr, void* (*start_routine)(void* ), void* arg )
{
    (void) thread;
    (void) attr;
    (void) arg;
    (void) start_routine;

    return 0;
}

void *pthread_getspecific(pthread_key_t key)
{
    (void) key;

    return 0;
}

int pthread_join(pthread_t thr, void **value_ptr)
{
    (void) thr;
    (void) value_ptr;

    return 0;
}

int pthread_key_create(pthread_key_t *key, void (*destructor)(void *))
{
    (void) key;
    (void) destructor;

    return 0;
}

int pthread_key_delete(pthread_key_t key)
{
    (void) key;

    return 0;
}

int pthread_setschedprio(pthread_t thr, int prio)
{
    (void) thr;
    (void) prio;

    return 0;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    (void) key;
    (void) value;

    return 0;
}

int pthread_cond_destroy( pthread_cond_t* cond )
{
    (void) cond;

    return 0;
}

int pthread_cond_signal( pthread_cond_t* cond )
{
    (void) cond;

    return 0;
}

int pthread_cond_init( pthread_cond_t *cond, const pthread_condattr_t *attr )
{
    (void) cond;
    (void) attr;

    return 0;
}

int pthread_attr_setstacksize( pthread_attr_t * attr, size_t stacksize )
{
    (void) attr;
    (void) stacksize;

    return 0;
}

int pthread_detach( pthread_t thread )
{
    (void) thread;

    return 0;
}

pthread_t pthread_self( void )
{
    return 0;
}

int pthread_condattr_destroy(pthread_condattr_t* attr )
{
    (void) attr;

    return 0;
}

int pthread_condattr_init( pthread_condattr_t* attr )
{
    (void) attr;

    return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t * mutex, const struct timespec * abs_timeout )
{
    (void) mutex;
    (void) abs_timeout;
    return 0;
}

int pthread_mutex_trylock( pthread_mutex_t* mutex )
{
    (void) mutex;
    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t* attr )
{
    (void) attr;
    return 0;
}

int pthread_mutexattr_init( pthread_mutexattr_t* attr )
{
    (void) attr;
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t * attr, int type )
{
    (void) attr;
    (void) type;
    return 0;
}

int pthread_mutex_timedlock_monotonic(pthread_mutex_t * mutex, const struct timespec * abs_timeout )
{
    (void) mutex;
    (void) abs_timeout;
    return 0;
}

int pthread_timedjoin_monotonic(pthread_t thread, void** value_ptr, const struct timespec* abstime )
{
    (void) thread;
    (void) value_ptr;
    (void) abstime;
    return 0;
}

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id)
{
    (void) attr;
    (void) clock_id;
    return 0;
}

int pthread_cond_wait( pthread_cond_t* cond, pthread_mutex_t* mutex )
{
    (void) cond;
    (void) mutex;
    return 0;
}

int pthread_equal( pthread_t t1, pthread_t t2 )
{
    (void) t1;
    (void) t2;
    return 0;
}

// qnx710/target/qnx7/usr/include/sched.h
int sched_get_priority_min(int policy)
{
    (void) policy;
    return 0;
}

// hw/inout.h
uint16_t in16( uintptr_t port )
{
    (void) port;
    return 0;
}

uint32_t in32( uintptr_t port )
{
    (void) port;
    return 0;
}

uint8_t in8( uintptr_t port )
{
    (void) port;
    return 0;
}

void out16( uintptr_t port, uint16_t val )
{
    (void) port;
    (void) val;
    return;
}

void out32( uintptr_t port, uint32_t val )
{
    (void) port;
    (void) val;
    return;
}

void out8( uintptr_t port, uint8_t val )
{
    (void) port;
    (void) val;
    return;
}

// sys/mman.h
uintptr_t mmap_device_io( size_t len, uint64_t io )
{
    (void) len;
    (void) io;
    return 0;
}

int munmap_device_io( uintptr_t io, size_t len )
{
    (void) io;
    (void) len;
    return 0;
}

int clock_nanosleep( clockid_t clock_id, int flags, const struct timespec * rqtp, struct timespec * rmtp )
{
    (void) clock_id;
    (void) flags;
    (void) rqtp;
    (void) rmtp;
    return 0;
}

extern int mlock(const void *addr, size_t len)
{
    (void) addr;
    (void) len;
    return 0;
}

extern int munlock(const void *addr, size_t len)
{
    (void) addr;
    (void) len;
    return 0;
}

int munmap_device_memory( void * addr, size_t len )
{
    (void) addr;
    (void) len;
    return 0;
}

void * mmap_device_memory(void * addr, size_t len, int prot, int flags, uint64_t physical)
{
    (void) addr;
    (void) len;
    (void) prot;
    (void) flags;
    (void) physical;

    return 0;
}

unsigned long strnlen(const char *s, size_t maxlen)
{
    (void) s;
    (void) maxlen;
    return 0;
}

long sysconf(int name)
{
    (void) name;
    return 0;
}

int error_stub(void)
{
    return 0;
}

// sys/slog.h
int slogb(int code, int severity, void *data, int size)
{
    (void)code;
    (void)severity;
    (void)data;
    (void)size;
    return 0;
}

#if 0 /* as stub, __NTO_va_list cannot be used on Armclang */
int vslogf(int code, int severity, const char *fmt, __NTO_va_list __arg ) __attribute__((__format__(__printf__, 3, 0)))
{
    (void)code;
    (void)severity;
    (void)fmt;
    return 0;
}
#endif

#if 0 /* as stub, __format__ cannot be used on Armclang */
int slogf(int code, int severity, const char *fmt, ...) __attribute__((__format__(__printf__, 3, 4)))
{
    (void)code;
    (void)severity;
    (void)fmt;
    return 0;
}
#endif

int slogi(int code, int severity, int nargs, ...)
{
    (void)code;
    (void)severity;
    (void)nargs;
    return 0;
}

/*int slogdecode(char *buf, int len, slogdecode_t *rec)
{
    (void)buf;
    (void)len;
    (void)rec;
    return 0;
}*/

// membarrier
void __cpu_membarrier(void)
{
    return;
}

unsigned long strlen(const char *s)
{
    (void) s;
    return 0;
}

char *strncpy(char *s1, const char *s2, unsigned long n)
{
    (void) s1;
    (void) s2;
    (void) n;
    return 0;
}

void *malloc(unsigned long size)
{
    (void) size;
    return 0;
}

void *memset(void *s, int c, unsigned long n)
{
    (void) s;
    (void) c;
    (void) n;
    return 0;
}

int strcmp(const char *s1, const char *s2)
{
    (void) s1;
    (void) s2;
    return 0;
}

int strncmp(const char *s1, const char *s2, unsigned long n)
{
    (void) s1;
    (void) s2;
    (void) n;
    return 0;
}

void free(void *ptr)
{
    (void) ptr;
    return;
}

void *memcpy(void *dst, const void *src, unsigned long nbytes)
{
    (void) dst;
    (void) src;
    (void) nbytes;
    return 0;
}

void *realloc(void *__ptr, unsigned long __size)
{
    (void) __ptr;
    (void) __size;
    return 0;
}

extern int InterruptWait_r(int __flags, const uint64_t *__timeout)

{
    (void) __flags;
    (void) __timeout;
    return 0;

}
