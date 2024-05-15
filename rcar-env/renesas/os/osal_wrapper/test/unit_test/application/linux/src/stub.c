#include "stub.h"
#include "rcar-xos/memory_allocator/r_meml.h"

void *calloc(size_t nmemb, size_t size)
{
    (void) nmemb;
    (void) size;
    return 0;
}

void *realloc(void *ptr, size_t size)
{
    (void) ptr;
    (void) size;
    return 0;
}

void free(void *ptr)
{
    (void) ptr;
    return;
}

void *memset(void *s, int c, size_t n)
{
    (void) s;
    (void) c;
    (void) n;
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    (void) dest;
    (void) src;
    (void) n;
    return 0;
}

clock_t clock(void)
{
    return 0;
}

int clock_getres(clockid_t clk_id, struct timespec *res)
{
    (void) clk_id;
    (void) res;

    return 0;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    (void) clk_id;
    (void) tp;

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

    return 0;
}

int ioctl(int fd, unsigned long request, ...)
{
    (void) fd;
    (void) request;

    return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout)
{
    (void) mutex;
    (void) abs_timeout;

    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}

int close(int fd)
{
    (void) fd;

    return 0;
}

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    (void) addr;
    (void) length;
    (void) prot;
    (void) flags;
    (void) fd;
    (void) offset;

    return 0;
}

int munmap(void *addr, size_t length)
{
    (void) addr;
    (void) length;

    return 0;
}

int open(const char *filename, int amode, ...)
{
    (void) filename;
    (void) amode;

    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    (void) mutex;

    return 0;
}
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
{
    (void) mutex;
    (void) attr;

    return 0;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    (void) fds;
    (void) nfds;
    (void) timeout;

    return 0;
}

int pthread_attr_init(pthread_attr_t *attr)
{
    (void) attr;

    return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr)
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
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inheritsched)
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

int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param)
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

int pthread_attr_getschedpolicy(const pthread_attr_t *restrict attr, int *restrict policy)
{
    (void) attr;
    (void) policy;

    return 0;
}

int pthread_cancel(pthread_t thread)
{
    (void) thread;

    return 0;
}

int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)
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

int pthread_setspecific(pthread_key_t key, const void *value)
{
    (void) key;
    (void) value;

    return 0;
}

int pthread_join(pthread_t thread, void **retval)
{
    (void) thread;
    (void) retval;

    return 0;
}

int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
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

int pthread_setschedprio(pthread_t thread, int prio)
{
    (void) thread;
    (void) prio;

    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
    (void) attr;

    return 0;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
    (void) attr;

    return 0;
}
int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr, int *restrict type)
{
    (void) attr;
    (void) type;

    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
    (void) attr;
    (void) type;

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

int pthread_cond_destroy(pthread_cond_t *cond)
{
    (void) cond;

    return 0;
}

int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr)
{
    (void) cond;
    (void) attr;

    return 0;
}

int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime)
{
    (void) cond;
    (void) mutex;
    (void) abstime;

    return 0;
}

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex)
{
    (void) cond;
    (void) mutex;

    return 0;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    (void) attr;
    (void) stacksize;

    return 0;
}

int pthread_attr_getstacksize(const pthread_attr_t *restrict attr, size_t *restrict stacksize)
{
    (void) attr;
    (void) stacksize;

    return 0;
}

void pthread_exit(void *retval)
{
    (void) retval;
}

pthread_t pthread_self(void)
{
    return 0;
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

int __sigsetjmp (struct __jmp_buf_tag *__env, int __savemask)
{
    (void)__env;
    (void)__savemask;
    return 0;
}

void __pthread_register_cancel (__pthread_unwind_buf_t *__buf)
{
    (void)__buf;
}

void __pthread_unregister_cancel (__pthread_unwind_buf_t *__buf)
{
    (void)__buf;
}

void __pthread_unwind_next(__pthread_unwind_buf_t * __buf)
{
    (void)__buf;
}

long int syscall (long int __sysno, ...)
{
    (void)__sysno;
    return 0;
}

ssize_t read(int fd, void *buf, size_t count)
{
    (void) fd;
    (void) buf;
    (void) count;

    return 0;
}

ssize_t write(int fd, const void *buf, size_t count)
{
    (void) fd;
    (void) buf;
    (void) count;

    return 0;
}

int access(const char *filename, int amode)
{
    (void) filename;
    (void) amode;

    return 0;
}

int flock(int fd, int operation)
{
    (void) fd;
    (void) operation;

    return 0;
}

int glob(const char *pattern, int flags, int (*errfunc) (const char *epath, int eerrno), glob_t *pglob)
{
    (void) pattern;
    (void) flags;
    (void) errfunc;
    (void) pglob;

    return 0;
}

void globfree(glob_t *pglob)
{
    (void) pglob;
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz)
{
    (void) pathname;
    (void) buf;
    (void) bufsiz;

    return 0;
}

uint32_t ntohl(uint32_t netlong)
{
    (void) netlong;

    return 0;
}

uint16_t ntohs(uint16_t netshort)
{
    (void) netshort;
    return 0;
}

int regcomp(regex_t *restrict preg, const char *restrict regex, int cflags)
{
    (void) preg;
    (void) regex;
    (void) cflags;

    return 0;
}

int regexec(const regex_t *restrict preg, const char *restrict string, size_t nmatch, regmatch_t pmatch[restrict], int eflags)
{
    (void) preg;
    (void) string;
    (void) nmatch;
    (void) pmatch;
    (void) eflags;

    return 0;
}

size_t regerror(int errcode, const regex_t *restrict preg, char *restrict errbuf, size_t errbuf_size)
{
    (void) errcode;
    (void) preg;
    (void) errbuf;
    (void) errbuf_size;

    return 0;
}

void regfree(regex_t *preg)
{
    (void) preg;
}

int fscanf(FILE *stream, const char *format, ...)
{
    (void) stream;
    (void) format;

    return 0;
}

size_t strnlen(const char *s, size_t maxlen)
{
    (void) s;
    (void) maxlen;

    return 0;
}

DIR *opendir(const char *name)
{
    (void) name;
    return 0;
}

struct dirent *readdir(DIR *dirp)
{
    (void) dirp;
    return 0;
}

int closedir(DIR *dirp)
{
    (void) dirp;
    return 0;
}

long int sysconf(int __name)
{
    (void) __name;
    return 0;
}

char *__xpg_basename(char *__path)
{
    (void) __path;
    return 0;
}

int pthread_equal (pthread_t __thread1, pthread_t __thread2)
{
    (void) __thread1;
    (void) __thread2;
    return 0;
}

char *strcpy(char *dest, const char *src)
{
    (void) dest;
    (void) src;
    return 0;
}

char *strncpy(char *dest, const char *src, size_t n)
{
    (void) dest;
    (void) src;
    (void) n;
    return 0;
}

char *strcat(char *dest, const char *src)
{
    (void) dest;
    (void) src;
    return 0;
}

void pthread_cleanup_push(void (*routine)(void *), void *arg)
{
    (void) routine;
    (void) arg;
    return;
}

void pthread_cleanup_pop(int execute)
{
    (void) execute;
    return;
}

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset)
{
    (void) thread;
    (void) cpusetsize;
    (void) cpuset;
    return 0;
}

int pthread_setname_np(pthread_t thread, const char *name)
{
    (void) thread;
    (void) name;
    return 0;
}

void CPU_ZERO(cpu_set_t *set)
{
    (void) set;
    return;
}

void CPU_SET(int cpu, cpu_set_t *set)
{
    (void) cpu;
    (void) set;
    return;
}
