#ifndef _STUB_H
#define _STUB_H

#include <hw/inout.h>
#include <sys/mman.h>
#include <sys/neutrino.h>
#include <sys/siginfo.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include <hw/rcar_cpg.h>
#include <hw/rcar_sysc.h>
#include <signal.h>
#include <sys/slog.h>
#include <sys/syspage.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef USE_STUB
void init_stub_value(void);
uint64_t get_stub_time(void);
uint64_t get_pthread_create_count(void);
uint64_t get_pthread_mutex_init_count(void);
uint64_t get_pthread_cond_init_count(void);
uint64_t get_malloc_count(void);
uint64_t get_realloc_count(void);
uint64_t get_calloc_count(void);
uint64_t get_timer_create_count(void);
extern int g_mutex_trylock_err;
void print_call_api(void);

#define stdout ((FILE*)0x4096)

#define STUB_CMEM_SIZE    (0x4000)
typedef union _stub_cmem_area_t
{
    void* dummy;
    char  array[STUB_CMEM_SIZE];
} stub_cmem_area_t;
stub_cmem_area_t * get_cmem_area(void);

#else /* #ifdef USE_STUB */
void init_stub_value(void){;}
uint64_t get_stub_time(void){return 0;}
uint64_t get_pthread_create_count(void){return 0;}
uint64_t get_pthread_mutex_init_count(void){return 0;}
uint64_t get_pthread_cond_init_count(void){return 0;}
uint64_t get_malloc_count(void){return 0;}
uint64_t get_realloc_count(void){return 0;}
uint64_t get_calloc_count(void){return 0;}
uint64_t get_timer_create_count(void){return 0;}
void print_call_api(void){;}

void * get_cmem_area(void){return NULL;}

#endif  /* #else USE_STUB */

#endif  /* _STUB_H */
