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

#define stdout ((FILE*)0x4096)

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

//#define stdout ((FILE*)0x4096)

#endif  /* #else USE_STUB */

#endif  /* _STUB_H */
