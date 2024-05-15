#include "stub.h"
#include "rcar-xos/memory_allocator/r_meml.h"
#include "target/linux/r_osal_os_types.h"
#include "target/common/dev_cfg/dynamic/r_osal_dev_cfg_share.h"

void *memcpy(void *dest, const void *src, size_t n);

/* malloc */
#define STUB_MALLOC_ALIGN   (8)
#define STUB_MALLOC_SIZE    (0x2000000)
#define STUB_MALLOC_COUNT   (8000)
typedef struct {
    size_t  size;
    void*   ptr;
} st_STUB_MALLOC_TBL;
union
{
    void* dummy;
    char  array[STUB_MALLOC_SIZE];
} stub_heap;
size_t  stub_malloc_pos = 0;
size_t  stub_malloc_count = 0;
st_STUB_MALLOC_TBL stub_malloc_tbl[STUB_MALLOC_COUNT];

uint64_t g_stub_time;

void init_stub_time()
{
    g_stub_time = 0;
}

uint64_t get_stub_time()
{
    return g_stub_time;
}

#ifndef WINAMS_STUB
struct timespec g_abstime_before;
struct timespec g_abstime_after;
#define SEC_TO_NSEC (1000000000)
#define START_TIMER do{clock_gettime(CLOCK_REALTIME, &g_abstime_before);}while(0);
#define STOP_TIMER do{clock_gettime(CLOCK_REALTIME, &g_abstime_after);g_stub_time+=(g_abstime_after.tv_sec * SEC_TO_NSEC + g_abstime_after.tv_nsec) - (g_abstime_before.tv_sec * SEC_TO_NSEC + g_abstime_before.tv_nsec);}while(0);
#else
#define START_TIMER do{}while(0);
#define STOP_TIMER do{}while(0);
#endif

void *malloc(size_t size)
{
    size_t size_a;
    void *ptr;

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
    ptr = (void*)&stub_heap.array[stub_malloc_pos];
    stub_malloc_tbl[stub_malloc_count].size = size;
    stub_malloc_tbl[stub_malloc_count].ptr = ptr;
    stub_malloc_pos += size_a;
    stub_malloc_count++;

    return ptr;
}

void *calloc(size_t num, size_t nsize)
{
    size_t size;
    void *p;
START_TIMER
    size = num * nsize;
    p = malloc(size);
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

void *realloc(void *ptr, size_t size)
{
START_TIMER
    size_t i;
    void *ptr_new;
    
    if (ptr == NULL)
    {
        ptr_new = malloc(size);    // malloc new area
    }
    else
    {
        /* search ptr */
        for (i=0; i<stub_malloc_count; i++)
        {
            if(stub_malloc_tbl[stub_malloc_count].ptr == ptr)
                break;
        }
        if (i > stub_malloc_count)
        {
STOP_TIMER
            return malloc(size);    // malloc new area
        }

        if (stub_malloc_tbl[i].size > size)
        {
            STOP_TIMER
            return ptr;
        }
        /* realloc */
        ptr_new = malloc(size);
        STOP_TIMER
        if (ptr_new != NULL)
        {
            memcpy(ptr_new, ptr, size);
            free(ptr);
        }
    }
    return ptr_new;
}

void *memcpy(void *dest, const void *src, size_t n)
{
START_TIMER
    uint8_t *p_dest = dest;
    uint8_t *p_src = src;
    size_t i;

    if ((dest != NULL) && (src != NULL))
    {
        for (i=0; i<n; i++)
        {
            *(p_dest++) = *(p_src++);
        }
    }
STOP_TIMER
    return dest;
}

void *memset(void *s, int c, size_t n)
{
START_TIMER
    volatile uint8_t *ptr = s;
    size_t i;

    if (s != NULL)
    {
        for (i=0; i<n; i++)
        {
            *(ptr++) = 0;
        }
    }
STOP_TIMER
    return s;
}

size_t strlen(const char *s)
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

char *strncpy(char *dest, const char *src, size_t n)
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
        if((*dest++ = *src++) == '\0')
            break;
    }
//    while (n--)
//        *dest++ = '\0';
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

int strncmp(const char *s1, const char *s2, size_t n)
{
  signed char res = 0;
 
START_TIMER
  while (n) {
    if ((res = *s1 - *s2++) != 0 || !*s1++) {
      break;
    }
 
    n--;
  }
STOP_TIMER
 
  return res;
}

/* file */
FILE *fopen(const char *pathname, const char *mode)
{
    FILE *stream;
    char *s;

    stream = malloc(sizeof(FILE) + 100);
    if (stream != NULL)
    {
        s = (char*)stream + sizeof(FILE);
        strncpy(s, pathname, 100);          /* save pathname */
    }

    if(strcmp(pathname, "%s%s") == 0)
    {
        stream = NULL;
    }

    return stream;
}

int fclose(FILE *stream)
{
    free(stream);
    return 0;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    char *pathname;

    pathname = (char*)stream + sizeof(FILE);
    if (strcmp(pathname, "/dev/uio0") == 0)
    {
        *(uint32_t*)ptr = 1;  /* local_ipmmu_id */
    }
    else if(strcmp(pathname, "/sys/module/cmemdrv/parameters/bsize") == 0)
    {
        *(uint32_t*)ptr = 0x1000;  /* cmem bsize */
    }
    else if(strcmp(pathname, "/sys/class/uio/%s/device/of_node/ipmmu-id") == 0)
    {
        *(uint32_t*)ptr = 0x02000000;  /* local_ipmmu_id=2 */
    }
    else if(strcmp(pathname, "%s/device/of_node/iommus") == 0)
    {
        *(uint32_t*)ptr = 0x02000000;  /* ipmmu_id=2 */
        *((uint32_t*)ptr + 1) = 0x02000000;  /* ipmmu_id=2 */
        size = 2;
        nmemb = 1;
    }

    return size*nmemb;
}

/* glob */
/*
    struct { 
        size_t gl_pathc;            //use
        char **gl_pathv;            //use
        size_t gl_offs; 
        int gl_flags; 
        void (*gl_closedir)(void *); 
        void *(*gl_readdir)(void *); 
        void *(*gl_opendir)(const char *); 
        int (*gl_lstat)(const char *, void *); 
        int (*gl_stat)(const char *, void *); 
    } glob_t; 
*/
int glob(const char *pattern, int flags, int (*errfunc) (const char *epath, int eerrno), glob_t *pglob)
{
    (void) pattern;
    (void) flags;
    (void) errfunc;
    (void) pglob;

START_TIMER
    if (strcmp("/dev/*", pattern) == 0)
    {
        static char *pathv_imp[8] = {"/dev/imp_00", "/dev/imp_01",
                                     "/dev/uio0", "/dev/uio1", 
                                     "/dev/irq_1_00", "/dev/irq_1_01", 
                                     "/dev/fba_imr0", "/dev/fba_imr1"};
        pglob->gl_pathc = 8;
        pglob->gl_pathv = pathv_imp;
STOP_TIMER
        return 0;
    }
    if (strcmp("/dev/%s_0[0-9]*", pattern) == 0)
    {
        static char *pathv_imp[2] = {"/dev/imp_00", "/dev/imp_01"};
        pglob->gl_pathc = 2;
        pglob->gl_pathv = pathv_imp;
STOP_TIMER
        return 0;
    }
    if (strcmp("/dev/uio*", pattern) == 0)
    {
        static char *pathv_uio[2] = {"/dev/uio0", "/dev/uio1"};
        pglob->gl_pathc = 2;
        pglob->gl_pathv = pathv_uio;
STOP_TIMER
        return 0;
    }
    if (strcmp("/dev/irq_%s_??", pattern) == 0)
    {
        static char *pathv_irq[2] = {"/dev/irq_1_00", "/dev/irq_1_01"};
        pglob->gl_pathc = 2;
        pglob->gl_pathv = pathv_irq;
STOP_TIMER
        return 0;
    }
    if (strcmp("/sys/class/uio/%s/device/of_node/ipmmu-id", pattern) == 0)
    {

    }
    if (strcmp("/dev/%s_[0-9a-z]*", pattern) == 0)
    {
        static char *pathv_fba[2] = {"/dev/fba_imr0", "/dev/fba_imr1"};
        pglob->gl_pathc = 2;
        pglob->gl_pathv = pathv_fba;
STOP_TIMER
        return 0;
    }
    if (strcmp("/sys/class/uio/uio*", pattern) == 0)
    {
        static char *pathv_fba[1] = {"/sys/class/uio/uio21"};
        pglob->gl_pathc = 1;
        pglob->gl_pathv = pathv_fba;
STOP_TIMER
        return 0;
    }

STOP_TIMER
    return -1;
}

void globfree(glob_t *pglob)
{
    (void) pglob;
    return;
}

/* pthread_mutex */
typedef struct {
    void* array[10];
} dummy_thread;
dummy_thread stub_thread_main;
dummy_thread stub_thread;
dummy_thread stub_thread_main;
int stub_pthread_create_count = 0;

void *stub_p_setspecific = NULL;

int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
{
    (void) mutex;
    (void) attr;

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

    return 0;
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
        free(stub_p_setspecific);
        stub_p_setspecific = NULL;
    }
    return 0;
}

void *pthread_getspecific(pthread_key_t key)
{
    (void) key;

    return stub_p_setspecific;   //
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    (void) key;
    (void) value;
    stub_p_setspecific = value;  // free in pthread_key_delete
    return 0;
}

/* pthread */
int pthread_join(pthread_t thread, void **retval)
{
    (void) thread;
    (void) retval;

    if (retval != NULL)
    {
        *retval = NULL;     //
    }
    return 0;
}

int pthread_setschedprio(pthread_t thread, int prio)
{
    (void) thread;
    (void) prio;

    return 0;
}

/* pthread_cond */
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr)
{
    (void) cond;
    (void) attr;

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
    (void) cond;
    (void) mutex;

    return 0;
}

int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime)
{
    (void) cond;
    (void) mutex;
    (void) abstime;

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

int pthread_attr_destroy(pthread_attr_t *attr)
{
    (void) attr;

    return 0;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    (void) attr;
    (void) stacksize;

    return 0;
}

/* pthread */
int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)
{
    (void) thread;
    (void) attr;
    (void) arg;
    (void) start_routine;

    void *ptr;
    st_osal_interrupt_control_t * p_local_int_handle;
 
    *thread = &stub_thread;    // dummy of created thread
    // ptr = start_routine(arg);

     p_local_int_handle = (st_osal_interrupt_control_t *)arg;
     p_local_int_handle->irq_thread_active = true;

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
    return &stub_thread_main;   // dummy of main thread
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

int pthread_setname_np(pthread_t thread, const char *name)
{
    (void)thread;
    (void)name;
    return 0;
}

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize, const cpu_set_t *cpuset)
{
    (void)thread;
    (void)cpusetsize;
    (void)cpuset;
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

long int syscall (long int __sysno, ...)
{
    (void)__sysno;
    return 0;
}

/* clock */
#ifdef WINAMS_STUB
clock_t clock(void)
{
    return 0;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
    (void) clk_id;
    (void) tp;
    static int clock_gettime_count = 0;

    if (tp)
    {
        tp->tv_sec = 0;
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
        res->tv_sec = 0;
        res->tv_nsec = 1000;     /* 1 us */
    }
    return 0;
}

int clock_settime(clockid_t clk_id, const struct timespec *tp)
{
    (void) clk_id;
    (void) tp;

    return 0;
}
#endif /* WINAMS_STUB */

int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain)
{
    (void) clockid;
    (void) flags;
    (void) request;
    (void) remain;

    if (remain)
    {
        remain->tv_sec = 0;
        remain->tv_nsec = 0;
    }
    return 0;
}

/* mmap */
#define STUB_MMAP_SIZE    (0x2000)
union
{
    void* dummy;
    char  array[STUB_MMAP_SIZE];
} stub_mmap_area;

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    (void) addr;
    (void) length;
    (void) prot;
    (void) flags;
    (void) fd;
    (void) offset;

    return &stub_mmap_area;    //
}

int munmap(void *addr, size_t length)
{
    (void) addr;
    (void) length;

    return 0;
}

#define STUB_FD_MAX     (100)
#define STUB_FD_CMEM    (0)
#define STUB_FD_ELSE    (100)
int stub_fd[STUB_FD_MAX];
int stub_fd_count = 0;

stub_cmem_area_t stub_cmem_area;

stub_cmem_area_t * get_cmem_area()
{
    return &stub_cmem_area;
}


/* open */
int open(const char *filename, int amode, ...)
{
    /* replace with AMSTB_open */
    (void) filename;
    (void) amode;

    if (strcmp(filename, "/dev/cmem0") == 0)
    {
        stub_fd[stub_fd_count] = STUB_FD_CMEM;
    }
    else
    {
        stub_fd[stub_fd_count] = STUB_FD_ELSE;
    }
    return stub_fd_count++;
}

int gPowerStatus=1;
int gClkStatus=0;
int ioctl(int fd, unsigned long request, ...)
{
    (void) fd;
    (void) request;
    int *tmp;
#ifndef WINAMS_STUB
    va_list args;
    void *ioinfo;

    /* ioinfo */
    va_start(args, request);

    if(UIO_PDRV_SET_PWR == request)
    {
        tmp = va_arg(args, int*);
        gPowerStatus = *tmp;
    }
    else if(UIO_PDRV_SET_CLK == request)
    {
        tmp = va_arg(args, int*);
        gClkStatus = *tmp;
    }
    else if(UIO_PDRV_SET_RESET == request)
    {
        // nop
    }
    else if(UIO_PDRV_GET_PWR == request)
    {
        tmp = va_arg(args, int*);
        *tmp = gPowerStatus;
    }
    else if(UIO_PDRV_GET_CLK == request)
    {
        tmp = va_arg(args, int*);
        *tmp = gClkStatus;
    }
    else if(UIO_PDRV_GET_RESET == request)
    {
        // nop
    }
    else
    {
        // nop
    }

    ioinfo = va_arg(args, void *);
    if (fd < stub_fd_count)
    {
        if (stub_fd[fd] == STUB_FD_CMEM)
        {
            ((struct mem_info *)ioinfo)->phys_addr = &stub_cmem_area;
        }
    }
    va_end(args);
#endif /* WINAMS_STUB */
    return 0;
}

ssize_t read(int fd, void *buf, size_t count)
{
    (void) fd;
    (void) buf;

    return count;
}

ssize_t write(int fd, const void *buf, size_t count)
{
    (void) fd;
    (void) buf;

    return count;
}

int flock(int fd, int operation)
{
    (void) fd;
    (void) operation;

    return 0;
}

int close(int fd)
{
    (void) fd;

    return 0;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    (void) fds;
    (void) nfds;
    (void) timeout;

    return 0;
}

int access(const char *filename, int amode)
{
    (void) filename;
    (void) amode;

    return 0;
}

ssize_t readlink(const char *pathname, char *buf, size_t bufsiz)
{
    (void) pathname;
    (void) buf;
    (void) bufsiz;

    strncpy(buf, pathname, bufsiz);
    return strlen(buf);
}

#ifndef WINAMS_STUB
extern uint32_t ntohl (uint32_t __netlong) __THROW __attribute__ ((__const__));
extern uint16_t ntohs (uint16_t __netshort) __THROW __attribute__ ((__const__));
#ifdef __OPTIMIZE__
# if __BYTE_ORDER == __BIG_ENDIAN
# define ntohl(x)       __uint32_identity (x)
# define ntohs(x)       __uint16_identity (x)
# else
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#   define ntohl(x)     __bswap_32 (x)
#   define ntohs(x)     __bswap_16 (x)
#  endif
# endif
#endif
#else
uint32_t ntohl(uint32_t netlong)
{
    return 0;
}

uint16_t ntohs(uint16_t netshort)
{
    return 0;
}
#endif /* WINAMS_STUB */

/* Use at R_OSAL_IoDeviceOpen */
/* Set a fixed value corresponding to dev_pattern_01-04 of r_osal_os_io_initialize_device() */
static char *g_dev_pattern[8] = {"/dev/imp_00", "/dev/imp_01",
                                 "/dev/ipmmu_cache_ds1", "/dev/ipmmu_cache_ir",
                                 "/dev/fcpra_imp", "/dev/fcpra_imr",
                                 "/dev/fba_imr0", "/dev/fba_imr1"};
int g_pattern = -2;
extern bool in_io_device_open;

/* Regular expression */
int regcomp(regex_t *restrict preg, const char *restrict regex, int cflags)
{
    preg->re_nsub = 4;
    (void) regex;
    (void) cflags;

START_TIMER
    /* Use at R_OSAL_IoDeviceOpen */
    if(in_io_device_open == true)
    {
        /* Set the index of g_dev_pattern */
        g_pattern += 2;
    }
STOP_TIMER
    return 0;
}

int regexec(const regex_t *restrict preg, const char *restrict string, size_t nmatch, regmatch_t pmatch[restrict], int eflags)
{
    (void) preg;
    (void) string;
    (void) nmatch;
    (void) pmatch;
    (void) eflags;

    static axi_parser_cnt = 0;
START_TIMER
    /* Use at R_OSAL_IoGetAxiIdListForAxiBusName, R_OSAL_IoGetNumOfAxiIdsForAxiBusName */
    if(strcmp(string, "vi0_02") == 0)
    {
        if (0 == axi_parser_cnt)
        {
            pmatch[0].rm_so = 0;
            pmatch[0].rm_eo = 6;
            axi_parser_cnt++;
        }
        else if (1 == axi_parser_cnt)
        {
            pmatch[0].rm_so = 3;
            pmatch[0].rm_eo = 6;
            axi_parser_cnt = 0;
        }
    }

    /* Use at R_OSAL_IoDeviceOpen */
    if(in_io_device_open == true)
    {
        if((strcmp(string, g_dev_pattern[g_pattern]) == 0) || (strcmp(string, g_dev_pattern[g_pattern+1]) == 0))
        {
STOP_TIMER
            return 0;
        }
        else {
STOP_TIMER
            return -1;
        }
    }

STOP_TIMER
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
    /* replace with AMSTB_fscanf */

    (void) stream;
    (void) format;

#ifndef WINAMS_STUB
    va_list args;
    va_start(args, format);
    if (0 == strcmp("%lx", format))
    {
        unsigned long *result;
        result = va_arg(args, unsigned long *);
        *result = 32;
    }
    else if (0 == strcmp("%s", format))
    {
        char *result;
        result = va_arg(args, char *);
        result[0] = 'f';
        result[1] = 'b';
        result[2] = 'a';
        result[3] = '_';
        result[4] = 'i';
        result[5] = 'm';
        result[6] = 'r';
        result[7] = '1';
        result[8] = '\0';
    }
    va_end(args);
#endif /* WINAMS_STUB */
    return 1;   // only call 1 element
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

DIR *opendir(const char *name)
{
    (void) name;

    return 1;
}

#define DIR_NUM 1
struct dirent g_s_entry = {1, 1, 1, 8, "uio1"};
int g_iDirNum = 0;
struct dirent *readdir(DIR *dirp)
{
    (void) dirp;

START_TIMER
    if ( g_iDirNum < DIR_NUM ) {
        g_iDirNum++;
STOP_TIMER
        return &g_s_entry;
    } else {
        g_iDirNum = 0;
STOP_TIMER
        return NULL;
    }
}

int closedir(DIR *dirp)
{
    (void) dirp;

    return 0;
}
