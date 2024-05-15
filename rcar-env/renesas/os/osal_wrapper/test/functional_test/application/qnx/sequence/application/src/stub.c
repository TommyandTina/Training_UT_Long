
#include "stub.h"
#include "target/qnx/r_osal_os_type.h"
#include "target/common/dev_cfg/static/r_osal_dev_cfg.h"
#include "target/common/res_cfg/static/r_osal_res_cfg.h"

#include "target/common/pma/r_pma.h"
#include "target/common/pma/statemachine/r_pma_statemachine.h"

/*
typedef struct st_osal_stub_mq_control
{
    bool                      send_wait;
    bool                      receive_wait;
    st_osal_queue_t           *queue;
    int32_t                   head;
    int32_t                   tail;
    pthread_mutex_t           inner_mtx;
    osal_milli_sec_t          inner_timeout;
    pthread_cond_t            send_cond;
    pthread_cond_t            receive_cond;
    uint32_t                  numofmsg;
    st_osal_inner_mq_config_t config;
}st_osal_stub_mq_control_t;
*/

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

syspage_entry_info_dummy     qtime_dummy = {8333333};
void *qtime = (void *)&qtime_dummy;

/* print information */
#define STUB_CALL_INFO_SIZE (150000)
#define STUB_CALL_API_LEN   (64)
typedef struct st_stub_api_info
{
    char        api_name[STUB_CALL_API_LEN];
    uint32_t    arg_count;
    uint64_t    arg1;
    uint64_t    arg2;
    uint64_t    arg3;
    uint64_t    arg4;
    uint64_t    arg5;
    uint64_t    arg6;
}st_stub_api_info_t;

st_stub_api_info_t call_info[STUB_CALL_INFO_SIZE] = {{0}};

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
uint64_t g_malloc_size;
uint64_t g_calloc_size;
uint64_t g_pthread_create_count;
uint64_t g_pthread_mutex_init_count;
uint64_t g_pthread_cond_init_count;
uint64_t g_malloc_count;
uint64_t g_realloc_count;
uint64_t g_calloc_count;
uint64_t g_timer_create_count;
uint64_t g_store_count;
bool     g_store_flag = false;

void init_stub_value(void)
{
    g_stub_time                 = 0;
    g_malloc_size               = 0;
    g_calloc_size               = 0;
    g_pthread_create_count      = 0;
    g_pthread_mutex_init_count  = 0;
    g_pthread_cond_init_count   = 0;
    g_malloc_count              = 0;
    g_realloc_count             = 0;
    g_calloc_count              = 0;
    g_timer_create_count        = 0;
    g_store_count               = 0;
    g_store_flag                = true;

    return;
}

uint64_t get_stub_time(void)
{
    return g_stub_time;
}

uint64_t get_malloc_size(void)
{
    return g_malloc_size;
}

uint64_t get_calloc_size(void)
{
    return g_calloc_size;
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

void print_call_api(void)
{
    uint32_t i;
    g_store_flag = false;
    for(i = 0; i < g_store_count; i++)
    {
        printf("  [%04d]:\t", i);
        if (6 == call_info[i].arg_count)
        {
            printf("%s()\n", call_info[i].api_name);
        }
        else if (5 == call_info[i].arg_count)
        {
            printf("%s()\n", call_info[i].api_name);
        }
        else if (4 == call_info[i].arg_count)
        {
            printf("%s()\n", call_info[i].api_name);
        }
        else if (3 == call_info[i].arg_count)
        {
            printf("%s()\n", call_info[i].api_name);
        }
        else if (2 == call_info[i].arg_count)
        {
            printf("%s()\n", call_info[i].api_name);
        }
        else if (1 == call_info[i].arg_count)
        {
            printf("%s()\n", call_info[i].api_name);
        }
        else
        {
            printf("%s()\n", call_info[i].api_name);
        }
    }

    return;
}

/*
void print_call_api(void)
{
    uint32_t i;
    g_store_flag = false;
    for(i = 0; i < g_store_count; i++)
    {
        printf("  [%04d]:\t", i);
        if (6 == call_info[i].arg_count)
        {
            printf("%s(0x%016lx, 0x%016lx, 0x%016lx, 0x%016lx, 0x%016lx, 0x%016lx)\n",
                call_info[i].api_name, call_info[i].arg1, call_info[i].arg2, call_info[i].arg3, call_info[i].arg4, call_info[i].arg5, call_info[i].arg6);
        }
        else if (5 == call_info[i].arg_count)
        {
            printf("%s(0x%016lx, 0x%016lx, 0x%016lx, 0x%016lx, 0x%016lx)\n",
                call_info[i].api_name, call_info[i].arg1, call_info[i].arg2, call_info[i].arg3, call_info[i].arg4, call_info[i].arg5);
        }
        else if (4 == call_info[i].arg_count)
        {
            printf("%s(0x%016lx, 0x%016lx, 0x%016lx, 0x%016lx)\n",
                call_info[i].api_name, call_info[i].arg1, call_info[i].arg2, call_info[i].arg3, call_info[i].arg4);
        }
        else if (3 == call_info[i].arg_count)
        {
            printf("%s(0x%016lx, 0x%016lx, 0x%016lx)\n",
                call_info[i].api_name, call_info[i].arg1, call_info[i].arg2, call_info[i].arg3);
        }
        else if (2 == call_info[i].arg_count)
        {
            printf("%s(0x%016lx, 0x%016lx)\n",
                call_info[i].api_name, call_info[i].arg1, call_info[i].arg2);
        }
        else if (1 == call_info[i].arg_count)
        {
            printf("%s(0x%016lx)\n",
                call_info[i].api_name, call_info[i].arg1);
        }
        else
        {
            printf("%s()\n", call_info[i].api_name);
        }
    }

    return;
}
*/

#if 1
#define START_TIMER do{}while(0);
#define STOP_TIMER  do{}while(0);
#else
#define SEC2NSEC    (1000000000ULL)
#define START_TIMER g_clocktime_before = ClockCycles();
#define STOP_TIMER  g_clocktime_after  = ClockCycles(); g_stub_time += (g_clocktime_after - g_clocktime_before);

uint64_t g_clocktime_before;
uint64_t g_clocktime_after;
#endif

void *alloc_proc(unsigned long size)
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
    ptr = (void*)&stub_heap[stub_malloc_pos];
    stub_malloc_tbl[stub_malloc_count].size = size;
    stub_malloc_tbl[stub_malloc_count].ptr = ptr;
    stub_malloc_pos += size_a;
    stub_malloc_count++;

    return ptr;
}

char *stub_strncpy(char *dest, const char *src, size_t n)
{
    char *ptr = dest;

    if ((dest == NULL) || (src == NULL))
    {
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

    return ptr;
}

void *malloc(unsigned long size)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "malloc\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)size;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif
    size_t size_a;

    size_a = ((size + STUB_MALLOC_ALIGN - 1) / STUB_MALLOC_ALIGN) * STUB_MALLOC_ALIGN;
    g_malloc_count++;
    g_malloc_size += size_a;

    return alloc_proc(size);
}

void *calloc(unsigned long num, unsigned long nsize)
{
    size_t size;
    void *p;
START_TIMER

#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "calloc\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)num;
        call_info[g_store_count].arg2 = (uint64_t)nsize;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    size = num * nsize;
    p = alloc_proc(size);
    g_calloc_count++;
    g_calloc_size += size;
STOP_TIMER
    return p;
}

void free(void *ptr)
{
START_TIMER
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "free\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)ptr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "realloc\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)ptr;
        call_info[g_store_count].arg2 = (uint64_t)size;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif
    size_t i;
    void *ptr_new;

    g_realloc_count++;
    if (ptr == NULL)
    {
        ptr_new = alloc_proc(size);    // malloc new area
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
            ptr_new = alloc_proc(size); // malloc new area
STOP_TIMER
            if (ptr_new != NULL)
            {
                uint8_t *dest = ptr_new;
                uint8_t *src  = ptr;
                size_t i;

                if ((dest != NULL) && (src != NULL))
                {
                    for (i=0; i<size; i++)
                    {
                        dest[i] = src[i];
                    }
                }
                if (ptr == NULL)
                {
                    /* do nothing */
                }
                else if (stub_malloc_count == 0)
                {
                    /* do nothing */
                }
                else
                {
                    stub_malloc_count--;
                    if (stub_malloc_count == 0)
                        stub_malloc_pos = 0;   // reset heap
                }
            }
            return ptr_new;
        }

        if (stub_malloc_tbl[i].size >= size)
        {
STOP_TIMER
            return ptr;
        }
        /* realloc */
        ptr_new = alloc_proc(size);
STOP_TIMER
        if (ptr_new != NULL)
        {
            uint8_t *dest = ptr_new;
            uint8_t *src  = ptr;
            size_t i;

            if ((dest != NULL) && (src != NULL))
            {
                for (i=0; i<size; i++)
                {
                    dest[i] = src[i];
                }
            }
            if (ptr == NULL)
            {
                /* do nothing */
            }
            else if (stub_malloc_count == 0)
            {
                /* do nothing */
            }
            else
            {
                stub_malloc_count--;
                if (stub_malloc_count == 0)
                    stub_malloc_pos = 0;   // reset heap
            }
        }
    }
    return ptr_new;
}

void *memcpy(void *dest, const void *src, unsigned long n)
{
START_TIMER
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "memcpy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)dest;
        call_info[g_store_count].arg2 = (uint64_t)src;
        call_info[g_store_count].arg3 = (uint64_t)n;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
#if defined(RCAR_V4H) || defined(RCAR_S4)|| defined(RCAR_V4M)
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "memset\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s;
        call_info[g_store_count].arg2 = (uint64_t)c;
        call_info[g_store_count].arg3 = (uint64_t)n;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#else   /* V3Hx, V3M2 */
    static bool memset_flag = false;
    if (false == memset_flag)
    {
        if (true == g_store_flag)
        {
            stub_strncpy(call_info[g_store_count].api_name, "memset\0", STUB_CALL_API_LEN);
            call_info[g_store_count].arg1 = (uint64_t)s;
            call_info[g_store_count].arg2 = (uint64_t)c;
            call_info[g_store_count].arg3 = (uint64_t)n;
            call_info[g_store_count].arg_count = 3;
            g_store_count++;
        }
        memset_flag = true;
    }
    if (1 == n)
    {
        memset_flag = false;
    }

#endif
#endif
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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strlen\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strnlen\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s;
        call_info[g_store_count].arg2 = (uint64_t)n;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strncpy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)dest;
        call_info[g_store_count].arg2 = (uint64_t)src;
        call_info[g_store_count].arg3 = (uint64_t)n;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strcmp\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s1;
        call_info[g_store_count].arg2 = (uint64_t)s2;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strncmp\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s1;
        call_info[g_store_count].arg2 = (uint64_t)s2;
        call_info[g_store_count].arg3 = (uint64_t)n;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strncat\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s1;
        call_info[g_store_count].arg2 = (uint64_t)s2;
        call_info[g_store_count].arg3 = (uint64_t)n;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    char *ptr = s1;

    if ((s1 == NULL) || (s2 == NULL))
    {
STOP_TIMER
        return ptr;
    }
    while (*ptr)
    {
        ptr++;
    }
    while (*ptr++ = *s2++)
    {
        /* Do nothing */
    }
STOP_TIMER
    return ptr;
}

char *strstr(const char *s1, const char *s2)
{
START_TIMER
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "strstr\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)s1;
        call_info[g_store_count].arg2 = (uint64_t)s2;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif
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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "bsearch\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__key;
        call_info[g_store_count].arg2 = (uint64_t)__base;
        call_info[g_store_count].arg2 = (uint64_t)__nmemb;
        call_info[g_store_count].arg2 = (uint64_t)__size;
        call_info[g_store_count].arg2 = (uint64_t)__compar;
        call_info[g_store_count].arg_count = 5;
        g_store_count++;
    }
#endif
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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mutex;
        call_info[g_store_count].arg2 = (uint64_t)attr;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) mutex;
    (void) attr;

    g_pthread_mutex_init_count++;

    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_destroy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mutex;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mutex;

    return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timespec *restrict abs_timeout)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_timedlock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mutex;
        call_info[g_store_count].arg2 = (uint64_t)abs_timeout;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) mutex;
    (void) abs_timeout;

    return 0;
}

int pthread_mutex_timedlock_monotonic(pthread_mutex_t *__mutex, const struct timespec *__abs_timeout)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_timedlock_monotonic\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__mutex;
        call_info[g_store_count].arg2 = (uint64_t)__abs_timeout;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __mutex;
    (void) __abs_timeout;

    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_lock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mutex;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mutex;

    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_unlock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mutex;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mutex;

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutex_trylock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mutex;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mutex;

    return g_mutex_trylock_err;
}

int pthread_mutexattr_init(pthread_mutexattr_t *attr)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutexattr_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) attr;

    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutexattr_destroy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) attr;

    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_mutexattr_settype\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg2 = (uint64_t)type;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) attr;
    (void) type;

    return 0;
}

/* pthread_key */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_key_create\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)key;
        call_info[g_store_count].arg2 = (uint64_t)destructor;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) key;
    (void) destructor;

    return 0;
}

int pthread_key_delete(pthread_key_t key)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_key_delete\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)key;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) key;

    if (stub_p_setspecific)
    {
        stub_p_setspecific = NULL;
    }
    return 0;
}

void *pthread_getspecific(pthread_key_t key)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_getspecific\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)key;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) key;

    return stub_p_setspecific;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_setspecific\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)key;
        call_info[g_store_count].arg2 = (uint64_t)value;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) key;
    (void) value;
    stub_p_setspecific = value;  /* free in pthread_key_delete */
    return 0;
}

/* pthread_cond */
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cond_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cond;
        call_info[g_store_count].arg2 = (uint64_t)attr;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) cond;
    (void) attr;

    g_pthread_cond_init_count++;

    return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cond_destroy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cond;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) cond;

    return 0;
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cond_broadcast\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cond;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) cond;

    return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cond_signal\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cond;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) cond;

    return 0;
}

int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex)
{
START_TIMER
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cond_wait\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cond;
        call_info[g_store_count].arg2 = (uint64_t)mutex;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cond_timedwait\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cond;
        call_info[g_store_count].arg2 = (uint64_t)mutex;
        call_info[g_store_count].arg3 = (uint64_t)abstime;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_condattr_destroy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__attr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) __attr;

    return 0;
}

int pthread_condattr_init(pthread_condattr_t *__attr)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_condattr_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__attr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) __attr;

    return 0;
}

int pthread_condattr_setclock(pthread_condattr_t *__attr, int __id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_condattr_setclock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__attr;
        call_info[g_store_count].arg2 = (uint64_t)__id;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __attr;
    (void) __id;

    return 0;
}

/* pthread_attr */
int pthread_attr_init(pthread_attr_t *attr)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_attr_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) attr;

    return 0;
}

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inheritsched)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_attr_setinheritsched\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg2 = (uint64_t)inheritsched;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) attr;
    (void) inheritsched;

    return 0;
}

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_attr_setschedpolicy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg2 = (uint64_t)policy;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) attr;
    (void) policy;

    return 0;
}

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_attr_setschedparam\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg2 = (uint64_t)param;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) attr;
    (void) param;

    return 0;
}

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_attr_setstacksize\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg2 = (uint64_t)stacksize;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) attr;
    (void) stacksize;

    return 0;
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_attr_destroy\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)attr;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) attr;

    return 0;
}

/* pthread */
#define PMA_THREAD_COUNT (1U)

int pthread_create(pthread_t *restrict thread, const pthread_attr_t *restrict attr, void *(*start_routine)(void *), void *restrict arg)
{
START_TIMER
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_create\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)thread;
        call_info[g_store_count].arg2 = (uint64_t)attr;
        call_info[g_store_count].arg3 = (uint64_t)start_routine;
        call_info[g_store_count].arg4 = (uint64_t)arg;
        call_info[g_store_count].arg_count = 4;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_cancel\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)thread;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) thread;

    return 0;
}

void pthread_exit(void *retval)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_exit\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)retval;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) retval;
}

pthread_t pthread_self(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_self\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return &stub_thread_main;   /* dummy of main thread */
}

int pthread_equal(pthread_t __t1, pthread_t __t2)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_equal\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__t1;
        call_info[g_store_count].arg2 = (uint64_t)__t2;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __t1;
    (void) __t2;

    return 0;
}

int pthread_setcancelstate(int state, int *oldstate)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_setcancelstate\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)state;
        call_info[g_store_count].arg2 = (uint64_t)oldstate;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void)state;
    (void)oldstate;
    return 0;
}

int pthread_detach(pthread_t thread)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_detach\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)thread;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void)thread;
    return 0;
}

int pthread_join(pthread_t thread, void **retval)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_join\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)thread;
        call_info[g_store_count].arg2 = (uint64_t)retval;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) thread;

    if (retval != NULL)
    {
        *retval = NULL; 
    }

    return 0;
}

int pthread_timedjoin(pthread_t __thr, void **__value_ptr, const struct timespec *__abstime)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_timedjoin\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__thr;
        call_info[g_store_count].arg2 = (uint64_t)__value_ptr;
        call_info[g_store_count].arg3 = (uint64_t)__abstime;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    (void) __thr;
    (void) __abstime;

    if (__value_ptr != NULL)
    {
        *__value_ptr = NULL;
    }

    return 0;
}

int pthread_timedjoin_monotonic(pthread_t __thr, void **__value_ptr, const struct timespec *__abstime)
{
START_TIMER
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_timedjoin_monotonic\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__thr;
        call_info[g_store_count].arg2 = (uint64_t)__value_ptr;
        call_info[g_store_count].arg3 = (uint64_t)__abstime;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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

int pthread_setschedprio(pthread_t thread, int prio)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_setschedprio\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)thread;
        call_info[g_store_count].arg2 = (uint64_t)prio;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) thread;
    (void) prio;

    return 0;
}

int pthread_setname_np(pthread_t __thr, const char *__name)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "pthread_setname_np\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__thr;
        call_info[g_store_count].arg2 = (uint64_t)__name;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __thr;
    (void) __name;
    return 0;
}

int __sigsetjmp (struct __jmp_buf_tag *__env, int __savemask)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "__sigsetjmp\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__env;
        call_info[g_store_count].arg2 = (uint64_t)__savemask;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void)__env;
    (void)__savemask;
    return 0;
}

int error_stub(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "error_stub\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return 0;
}

void __cpu_membarrier(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "__cpu_membarrier\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return;
}

long int syscall (long int __sysno, ...)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "syscall\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__sysno;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void)__sysno;
    return 0;
}

/* Interrupt */
int InterruptAttachEvent(int __intr, const struct sigevent *__event, unsigned __flags)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "InterruptAttachEvent\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__intr;
        call_info[g_store_count].arg2 = (uint64_t)__event;
        call_info[g_store_count].arg3 = (uint64_t)__flags;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    (void) __intr;
    (void) __event;
    (void) __flags;

    return 0;
}

int InterruptWait(int __flags, const uint64_t *__timeout)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "InterruptWait\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__flags;
        call_info[g_store_count].arg2 = (uint64_t)__timeout;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __flags;
    (void) __timeout;

    return 0;
}

int InterruptWait_r(int __flags, const uint64_t *__timeout)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "InterruptWait_r\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__flags;
        call_info[g_store_count].arg2 = (uint64_t)__timeout;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __flags;
    (void) __timeout;

    return 0;
}

int  InterruptMask(int __intr, int __id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "InterruptMask\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__intr;
        call_info[g_store_count].arg2 = (uint64_t)__id;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __intr;
    (void) __id;

    return 0;
}

int  InterruptUnmask(int __intr, int __id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "InterruptUnmask\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__intr;
        call_info[g_store_count].arg2 = (uint64_t)__id;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __intr;
    (void) __id;

    return 0;
}

/* Timer */
int TimerTimeout(clockid_t __id, int __flags, const struct sigevent *__notify, const uint64_t *__ntime, uint64_t *__otime)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "TimerTimeout\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__id;
        call_info[g_store_count].arg2 = (uint64_t)__flags;
        call_info[g_store_count].arg3 = (uint64_t)__notify;
        call_info[g_store_count].arg4 = (uint64_t)__ntime;
        call_info[g_store_count].arg5 = (uint64_t)__otime;
        call_info[g_store_count].arg_count = 5;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "timer_create\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__clock_id;
        call_info[g_store_count].arg2 = (uint64_t)__evp;
        call_info[g_store_count].arg3 = (uint64_t)__timerid;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    (void) __clock_id;
    (void) __evp;
    (void) __timerid;

    g_timer_create_count++;

    return 0;
}

clock_t clock(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "clock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return 0;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "clock_gettime\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)clk_id;
        call_info[g_store_count].arg2 = (uint64_t)tp;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "clock_getres\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)clk_id;
        call_info[g_store_count].arg2 = (uint64_t)res;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "clock_settime\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)clk_id;
        call_info[g_store_count].arg2 = (uint64_t)tp;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) clk_id;
    (void) tp;

    return 0;
}

int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "clock_nanosleep\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)clockid;
        call_info[g_store_count].arg2 = (uint64_t)flags;
        call_info[g_store_count].arg3 = (uint64_t)request;
        call_info[g_store_count].arg4 = (uint64_t)remain;
        call_info[g_store_count].arg_count = 4;
        g_store_count++;
    }
#endif

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

#if 1
uint64_t ClockCycles(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "ClockCycles\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    static uint64_t clock_cycles = 1000U;

    return clock_cycles++;
}
#else
__inline__ uint64_t __attribute__((__unused__))
ClockCycles(void)
{
    uint64_t    cntv;

    /*
     * We need an isb barrier to ensure our read occurs in program order
     */
    __asm__ __volatile__(
        "isb    \n"
        "mrs    %0, cntvct_el0"
        : "=r"(cntv)
    );
    return cntv;
}
#endif

int ThreadCtl(int cmd, void * data)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "ThreadCtl\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)cmd;
        call_info[g_store_count].arg2 = (uint64_t)data;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) cmd;
    (void) data;

    return 0;
}

/* mmap */
#define STUB_MMAP_SIZE    (64*1024*1024) /* 64MB */
char  stub_mmap_area[STUB_MMAP_SIZE] = {0};

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "mmap\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)addr;
        call_info[g_store_count].arg2 = (uint64_t)length;
        call_info[g_store_count].arg3 = (uint64_t)prot;
        call_info[g_store_count].arg4 = (uint64_t)flags;
        call_info[g_store_count].arg5 = (uint64_t)fd;
        call_info[g_store_count].arg6 = (uint64_t)offset;
        call_info[g_store_count].arg_count = 6;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "mmap64\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)addr;
        call_info[g_store_count].arg2 = (uint64_t)length;
        call_info[g_store_count].arg3 = (uint64_t)prot;
        call_info[g_store_count].arg4 = (uint64_t)flags;
        call_info[g_store_count].arg5 = (uint64_t)fd;
        call_info[g_store_count].arg6 = (uint64_t)offset;
        call_info[g_store_count].arg_count = 6;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "munmap\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)addr;
        call_info[g_store_count].arg2 = (uint64_t)length;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) addr;
    (void) length;

    return 0;
}

void *mmap_device_memory(void *__addr, size_t __len, int __prot, int __flags, uint64_t __physical)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "mmap_device_memory\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__addr;
        call_info[g_store_count].arg2 = (uint64_t)__len;
        call_info[g_store_count].arg3 = (uint64_t)__prot;
        call_info[g_store_count].arg4 = (uint64_t)__flags;
        call_info[g_store_count].arg5 = (uint64_t)__physical;
        call_info[g_store_count].arg_count = 5;
        g_store_count++;
    }
#endif

    (void) __addr;
    (void) __len;
    (void) __prot;
    (void) __flags;
    (void) __physical;

    return &stub_mmap_area[0];
}

int munmap_device_memory(void *__addr, size_t __len)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "munmap_device_memory\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__addr;
        call_info[g_store_count].arg2 = (uint64_t)__len;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __addr;
    (void) __len;

    return 0;
}

int mem_offset64(__const void *__addr, int __fd, size_t __len, off64_t *__off, size_t *__contig_len)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "mem_offset64\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__addr;
        call_info[g_store_count].arg2 = (uint64_t)__fd;
        call_info[g_store_count].arg3 = (uint64_t)__len;
        call_info[g_store_count].arg4 = (uint64_t)__off;
        call_info[g_store_count].arg5 = (uint64_t)__contig_len;
        call_info[g_store_count].arg_count = 5;
        g_store_count++;
    }
#endif

    (void) __addr;
    (void) __fd;
    (void) __len;
    (void) __contig_len;

    if (NULL != __off)
    {
        *__off = 4096U;
    }

    return 0;
}

int mlock(__const void * __addr, size_t __len)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "mlock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__addr;
        call_info[g_store_count].arg2 = (uint64_t)__len;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __addr;
    (void) __len;

    return 0;
}

int munlock(__const void *__addr, size_t __len)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "munlock\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__addr;
        call_info[g_store_count].arg2 = (uint64_t)__len;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) __addr;
    (void) __len;

    return 0;
}

int msync(void *__addr, size_t __len, int __flags)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "msync\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__addr;
        call_info[g_store_count].arg2 = (uint64_t)__len;
        call_info[g_store_count].arg3 = (uint64_t)__flags;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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

stub_cmem_area_t stub_cmem_area;

stub_cmem_area_t * get_cmem_area()
{
    return &stub_cmem_area;
}


/* open */
int open(const char *filename, int amode, ...)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "open\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)filename;
        call_info[g_store_count].arg2 = (uint64_t)amode;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    /* replace with AMSTB_open */
    (void) filename;
    (void) amode;

    return stub_fd_count++;
}

int close(int fd)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "close\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)fd;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) fd;

    return 0;
}

int posix_typed_mem_open(const char *__name, int __oflag, int __tflag)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "posix_typed_mem_open\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)__name;
        call_info[g_store_count].arg2 = (uint64_t)__oflag;
        call_info[g_store_count].arg3 = (uint64_t)__tflag;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    (void) __name;
    (void) __oflag;
    (void) __tflag;

    return stub_fd_count++;
}

int fprintf( FILE *stream , const char *format , ... )
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "fprintf\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)stream;
        call_info[g_store_count].arg2 = (uint64_t)format;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    /* replace with AMSTB_fprintf */
    (void) stream;
    (void) format;

    return 0;
}

int snprintf(char *str, size_t size, const char *format, ...)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "snprintf\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)str;
        call_info[g_store_count].arg2 = (uint64_t)size;
        call_info[g_store_count].arg3 = (uint64_t)format;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    va_list ap;
    va_start(ap, format);

    str = va_arg(ap, char *);

    va_end(ap);
    return 1;
}

/* CPG */
int cpg_init(int vbose)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)vbose;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) vbose;

    return 0;
}

int cpg_deinit(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_deinit\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return 0;
}

int cpg_mstp_is_enabled(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_is_enabled\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    /* 0 - Module is not enabled */
    return 0;
}

int cpg_mstp_enable(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_enable\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    return 0;
}

int cpg_mstp_disable(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_disable\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    return 0;
}

int cpg_mstp_reset(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_reset\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    return 0;
}

int cpg_mstp_assert(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_assert\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    return 0;
}

int cpg_mstp_deassert(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_deassert\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    return 0;
}

int cpg_mstp_reset_state(int mod_id)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mstp_reset_state\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)mod_id;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) mod_id;

    return 0;
}

int cpg_clk_is_enabled(cpg_clk_id_t id, int idx)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_is_enabled\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) id;
    (void) idx;

    /* 0 - Module is not enabled */
    return 0;
}

int cpg_clk_enable(cpg_clk_id_t id, int idx)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_enable\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) id;
    (void) idx;

    return 0;
}

int cpg_clk_disable(cpg_clk_id_t id, int idx)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_disable\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) id;
    (void) idx;

    return 0;
}

int cpg_clk_rate_get(cpg_clk_id_t id, int idx, uint32_t *rate)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_rate_get\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg3 = (uint64_t)rate;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_rate_set\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg3 = (uint64_t)rate;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    (void) id;
    (void) idx;
    (void) rate;

    return 0;
}

int cpg_clk_divider_get(cpg_clk_id_t id, int idx, uint32_t *div)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_divider_get\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg3 = (uint64_t)div;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_clk_divider_set\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)id;
        call_info[g_store_count].arg2 = (uint64_t)idx;
        call_info[g_store_count].arg3 = (uint64_t)div;
        call_info[g_store_count].arg_count = 3;
        g_store_count++;
    }
#endif

    (void) id;
    (void) idx;
    (void) div;

    return 0;
}

int cpg_mgr_getfuncs(cpg_mgr_funcs_t *funcs, int tabsize)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "cpg_mgr_getfuncs\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)funcs;
        call_info[g_store_count].arg2 = (uint64_t)tabsize;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

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
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_init\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return 0;
}

void sysc_deinit(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_deinit\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return 0;
}

int sysc_power_is_off(int pdr_number)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_power_is_off\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)pdr_number;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) pdr_number;

    /* 0 - Power is ON */
    return 0;
}

int sysc_power_off(int pdr_number)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_power_off\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)pdr_number;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) pdr_number;

    return 0;
}

int sysc_power_on(int pdr_number)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_power_on\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)pdr_number;
        call_info[g_store_count].arg_count = 1;
        g_store_count++;
    }
#endif

    (void) pdr_number;

    return 0;
}

int sysc_read_pdmode(void)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_read_pdmode\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg_count = 0;
        g_store_count++;
    }
#endif

    return 0;
}

int sysc_mgr_getfuncs(sysc_mgr_funcs_t *funcs, int tabsize)
{
#ifdef SEQUENCE_CHECK
    if (true == g_store_flag)
    {
        stub_strncpy(call_info[g_store_count].api_name, "sysc_mgr_getfuncs\0", STUB_CALL_API_LEN);
        call_info[g_store_count].arg1 = (uint64_t)funcs;
        call_info[g_store_count].arg2 = (uint64_t)tabsize;
        call_info[g_store_count].arg_count = 2;
        g_store_count++;
    }
#endif

    (void) tabsize;

    funcs->init         = sysc_init;
    funcs->deinit       = sysc_deinit;
    funcs->power_is_off = sysc_power_is_off;
    funcs->power_off    = sysc_power_off;
    funcs->power_on     = sysc_power_on;
    funcs->read_pdmode  = sysc_read_pdmode;

    return 0;
}
