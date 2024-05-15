#ifndef _STUB_H
#define _STUB_H

#include <stdlib.h>                         // ./zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/stdlib.h
#include <stdio.h>                          // ./zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/stdio.h
#include <string.h>                         // ./zephyr-sdk-0.14.2/arm-zephyr-eabi/arm-zephyr-eabi/sys-include/string.h
#include <zephyr/irq.h>                     // ./zephyrproject/zephyr/include/zephyr/irq.h
#include <zephyr/spinlock.h>                // ./zephyrproject/zephyr/include/zephyr/spinlock.h
#include <generated/syscalls/kernel.h>      // ./zephyrproject/zephyr/include/generated/syscalls/kernel.h
#include <zephyr/kernel.h>                  // ./zephyrproject/zephyr/include/zephyr/kernel.h
#include <zephyr/cache.h>                   // ./zephyrproject/zephyr/include/zephyr/cache.h
#include <zephyr/device.h>                  // ./zephyrproject/zephyr/include/zephyr/device.h
#include <zephyr/devicetree.h>              // ./zephyrproject/zephyr/include/zephyr/devicetree.h
#include <zephyr/drivers/clock_control.h>   // ./zephyrproject/zephyr/include/zephyr/drivers/clock_control.h
#include <zephyr/posix/time.h>              // ./zephyrproject/zephyr/include/zephyr/posix/time.h
#include <zephyr/posix/pthread.h>           // ./zephyrproject/zephyr/include/zephyr/posix/pthread.h
#include <zephyr/posix/semaphore.h>         // ./zephyrproject/zephyr/include/zephyr/posix/semaphore.h

#ifdef USE_STUB
void init_stub_value(void);
uint32_t get_stub_cycle(void);
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
uint32_t get_stub_cycle(void){return 0;}
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
