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
[pthread_mutex.c] - MCOS Pthread mutex attributes API
****************************************************************************/
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <mcos/mcos.h>
#include <mcos/atomic.h>
#include <sys/mfutex.h>
//#include <pt_api_impl.h>
#include <signal.h>
#include <pt_api/pt.cfg>
#include <posix/process.cfg>

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

#include "libpthread_local.h"
#include "stub.h"

/* Invalidation and validation check */
#define MUTEX_INVALID               ((uintptr_t)(~0ULL))

/* copied from pthread_mutex_if.h */
 /* Mutex object states */
#define MUTEX_STATE_UNLOCKED    0U
#define MUTEX_STATE_LOCKED      1U
#define MUTEX_STATE_CONTENDED   2U
/* This is used as a temporary state where the mutex is technically locked/unlocked,
 * but not yet inserted to/removed from the owned mutex list.
 * In case of a thread cancel, it is recoverable. But no other thread can acquire it.
 */
#define MUTEX_STATE_LOCKING     3U
#define MUTEX_STATE_MASK        3U
#define MUTEX_STATE_INVALID     MUTEX_STATE_MASK

#define MUTEX_OWNER_NOTRECOVERABLE  ((mcos_id_t)-1)

/* In pthread_mutexattr_t, bit 7 is pshared.
 * In pthread_mutex_t, bit 7 is ownerdead, and owner pid is stored in higher bits.
 */
#define MUTEX_OWNERDEAD_MASK        MUTEX_ATTR1_MASK
#define MUTEX_OWNERDEAD_SHIFT       7U

#define MUTEX_IS_ERRCHECK   ((uint32_t)PTHREAD_MUTEX_ERRORCHECK << MUTEX_ATTR_TYPE_SHIFT)
#define MUTEX_IS_RECURSIVE  ((uint32_t)PTHREAD_MUTEX_RECURSIVE << MUTEX_ATTR_TYPE_SHIFT)
#define MUTEX_IS_OWNERDEAD  ((uint32_t)MUTEX_OWNERDEAD_MASK << MUTEX_OWNERDEAD_SHIFT)
#define MUTEX_IS_ROBUST     ((uint32_t)PTHREAD_MUTEX_ROBUST << MUTEX_ATTR_ROBUST_SHIFT)
#define MUTEX_IS_PROTECT    ((uint32_t)PTHREAD_PRIO_PROTECT << MUTEX_ATTR_PROTOCOL_SHIFT)
#define MUTEX_IS_INHERIT    ((uint32_t)PTHREAD_PRIO_INHERIT << MUTEX_ATTR_PROTOCOL_SHIFT)

#define MUTEX_ATTR1_MASK             0x0001ULL
#define MUTEX_ATTR2_MASK             0x0003ULL

/* Mutex attributes */
#define MUTEX_ATTR_TYPE_MASK        MUTEX_ATTR2_MASK
#define MUTEX_ATTR_TYPE_SHIFT       2U
#define MUTEX_ATTR_PROTOCOL_MASK    MUTEX_ATTR2_MASK
#define MUTEX_ATTR_PROTOCOL_SHIFT   4U
#define MUTEX_ATTR_ROBUST_MASK      MUTEX_ATTR1_MASK
#define MUTEX_ATTR_ROBUST_SHIFT     6U
#define MUTEX_ATTR_SHARED_MASK      MUTEX_ATTR1_MASK
#define MUTEX_ATTR_SHARED_SHIFT     7U
#define MUTEX_ATTR_PRIO_MASK        UINT8_MAX
#define MUTEX_ATTR_PRIO_SHIFT       8U

/* Invalidation and validation check */
#define MUTEX_ATTR_INVALID          0xffffULL
#define MUTEX_ATTR_DEFAULT          ((uint32_t)((MCOS_CFG_PT_MAX_PRI - MCOS_CFG_PT_MIN_PRI + 1) / 2) << MUTEX_ATTR_PRIO_SHIFT)

#define CHECK_ALIGN(_x_, _a_)  ((uintptr_t)(_x_) & ((uintptr_t)(_a_) - 1))

static int32_t pthread_mutex_timedlock_internal(pthread_mutex_t* restrict mutex,
                            const struct timespec* restrict abstmo,
                            bool timed);
static int32_t pthread_mutex_trylock_internal(pthread_mutex_t* mutex, uintptr_t *val);
static void mutex_link(pthread_mutex_t *mutex, pttcb_t *pttcb);
static int32_t pthread_mutexattr_valid(const pthread_mutexattr_t* attr);

static inline pthread_mutexattr_t MUTEX_PRIOCEIL(pthread_mutexattr_t x)
{
    return (pthread_mutexattr_t)((pthread_mutexattr_t)((x) >> MUTEX_ATTR_PRIO_SHIFT));
}

static int32_t pthread_mutexattr_valid(const pthread_mutexattr_t* attr)
{
    int32_t ret;
#ifdef MCOS_CFG_PT_CHECK_ARGUMENTS
    uint32_t align = CHECK_ALIGN(attr, (uint32_t)sizeof(*attr));
    ret = (((attr != NULL_PNTR) && (align == 0U)) ? 1 : 0);
#else
    ret = 1;
#endif
    return ret;
}

/* pthread_mutexattr_init() API */
int32_t pthread_mutexattr_init(pthread_mutexattr_t* attr)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if (valid != 0)
    {
        *attr = (pthread_mutexattr_t)MUTEX_ATTR_DEFAULT;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

/* pthread_mutexattr_destroy() API */
int32_t pthread_mutexattr_destroy(pthread_mutexattr_t* attr)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if (valid != 0)
    {
        *attr = (uint16_t)MUTEX_ATTR_INVALID;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

/* pthread_mutexattr_getpshared() API */
int pthread_mutexattr_getpshared(const pthread_mutexattr_t* restrict attr, int* restrict pshared)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN(pshared, (uint32_t)sizeof(*pshared));
        if ((pshared != NULL_PNTR) && (align == 0U))
        {
            ret = 0;
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *pshared  = (((*attr & ((uint32_t)MUTEX_ATTR_SHARED_MASK
                                << MUTEX_ATTR_SHARED_SHIFT)) != 0U)
                     ? (int32_t)PTHREAD_PROCESS_SHARED
                     : (int32_t)PTHREAD_PROCESS_PRIVATE);
    }
    return ret;
}

/* pthread_mutexattr_setpshared() API */
int pthread_mutexattr_setpshared(pthread_mutexattr_t* attr, int pshared)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if (((pshared == (int32_t)PTHREAD_PROCESS_SHARED)
         || (pshared == (int32_t)PTHREAD_PROCESS_PRIVATE))
        && (valid != 0) )
    {
            *attr = ((*attr & (pthread_mutexattr_t)~((pthread_mutexattr_t)MUTEX_ATTR_SHARED_MASK
                            << MUTEX_ATTR_SHARED_SHIFT))
                 | (pshared == PTHREAD_PROCESS_SHARED
                    ? (pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_SHARED_MASK << MUTEX_ATTR_SHARED_SHIFT)
                    : (pthread_mutexattr_t)0));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_mutexattr_getrobust() API */
int32_t pthread_mutexattr_getrobust(const pthread_mutexattr_t* restrict attr, int32_t* restrict robust)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN(robust, (uint32_t)sizeof(*robust));
        if((robust != NULL_PNTR) && (align == 0U))
        {
            ret = 0;
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *robust  = (((*attr & ((uint32_t)MUTEX_ATTR_ROBUST_MASK
                               << MUTEX_ATTR_ROBUST_SHIFT)) != 0U)
                    ? (int32_t)PTHREAD_MUTEX_ROBUST
                    : (int32_t)PTHREAD_MUTEX_STALLED);
    }
    return ret;
}

/* pthread_mutexattr_setrobust() API */
int32_t pthread_mutexattr_setrobust(pthread_mutexattr_t* attr, int32_t robust)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if (   (   (robust == (int32_t)PTHREAD_MUTEX_STALLED)
            || (robust == (int32_t)PTHREAD_MUTEX_ROBUST) )
        && (valid != 0) )
    {
        *attr = ((*attr & ((pthread_mutexattr_t)~(pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_ROBUST_MASK
                            << MUTEX_ATTR_ROBUST_SHIFT)))
                 | (robust == (int32_t)PTHREAD_MUTEX_ROBUST
                    ? (pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_ROBUST_MASK << MUTEX_ATTR_ROBUST_SHIFT)
                    : (pthread_mutexattr_t)0U));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_mutexattr_getprotocol() API */
int32_t pthread_mutexattr_getprotocol(const pthread_mutexattr_t* restrict attr, int32_t* restrict protocol)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN(protocol, (uint32_t)sizeof(*protocol));
        if((protocol != NULL_PNTR) && (align == 0U))
        {
            ret = 0;
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        uint32_t tmp_protocol = (((uint32_t)*attr >> MUTEX_ATTR_PROTOCOL_SHIFT) & (uint32_t)MUTEX_ATTR_PROTOCOL_MASK);
        *protocol = (int32_t)tmp_protocol;
    }
    return ret;
}

/* pthread_mutexattr_setprotocol() API */
int32_t pthread_mutexattr_setprotocol(pthread_mutexattr_t* attr, int32_t protocol)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if (((protocol == (int32_t)PTHREAD_PRIO_NONE)
         || (protocol == (int32_t)PTHREAD_PRIO_INHERIT)
         || (protocol == (int32_t)PTHREAD_PRIO_PROTECT))
        && (valid != 0) )
    {
        *attr = ((*attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_PROTOCOL_MASK
                            << MUTEX_ATTR_PROTOCOL_SHIFT))
                 | (pthread_mutexattr_t)((pthread_mutexattr_t)protocol << MUTEX_ATTR_PROTOCOL_SHIFT));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_mutexattr_gettype() API */
int32_t pthread_mutexattr_gettype(const pthread_mutexattr_t* restrict attr, int32_t* restrict type)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN(type, (uint32_t)sizeof(*type));
        if((type != NULL_PNTR) && (align == 0U))
        {
            ret = 0;
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        uint32_t type_temp;
        type_temp = (uint32_t)*attr;
        type_temp = (type_temp >> MUTEX_ATTR_TYPE_SHIFT) & (uint32_t)MUTEX_ATTR_TYPE_MASK;
        *type = (int32_t)type_temp;
    }
    return ret;
}

/* pthread_mutexattr_settype() API */
int32_t pthread_mutexattr_settype(pthread_mutexattr_t* attr, int32_t type)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if (((type == (int32_t)PTHREAD_MUTEX_NORMAL)
         || (type == (int32_t)PTHREAD_MUTEX_ERRORCHECK)
         || (type == (int32_t)PTHREAD_MUTEX_RECURSIVE))
        && (valid != 0) )
    {
        *attr = ((*attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_TYPE_MASK
                            << MUTEX_ATTR_TYPE_SHIFT))
                 | (pthread_mutexattr_t)((pthread_mutexattr_t)type << MUTEX_ATTR_TYPE_SHIFT));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

/* pthread_mutexattr_getprioceiling() API */
int32_t pthread_mutexattr_getprioceiling(const pthread_mutexattr_t* restrict attr, int32_t* restrict prioceiling)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN(prioceiling, (uint32_t)sizeof(*prioceiling));
        if((prioceiling != NULL_PNTR) && (align == 0U))
        {
            ret = 0;
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        pthread_mutexattr_t temp_ceil;
        temp_ceil = (*attr >> MUTEX_ATTR_PRIO_SHIFT);
        *prioceiling = (int32_t)temp_ceil;
    }
    return ret;
}

/* pthread_mutexattr_setprioceiling() API */
int32_t pthread_mutexattr_setprioceiling(pthread_mutexattr_t* attr, int32_t ceil)
{
    int32_t ret;
    int32_t valid = pthread_mutexattr_valid((const pthread_mutexattr_t*)attr);
    if ( (ceil >= MCOS_CFG_PT_MIN_PRI)
         && (ceil <= MCOS_CFG_PT_MAX_PRI)
         && (valid != 0))
    {
        *attr = ((*attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_PRIO_MASK << MUTEX_ATTR_PRIO_SHIFT))
                 | (pthread_mutexattr_t)((pthread_mutexattr_t)ceil << MUTEX_ATTR_PRIO_SHIFT));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}


int32_t pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* restrict attr)
{
START_TIMER
    g_pthread_mutex_init_count++;
    int32_t ret = 0;
    uint32_t alignmutex = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    uint32_t alignattr = CHECK_ALIGN(attr, (uint32_t)sizeof(pthread_mutexattr_t));
    if (   (mutex == NULL_PNTR)
        || (alignmutex != 0U)
        || (   (attr != NULL_PNTR)
            && (alignattr != 0U))
        )
    {
        ret = EINVAL;
    }
    else
    {
        mutex_lockval_t mtx = {0};
        pthread_mutexattr_t mattr;
        if (attr != NULL_PNTR)
        {
            mattr = attr[0];
        }
        else
        {
            mattr = (pthread_mutexattr_t)0;
        }
        if ((((mattr >> MUTEX_ATTR_TYPE_SHIFT) & (pthread_mutexattr_t)MUTEX_ATTR2_MASK) == (pthread_mutexattr_t)MUTEX_ATTR2_MASK)
                || (((mattr >> MUTEX_ATTR_PROTOCOL_SHIFT) & (pthread_mutexattr_t)MUTEX_ATTR2_MASK) == (pthread_mutexattr_t)MUTEX_ATTR2_MASK))
        {
            ret = EINVAL;
        }
        else
        {
            if ((mattr & (pthread_mutexattr_t)MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
            {
                pthread_mutexattr_t prioceil = MUTEX_PRIOCEIL(mattr);
                if (   (prioceil > (pthread_mutexattr_t)MCOS_CFG_PT_MAX_PRI)
                    || (prioceil < (pthread_mutexattr_t)MCOS_CFG_PT_MIN_PRI))
                {
                    ret = EINVAL;
                }
            }
        }
        if (ret != EINVAL)
        {
            (void)memset(mutex, 0, sizeof(*mutex));
            if (((mattr & (pthread_mutexattr_t)MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0)
                    || ((mattr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                    || ((mattr & (pthread_mutexattr_t)MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0) )
            {
                mutex->list.next = &mutex->list;
                mutex->list.prev = &mutex->list;
            }
            /* mask out the pshared attribute */
            mtx.m.attr = mattr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_IS_OWNERDEAD;
            mutex->lock.val = mtx.val;
            atomic_thread_fence(memory_order_seq_cst);
        }
    }
STOP_TIMER
    return ret;
}

int32_t pthread_mutex_destroy(pthread_mutex_t* mutex)
{
START_TIMER
    int32_t ret = 0;
    mutex_lockval_t mtx;
    bool wakeup = false;
    uint32_t align = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    if ((mutex == NULL_PNTR)
            || (align != 0U)
            || (mutex->lock.val == MUTEX_INVALID))
    {
        ret = EINVAL;
    }
    else
    {
        _Bool comp;
        mtx.val = atomic_load(&mutex->lock.val);
        do {
            if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_STATE_MASK) != (pthread_mutexattr_t)MUTEX_STATE_UNLOCKED)
            {
                if (((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0)
                        && ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_OWNERDEAD) != (pthread_mutexattr_t)0))
                {
                    /* It is acceptable to destroy a OWNERDEAD robust mutex.
                     * Wake up remaining waiters though.
                     */
                    if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_STATE_MASK) == (pthread_mutexattr_t)MUTEX_STATE_CONTENDED)
                    {
                        wakeup = true;
                    }
                }
                else
                {
                    ret = EBUSY;
                    break;
                }
            }
            comp = atomic_compare_exchange_strong(&mutex->lock.val,
                &mtx.val,
                    MUTEX_INVALID);
        } while (comp == false);
        if (wakeup == true)
        {
            /* wake up all waiters */
            (void)mcos_mfutex_wake(&mutex->lock, (uint32_t)MCOS_MFUTEX_WAKE_ALL);
        }
    }
STOP_TIMER
    return ret;
}

/* Only the current thread should be able to access the owned mutex list
 * of the current thread, except when the thread is cancelled. In this case,
 * the current thread would not be able to access the owned mutex list, and
 * there is still no concurrency.
 * Therefore, no locking is necessary to add/remove a owned mutex from the list.
 */
static void mutex_link(pthread_mutex_t *mutex, pttcb_t *pttcb)
{
    pthread_mutex_t *ptmtx = pttcb->ownmutex;
    mfutex_list_t *list;
    if (ptmtx != NULL_PNTR)
    {
        list = &ptmtx->list;
        mutex->list.next = list;
        mutex->list.prev = list->prev;
        list->prev->next = &mutex->list;
        list->prev = &mutex->list;
    }
    else
    {
        pttcb->ownmutex = mutex;
        /* mutex already points to itself */
    }
}

void mutex_unlink(pthread_mutex_t *mutex, pttcb_t *pttcb)
{
    if (pttcb->ownmutex == mutex)
    {
        /* relies on the fact that list is first entry ! */
        if (mutex != (pthread_mutex_t*)mutex->list.next)
        {
            pttcb->ownmutex = (pthread_mutex_t*)mutex->list.next;
        }
        else
        {
            pttcb->ownmutex = NULL_PNTR;
        }
    }
    mutex->list.next->prev = mutex->list.prev;
    mutex->list.prev->next = mutex->list.next;
    mutex->list.next = &mutex->list;
    mutex->list.prev = &mutex->list;
}

#define TPRI_MAX        (254)       /* max thread priority */

static inline uint16_t MCOS_PT_PRI(uint8_t _pri_)
{
    return ((uint16_t)_pri_ - ((uint16_t)TPRI_MAX-(uint16_t)MCOS_CFG_PT_MAX_PRI));
}

static inline uint16_t PT_EXT_PRI(uint8_t _pri_)
{
    return ((uint16_t)MCOS_CFG_PT_MAX_PRI + (uint16_t)1U - (uint16_t)(_pri_));
}

/* Get the highest posix priority of owned list */
uint16_t mutex_owner_get_highprio(pthread_mutex_t *first)
{
    uint16_t ret = (uint16_t)0U;
    uint16_t inhertpri = (uint16_t)0U;
    uint16_t protectpri = (uint16_t)0U;
    pthread_mutex_t *ptmtx = first;

    if (ptmtx != NULL_PNTR)
    {
        /* Loop owned mutex queue */
        do
        {
            mutex_lockval_t mtx;
            mtx.val = ptmtx->lock.val;
            if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0U)
            {
                /* Get the highest of waiting threads, note: it is a mcos priority. */
                ret = mfutex_waiter_get_highprio(&ptmtx->lock);
                if( (ret > (uint16_t)0U) && ((inhertpri == (uint16_t)0U) || (ret < inhertpri)) )
                {
                    inhertpri = ret;
                }
            }
            else if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
            {
                /* Get the highest of the priority ceilings, note: it is a pt priority. */
                ret = (uint16_t)MUTEX_PRIOCEIL(mtx.m.attr);
                if( (ret > (uint16_t)0U) && ((protectpri == (uint16_t)0U) || (ret > protectpri)) )
                {
                    protectpri = ret;
                }
            }
            else
            {
                /* do nothing */
            }
            ptmtx = (pthread_mutex_t *)ptmtx->list.next;
        }while(ptmtx != first);

        if(inhertpri != (uint16_t)0)
        {
            /* Convert mcos priority to pt priority. */
            inhertpri = MCOS_PT_PRI((uint8_t)inhertpri);
            ret = PT_EXT_PRI((uint8_t)inhertpri);
        }
        if(protectpri > ret)
        {
            ret = protectpri;
        }
    }
    return ret;
}

/* Check if the thread owns any prioritized mutex */
mcos_bool_t mutex_has_prioritized(const pttcb_t *pttcb)
{
    mcos_bool_t     prioritize = mcos_false;
    pthread_mutex_t *first = pttcb->ownmutex;
    pthread_mutex_t *ptmtx = first;

    if (ptmtx != NULL_PNTR)
    {
        /* Loop owned mutex queue */
        do
        {
            mutex_lockval_t mtx;
            mtx.val = ptmtx->lock.val;
            if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
            {
                prioritize = mcos_true;
            }
            else if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
            {
                prioritize = mcos_true;
            }
            else
            {
                /* do nothing */
            }
            ptmtx = (pthread_mutex_t *)ptmtx->list.next;
        } while ((ptmtx != first) && (prioritize != mcos_false));
    }
    return prioritize;
}

int32_t pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    int32_t ret = 0;
    bool do_unlock;
    bool do_wake;
    mutex_lockval_t  mtx;
    mutex_lockval_t  newmtx;
    mutex_lockval_t  oldmtx;
    uint32_t flags;

    uint32_t align = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    if ((mutex == NULL_PNTR)
            || (align != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        mtx.val = mutex->lock.val;
        if (mtx.val == MUTEX_INVALID)
        {
            ret = EINVAL;
        }
        else
        {
            pthread_t tid = pthread_self();
            if (mtx.m.owner != tid)
            {
                ret = EPERM;
            }
            else
            {
                do_unlock = true;
                if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_RECURSIVE) != (pthread_mutexattr_t)0)
                {
                    uint16_t recursion;
                    recursion = atomic_fetch_add(&mutex->recursion, -1) - 1;
                    if (recursion > (uint16_t)0)
                    {
                        do_unlock = false;
                    }
                }
                if (do_unlock)
                {
                    pttcb_t* pttcb = pt_get_pttcb();

                    if (((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                            || ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                            || ((mtx.m.attr & MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0) )
                    {
                        uint16_t savedprio;
                        pttcb = pt_get_pttcb();
                        savedprio = PT_EXT_PRI(pttcb->pthrpri);

                        /* set state to LOCKING for unlinking mutex */
                        mtx.m.attr = (mtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK) | (pthread_mutexattr_t)MUTEX_STATE_LOCKING;
                        oldmtx.val = atomic_exchange(&mutex->lock.val, mtx.val);

                        /* unlink mutex from owned mutex queue */
                        mutex_unlink(mutex, pttcb);

                        /* now really free the mutex */
                        atomic_thread_fence(memory_order_seq_cst);
                        newmtx.val = mtx.val;
                        if (((mtx.m.attr & MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0)
                                && ((mtx.m.attr & MUTEX_IS_OWNERDEAD) != (pthread_mutexattr_t)0) )
                        {
                            newmtx.m.owner = MUTEX_OWNER_NOTRECOVERABLE;
                        }
                        else
                        {
                            newmtx.m.owner = 0;
                            /* "MUTEX_STATE_UNLOCKED" is 0, so shift operation is not required. */
                            newmtx.m.attr = newmtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK;
                        }
                        /* no need for exchange atomic operation, because it can only be
                         * changed from LOCKING to UNLOCKED
                         */

                        (void)atomic_exchange(&mutex->lock.val, newmtx.val);

                        /* If the priority has been changed for MUTEX_STATE_LOCKING,
                         * then change it back.
                         */
                        if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0)
                        {
                            uint16_t prior = PT_EXT_PRI(pttcb->pthrpri);
                            if (prior > savedprio)
                            {
                                (void)pthread_setschedprio_mutex(pthread_self(), 0, 1);
                            }
                        }
                        mtx.val = oldmtx.val;
                        /* mtx.val still contains the original value before the LOCKING step */
                    }
                    else
                    {
                        mtx.m.owner = 0;
                        /* "MUTEX_STATE_UNLOCKED" is 0, so shift operation is not required. */
                        mtx.m.attr = mtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK;
                        mtx.val = atomic_exchange(&mutex->lock.val, mtx.val);
                    }

                    do_wake = false;
                    /* if a new waiter has arrived, it will be contended and should be woken up */
                    if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_STATE_MASK) == (pthread_mutexattr_t)MUTEX_STATE_CONTENDED)
                    {
                        do_wake = true;
                    }
                    /* if we previously woke up a waiter and no new one has arrived, the
                     * MUTEX_STATE_CONTENDED flag is no longer set. But there may still
                     * be waiters.
                     */
                    else
                    {
                        uint_fast8_t idx = (uint_fast8_t)0;
                        while ((idx < (uint_fast8_t)MFUTEX_LOCK_CORE_COUNT) && (mutex->lock.percpu[idx].prio == (uint8_t)0))
                        {
                            idx++;
                        }
                        if (idx < (uint_fast8_t)MFUTEX_LOCK_CORE_COUNT)
                        {
                            do_wake = true;
                        }
                    }

                    if (do_wake)
                    {
                        if (((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                                || ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0))
                        {
                            flags = (uint32_t)(MCOS_MFUTEX_WAKE_ONE|MCOS_MFUTEX_ORDERING_PRIO);
                        }
                        else
                        {
                            flags = (uint32_t)MCOS_MFUTEX_WAKE_ONE;
                        }
                        /* FIXME: failure means a corrupted lockobj.
                         * What should we do then?
                         */
                        (void)mcos_mfutex_wake(&mutex->lock, flags);
                    }

                    if ((((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                            || ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0))
                            && (pttcb->pbasepri != pttcb->pthrpri))
                    {
                        (void)pthread_setschedprio_mutex(pthread_self(), 0, 1);
                    }
                }
            }
        }
    }

    return ret;
}

static int32_t pthread_mutex_trylock_internal(pthread_mutex_t* mutex, uintptr_t *val)
{
    mutex_lockval_t mtx;
    pttcb_t *pttcb;
    int32_t ret = 0;
    bool loop;
    uint16_t savedprio;

    pttcb = pt_get_pttcb();
    do
    {
        loop = false;
        mtx.val = atomic_load(&mutex->lock.val);
        val[0] = mtx.val;

        /* If the mutex is OWNERDEAD and not already taken, successfully lock,
         * but return EOWNERDEAD */
        if (mtx.m.owner == MUTEX_OWNER_NOTRECOVERABLE)
        {
            ret = ENOTRECOVERABLE;
        }
        else if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
        {
            uint16_t prioceil = (uint16_t)MUTEX_PRIOCEIL(mtx.m.attr);
            uint16_t prior = PT_EXT_PRI(pttcb->pbasepri);
            if (prior > prioceil)
            {
                ret = EINVAL;
                break;
            }
        }
        else
        {
            /* do nothing */
        }

        if (ret != ENOTRECOVERABLE)
        {
            if (   ((mtx.m.attr & MUTEX_IS_OWNERDEAD) != (pthread_mutexattr_t)0)
                && (mtx.m.owner == 0))
            {
                /* "MUTEX_STATE_UNLOCKED" is 0, so shift operation is not required. */
                mtx.m.attr = mtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK;
                ret = EOWNERDEAD;
            }
            else
            {
                ret = 0;
            }
        }
        /* see if we can try to acquire the mutex */
        if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_STATE_MASK) == (pthread_mutexattr_t)MUTEX_STATE_UNLOCKED)
        {
            if (((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                    || ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                    || ((mtx.m.attr & MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0) )
            {
                /* First set to LOCKING to make it non-unlocked.
                 * Then we will link it into our owned mutex list
                 * Then mark it LOCKED.
                 */
                mtx.m.attr |= (pthread_mutexattr_t)MUTEX_STATE_LOCKING;
            }
            else
            {
                /* The following operation is redundant, so delete. */
                /* mtx.m.attr &= ~(pthread_mutexattr_t)MUTEX_STATE_MASK;*/
                mtx.m.attr |= (pthread_mutexattr_t)MUTEX_STATE_LOCKED;
            }
            if ((ret == 0) || (ret == EOWNERDEAD))
            {
                _Bool comp;
                mtx.m.owner = pthread_self();
                if (((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                        || ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                        || ((mtx.m.attr & MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0) )
                {
                    savedprio = PT_EXT_PRI(pttcb->pthrpri);
                }
                comp = atomic_compare_exchange_strong(&mutex->lock.val, val, mtx.val);
                if (comp == false)
                {
                    /* start over when the mutex state was changed */
                    loop = true;
                }
            }
            if ((loop != true) &&
                ((ret == 0) || (ret == EOWNERDEAD)))
            {
                if (((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                        || ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                        || ((mtx.m.attr & MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0) )
                {
                    uintptr_t oldval;
                    uint8_t prior;

                    /* link owned mutex to thread queue */
                    mutex_link(mutex, pttcb);
                    atomic_thread_fence(memory_order_seq_cst);

                    /* Now really mark it locked.
                    * Only switch from LOCKING to LOCKED using cmpxchg.
                    */
                    oldval = mtx.val;
                    mtx.m.attr = (mtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK) | (pthread_mutexattr_t)MUTEX_STATE_LOCKED;
                    (void)atomic_compare_exchange_strong(&mutex->lock.val, &oldval, mtx.val);

                    /* If the priority has been changed for MUTEX_STATE_LOCKING,
                    * then change it back.
                    */
                    prior = (uint8_t)PT_EXT_PRI(pttcb->pthrpri);
                    if (prior > savedprio)
                    {
                        (void)pthread_setschedprio_mutex(mtx.m.owner, 0, 1);
                    }
                }
                if ((mtx.m.attr & MUTEX_IS_RECURSIVE) != (pthread_mutexattr_t)0)
                {
                    (void)atomic_fetch_add(&mutex->recursion, 1);
                }
                /* raise ourselves since we own the mutex */
                if ((mtx.m.attr & MUTEX_IS_PROTECT) != (pthread_mutexattr_t)0)
                {
                    uint16_t prioceil = (uint16_t)MUTEX_PRIOCEIL(mtx.m.attr);
                    uint16_t prior = PT_EXT_PRI(pttcb->pthrpri);
                    if (prior < prioceil)
                    {
                        (void)pthread_setschedprio_mutex(mtx.m.owner, 0, 1);
                    }
                }
            }
        }
        else if (mtx.val == MUTEX_INVALID)
        {
            ret = EINVAL;
        }
        else
        {
            pthread_t tid = pthread_self();
            if (mtx.m.owner == tid)
            {
                if ((mtx.m.attr & MUTEX_IS_RECURSIVE) != (pthread_mutexattr_t)0)
                {
                    /* increase recursion */
                    _Bool comp;
                    uint16_t oldr;
                    oldr = atomic_load(&mutex->recursion);
                    do
                    {
                        if ((oldr + (uint16_t)1) == (uint16_t)UINT16_MAX)
                        {
                            ret = EAGAIN;
                            break;
                        }
                        comp = atomic_compare_exchange_strong(&mutex->recursion, &oldr, (uintptr_t)(oldr + (uint16_t)1));
                    }
                    while (comp == false);
                }
                else if ((mtx.m.attr & MUTEX_IS_ERRCHECK) != (pthread_mutexattr_t)0)
                {
                    ret = EDEADLK;
                }
                else
                {
                    ret = EBUSY;
                }
            }
            else if (ret == 0)
            {
                ret = EBUSY;
            }
            else
            {
                /* No operation */
            }
        }
    }
    while (loop != false);

    return ret;
}

int32_t pthread_mutex_trylock(pthread_mutex_t* mutex)
{
START_TIMER
    int32_t ret;
    uint32_t align = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    if ((mutex == NULL_PNTR)
            || (align != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        uintptr_t val;
        ret = pthread_mutex_trylock_internal(mutex, &val);
    }


    if (ret == EDEADLK)
    {
        ret = EBUSY;
    }
STOP_TIMER
    return ret;
}

static int32_t pthread_mutex_timedlock_internal(pthread_mutex_t* restrict mutex,
                            const struct timespec* restrict abstmo,
                            bool timed)
{
    int32_t ret;
    mcos_er_t mfret;
    uintptr_t val;
    uint16_t pflags;
    pttcb_t *pttcb;

    uint32_t align = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    if ((mutex == NULL_PNTR)
            || (align != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        ret = pthread_mutex_trylock_internal(mutex, &val);
    }
    if (ret == 0)
    {
        /* nothing to do, we own it. */
    }
    else if (ret == EBUSY)
    {
        uint32_t flags = 0U;
        mcos_systime_t systime = {0};
        mutex_lockval_t mtx;
        mcos_bool_t setval;

        align = CHECK_ALIGN(abstmo, (uint32_t)sizeof(abstmo->tv_nsec));
        if ((timed != false) && ((abstmo == NULL_PNTR)
            || (align != 0U)
            || (abstmo->tv_nsec < 0L)
            || (abstmo->tv_nsec >= 1000000000L)))
        {
            ret = EINVAL;
        }
        else if (timed != false)
        {
            (void)pt_abstime_convert(abstmo, &systime);
        }
        else
        {
            /* do nothing */
        }

        if (ret == EBUSY)
        {
            mtx.val = val;
            if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
            {
                /* The initial value of "flags" is 0,
                    so OR operation is not required. */
                flags = (uint32_t)MCOS_MFUTEX_ORDERING_PRIO;
            }

            /* start no-cancellation point */
            pttcb = pt_get_pttcb();
            pflags = atomic_fetch_and(&pttcb->flags, ~PTHREAD_CANCELOK) & PTHREAD_CANCELOK;
            do
            {
                uint_fast8_t enable_break = 0U;
                /* mark the mutex as contended */
                setval = mcos_false;
                val = atomic_load(&mutex->lock.val);
                do
                {
                    uint_fast8_t enable_continue = 0U;

                    mtx.val = val;
                    if (mtx.val == MUTEX_INVALID)
                    {
                        ret = EINVAL;
                        enable_break = 1U;
                    }
                    /* This case can be reached if the below cmpxchg failed
                     * because the mutex was freed. */
                    else
                    {
                        if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_STATE_MASK) == (pthread_mutexattr_t)MUTEX_STATE_UNLOCKED)
                        {
                            ret = pthread_mutex_trylock_internal(mutex, &val);
                            if (ret == 0)
                            {
                                enable_break = 1U;
                            }
                            else
                            {
                                enable_continue = 1U;
                            }
                        }
                        else if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_STATE_MASK) == (pthread_mutexattr_t)MUTEX_STATE_LOCKING)
                        {
                            mfutex_sched_change(mtx.m.owner);
                            val = atomic_load(&mutex->lock.val);
                            enable_continue = 1U;
                        }
                        else
                        {
                            /* do nothing */
                        }
                    }

                    if (enable_break != 0U)
                    {
                        break;
                    }
                    else if (enable_continue != 1U)
                    {
                        mtx.m.attr = (mtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK) | (pthread_mutexattr_t)MUTEX_STATE_CONTENDED;
                        setval = atomic_compare_exchange_strong((uintptr_t *)&mutex->lock.val, &val, mtx.val);
                    }
                    else
                    {
                        /* do nothing */
                    }

                } while (setval == mcos_false);

                if (setval != mcos_false)
                {
                    /* block */
                    if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                    {
                        flags |= (uint32_t)MCOS_MFUTEX_ORDERING_PRIO;
                    }
                    do {
                        mfret = mcos_mfutex_wait(&mutex->lock, mtx.val, flags, &systime);
                        if (mfret == MCOS_MFUTEX_RET_SIGNAL)
                        {
                            pt_handle_interrupt(pttcb);
                        }
                        else if (mfret == MCOS_MFUTEX_RET_TIMEOUT)
                        {
                            if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                            {
                                (void)pthread_setschedprio_mutex(mtx.m.owner, 0, 1);
                            }
                            ret = ETIMEDOUT;
                        }
                        else if (mfret == MCOS_EMACV)
                        {
                            if ((mtx.m.attr & (pthread_mutexattr_t)MUTEX_IS_INHERIT) != (pthread_mutexattr_t)0)
                            {
                                (void)pthread_setschedprio_mutex(mtx.m.owner, 0, 1);
                            }
                            ret = EFAULT;
                        }
                        else /* 0 or NOTVALUE */
                        {
                            ret = pthread_mutex_trylock_internal(mutex, &val);
                        }
                    } while (mfret == MCOS_MFUTEX_RET_SIGNAL);
                }
            } while (ret == EBUSY);

            /* end nocancel range */
            (void)atomic_fetch_or(&pttcb->flags, pflags);
        }
    }
    else
    {
        /* do nothing */
    }
    /* For other errors, just return them */
    return ret;
}

int32_t pthread_mutex_timedlock(pthread_mutex_t* restrict mutex,
                            const struct timespec* restrict abstmo)
{
START_TIMER
    int32_t ret = pthread_mutex_timedlock_internal(mutex, abstmo, true);
STOP_TIMER
    return ret;
}

int32_t pthread_mutex_lock(pthread_mutex_t* mutex)
{
    return pthread_mutex_timedlock_internal(mutex, NULL_PNTR, false);
}

int32_t pthread_mutex_getprioceiling(const pthread_mutex_t *restrict mutex,
                                 int32_t *restrict prioceiling)
{
    int32_t ret = 0;
    uint32_t alignmutex = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    uint32_t alignceil = CHECK_ALIGN(prioceiling, (uint32_t)sizeof(int32_t));
    if ((mutex == NULL_PNTR)
            || (alignmutex != 0U)
            || (prioceiling == NULL_PNTR)
            || (alignceil != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        mutex_lockval_t mtx;
        mtx.val = mutex->lock.val;
        prioceiling[0] = (int32_t)MUTEX_PRIOCEIL(mtx.m.attr);
        if (prioceiling[0] == (int32_t)PTHREAD_PRIO_NONE)
        {
            ret = EINVAL;
        }
    }
    return ret;
}
int32_t pthread_mutex_setprioceiling(pthread_mutex_t *restrict mutex,
                                     int32_t prio, int32_t *restrict old_ceil)
{
    int32_t ret;
    uint32_t alignmutex = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    uint32_t alignceil = CHECK_ALIGN(old_ceil, (uint32_t)sizeof(int32_t));
    if ((mutex == NULL_PNTR)
            || (alignmutex != 0U)
            || (prio > MCOS_CFG_PT_MAX_PRI)
            || (prio < MCOS_CFG_PT_MIN_PRI)
            || ((old_ceil != NULL_PNTR) && (alignceil != 0U)))
    {
        ret = EINVAL;
    }
    else
    {
        mutex_lockval_t mtx;
        pthread_mutexattr_t prioceil;

        mtx.val = mutex->lock.val;
        prioceil = MUTEX_PRIOCEIL(mtx.m.attr);
        if (prioceil == (pthread_mutexattr_t)PTHREAD_PRIO_NONE)
        {
            ret = EINVAL;
        }
        else
        {
            ret = pthread_mutex_lock(mutex);
            if (ret == 0)
            {
                mtx.val = mutex->lock.val;
                if (old_ceil != NULL_PNTR)
                {
                    old_ceil[0] = (int32_t)MUTEX_PRIOCEIL(mtx.m.attr);
                }
                mtx.m.attr &= (pthread_mutexattr_t)~(pthread_mutexattr_t)((pthread_mutexattr_t)MUTEX_ATTR_PRIO_MASK << MUTEX_ATTR_PRIO_SHIFT);
                mtx.m.attr |= (pthread_mutexattr_t)((uint32_t)prio << MUTEX_ATTR_PRIO_SHIFT);
                mutex->lock.val = mtx.val;
                (void)pthread_mutex_unlock(mutex);
            }
        }
    }
    return ret;
}

int32_t pthread_mutex_consistent(pthread_mutex_t *mutex)
{
    int32_t ret = 0;
    uint32_t align = CHECK_ALIGN(mutex, (uint32_t)sizeof(uintptr_t));
    if ((mutex == NULL_PNTR)
            || (align != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        mutex_lockval_t mtx;
        pthread_t tid = pthread_self();
        mtx.val = atomic_load(&mutex->lock.val);
        if (((mtx.m.attr & MUTEX_IS_ROBUST) == (pthread_mutexattr_t)0)
            || ((mtx.m.attr & MUTEX_IS_OWNERDEAD) == (pthread_mutexattr_t)0)
            || (mtx.m.owner != tid) )
        {
            ret = EINVAL;
        }
        else
        {
            /* remove OWNERDEAD bit */
            mtx.m.attr = (mtx.m.attr & (pthread_mutexattr_t)~ (pthread_mutexattr_t)MUTEX_IS_OWNERDEAD);

            (void)atomic_exchange(&mutex->lock.val, mtx.val);
        }
    }
    return ret;
}

/* implementation-internal call to mark the mutex as inconsistent */
void pthread_mutex_inconsistent(pthread_mutex_t *mutex)
{
    mutex_lockval_t mtx;
    uintptr_t oldval;

    mtx.val = atomic_load(&mutex->lock.val);
    if ((mtx.m.attr & MUTEX_IS_ROBUST) != (pthread_mutexattr_t)0)
    {

        uint32_t mutex_is_ownerdead_val;
        
        oldval = mtx.val;
        mutex_is_ownerdead_val = (uint32_t)MUTEX_IS_OWNERDEAD & (uint32_t)UINT16_MAX;
        mtx.m.attr = (mtx.m.attr & (pthread_mutexattr_t)~(pthread_mutexattr_t)MUTEX_STATE_MASK)
            | (pthread_mutexattr_t)mutex_is_ownerdead_val;
        mtx.m.owner = 0;
        /* This is not done as an atomic change because the mutex is anyway.
         * Formally-speaking, this may require a barrier, but on the vast majority
         * of architectures (except the long defunct DEC Alpha), atomic instructions
         * include a barrier.
         */
        mutex->recursion = (uint16_t)0;
        /* Try to switch from LOCKED to ROBUST.
         * UNLOCKED is left untouched because it is not inconsistent.
         * INVALID is left untouched because it is not inconsistent.
         * CONTENDED is left untouched because pthread_mutex_consistent() will
         * recognize this case and unlock waiters.
         */
        (void)atomic_compare_exchange_strong(&mutex->lock.val, &oldval, mtx.val);
        /* Wake any remaining waiter */
        (void)mcos_mfutex_wake(&mutex->lock, (uint32_t)MCOS_MFUTEX_WAKE_ALL);
    }
}
