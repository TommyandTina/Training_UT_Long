/*
    Copyright (C) 2013 By eSOL Co.,Ltd. Tokyo, Japan

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
[pthread_rwlock_attr.c] - MCOS Pthread rwlock attributes API
****************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <mcos/mcos.h>
#include <mcos/atomic.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/mfutex.h>
//#include <pt_api_impl.h>
#include <signal.h>

typedef struct pttcb
{
#if MCOS_TK_API
    tktcb_t     tktcb;
#define pthrent  tktcb.task
#define pthrid   tktcb.tskid
#define pthrpri  tktcb.tskpri
#define pbasepri tktcb.basepri
#define ptname   tktcb.dsname
#else
    void        (*pthrent)();       /* Thread entry */
    mcos_id_t   pthrid;             /* Thread MCOS id */
    uint8_t     pthrpri;            /* Thread MCOS priority */
    uint8_t     pbasepri;           /* Base priority */
    char        ptname[PTHREAD_NAME_MAX]; /* Thread name */
#endif
 /* Make sure flags is lower 16 bit of 32-bit word */
#if PT_LITTLE_ENDIAN
    uint16_t    flags;              /* Various flags */
    mcos_id_t   joiner;             /* Joiner thread id */
#else
    mcos_id_t   joiner;             /* Joiner thread id */
    uint16_t    flags;              /* Various flags */
#endif
    sigset_t    sigenab;            /* Enabled signals */
    sigset_t    sigpend;            /* Pending signals */
    void*       cleanup;            /* Cleanup handlers list */
    pthread_mutex_t *ownmutex;      /* Owned mutexes list */
    uintptr_t   inhmutex;           /* Waiting inherit mutex */
#if MCOS_USE_PROCESS_MODEL
    void*       sigentry;           /* Signal handler enrty */
    void*       saventry;           /* Saved return address */
    void*       abtentry;           /* Default abort entry */
    pid_t       pid;                /* Process id */
#endif /* #if MCOS_USE_PROCESS_MODEL */
    uint32_t    blocking_cause;     /* blocking cause */
    void*       keys;               /* Pthread keys */
} pttcb_t;

#define RWLOCK_INVALID ((uintptr_t)(~0ULL))
#define RWLOCK_READOWNED ((mcos_id_t)(-1))

#define CHECK_ALIGN(_x_, _a_)  ((uintptr_t)(_x_) & ((uintptr_t)(_a_) - 1))

struct rwlockers {
    uint8_t readers;
    uint8_t writers;
    mcos_id_t owner;
    uint32_t readings;
};

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
    int ret = 0;
    if ((attr == NULL)
            || (CHECK_ALIGN(attr, sizeof(pthread_rwlockattr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        attr[0] = PTHREAD_PROCESS_PRIVATE;
    }
    return ret;
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    /* There is no memory allocation on init, so destroy could be nop.
     * To increase robustness, just reset the attributes to the default on
     * destroy.
     */
    return pthread_rwlockattr_init(attr);
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
        int *restrict pshared)
{
    int ret = 0;
    if ((attr == NULL)
            || (CHECK_ALIGN(attr, sizeof(pthread_rwlockattr_t)) != 0)
            || (pshared == NULL)
            || (CHECK_ALIGN(pshared, sizeof(int)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        pshared[0] = attr[0];
    }
    return ret;
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared)
{
    int ret = 0;
    if ((attr == NULL)
            || (CHECK_ALIGN(attr, sizeof(pthread_rwlockattr_t)) != 0)
            || ((pshared != PTHREAD_PROCESS_SHARED)
                    && (pshared != PTHREAD_PROCESS_PRIVATE)))
    {
        ret = EINVAL;
    }
    else
    {
        attr[0] = pshared;
    }
    return ret;
}

int pthread_rwlock_init(pthread_rwlock_t* restrict rwlock,
                        const pthread_rwlockattr_t* restrict attr)
{
    int ret = 0;
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0)
            || ((attr != NULL) && (CHECK_ALIGN(attr, sizeof(pthread_rwlockattr_t)) != 0)))
    {
        ret = EINVAL;
    }
    else
    {
        /* we don't really care about PSHARED attribute... */
        memset(rwlock, 0, sizeof(pthread_rwlock_t));
    }
    return ret;
}

int pthread_rwlock_destroy(pthread_rwlock_t* rwlock)
{
    int ret = 0;
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        uintptr_t oldval = atomic_load(&rwlock->lock.val);
        struct rwlockers rwl = *(struct rwlockers*)&oldval;
        /* EBUSY if there is a reader or a writer */
        if ((rwl.readers + rwl.writers) > 0)
        {
            ret = EBUSY;
        }
        else
        {
            rwlock->lock.val = RWLOCK_INVALID;
        }
    }
    return ret;
}

static int pthread_rwlock_unregister(pthread_rwlock_t* rwlock, uintptr_t *val, bool isreader, bool isowner)
{
    int ret = 0;
    struct rwlockers rwl;
    uintptr_t oldval = val[0];
    do {
        if (oldval == RWLOCK_INVALID)
        {
            ret = EINVAL;
            break;
        }
        rwl = *(struct rwlockers*)&oldval;
        if (isreader)
        {
            rwl.readers--;
            if (isowner)
            {
                rwl.readings--;
            }
            if (isowner && (rwl.readings == 0))
            {
                rwl.owner = 0;
            }
        }
        else
        {
            rwl.writers--;
            if (isowner)
            {
                rwl.owner = 0;
            }
        }
    } while (atomic_compare_exchange_strong(&rwlock->lock.val, &oldval,
                    *(uintptr_t*)&rwl) == false);
    val[0] = *(uintptr_t*)&rwl;
    return ret;
}

int pthread_rwlock_unlock(pthread_rwlock_t* rwlock)
{
    int ret = 0;
    bool isreadowner = false;
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        uintptr_t oldval = atomic_load(&rwlock->lock.val);
        struct rwlockers rwl = *(struct rwlockers*)&oldval;
        /* if there is a writer, maybe we are the one locking */
        mcos_id_t tid = pthread_self();
        if (rwl.owner == 0)
        {
            ret = EPERM;
        }
        else if (rwl.owner == RWLOCK_READOWNED)
        {
            isreadowner = true;
        }
        else if (rwl.owner != tid)
        {
            ret = EPERM;
        }

        if (ret == 0)
        {
            ret = pthread_rwlock_unregister(rwlock, &oldval, isreadowner, true);
            /* If there is any remaining waiter, wake one of them */
            /* oldval is updated following above unregister call */
            rwl = *(struct rwlockers*)&oldval;
            if ((rwl.readers + rwl.writers > 0) && (rwl.owner == 0))
            {
                /* FIXME: failure means a corrupted lockobj.
                 * What should we do then?
                 */
                mcos_mfutex_wake(&rwlock->lock,
                                MCOS_MFUTEX_ORDERING_NONE | MCOS_MFUTEX_WAKE_ONE);
            }
        }
    }
    return ret;
}

static int pthread_rwlock_tryrdlock_internal(pthread_rwlock_t* rwlock,
                                             uintptr_t *val, bool incwaiters, bool dowait)
{
    int ret;
    bool loop;
    uintptr_t oldval;
    struct rwlockers rwl;
    oldval = atomic_load(&rwlock->lock.val);
    do {
        ret = 0;
        loop = false;
        rwl = *(struct rwlockers*)&oldval;
        if (oldval == RWLOCK_INVALID)
        {
            ret = EINVAL;
        }
        else if (rwl.writers > 0)
        {
            if (rwl.owner == pthread_self())
            {
                ret = EDEADLK;
            }
            else
            {
                ret = EBUSY;
            }
        }
        /* if not owned, make it read-locked */
        else if (rwl.owner != RWLOCK_READOWNED)
        {
            rwl.owner = RWLOCK_READOWNED;
        }
        else if (rwl.readers == UINT8_MAX)
        {
            ret = EAGAIN;
        }
        if ((ret == 0) || ((ret == EBUSY) && (incwaiters) && (dowait)))
        {
            if (incwaiters != false)
            {
                rwl.readers++;
            }
            if (ret == 0)
            {
                rwl.readings++;
            }

            if (atomic_compare_exchange_strong(&rwlock->lock.val, &oldval,
                            *(uintptr_t*)&rwl) == false)
            {
                /* This may occur if a reader or another writer was faster */
                loop = true;
            }
        }
    } while (loop != false);
    val[0] = *(uintptr_t*)&rwl;
    return ret;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock)
{
    int ret = 0;
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        uintptr_t val;
        ret = pthread_rwlock_tryrdlock_internal(rwlock, &val, true, false);
    }

    return ret;
}

static void pthread_rwlock_rdcleanup(void *arg)
{
    pthread_rwlock_t *rwlock = (pthread_rwlock_t *)arg;
    uintptr_t oldval = atomic_load(&rwlock->lock.val);
    pthread_rwlock_unregister(rwlock, &oldval, true, false);
}

static int pthread_rwlock_timedrdlock_internal(pthread_rwlock_t* restrict rwlock,
                                               const struct timespec* restrict abstmo,
                                               uint_fast8_t timed)
{
    int ret = 0;
    bool loop = true;
    bool dowait = true;
    mcos_er_t err;
    uintptr_t val;
    int oldcancel;
    mcos_systime_t systime = {0};

    if (timed != false)
    {
        if ((abstmo == NULL)
            || (CHECK_ALIGN(abstmo, sizeof(abstmo->tv_nsec)) != 0)
            || (abstmo->tv_nsec < 0)
            || (abstmo->tv_nsec >= 1000000000))
        {
            dowait = false;
        }
        else
        {
            pt_abstime_convert(abstmo, &systime);
        }
    }
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldcancel);
        ret = pthread_rwlock_tryrdlock_internal(rwlock, &val, true, dowait);
        pthread_cleanup_push(pthread_rwlock_rdcleanup, rwlock);
        pthread_setcanceltype(oldcancel, NULL);
        if ((ret == EBUSY) && (dowait == false))
        {
            ret = EINVAL; /* Invalid abstmo on busy */
        }
        if (ret == EBUSY)
        {
            while (loop != false)
            {
                ret = pthread_rwlock_tryrdlock_internal(rwlock, &val, false, false);
                loop = false;
                if (ret == EBUSY)
                {
                    err = mcos_mfutex_wait(&rwlock->lock, val, 0, &systime);
                    /* time out, return error */
                    if (err == MCOS_MFUTEX_RET_TIMEOUT)
                    {
                        ret = ETIMEDOUT;
                        break;
                    }
                    else if (err == MCOS_EMACV)
                    {
                        ret = EFAULT;
                    }
                    /* EINTR, handle interrupt and try again */
                    else if (err == MCOS_MFUTEX_RET_SIGNAL)
                    {
                        pttcb_t *pttcb = pt_get_pttcb();
                        pt_handle_interrupt(pttcb);
                        loop = true;
                    }
                    /* val changed or wait succeeded, try again */
                    else
                    {
                        loop = true;
                    }
                }
            }
            if (ret != 0)
            {
                (void)pthread_rwlock_unregister(rwlock, &val, true, false);
            }
        }
        pthread_cleanup_pop(0);
    }
    return ret;
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t* restrict rwlock,
                               const struct timespec* restrict abstmo)
{
    return pthread_rwlock_timedrdlock_internal(rwlock, abstmo, true);
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock)
{
    return pthread_rwlock_timedrdlock_internal(rwlock, NULL, false);
}

static int pthread_rwlock_trywrlock_internal(pthread_rwlock_t* rwlock,
                                             uintptr_t *val, bool incwaiters, bool dowait)
{
    int ret;
    bool loop;
    uintptr_t oldval;
    struct rwlockers rwl;
    pthread_t tid = pthread_self();
    oldval = atomic_load(&rwlock->lock.val);
    do {
        ret = 0;
        loop = false;
        rwl = *(struct rwlockers*)&oldval;
        if (oldval == RWLOCK_INVALID)
        {
            ret = EINVAL;
        }
        else if (rwl.owner == tid)
        {
            ret = EDEADLK;
        }
        else if (rwl.owner == RWLOCK_READOWNED)
        {
            ret = EBUSY;
        }
        else if (rwl.owner != 0)
        {
            ret = EBUSY;
        }
        else if (rwl.writers == UINT8_MAX)
        {
            ret = EAGAIN;
        }
        if ((ret == 0) || ((ret == EBUSY) && (incwaiters) && (dowait)))
        {
            if (incwaiters != false)
            {
                rwl.writers++;
            }
            if (rwl.owner == 0)
            {
                rwl.owner = tid;
            }

            if (atomic_compare_exchange_strong(&rwlock->lock.val, &oldval,
                            *(uintptr_t*)&rwl) == false)
            {
                /* This may occur if a reader or another writer was faster */
                loop = true;
            }
        }
    } while (loop != false);
    val[0] = *(uintptr_t*)&rwl;
    return ret;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock)
{
    int ret = 0;
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        uintptr_t val;
        ret = pthread_rwlock_trywrlock_internal(rwlock, &val, true, false);
    }
    return ret;
}

static void pthread_rwlock_wrcleanup(void *arg)
{
    pthread_rwlock_t *rwlock = (pthread_rwlock_t *)arg;
    uintptr_t oldval = atomic_load(&rwlock->lock.val);
    pthread_rwlock_unregister(rwlock, &oldval, false, false);
}

static int pthread_rwlock_timedwrlock_internal(pthread_rwlock_t* restrict rwlock,
                                               const struct timespec* restrict abstmo, bool timed)
{
    int ret = 0;
    bool loop = true;
    bool dowait = true;
    mcos_er_t err;
    uintptr_t val;
    int oldcancel;
    mcos_systime_t systime = {0};

    if (timed != false)
    {
        if ((abstmo == NULL)
            || (CHECK_ALIGN(abstmo, sizeof(abstmo->tv_nsec)) != 0)
            || (abstmo->tv_nsec < 0)
            || (abstmo->tv_nsec >= 1000000000))
        {
            dowait = false;
        }
        else
        {
            pt_abstime_convert(abstmo, &systime);
        }
    }
    if ((rwlock == NULL)
            || (CHECK_ALIGN(rwlock, sizeof(uintptr_t)) != 0))
    {
        ret = EINVAL;
    }
    else
    {
        pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldcancel);
        ret = pthread_rwlock_trywrlock_internal(rwlock, &val, true, dowait);
        pthread_cleanup_push(pthread_rwlock_wrcleanup, rwlock);
        pthread_setcanceltype(oldcancel, NULL);
        if ((ret == EBUSY) && (dowait == false))
        {
            ret = EINVAL; /* Invalid abstmo on busy */
        }
        if (ret == EBUSY)
        {
            while (loop != false)
            {
                ret = pthread_rwlock_trywrlock_internal(rwlock, &val, false, false);
                loop = false;
                if (ret == EBUSY)
                {
                    err = mcos_mfutex_wait(&rwlock->lock, val, 0, &systime);
                    /* time out, return error */
                    if (err == MCOS_MFUTEX_RET_TIMEOUT)
                    {
                        ret = ETIMEDOUT;
                        break;
                    }
                    else if (err == MCOS_EMACV)
                    {
                        ret = EFAULT;
                    }
                    /* EINTR, handle interrupt and try again */
                    else if (err == MCOS_MFUTEX_RET_SIGNAL)
                    {
                        pttcb_t *pttcb = pt_get_pttcb();
                        pt_handle_interrupt(pttcb);
                        loop = true;
                    }
                    /* val changed or wait succeeded, try again */
                    else
                    {
                        loop = true;
                    }
                }
            }
            if (ret != 0)
            {
                (void)pthread_rwlock_unregister(rwlock, &val, false, false);

                /* When the writer which waiting for reader is timeout,
                   we need wake up the readers which waiting for this writer. */
                if (err == MCOS_MFUTEX_RET_TIMEOUT)
                {
                    uintptr_t oldval = atomic_load(&rwlock->lock.val);
                    struct rwlockers rwl = *(struct rwlockers*)&oldval;
                    if (rwl.owner == RWLOCK_READOWNED)
                    {
                        mcos_mfutex_wake(&rwlock->lock,
                                MCOS_MFUTEX_ORDERING_NONE | MCOS_MFUTEX_WAKE_ALL);
                    }
                }
            }
        }
        pthread_cleanup_pop(0);
    }
    return ret;
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t* restrict rwlock,
                               const struct timespec* restrict abstime)
{
    return pthread_rwlock_timedwrlock_internal(rwlock, abstime, true);
}

int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock)
{
    return pthread_rwlock_timedwrlock_internal(rwlock, NULL, false);
}
