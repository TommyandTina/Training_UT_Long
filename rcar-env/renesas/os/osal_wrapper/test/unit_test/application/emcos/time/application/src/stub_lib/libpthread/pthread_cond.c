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
[pthread_cond.c] - MCOS Pthread cond attributes API
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

#include "stub.h"
#include "target/emcos/r_osal_common.h"

/*
  Bitfield of cond value
  monotime  :  1 unused
  pshared   :  1 unused
  wakereq   :  1
  invalid   :  1
  waiting   : 28
*/
#define COND_MONOTIME_MASK      1
#define COND_MONOTIME_SHIFT     0U
#define COND_MONOTIME           ((uint32_t)COND_MONOTIME_MASK << COND_MONOTIME_SHIFT)

#define COND_PSHARED_MASK       1
#define COND_PSHARED_SHIFT      1U
#define COND_PSHARED            ((uint32_t)COND_PSHARED_MASK << COND_PSHARED_SHIFT)

#define COND_WAKEREQ_MASK       1
#define COND_WAKEREQ_SHIFT      2U
#define COND_WAKEREQ            ((uint32_t)COND_WAKEREQ_MASK << COND_WAKEREQ_SHIFT)

#define COND_INVALID_MASK       1
#define COND_INVALID_SHIFT      3U
#define COND_INVALID            ((uint32_t)COND_INVALID_MASK << COND_INVALID_SHIFT)

#define COND_WAITING_SHIFT      4U
#define COND_WAITING_MASK       0x0fffffff
#define COND_WAITING            ((uint32_t)COND_WAITING_MASK << COND_WAITING_SHIFT)

 /* Cond attributes */
#define COND_ATTR_MONOTIME      COND_MONOTIME
#define COND_ATTR_SHARED        COND_PSHARED

#define CHECK_ALIGN(_x_, _a_)  ((uintptr_t)(_x_) & ((uintptr_t)(_a_) - 1))

static int32_t pthread_cond_timedwait_internal(pthread_cond_t *restrict cond,
                                           pthread_mutex_t *restrict mtx,
                                           const struct timespec *restrict timeout,
                                           bool timed);
static int32_t pthread_cond_unlock(pthread_cond_t *cond, uint32_t flags);
static int32_t pthread_condattr_valid(const pthread_condattr_t* attr);

static int32_t pthread_condattr_valid(const pthread_condattr_t* attr)
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

/* pthread_condattr_init() API */
int32_t pthread_condattr_init(pthread_condattr_t* attr)
{
    int32_t ret;
    int32_t valid = pthread_condattr_valid((const pthread_condattr_t*)attr);
    if (valid != 0)
    {
        *attr = 0U;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_condattr_destroy() API */
int32_t pthread_condattr_destroy(pthread_condattr_t* attr)
{
    int32_t ret;
    int32_t valid = pthread_condattr_valid((const pthread_condattr_t*)attr);
    if (valid != 0)
    {
        *attr = 0xffffffffU;
        ret =  0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_condattr_getpshared() API */
int pthread_condattr_getpshared(const pthread_condattr_t* restrict attr, int* restrict pshared)
{
    int32_t ret;
    int32_t valid = pthread_condattr_valid(attr);

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
        *pshared  = ((*attr & (pthread_condattr_t)COND_ATTR_SHARED) != 0U
                     ? (int32_t)PTHREAD_PROCESS_SHARED
                     : (int32_t)PTHREAD_PROCESS_PRIVATE);
    }
    return ret;
}

/* pthread_condattr_setpshared() API */
int pthread_condattr_setpshared(pthread_condattr_t* attr, int32_t pshared)
{
    int32_t ret;
    int32_t valid = pthread_condattr_valid((const pthread_condattr_t*)attr);
    if (((pshared == (int32_t)PTHREAD_PROCESS_SHARED)
         || (pshared == (int32_t)PTHREAD_PROCESS_PRIVATE))
        && (valid != 0) )
    {
        *attr = ((*attr & (pthread_condattr_t)~(pthread_condattr_t)COND_ATTR_SHARED)
                 | (pshared == (int32_t)PTHREAD_PROCESS_SHARED
                    ? (pthread_condattr_t)COND_ATTR_SHARED
                    : 0U));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_condattr_getclock() API */
int32_t pthread_condattr_getclock(const pthread_condattr_t* restrict attr,
                              clockid_t* restrict clock_id)
{
    int32_t ret;
    int32_t valid = pthread_condattr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN(clock_id, (uint32_t)sizeof(*clock_id));
        if ((clock_id != NULL_PNTR) && (align == 0U))
        {
            ret = 0;
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *clock_id  = ((*attr & (pthread_condattr_t)COND_ATTR_MONOTIME) != 0U
                     ? CLOCK_MONOTONIC
                     : CLOCK_REALTIME);
    }
    return ret;
}

/* pthread_condattr_setclock() API */
int32_t pthread_condattr_setclock(pthread_condattr_t* attr, clockid_t clock_id)
{
    int32_t ret;
    int32_t valid = pthread_condattr_valid((const pthread_condattr_t*)attr);
    if (   (   (clock_id == CLOCK_MONOTONIC)
            || (clock_id == CLOCK_REALTIME) )
        && (valid != 0) )
    {
        *attr = ((*attr & (pthread_condattr_t)~(pthread_condattr_t)(COND_ATTR_MONOTIME))
                 | (uint32_t)(clock_id == CLOCK_MONOTONIC
                    ? (pthread_condattr_t)COND_ATTR_MONOTIME
                    : 0U));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* Condition API */
int32_t pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *attr)
{
START_TIMER
    g_pthread_cond_init_count++;
    int32_t ret = 0;
    uint32_t aligncond = CHECK_ALIGN(cond, (uint32_t)sizeof(uintptr_t));
    uint32_t alignattr = CHECK_ALIGN(attr, (uint32_t)sizeof(uint32_t));
    if (   (cond == NULL_PNTR)
        || (aligncond != 0U)
        || (   (attr != NULL_PNTR)
            && (alignattr != 0U)))
    {
        ret = EINVAL;
    }
    else
    {
        (void)memset(cond, 0, sizeof(pthread_cond_t));
        if (attr != NULL_PNTR)
        {
            cond->lock.val = (uintptr_t)attr[0];
        }
    }
STOP_TIMER
    return ret;
}
int32_t pthread_cond_destroy(pthread_cond_t *cond)
{
START_TIMER
    int32_t ret = 0;
    uint32_t align = CHECK_ALIGN(cond, (uint32_t)sizeof(uintptr_t));
    if ((cond == NULL_PNTR) || (align != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        uintptr_t val;
        uint32_t masked_value = (uint32_t)COND_INVALID;
        bool comp;
        val = atomic_load(&cond->lock.val);
        do
        {
            if ((val & (uintptr_t)masked_value) != (uintptr_t)0)
            {
                ret = EINVAL;
            }
            else if ((val >> COND_WAITING_SHIFT) > (uintptr_t)0)
            {
                ret = EBUSY;
            }
            else
            {
                /* normal case */
            }
            if (ret != 0)
            {
                break;
            }
            else
            {
                comp = atomic_compare_exchange_strong(&cond->lock.val, &val, COND_INVALID);
            }
        }
        while (comp == false);
    }
STOP_TIMER
    return ret;
}

static int32_t pthread_cond_unlock(pthread_cond_t *cond, uint32_t flags)
{
    int32_t ret = 0;
    uintptr_t oldval;
    uintptr_t val;
    uint32_t align = CHECK_ALIGN(cond, (uint32_t)sizeof(uintptr_t));
    if ((cond == NULL_PNTR) || (align != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        /* set wake-up request flag */
        _Bool comp;
        uint32_t masked_invalid = (uint32_t)COND_INVALID;
        uint32_t masked_wakereq = (uint32_t)COND_WAKEREQ;
        oldval = atomic_load(&cond->lock.val);
        do
        {
            if ((oldval & (uintptr_t)masked_invalid) != (uintptr_t)0)
            {
                ret = EINVAL;
                break;
            }
            val = oldval | (uintptr_t)masked_wakereq;
            comp = atomic_compare_exchange_strong(&cond->lock.val, &oldval, val);
        }
        while (comp == false);
        if (ret == 0)
        {
            /* wake waiters */
            (void)mcos_mfutex_wake(&cond->lock, flags);
        }
    }
    return ret;
}

int32_t pthread_cond_broadcast(pthread_cond_t *cond)
{
START_TIMER
    int32_t ret = pthread_cond_unlock(cond, (uint32_t)MCOS_MFUTEX_WAKE_ALL);
STOP_TIMER
    return ret;
}

int32_t pthread_cond_signal(pthread_cond_t *cond)
{
START_TIMER
    int32_t ret = pthread_cond_unlock(cond, (uint32_t)(MCOS_MFUTEX_WAKE_ONE|MCOS_MFUTEX_ORDERING_PRIO));
STOP_TIMER
    return ret;
}

static int32_t pthread_cond_timedwait_internal(pthread_cond_t *restrict cond,
                                           pthread_mutex_t *restrict mtx,
                                           const struct timespec *restrict timeout,
                                           bool timed)
{
    int32_t ret = 0;
    int32_t mtxret;
    uintptr_t oldval, tmp_oldval;
    uintptr_t val;
    uint32_t waiters;
    uintptr_t oldmtx;
    uint32_t masked_invalid = (uint32_t)COND_INVALID;
    uint32_t masked_wakereq = (uint32_t)COND_WAKEREQ;

    uint32_t aligncond = CHECK_ALIGN(cond, (uint32_t)sizeof(uintptr_t));
    uint32_t alignmtx = CHECK_ALIGN(mtx, (uint32_t)sizeof(uintptr_t));
    if (   (cond == NULL_PNTR)
        || (aligncond != 0U)
        || (mtx == NULL_PNTR)
        || (alignmtx != 0U)
        )
    {
        ret = EINVAL;
    }
    else
    {
        bool real;
        bool comp;

        oldval = atomic_load(&cond->lock.val);
        do
        {
            if ((oldval & (uintptr_t)masked_invalid) != (uintptr_t)0)
            {
                ret = EINVAL;
                break;
            }
            else
            {
                tmp_oldval = oldval >> COND_WAITING_SHIFT;
                waiters = (uint32_t)tmp_oldval;
                ++waiters;
                tmp_oldval = (uintptr_t)waiters;
                val = (oldval & (((uintptr_t)1 << (uint32_t)COND_WAITING_SHIFT) - (uintptr_t)1)) + (uintptr_t)(tmp_oldval << (uint32_t)COND_WAITING_SHIFT);
                /* clear COND_WAKEREQ flag so that the subsequent waiting requests are not canceled */
                val &= (uintptr_t)~((uint32_t)masked_wakereq);
            }
            comp = atomic_compare_exchange_strong(&cond->lock.val, &oldval, val);
        } while (comp == false);

        real = ((oldval & (uintptr_t)COND_ATTR_MONOTIME) != (uintptr_t)COND_ATTR_MONOTIME);
        if (ret == 0)
        {
            uint_fast8_t enable_break = 0U;
            mcos_bool_t cmpswap;
            oldmtx = atomic_load(&cond->mutex);
            do
            {
                if (oldmtx == (uintptr_t)mtx)
                {
                    /* regular case, someone else already waiting */
                    enable_break = 1U;
                }
                else if (oldmtx != (uintptr_t)0)
                {
                    /* if mutex is already set but not to the mutex we want,
                     * fail.
                     */
                    ret = EINVAL;
                    enable_break = 1U;
                }
                else
                {
                    /* do nothing */
                }

                if (enable_break != 0U)
                {
                    break;
                }
                cmpswap = atomic_compare_exchange_strong(&cond->mutex, &oldmtx, (uintptr_t)mtx);
            }
            while (cmpswap != mcos_false);
            if (ret == 0)
            {
                ret = pthread_mutex_unlock(mtx);
            }
            if (ret == 0)
            {
                int16_t mfret;
                mcos_systime_t systime = {0};
                uint32_t flags = (uint32_t)MCOS_MFUTEX_ORDERING_NONE;
                bool comp;

                uint32_t align = CHECK_ALIGN(timeout, (uint32_t)sizeof(timeout->tv_nsec));
                if ((timed != false) && ((timeout == NULL_PNTR)
                    || (align != 0U)
                    || (timeout->tv_nsec < 0L)
                    || (timeout->tv_nsec >= 1000000000L)))
                {
                    ret = EINVAL;
                }
                else if (timed != false)
                {
                    (void)pt_abstime_convert(timeout, &systime);
                    if (real != false)
                    {
                        /* REALTIME */
                    }
                    else
                    {
                        /* MONOTONIC */
                        /* The initial value of "flags" is 0,
                            so OR operation is not required. */
                        flags = (uint32_t)MCOS_MFUTEX_CLOCK_MONO;
                    }
                }
                else
                {
                    /* do nothing */
                }
                if (ret == 0)
                {
                    oldval = atomic_load(&cond->lock.val);
                    if ((oldval & (uintptr_t)masked_invalid) != (uintptr_t)0)
                    {
                        ret = EINVAL;
                    }
                    else
                    {
                        if ((oldval & (uintptr_t)masked_wakereq) == (uintptr_t)0)
                        {
                            do
                            {
                                mfret = mcos_mfutex_wait(&cond->lock, oldval,
                                                         flags, &systime);
                                if (mfret == MCOS_MFUTEX_RET_TIMEOUT)
                                {
                                    ret = ETIMEDOUT;
                                }
                                else if (mfret == MCOS_MFUTEX_RET_SIGNAL)
                                {
                                    pttcb_t *pttcb = pt_get_pttcb();
                                    pt_handle_interrupt(pttcb);
                                }
                                else if (mfret == MCOS_EMACV)
                                {
                                    ret = EFAULT;
                                }
                                else
                                {
                                    /* normal case */
                                }
                            } while (mfret == MCOS_MFUTEX_RET_SIGNAL);
                        }

                        mtxret = pthread_mutex_lock(mtx);
                        if (mtxret != 0)
                        {
                            ret = mtxret;
                        }
                    }
                }
                do
                {
                    oldval = atomic_load(&cond->lock.val);
                    if ((oldval & (uintptr_t)masked_invalid) != (uintptr_t)0)
                    {
                        ret = EINVAL;
                        break;
                    }
                    else
                    {
                        tmp_oldval = oldval >> COND_WAITING_SHIFT;
                        waiters = (uint32_t)tmp_oldval;
                        --waiters;
                        val = (oldval & ((uintptr_t)((uint8_t)((uint8_t)1U << COND_WAITING_SHIFT)) - (uintptr_t)1))
                                + (uintptr_t)((uintptr_t)waiters << COND_WAITING_SHIFT);
                        if (waiters == 0U)
                        {
                            val &= ~((uintptr_t)masked_wakereq);
                        }
                    }
                    comp = atomic_compare_exchange_strong(&cond->lock.val, &oldval, val);
                }
                while (comp == false);
            }
        }
    }
    return ret;
}

int32_t pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mtx)
{
    if (false == g_stub_start)
    {
        return pthread_cond_timedwait_internal(cond, mtx, NULL_PNTR, false);
    }
    else
    {
START_TIMER
        uint32_t num_of_thread = (uint32_t)R_OSAL_RCFG_GetNumOfThread();
        uint32_t i;
        st_osal_thread_control_t *p_control;

        for (i = 0U; i < num_of_thread; i++)
        {
            (void)R_OSAL_RCFG_GetThreadInfo(i, &p_control);
            p_control->active = false;
        }
STOP_TIMER
    }
    return 0;
}
int32_t pthread_cond_timedwait(pthread_cond_t *restrict cond,
        pthread_mutex_t *restrict mtx,
        const struct timespec *restrict timeout)
{
    if (false == g_stub_start)
    {
        return pthread_cond_timedwait_internal(cond, mtx, timeout, true);
    }
    else
    {
        /* Do nothing */
    }
    return 0;
}
