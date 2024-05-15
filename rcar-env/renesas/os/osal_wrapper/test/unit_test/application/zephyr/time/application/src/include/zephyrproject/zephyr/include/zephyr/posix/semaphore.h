/*
 * Copyright (c) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ZEPHYR_INCLUDE_POSIX_SEMAPHORE_H_
#define ZEPHYR_INCLUDE_POSIX_SEMAPHORE_H_

#if defined(USE_STUB)
#define sem_destroy     sem_destroy_stub
#define sem_getvalue    sem_getvalue_stub
#define sem_init        sem_init_stub
#define sem_post        sem_post_stub
#define sem_timedwait   sem_timedwait_stub
#define sem_trywait     sem_trywait_stub
#endif

#include <zephyr/posix/time.h>
#include "posix_types.h"

#ifdef __cplusplus
extern "C" {
#endif

int sem_destroy(sem_t *semaphore);
int sem_getvalue(sem_t *ZRESTRICT semaphore, int *ZRESTRICT value);
int sem_init(sem_t *semaphore, int pshared, unsigned int value);
int sem_post(sem_t *semaphore);
int sem_timedwait(sem_t *ZRESTRICT semaphore, struct timespec *ZRESTRICT abstime);
int sem_trywait(sem_t *semaphore);
int sem_wait(sem_t *semaphore);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_POSIX_SEMAPHORE_H_ */
