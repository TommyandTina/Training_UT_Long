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
[pthread_barrier_attr.c] - MCOS Pthread barrier attributes API
****************************************************************************/
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include <sys/mfutex.h>
#include <mcos/atomic.h>
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

/* Thread flags */
#define PTHREAD_PTHREAD     0x0001U /* POSIX thread (else TK) */
#define PTHREAD_CANCELOK    0x0002U /* Cancelable */
#define PTHREAD_CANENAB     0x0004U /* Cancelation enabled */
#define PTHREAD_CANASYNC    0x0008U /* Async cancelation */
#define PTHREAD_WAITING     0x0010U /* Thread is waiting */
#define PTHREAD_WAITEND     0x0020U /* Wait already interrupted */
#define PTHREAD_CANPEND     0x0040U /* Cancellation pending */
#define PTHREAD_WAKEUP      0x0080U /* Wakeup pending */
#define PTHREAD_DETACHED    0x0100U /* Thread is detached */
#define PTHREAD_RRSCHED     0x0200U /* Round-robin (unused) */
#define PTHREAD_TERMINATED  0x0400U /* Terminated (awaits join) */
#define PTHREAD_SPECIFIC    0x0800U /* TS data was accessed */
#define PTHREAD_JOIN        0x0800U /* Join request missed */
#define PTHREAD_USERSTACK   0x1000U /* User stack (unused) */
#if MCOS_USE_PROCESS_MODEL
#define PTHREAD_PROCESS     0x2000U /* Process thread */
#endif /* #if MCOS_USE_PROCESS_MODEL */

/* barrier bits:
 * 0-15: current waiter count
 * 16-31: max waiter count
 */
#define BARRIER_COUNT_SHIFT 16
#define BARRIER_MASK 0xffffU
#define BARRIER_MAX  BARRIER_MASK

/* zero count is invalid */
#define BARRIER_INVALID 0ULL

 /* Barrier attributes */
#define BARRIER_ATTR_SHARED  0x0001

#define CHECK_ALIGN(_x_, _a_)  ((uintptr_t)(_x_) & ((uintptr_t)(_a_) - 1))

static int pthread_barrierattr_valid(const pthread_barrierattr_t* attr)
{
    return (1
#ifdef MCOS_CFG_PT_CHECK_ARGUMENTS
            && (attr != 0)
            && (CHECK_ALIGN(attr, sizeof(*attr)) == 0)
#endif
    );
}

/* pthread_barrierattr_init() API */
int pthread_barrierattr_init(pthread_barrierattr_t* attr)
{
    if (pthread_barrierattr_valid((const pthread_barrierattr_t*)attr))
    {
        *attr = 0;
        return 0;
    }
    return EINVAL;
}

/* pthread_barrierattr_destroy() API */
int pthread_barrierattr_destroy(pthread_barrierattr_t* attr)
{
    if (pthread_barrierattr_valid((const pthread_barrierattr_t*)attr))
    {
        *attr = -1U;
        return 0;
    }
    return EINVAL;
}

/* pthread_barrierattr_getpshared() API */
int pthread_barrierattr_getpshared(const pthread_barrierattr_t* restrict attr, int* restrict pshared)
{
    if (pthread_barrierattr_valid(attr)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
         && (pshared != 0)
         && (CHECK_ALIGN(pshared, sizeof(*pshared)) == 0)
#endif
    )
    {
        *pshared  = ((*attr & BARRIER_ATTR_SHARED) != 0
                     ? PTHREAD_PROCESS_SHARED
                     : PTHREAD_PROCESS_PRIVATE);
        return 0;
    }
    return EINVAL;
}

/* pthread_barrierattr_setpshared() API */
int pthread_barrierattr_setpshared(pthread_barrierattr_t* attr, int pshared)
{
    if (((pshared == PTHREAD_PROCESS_SHARED)
         || (pshared == PTHREAD_PROCESS_PRIVATE))
        && pthread_barrierattr_valid((const pthread_barrierattr_t*)attr) )
    {
        *attr = ((*attr & ~BARRIER_ATTR_SHARED)
                 | (pshared == PTHREAD_PROCESS_SHARED
                    ? BARRIER_ATTR_SHARED
                    : 0));
        return 0;
    }
    return EINVAL;
}

int pthread_barrier_init(pthread_barrier_t* restrict barrier,
                         const pthread_barrierattr_t* restrict attr,
                         unsigned int count)
{
    int ret = 0;
    if ((barrier == NULL)
            || (count == 0)
            || (count > BARRIER_MAX)
            || CHECK_ALIGN(barrier, sizeof(void*))
            || CHECK_ALIGN(attr, sizeof(uint32_t)))
    {
        ret = EINVAL;
    }
    else
    {
        memset(barrier, 0, sizeof(pthread_barrier_t));
        /* TODO: get pid and store it in word if private */
        barrier->lock.val = count << BARRIER_COUNT_SHIFT;
    }
    return ret;
}

int pthread_barrier_destroy(pthread_barrier_t* barrier)
{
    int ret = 0;
    uintptr_t val;
    uint16_t count;
    if ((barrier == NULL)
            || CHECK_ALIGN(barrier, sizeof(void*)))
    {
        ret = EINVAL;
    }
    else
    {
        val = atomic_load(&barrier->lock.val);
        if (val != BARRIER_INVALID)
        {
            /* do we care for atomic exchange? */
            atomic_exchange(&barrier->lock.val, BARRIER_INVALID);
            /* wake up any waiter on this barrier */
            (void)mcos_mfutex_wake(&barrier->lock,
                    MCOS_MFUTEX_ORDERING_NONE | MCOS_MFUTEX_WAKE_ALL);
        }
    }
    return ret;
}

int pthread_barrier_wait(pthread_barrier_t* barrier)
{
    int ret = 0;
    bool loop;
    int16_t mfret;
    uint16_t waiters;
    uint16_t newwaiters;
    uint16_t count;
    uintptr_t val;
    uintptr_t newval;
    uint16_t pflags;
    pttcb_t *pttcb;

    if ((barrier == NULL)
            || CHECK_ALIGN(barrier, sizeof(void*)))
    {
        ret = EINVAL;
    }
    else
    {
        val = atomic_load(&barrier->lock.val);
        do {
            loop = false;
            count = (val >> BARRIER_COUNT_SHIFT) & BARRIER_MASK;
            if (count == 0)
            {
                ret = EINVAL;
                break;
            }
            waiters = val & BARRIER_MASK;

            newwaiters = waiters + 1;
            newval = (val & ~BARRIER_MASK) | newwaiters;
            if (atomic_compare_exchange_strong(&barrier->lock.val,
                                    &val, newval) != false)
            {
                if (newwaiters == count)
                {
                    /* We're last in the barrier.
                     */

                    /* Initialize val. */
                    atomic_exchange(&barrier->lock.val,  count << BARRIER_COUNT_SHIFT);
                    /* Wake up everyone. */
                    (void)mcos_mfutex_wake(&barrier->lock,
                            MCOS_MFUTEX_ORDERING_NONE | MCOS_MFUTEX_WAKE_ALL);
                    ret = PTHREAD_BARRIER_SERIAL_THREAD;
                }
                else
                {
                    pttcb = pt_get_pttcb();
                    do
                    {
                        /* start no-cancellation point */
                        pflags = atomic_fetch_and(&pttcb->flags, ~PTHREAD_CANCELOK) & PTHREAD_CANCELOK;

                        /* wait until someone sets the barrier */
                        mfret = mcos_mfutex_wait(&barrier->lock, newval,
                                MCOS_MFUTEX_ORDERING_NONE, NULL);

                        /* end nocancel range */
                        atomic_fetch_or(&pttcb->flags, pflags);

                        if (mfret == MCOS_MFUTEX_RET_NOTVALUE)
                        {
                            /* continue inner loop if waiters are less than counter */
                            newval = atomic_load(&barrier->lock.val);
                            newwaiters = newval & BARRIER_MASK;
                        }
                        else
                        {
                            /* exit inner loop */
                            newwaiters = count;
                        }
                    }
                    while ((newwaiters != count) && (newwaiters != 0));
                }
            }
            else
            {
                /* atomic modification failed, retry. */
                loop = true;
            }
        } while (loop != false);
    }
    return ret;
}
