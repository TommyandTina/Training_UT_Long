/*
    Copyright(C) 2020 eSOL Co., Ltd. All rights reserved.

    This software is protected by the law and the agreement concerning
    a Japanese country copyright method, an international agreement,
    and other intellectual property right and may be used and copied
    only in accordance with the terms of such license and with the inclusion
    of the above copyright notice.

    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by eSOL Co.,Ltd.
*/
/****************************************************************************
[libpthread_local.h] - POSIX Thread library API local header
****************************************************************************/

#ifndef LIBPTHREAD_LOCAL_H
#define LIBPTHREAD_LOCAL_H

#include <semaphore.h>

/* Definitions for pthread.c */
#if MCOS_USE_PROCESS_MODEL
extern void pthread_destroy_allspecific(pttcb_t *pttcb);
#endif /* #if MCOS_USE_PROCESS_MODEL */
extern int32_t nsem_init(sem_t *sem, int32_t pshared, int32_t named, uint32_t value);
extern int32_t nsem_destroy(sem_t* sem);
#if MCOS_USE_PROCESS_MODEL
extern void pt_thread_abort_entry(void *value_ptr);
#endif

/* Definitions for pthread_setschedprio.c */
extern int32_t pthread_setschedprio_mutex(pthread_t tid, int32_t priority, int32_t formutex);

#endif /* LIBPTHREAD_LOCAL_H */
