#ifndef _STUB_H
#define _STUB_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/lfs.h>
#include <mcos/pdev.h>
#include <mcos/uring.h>
#include <bios/bios_cache.h>

#ifdef USE_STUB
void init_stub_value(void);
extern uint64_t g_pthread_create_count;
extern uint64_t g_pthread_mutex_init_count;
extern uint64_t g_pthread_cond_init_count;
extern uint64_t g_malloc_count;
extern uint64_t g_realloc_count;
extern uint64_t g_calloc_count;
extern uint64_t g_timer_create_count;

#define stdout ((FILE*)0x4096)

extern bool g_stub_start;

extern struct timespec g_abstime_before;
extern struct timespec g_abstime_after;
extern uint32_t g_timer_start;
extern uint64_t g_stub_time;
#define SEC2NSEC        (1000000000ULL)
#define START_TIMER     if(0U == g_timer_start) {clock_gettime(CLOCK_REALTIME, &g_abstime_before);} g_timer_start++;
#define STOP_TIMER      g_timer_start--; if(0U == g_timer_start) {clock_gettime(CLOCK_REALTIME, &g_abstime_after); g_stub_time+=((g_abstime_after.tv_sec * SEC2NSEC) + g_abstime_after.tv_nsec) - ((g_abstime_before.tv_sec * SEC2NSEC) + g_abstime_before.tv_nsec);}


#else /* #ifdef USE_STUB */

void init_stub_value(void){;}
uint64_t get_stub_time(void){return 0;}

#endif  /* #else USE_STUB */

#endif  /* _STUB_H */
