
#include "stub.h"
#include "target/emcos/r_osal_common.h"
#include "target/common/pma/r_pma.h"

void *memcpy(void *dest, const void *src, size_t n);

uint64_t g_stub_time                = 0U;
uint64_t g_pthread_create_count     = 0U;
uint64_t g_pthread_mutex_init_count = 0U;
uint64_t g_pthread_cond_init_count  = 0U;
uint64_t g_malloc_count             = 0U;
uint64_t g_realloc_count            = 0U;
uint64_t g_calloc_count             = 0U;
uint64_t g_timer_create_count       = 0U;
uint32_t g_timer_start              = 0U;
bool     g_stub_start               = false;

struct timespec g_abstime_before;
struct timespec g_abstime_after;

void init_stub_value(void)
{
    g_stub_time                 = 0U;
    g_pthread_create_count      = 0U;
    g_pthread_mutex_init_count  = 0U;
    g_pthread_cond_init_count   = 0U;
    g_malloc_count              = 0U;
    g_realloc_count             = 0U;
    g_calloc_count              = 0U;
    g_timer_create_count        = 0U;

    return;
}

uint64_t get_stub_time(void)
{
    return g_stub_time;
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
            p_dest[i] = p_src[i];
        }
    }
STOP_TIMER
    return dest;
}

void *memset(void *s, int c, size_t n)
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

size_t strnlen(const char *s, size_t n)
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

int strncmp(const char *s1, const char *s2, size_t n)
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

char* strncat(char* restrict s1, const char* restrict s2, size_t n)
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

e_pma_return_t R_PMA_IF_RegInit(uint64_t osal_soc_typev)
{
    (void) osal_soc_typev;

    return 0;
}

e_pma_return_t R_PMA_IF_RegDeinit(void)
{
    return 0;
}
