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
[pthread.c] - POSIX Thread library API
****************************************************************************/
#include <errno.h>
#include <string.h>
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define NOT_USED_GLOBAL_HEAP_FREE
//#include <pt_api_impl.h>
#include <pt_api/pt.cfg>
#include <posix/process.cfg>
#include <mcos/mcos_hwcl.cfg>

//#include <pt_thread_if.h>

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

/* Blocking cause */
#define PTHREAD_BLK_NON           0x00000000U     /* Not Blocked              */
#define PTHREAD_BLK_WAITPID       0x00000001U     /* Wait by waitpid()        */
#define PTHREAD_BLK_JOIN          0x00000002U     /* Wait by pthread_join()   */
#define PTHREAD_BLK_SLEEP         0x00000003U     /* Wait by sleep            */
#define PTHREAD_BLK_SIGNAL        0x00000004U     /* Blocked by signal        */
#define PTHREAD_BLK_AIO           0x00000005U     /* Blocked by aio           */
#define PTHREAD_BLK_MQUEUE        0x00000006U     /* Blocked by mqueue        */
#define PTHREAD_BLK_PIPE          0x00000007U     /* Blocked by pipe          */
#define PTHREAD_BLK_SOCKET        0x00000008U     /* Blocked by socket        */
#define PTHREAD_BLK_BARRIER       0x00000009U     /* Blocked by barrier       */
#define PTHREAD_BLK_COND          0x0000000AU     /* Blocked by cond          */
#define PTHREAD_BLK_MUTEX         0x0000000BU     /* Blocked by mutex         */
#define PTHREAD_BLK_RWLOCK        0x0000000CU     /* Blocked by rwlock        */
#define PTHREAD_BLK_SEMAPHORE     0x0000000DU     /* Blocked by semaphore     */
#define PTHREAD_BLK_RAW_FUTEX     0x0000000EU     /* Blocked by raw futex     */
#define PTHREAD_BLK_FLOCK         0x0000000FU     /* Blocked by flock         */
#define PTHREAD_BLK_PFS           0x00000010U     /* Blocked by pfs           */
#define PTHREAD_BLK_DEV           0x00000011U     /* Blocked by device        */
#define PTHREAD_BLK_OTHER         0x0000001FU     /* Blocked by other factor  */

 /* Bitfield attr[0]
    priority    :  8  priority
    detached    :  1  detached
    policy      :  2  SCHED_FIFO, SCHED_RR etc
    scope       :  1  scope system (never used)
    userstack   :  1  user-supplied stack
    inherit     :  1  inheritsched
    stdmsg      :  1  standard message
    msginherit  :  1  message inherit
    lcid        : 16  LCID */
#define PTHREAD_ATTR_PRIO_MASK         0xffU
#define PTHREAD_ATTR_PRIO_SHIFT           0U
#define PTHREAD_ATTR_DETACHED_MASK        1U
#define PTHREAD_ATTR_DETACHED_SHIFT       8U
#define PTHREAD_ATTR_POLICY_MASK          3U
#define PTHREAD_ATTR_POLICY_SHIFT         9U
#define PTHREAD_ATTR_SCOPE_MASK           1U
#define PTHREAD_ATTR_SCOPE_SHIFT         11U
#define PTHREAD_ATTR_USERSTACK_MASK       1U
#define PTHREAD_ATTR_USERSTACK_SHIFT     12U
#define PTHREAD_ATTR_INHERIT_MASK         1U
#define PTHREAD_ATTR_INHERIT_SHIFT       13U
#define PTHREAD_ATTR_STDMSG_MASK          1U
#define PTHREAD_ATTR_STDMSG_SHIFT        14U
#define PTHREAD_ATTR_MSGINHERIT_MASK      1U
#define PTHREAD_ATTR_MSGINHERIT_SHIFT    15U
#define PTHREAD_ATTR_LCID_MASK       0xffffU
#define PTHREAD_ATTR_LCID_SHIFT          16U

#define PTHREAD_ATTR_INVALID   0

#define ATIX    0     /* Attribute bits */
#define SZIX    1     /* Stack size */

#if defined __LP64__
#define STACK_ALIGN     8
#else /* #if defined __LP64__ */
#define STACK_ALIGN     4
#endif /* #if defined __LP64__ */

#include <mcos/atomic.h>
#include "libpthread_local.h"
#include "stub.h"

static int32_t pthread_attr_valid(const pthread_attr_t* attr);
static void once_cleanup(void *arg);
static bool key_reserve(uintptr_t *ptr, uintptr_t func);
#if !MCOS_USE_PROCESS_MODEL
static void pthread_destroy_allspecific(pttcb_t *pttcb);
#endif

/* Alignment check */
#define CHECK_ALIGN(_x_, _a_)  ((uintptr_t)(_x_) & ((uintptr_t)(_a_) - 1))

static int32_t sem_timedwait_internal(sem_t* sem, const struct timespec* restrict abstmo,
        uint_fast8_t timed);
static void sem_cleanup(void *arg);

static int32_t pthread_attr_valid(const pthread_attr_t* attr)
{
    int32_t ret;
#ifdef MCOS_CFG_PT_CHECK_ARGUMENTS
    ret = 0;
    if (attr != NULL_PNTR)
    {
        uint32_t align = CHECK_ALIGN(attr, (uint32_t)sizeof(attr->addr));
        if (align == 0U)
        {
            ret = 1;
        }
    }
#else
    ret = 1;
#endif
    return ret;
}

/* pthread_attr_init() API */
int32_t pthread_attr_init(pthread_attr_t* attr)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (valid == 0)
    {
        ret = EINVAL;
    }
    else
    {
        attr->attr[ATIX] = ((uint32_t)((MCOS_CFG_PT_MAX_PRI - MCOS_CFG_PT_MIN_PRI + 1) / 2)
                            | ((uint32_t)MCOS_LCID_INHERIT << PTHREAD_ATTR_LCID_SHIFT)
                            | ((uint32_t)SCHED_FIFO << PTHREAD_ATTR_POLICY_SHIFT));

#if defined(__PIC__) && MCOS_USE_PROCESS_MODEL
            attr->attr[SZIX] = (uint32_t)MCOS_CFG_POSIX_PROCESS_DEFAULT_USR_STACKSIZE;
#else /* #if defined(__PIC__) && MCOS_USE_PROCESS_MODEL */

#if MCOS_USE_PROCESS_MODEL
            attr->attr[SZIX] = (uint32_t)MCOS_CFG_POSIX_PROCESS_DEFAULT_USR_STACKSIZE;
#else  /* #if MCOS_USE_PROCESS_MODEL */
            attr->attr[SZIX] = (uint32_t)MCOS_CFG_THR_DEFAULT_STACKSIZE;
#endif  /* #if MCOS_USE_PROCESS_MODEL */

#endif /* #if defined(__PIC__) && MCOS_USE_PROCESS_MODEL */

        attr->addr = NULL_PNTR;
        attr->name[0] = '\0';
        ret = 0;
    }
STOP_TIMER
    return ret;
}

/* pthread_attr_destroy() API */
int32_t pthread_attr_destroy(pthread_attr_t* attr)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (valid != 0)
    {
        attr->attr[ATIX] = (uint32_t)PTHREAD_ATTR_INVALID;
        attr->attr[SZIX] = 0U;
        attr->addr = NULL_PNTR;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

/* pthread_attr_getdetachstate() API */
int32_t pthread_attr_getdetachstate(const pthread_attr_t* attr, int32_t* detach)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (detach != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(detach, (uint32_t)sizeof(*detach));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *detach = (((attr->attr[ATIX]
                     & ((uint32_t)PTHREAD_ATTR_DETACHED_MASK
                        << PTHREAD_ATTR_DETACHED_SHIFT)) != 0U)
                   ? (int32_t)PTHREAD_CREATE_DETACHED
                   : (int32_t)PTHREAD_CREATE_JOINABLE);
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_attr_setdetachstate() API */
int32_t pthread_attr_setdetachstate(pthread_attr_t* attr, int32_t detach)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (   (   (detach == (int32_t)PTHREAD_CREATE_JOINABLE)
            || (detach == (int32_t)PTHREAD_CREATE_DETACHED) )
        && (valid != 0))
    {
        attr->attr[ATIX] = ((attr->attr[ATIX]
                             & ~(uint32_t)((uint32_t)PTHREAD_ATTR_DETACHED_MASK
                                 << PTHREAD_ATTR_DETACHED_SHIFT))
                            | ((detach == (int32_t)PTHREAD_CREATE_DETACHED)
                               ? ((uint32_t)PTHREAD_ATTR_DETACHED_MASK
                                  << PTHREAD_ATTR_DETACHED_SHIFT)
                               : 0U));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_attr_getinheritsched() API */
int32_t pthread_attr_getinheritsched(const pthread_attr_t *restrict attr,
                                     int32_t *restrict inherit)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (inherit != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(inherit, (uint32_t)sizeof(*inherit));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *inherit = (((attr->attr[ATIX]
                      & ((uint32_t)PTHREAD_ATTR_INHERIT_MASK
                         << PTHREAD_ATTR_INHERIT_SHIFT)) != 0U)
                    ? (int32_t)PTHREAD_INHERIT_SCHED
                    : (int32_t)PTHREAD_EXPLICIT_SCHED);
    }

    return ret;
}

/* pthread_attr_setinheritsched() API */
int32_t pthread_attr_setinheritsched(pthread_attr_t* attr, int32_t inherit)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (((inherit == (int32_t)PTHREAD_EXPLICIT_SCHED)
         || (inherit == (int32_t)PTHREAD_INHERIT_SCHED))
        && (valid != 0))
    {
        attr->attr[ATIX] = ((attr->attr[ATIX]
                             & (~(uint32_t)((uint32_t)PTHREAD_ATTR_INHERIT_MASK
                                 << PTHREAD_ATTR_INHERIT_SHIFT)))
                            | ((inherit == (int32_t)PTHREAD_INHERIT_SCHED)
                               ? ((uint32_t)PTHREAD_ATTR_INHERIT_MASK
                                  << PTHREAD_ATTR_INHERIT_SHIFT)
                               : 0U));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

int32_t pthread_attr_getschedparam(const pthread_attr_t *restrict attr,
                               struct sched_param *restrict param)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (param != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(param, (uint32_t)sizeof(param->sched_priority));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        /* "PTHREAD_ATTR_PRIO_SHIFT" is 0, so shift operation is not required. */
        uint32_t masked_value = attr->attr[ATIX] & PTHREAD_ATTR_PRIO_MASK;
        param->sched_priority = (int32_t)masked_value;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
    return ret;
}

/* pthread_attr_setschedparam() API */
int32_t pthread_attr_setschedparam(pthread_attr_t *restrict attr,
                               const struct sched_param *restrict param)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (param != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(param, (uint32_t)sizeof(param->sched_priority));
            if (align == 0U)
            {
#else
        {
            {
#endif
                if(   (param->sched_priority >= MCOS_CFG_PT_MIN_PRI)
                   && (param->sched_priority <= MCOS_CFG_PT_MAX_PRI))
                {
                    ret = 0;
                }
            }
        }
    }

    if (ret != EINVAL)
    {
        /* "PTHREAD_ATTR_PRIO_SHIFT" is 0, so shift operation is not required. */
        attr->attr[ATIX] = ((attr->attr[ATIX]
                             & ~(uint32_t)(PTHREAD_ATTR_PRIO_MASK)) | (uint32_t)(param->sched_priority));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

/* pthread_attr_getschedpolicy() API */
int32_t pthread_attr_getschedpolicy(const pthread_attr_t *restrict attr,
                                    int32_t *restrict policy)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (policy != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(policy, (uint32_t)sizeof(*policy));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        uint32_t masked_value = (attr->attr[ATIX] >> PTHREAD_ATTR_POLICY_SHIFT) & PTHREAD_ATTR_POLICY_MASK;
        *policy = (int32_t)masked_value;
    }
    return ret;
}

/* pthread_attr_setschedpolicy() API */
int32_t pthread_attr_setschedpolicy(pthread_attr_t* attr, int32_t policy)
{
START_TIMER
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (((policy == (int32_t)SCHED_FIFO)
         || (policy == (int32_t)SCHED_RR)
         || (policy == (int32_t)SCHED_SPORADIC)
         || (policy == (int32_t)SCHED_OTHER))
        && (valid != 0) )
    {
        if ((policy == (int32_t)SCHED_SPORADIC)
            || (policy == (int32_t)SCHED_OTHER))
        {
            ret =  ENOTSUP;
        }
        else
        {
            attr->attr[ATIX] = ((attr->attr[ATIX]
                                & ~(uint32_t)((uint32_t)PTHREAD_ATTR_POLICY_MASK
                                    << PTHREAD_ATTR_POLICY_SHIFT))
                                | ((uint32_t)policy << PTHREAD_ATTR_POLICY_SHIFT));
            ret = 0;
        }
    }
    else
    {
        ret = EINVAL;
    }
STOP_TIMER
    return ret;
}

/* pthread_attr_getscope() API */
int32_t pthread_attr_getscope(const pthread_attr_t *restrict attr,
                              int32_t *restrict scope)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (scope != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(scope, (uint32_t)sizeof(*scope));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *scope = (int32_t)PTHREAD_SCOPE_SYSTEM;
    }
    return ret;
}

/* pthread_attr_setscope() API */
int32_t pthread_attr_setscope(pthread_attr_t* attr, int32_t scope)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (((scope == (int32_t)PTHREAD_SCOPE_SYSTEM)
         || (scope == (int32_t)PTHREAD_SCOPE_PROCESS))
        && (valid != 0) )
    {
     /* Process scope scheduling not supported */
        if (scope == (int32_t)PTHREAD_SCOPE_PROCESS)
        {
            ret = ENOTSUP;
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_attr_getstacksize() API */
int32_t pthread_attr_getstacksize(const pthread_attr_t *restrict attr,
                              size_t *restrict ssize)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (ssize != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(ssize, (uint32_t)sizeof(*ssize));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *ssize = (size_t)attr->attr[SZIX];
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_attr_setstacksize() API */
int32_t pthread_attr_setstacksize(pthread_attr_t* attr, size_t ssize)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        uint32_t align = CHECK_ALIGN((const void *)ssize, (uint32_t)STACK_ALIGN);
        if (align == 0U)
#endif
        {
            if (   (ssize >= (size_t)MCOS_CFG_PT_SSIZE_MIN)
                && (ssize <= (size_t)MCOS_CFG_PT_SSIZE_MAX))
            {
                ret = 0;
            }
        }
    }

    if (ret != EINVAL)
    {
        attr->attr[SZIX] = (uint32_t)ssize;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_attr_getstackaddr() API */
int32_t pthread_attr_getstackaddr(const pthread_attr_t *restrict attr,
                              void **restrict stack)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (stack != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(stack, (uint32_t)sizeof(*stack));
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *stack = attr->addr;
    }
    return ret;
}

/* pthread_attr_setstackaddr() API */
int32_t pthread_attr_setstackaddr(pthread_attr_t* attr, void* stack)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (stack != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(stack, (uint32_t)STACK_ALIGN);
            if (align == 0U)
            {
                ret = 0;
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        attr->attr[ATIX] |= (uint32_t)PTHREAD_ATTR_USERSTACK_MASK << PTHREAD_ATTR_USERSTACK_SHIFT;
        attr->addr = stack;
    }
    return ret;
}

/* pthread_attr_getstack() API */
int32_t pthread_attr_getstack(const pthread_attr_t *restrict attr,
                          void **restrict stack, size_t *restrict ssize)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (stack != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(stack, (uint32_t)sizeof(*stack));
            if (align == 0U)
            {
                if (ssize != NULL_PNTR)
                {
                    align = CHECK_ALIGN(ssize, (uint32_t)sizeof(*ssize));
                    if (align == 0U)
                    {
                        ret = 0;
                    }
                }
            }
        }
#else
        ret = 0;
#endif
    }

    if (ret != EINVAL)
    {
        *ssize = (size_t)attr->attr[SZIX];
        *stack = attr->addr;
    }
    return ret;
}

/* pthread_attr_setstack() API */
int32_t pthread_attr_setstack(pthread_attr_t* attr, void* stack, size_t ssize)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);

    ret = EINVAL;
    if (valid != 0)
    {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (stack != NULL_PNTR)
        {
            uint32_t align = CHECK_ALIGN(stack, (uint32_t)STACK_ALIGN);
            if (align == 0U)
            {
                align = CHECK_ALIGN((const void *)ssize, (uint32_t)STACK_ALIGN);
                if (align == 0U)
#else
        {
            {
#endif
                {
                    if (   (ssize >= (size_t)MCOS_CFG_PT_SSIZE_MIN)
                        && (ssize <= (size_t)MCOS_CFG_PT_SSIZE_MAX))
                    {
                        ret = 0;
                    }
                }
            }
        }
    }

    if (ret != EINVAL)
    {
        attr->attr[ATIX] |= (uint32_t)PTHREAD_ATTR_USERSTACK_MASK << PTHREAD_ATTR_USERSTACK_SHIFT;
        attr->attr[SZIX] = (uint32_t)ssize;
        attr->addr = stack;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* Not supported: pthread_attr_getguardsize() API */
int32_t pthread_attr_getguardsize(const pthread_attr_t *restrict attr,
                              size_t *restrict guard)
{
    return ENOTSUP;
}

/* Not supported: pthread_attr_setguardsize() */
int32_t pthread_attr_setguardsize(pthread_attr_t* attr, size_t guard)
{
    return ENOTSUP;
}

/* non-portable */
int32_t pthread_attr_getlcid_np(const pthread_attr_t *restrict attr,
                                int32_t *restrict lcid)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);
#if MCOS_CFG_PT_CHECK_ARGUMENTS
    uint32_t align = CHECK_ALIGN(lcid, (uint32_t)sizeof(*lcid));
#endif
    if (   (valid != 0)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        && (lcid != NULL_PNTR)
        && (align == 0U)
#endif
    )
    {
        uint16_t lcid_attr = (uint16_t)((uint32_t)attr->attr[ATIX] >> (uint32_t)PTHREAD_ATTR_LCID_SHIFT);
        if (lcid_attr <= (uint16_t)SHRT_MAX)
        {
            *lcid = (int32_t)lcid_attr;
        }
        else
        {
            uint16_t lcid_tmp = (uint16_t)~(uint32_t)(lcid_attr) + (uint16_t)1;
            *lcid = (int32_t)lcid_tmp * (int32_t)(-1);
        }
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

#define MCOS_BASE_LCID  ((((int32_t)MCOS_CFG_HWCL_ID - (int32_t)1) * (int32_t)MCOS_CFG_GLB_CORE_PER_HWCL_MAX) + (int32_t)1)

/*---------------------------------------------------------------------------
  mcos_is_validlcid
     Specified logical core id is valid or not (on the current HW cluster only)
  Input     :   lcid    logical core id
  Output    :   none
  Return    :   0       is not valid
            :   !0      is valid
  Note  :   none
 ---------------------------------------------------------------------------*/
static inline mcos_bool_t mcos_is_validlcid(
    const mcos_id_t lcid)
{
    int32_t s_lcid32 = (int32_t)lcid;

    return (((s_lcid32 >= (int32_t)MCOS_BASE_LCID)
        && (s_lcid32 < ((int32_t)MCOS_BASE_LCID + (int32_t)MP_CONTROL_CORE_COUNT))) ? mcos_true : mcos_false);
}

/* non-portable */
int32_t pthread_attr_setlcid_np(pthread_attr_t* attr, int32_t lcid)
{
    int32_t ret = EINVAL;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (valid != 0)
    {
        mcos_bool_t temp =  mcos_is_validlcid((mcos_id_t)lcid);
        if((lcid == MCOS_LCID_ANY)
            || (lcid == MCOS_LCID_SELF)
            || (lcid == MCOS_LCID_INHERIT)
            || (temp != mcos_false))
        {
            uint32_t lcid_temp;
            lcid_temp = (uint32_t)lcid;
            attr->attr[ATIX] = ((attr->attr[ATIX] &
                             ~(0xffffffffU << PTHREAD_ATTR_LCID_SHIFT))
                            | ((uint32_t)lcid_temp << PTHREAD_ATTR_LCID_SHIFT));
            ret = 0;
        }
    }

    return ret;
}

/* non-portable */
int32_t pthread_attr_getname_np(const pthread_attr_t* attr, char* name)
{
    int32_t ret;
    size_t  len;
    int32_t valid = pthread_attr_valid(attr);
    if (    (valid != 0)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
         && (name != NULL_PNTR)
#endif
    )
    {
        len = sizeof(attr->name);
        (void)strncpy(name, attr->name, len);
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* non-portable */
int32_t pthread_attr_setname_np(pthread_attr_t* attr, const char* name)
{
    int32_t ret = EINVAL;
    size_t  len;
    int32_t valid = pthread_attr_valid((const pthread_attr_t*)attr);
    if (valid != 0)
    {
        if (name != NULL_PNTR)
        {
            len = strlen(name);
            if (len < sizeof(attr->name))
            {
                (void)strncpy(attr->name, name, len+(size_t)1);
                ret = 0;
            }
        }
    }

    return ret;
}

/* non-portable */
int32_t pthread_attr_getstdmsg_np(const pthread_attr_t *restrict attr, mcos_bool_t *restrict stdmsg)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);
    if (   (valid != 0)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        && (stdmsg != NULL_PNTR)
#endif
    )
    {
        *stdmsg = (((attr->attr[ATIX] >> PTHREAD_ATTR_STDMSG_SHIFT) & PTHREAD_ATTR_STDMSG_MASK) != 0U) ? mcos_true : mcos_false;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* non-portable */
int32_t pthread_attr_setstdmsg_np( pthread_attr_t *attr, mcos_bool_t stdmsg)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);
    if (   (valid != 0)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        && ((stdmsg == mcos_true) || (stdmsg == mcos_false))
#endif
    )
    {
        uint32_t flag;

        flag = (stdmsg == mcos_false) ? 0U : 1U;
        attr->attr[ATIX] = ((attr->attr[ATIX]
                             & ~(uint32_t)((uint32_t)PTHREAD_ATTR_STDMSG_MASK
                                 << PTHREAD_ATTR_STDMSG_SHIFT))
                            | (flag << PTHREAD_ATTR_STDMSG_SHIFT));

        ret =  0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* non-portable */
int32_t pthread_attr_getpriorityinheritance_np(const pthread_attr_t *restrict attr, mcos_bool_t *restrict inherit)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);
    if (   (valid != 0)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        && (inherit != NULL_PNTR)
#endif
    )
    {
        *inherit = (((attr->attr[ATIX] >> PTHREAD_ATTR_MSGINHERIT_SHIFT) & PTHREAD_ATTR_MSGINHERIT_MASK) != 0U) ? mcos_true : mcos_false;
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* non-portable */
int32_t pthread_attr_setpriorityinheritance_np( pthread_attr_t *attr, mcos_bool_t inherit)
{
    int32_t ret;
    int32_t valid = pthread_attr_valid(attr);
    if (   (valid != 0)
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        && ((inherit == mcos_true) || (inherit == mcos_false))
#endif
    )
    {
        uint32_t flag;

        flag = (inherit == mcos_false) ? 0U : 1U;
        attr->attr[ATIX] = ((attr->attr[ATIX]
                             & ~(uint32_t)((uint32_t)PTHREAD_ATTR_MSGINHERIT_MASK
                                 << PTHREAD_ATTR_MSGINHERIT_SHIFT))
                            | (flag << PTHREAD_ATTR_MSGINHERIT_SHIFT));
        ret = 0;
    }
    else
    {
        ret = EINVAL;
    }

    return ret;
}

/* pthread_testcancel() API */
void pthread_testcancel(void)
{
 /* Get own pttcb */
    pttcb_t* pttcb = pt_get_pttcb();
    if (pttcb != NULL_PNTR)
    {
     /* Check cancel enabled and pending */
        if ((pttcb->flags & (PTHREAD_CANCELOK
                             | PTHREAD_CANENAB
                             | PTHREAD_CANPEND))
            == (PTHREAD_CANCELOK
                | PTHREAD_CANENAB
                | PTHREAD_CANPEND))
        {
            pthread_exit(PTHREAD_CANCELED);
        }
    }
}

/* pthread_setcancelstate() API */
int32_t pthread_setcancelstate(int32_t state, int32_t* old)
{
    int32_t ret;
 /* Check argument */
#if MCOS_CFG_PT_CHECK_ARGUMENTS
    uint32_t align = CHECK_ALIGN(old, (uint32_t)sizeof(*old));
    if (   (align != 0U)
        || (   (state != (int32_t)PTHREAD_CANCEL_ENABLE)
            && (state != (int32_t)PTHREAD_CANCEL_DISABLE)))
#else
    if (   (state != (int32_t)PTHREAD_CANCEL_ENABLE)
        && (state != (int32_t)PTHREAD_CANCEL_DISABLE))
#endif
    {
        ret = EINVAL;
    }
    else
    {
    /* Get own pttcb */
        pttcb_t* pttcb = pt_get_pttcb();
        if (pttcb == NULL_PNTR)
        {
            ret = EPERM;
        }
        else
        {
        /* Get current state */
            int32_t cur = (((pttcb->flags & (uint16_t)PTHREAD_CANENAB) != (uint16_t)0)
                    ? (int32_t)PTHREAD_CANCEL_ENABLE
                    : (int32_t)PTHREAD_CANCEL_DISABLE);

        /* If must change state */
            if (cur != state)
            {
                if (state == (int32_t)PTHREAD_CANCEL_ENABLE)
                {
                    (void)atomic_fetch_or(&pttcb->flags, (uintptr_t)PTHREAD_CANENAB);
                }
                else
                {
                    uint32_t reversed_value = ~(uint32_t)(PTHREAD_CANENAB);
                    (void)atomic_fetch_and((uint32_t*)&pttcb->flags, (uintptr_t)reversed_value);
                }
            }
        /* Return old state and success */
            if (old != NULL_PNTR)
            {
                *old = cur;
            }
            ret = 0;
        }
    }

    return ret;
}

/* pthread_setcanceltype() API */
int32_t pthread_setcanceltype(int32_t type, int32_t* old)
{
    int32_t ret;
 /* Check argument */
#if MCOS_CFG_PT_CHECK_ARGUMENTS
    uint32_t align = CHECK_ALIGN(old, (uint32_t)sizeof(*old));
    if (   (align != 0U)
        || (   (type != (int32_t)PTHREAD_CANCEL_DEFERRED)
            && (type != (int32_t)PTHREAD_CANCEL_ASYNCHRONOUS)))
#else
    if (       (type != (int32_t)PTHREAD_CANCEL_DEFERRED)
            && (type != (int32_t)PTHREAD_CANCEL_ASYNCHRONOUS))
#endif
    {
        ret = EINVAL;
    }
    else
    {
    /* Get own pttcb */
        pttcb_t* pttcb = pt_get_pttcb();
        if (pttcb == NULL_PNTR)
        {
            ret = EPERM;
        }
        else
        {
        /* Get current type */
            int32_t cur = (((pttcb->flags & (uint16_t)PTHREAD_CANASYNC) != (uint16_t)0)
                    ? (int32_t)PTHREAD_CANCEL_ASYNCHRONOUS
                    : (int32_t)PTHREAD_CANCEL_DEFERRED);

        /* If must change type */
            if (cur != type)
            {
                if (type == (int32_t)PTHREAD_CANCEL_ASYNCHRONOUS)
                {
                    (void)atomic_fetch_or(&pttcb->flags, (uintptr_t)PTHREAD_CANASYNC);
                }
                else
                {
                    uint32_t reversed_value = ~(PTHREAD_CANASYNC);
                    (void)atomic_fetch_and((uint32_t*)&pttcb->flags, (uintptr_t)reversed_value);
                }
            }
        /* Return old type and success */
            if (old != NULL_PNTR)
            {
                *old = cur;
            }
            ret = 0;
        }
    }

    return ret;
}

void _pthread_cleanup_push(struct _pthread_cleanup* data, void (*func)(void *arg), void* para)
{
    pttcb_t* pttcb = pt_get_pttcb();

    if (pttcb != NULL_PNTR)
    {
        data->_func_ = func;
        data->_para_ = para;
        data->_prev_ = pttcb->cleanup;
        atomic_thread_fence(memory_order_seq_cst);
        pttcb->cleanup = data;
    }
}

void _pthread_cleanup_pop(struct _pthread_cleanup* data, int32_t exec)
{
    pttcb_t* pttcb = pt_get_pttcb();

    if (pttcb != NULL_PNTR)
    {
        pttcb->cleanup = data->_prev_;
        if ((exec != 0) && (data->_func_ != NULL_PNTR))
        {
            (*data->_func_)(data->_para_);
        }
    }
}

/* sched_get_priority_max() API */
int32_t sched_get_priority_max(int32_t policy)
{
    int32_t ret;
    int32_t status;

    ret = -1;
    status = EINVAL;
    if (   (policy == (int32_t)SCHED_FIFO)
        || (policy == (int32_t)SCHED_RR)
        || (policy == (int32_t)SCHED_SPORADIC)
        || (policy == (int32_t)SCHED_OTHER) )
    {
        if (policy != (int32_t)SCHED_SPORADIC)
        {
            ret = MCOS_CFG_PT_MAX_PRI;
        }
        else
        {
            status = ENOTSUP;
        }
    }

    if (ret == -1)
    {
        errno = status;
    }
    return ret;
}

/* sched_get_priority_min() API */
int32_t sched_get_priority_min(int32_t policy)
{
    int32_t ret;
    int32_t status;

    ret = -1;
    status = EINVAL;
    if (   (policy == (int32_t)SCHED_FIFO)
        || (policy == (int32_t)SCHED_RR)
        || (policy == (int32_t)SCHED_SPORADIC)
        || (policy == (int32_t)SCHED_OTHER) )
    {
        if (policy != (int32_t)SCHED_SPORADIC)
        {
            ret = MCOS_CFG_PT_MIN_PRI;
        }
        else
        {
            status = ENOTSUP;
        }
    }

    if (ret == -1)
    {
        errno = status;
    }
    return ret;
}

#include <stdbool.h>
#include <stdint.h>
#include <mcos/atomic.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <sys/mfutex.h>

    #if 0
/* Alignment check */
#define CHECK_ALIGN(_x_, _a_)  ((uintptr_t)(_x_) & ((uintptr_t)(_a_) - 1))
    #endif

#define ONCE_INIT_NOTSTARTED  0U
#define ONCE_INIT_ONGOING     1U
#define ONCE_INIT_DONE        2U

static void once_cleanup(void *arg)
{
    pthread_once_t *once = (pthread_once_t *)arg;
    if (once != NULL_PNTR) {
        /* Reset once to a state as if pthread_once was never called.
         * This will allow someone else to retry the once.
         */
        once->lock.val = ONCE_INIT_NOTSTARTED;
        (void)mcos_mfutex_wake(&once->lock,
            (uint32_t)(MCOS_MFUTEX_ORDERING_NONE|MCOS_MFUTEX_WAKE_ALL));
    }
}

/* One time library initialization */
int32_t pthread_once(pthread_once_t *once, void (*func)(void))
{
    int32_t ret = 0;

    if ((once == NULL_PNTR) || (func == NULL_PNTR))
    {
        ret = EINVAL;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(once, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            ret = EINVAL;
        }
    }

    if (ret != EINVAL)
    {
        uintptr_t val;
        _Bool comp;
        do {
            val = atomic_load(&once->lock.val);
            switch (val) {
            case ONCE_INIT_ONGOING:
                (void)mcos_mfutex_wait(&once->lock, val,
                        (uint32_t)MCOS_MFUTEX_ORDERING_NONE,
                        NULL_PNTR);
                val = atomic_load(&once->lock.val);
                break;
            case ONCE_INIT_NOTSTARTED:
                comp = atomic_compare_exchange_weak(&once->lock.val, &val,
                        ONCE_INIT_ONGOING);
                if (comp != false)
                {
                    pthread_cleanup_push(&once_cleanup, once);
                    func();
                    pthread_cleanup_pop(0);

                    val = atomic_load(&once->lock.val);
                    if (val == (uintptr_t)ONCE_INIT_ONGOING) {
                        (void)atomic_exchange(&once->lock.val, (uintptr_t)ONCE_INIT_DONE);
                        (void)mcos_mfutex_wake(&once->lock,
                                (uint32_t)(MCOS_MFUTEX_ORDERING_NONE|MCOS_MFUTEX_WAKE_ALL));
                    }
                    /* Either we successfully initialized, or func was cancelled
                     * In both cases, return successfully.
                     */
                    val = ONCE_INIT_DONE;
                }
                break;
            case ONCE_INIT_DONE:
            default:
                break;
            }
        } while (val != (uintptr_t)ONCE_INIT_DONE);
    }
    return ret;
}

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <mcos/atomic.h>
//#include <pt_api_impl.h>

/* This defines the number of key-value allocated in a group - power of two */
#define BLOCK_SIZE 4U
/* Initialized value for func that represents no optional destructor.
 * This is used to note that the entry is currently in use, but no func set.
 */
#define NOFUNC 1ULL

/* Used for keys in the following static global list, and in per-thread values*/
struct keyblock {
    struct keyblock *next;
    atomic_uintptr_t ptr[BLOCK_SIZE];
};

struct perthread_val {
    struct keyblock *head;

};

static struct keyblock *ptkey_head;
static int32_t ptkey_count;

static bool key_reserve(uintptr_t *ptr, uintptr_t func)
{
    uintptr_t prevnull = (uintptr_t)0;
    uintptr_t newval = func;
    if (func == (uintptr_t)0)
    {
        newval = (uintptr_t)NOFUNC;
    }
    return atomic_compare_exchange_strong(ptr, &prevnull, newval);
}

int32_t pthread_key_create(pthread_key_t* keyp, void (*func)(void *arg))
{
START_TIMER
    int32_t ret = 0;
    struct keyblock  *block;
    struct keyblock **pblock;
    pthread_key_t idx;
    uint_fast8_t lidx;
    bool found;
    if (keyp == NULL_PNTR)
    {
        ret = EINVAL;
    }
    else
    {
        /* find an unused entry */
        pblock = &ptkey_head;
        block = pblock[0];
        found = false;
        idx = 0U;
        while (block != NULL_PNTR)
        {
            for (lidx = 0U; lidx < BLOCK_SIZE; lidx++)
            {
                if (block->ptr[lidx] == (uintptr_t)NULL)
                {
                    /* found a free entry, try to reserve it */
                    bool status = key_reserve(&block->ptr[lidx], (uintptr_t)func);
                    if (status != false)
                    {
                        found = true;
                        keyp[0] = idx + (pthread_key_t)lidx;
                        break;
                    }
                }
            }
            if (found != false)
            {
                break;
            }
            else
            {
                if (block->next == NULL_PNTR)
                {
                    block = NULL_PNTR;
                }
                else
                {
                    pblock = &block->next;
                    block = block->next;
                    idx += (pthread_key_t)BLOCK_SIZE;
                }
            }
        }
        /* If we need to allocate a new key block */
        if (found == false)
        {
            /* limit at key index overflow, currently UINT16_MAX */
            if (((uint32_t)ptkey_count + BLOCK_SIZE) < PTHREAD_KEYS_MAX)
            {
                block = calloc((size_t)1, sizeof(struct keyblock));
            }
            if (block == NULL_PNTR)
            {
                ret = ENOMEM;
            }
            else
            {
                struct keyblock *oldval = NULL_PNTR;
                bool comp;
                (void)key_reserve(&block->ptr[0], (uintptr_t)func);
                do
                {
                    comp = atomic_compare_exchange_strong((uintptr_t *)pblock, (uintptr_t *)&oldval, (uintptr_t)block);
                    if (comp == false)
                    {
                        /* atomic addition of the block failed. This can only mean
                        * someone else enqueued before us. We still can append this
                        * block, but need to add it at the end.
                        */
                        oldval = NULL_PNTR;
                        pblock = &pblock[0]->next;
                        idx += (pthread_key_t)BLOCK_SIZE;
                    }
                } while (comp == false);
                (void)atomic_fetch_add(&ptkey_count, (uintptr_t)BLOCK_SIZE);
                keyp[0] = idx;
            }
        }
    }
STOP_TIMER
    return ret;
}

int32_t pthread_key_delete(pthread_key_t key)
{
START_TIMER
    int32_t ret = 0;
    struct keyblock *block;
    pthread_key_t idx;
    pthread_key_t lidx;

 /* Check key argument */
    if (key >= (pthread_key_t)ptkey_count)
    {
        ret = EINVAL;
    }
    else
    {
        uint_fast16_t i;
        block = ptkey_head;
        idx = key / (pthread_key_t)BLOCK_SIZE;
        lidx = key - (idx * (pthread_key_t)BLOCK_SIZE);
        for (i = (uint_fast16_t)0; i < idx; i++)
        {
            block = block->next;
        }
        if (block->ptr[lidx] == (uintptr_t)NULL)
        {
            ret = EINVAL;
        }
        else
        {
            /* set key as uninitialized and reusable */
            atomic_store(&block->ptr[lidx], 0U);      /* TOBEFIXED: #9, #10 */
        }
    }
STOP_TIMER
    return ret;
}

void *pthread_getspecific(pthread_key_t key)
{
    void *ret = NULL_PNTR;

    if (false == g_stub_start)
    {
        struct keyblock *block;
        pthread_key_t idx;
        pthread_key_t lidx;
        uint_fast16_t i;
        pttcb_t *pttcb = pt_get_pttcb();
        /* Only check that the key is within bounds, not that the key is
         * currently valid. This is faster and explicitly undefined by POSIX.
         */
        if ((pttcb != NULL_PNTR)
                && (key < (pthread_key_t)ptkey_count))
        {
            block = pttcb->keys;
            idx = key / (pthread_key_t)BLOCK_SIZE;
            lidx = key - (idx * (pthread_key_t)BLOCK_SIZE);
            /* blocks are lazily allocated during setspecific.
             * There may be less value blocks than key blocks.
             */
            for (i = 0U; (block != NULL_PNTR) && (i < idx); i++)
            {
                block = block->next;
            }
            if ((block != NULL_PNTR) && (i == idx))
            {
                ret = (void*)block->ptr[lidx];
            }
        }
    }
    else
    {
        /* do nothing */
    }

    return ret;
}

int32_t pthread_setspecific(pthread_key_t key, const void *value)
{
    int32_t ret = 0;
    struct keyblock **pblock;
    pthread_key_t idx;
    pthread_key_t lidx;
    pttcb_t *pttcb = pt_get_pttcb();
    if ((key >= (pthread_key_t)ptkey_count) || (pttcb == NULL_PNTR))        /* TOBEFIXED: #9 */
    {
        ret = EINVAL;
    }
    else
    {
        pblock = (struct keyblock**)&pttcb->keys;
        idx = key / (pthread_key_t)BLOCK_SIZE;
        lidx = key - (idx * (pthread_key_t)BLOCK_SIZE);

        /* if we are trying to access a key value for which no block is defined,
         * allocate the missing ones.
         */
        if (pblock[0] == NULL_PNTR)
        {
            pblock[0] = (struct keyblock*)calloc((size_t)1, sizeof(struct keyblock));
            if (pblock[0] == NULL_PNTR)
            {
                ret = ENOMEM;
            }
        }

        if (ret == 0)
        {
            while (idx > (pthread_key_t)0)
            {
                if (pblock[0]->next == NULL_PNTR)
                {
                    pblock[0]->next = (struct keyblock*)calloc((size_t)1, sizeof(struct keyblock));
                    if (pblock[0]->next == NULL_PNTR)
                    {
                        ret = ENOMEM;
                        break;
                    }
                }
                /* iterate enough existing blocks */
                pblock = &pblock[0]->next;
                idx--;
            }
        }

        if (ret == 0)
        {
            pblock[0]->ptr[lidx] = (uintptr_t)value;
        }
    }
    return ret;
}

#if !MCOS_USE_PROCESS_MODEL
static
#endif
void pthread_destroy_allspecific(pttcb_t *pttcb)
{
    struct keyblock *block;
    struct keyblock *oblock;
    struct keyblock *kblock;
    void (*func)(void *arg);
    void *arg;
    pthread_key_t count;
    uint_fast8_t attempts_left = PTHREAD_DESTRUCTOR_ITERATIONS;
    /* first call destructors */
    do {
        uint_fast16_t i;
        count = 0U;
        block = pttcb->keys;
        kblock = ptkey_head;
        while ((kblock != NULL_PNTR) && (block != NULL_PNTR))
        {
            for (i = 0U; i < BLOCK_SIZE; i++)
            {
                func = (void(*)(void *arg))kblock->ptr[i];
                if ((func != NULL_PNTR) && ((uintptr_t)func != (uintptr_t)NOFUNC))
                {
                    arg = (void*)block->ptr[i];
                    /* zero the specific value before the destructor call */
                    block->ptr[i] = (uintptr_t)0;
                    if (arg != NULL_PNTR)
                    {
                        count++;
                        func(arg);
                    }
                }
            }
            block = block->next;
            kblock = kblock->next;
        }
        attempts_left--;
    } while ((count > (pthread_key_t)0) && (attempts_left > 0U));

    /* now free the chain */
    block = pttcb->keys;
    while (block != NULL_PNTR)
    {
        oblock = block;
        block = block->next;
        free(oblock);
    }

    pttcb->keys = NULL_PNTR;
}

#if MCOS_USE_PROCESS_MODEL
/* Default handler for signal of thread termination */
void pt_thread_abort_entry(void *value_ptr)
{
    pthread_exit(value_ptr);
}
#endif /* #if MCOS_USE_PROCESS_MODEL */

/* Library implementation for pthread_exit() */
void pt_thread_cleanup(pttcb_t *pttcb)
{
    struct _pthread_cleanup* clup;

    if (pttcb != NULL_PNTR)
    {
     /* Disable cancellation and signals */
         uint32_t reversed_value = (uint32_t)~(uint32_t)(PTHREAD_CANCELOK | PTHREAD_CANENAB | PTHREAD_CANASYNC);
        (void)atomic_fetch_and((uint32_t *)&pttcb->flags, (uintptr_t)reversed_value);
        pttcb->sigpend = 0U;
        pttcb->sigenab = 0U;

     /* Pop and call cleanup handlers */
        do
        {
            clup = pttcb->cleanup;
            if ( clup != NULL_PNTR)
            {
                /* Allow handler to install cleanup itself */
                pttcb->cleanup = clup->_prev_;

                /* Execute handler */
                (clup->_func_)(clup->_para_);
            }
        } while (clup != NULL_PNTR);

     /* Execute key destructors */
        pthread_destroy_allspecific(pttcb);

     /* Mark robust mutex as inconsistent and unlink owned mutexes from thread chain */
        while (pttcb->ownmutex != NULL_PNTR)
        {
            pthread_mutex_t *mutex = pttcb->ownmutex;
            mutex_unlink(pttcb->ownmutex, pttcb);
            pthread_mutex_inconsistent(mutex);
        }
    }
}

/* Check cancel/signal */
int32_t pt_check_interrupt(pttcb_t* pttcb)
{
    return (( ((pttcb->flags
                & (PTHREAD_CANCELOK | PTHREAD_CANENAB | PTHREAD_CANPEND))
               == (PTHREAD_CANCELOK | PTHREAD_CANENAB | PTHREAD_CANPEND))
              || ((pttcb->sigenab & pttcb->sigpend) != 0U) )
            ? (int32_t)EINTR : (int32_t)0);
}

/* Convert absolute timespec to MCOS absolute time */
mcos_systime_t* pt_abstime_convert(const struct timespec* abstime, mcos_systime_t* systime)
{
    int64_t absval = (((int64_t)abstime->tv_sec * 1000L)
                      + (abstime->tv_nsec / 1000000L)
                      + ((abstime->tv_nsec % 1000000L) != (int64_t)0 ? (int64_t)1 : (int64_t)0));
    uint64_t absval_upper = (uint64_t)absval >> 32U;
    systime->upper = (int32_t)absval_upper;
    systime->lower = (uint32_t)absval;
    return systime;
}

#include <stdint.h>
#include <mcos/mcos.h>
#include <mcos/atomic.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mfutex.h>
//#include <pt_api_impl.h>

static uintptr_t sem_dec(sem_t *sem, _Bool waiter, int32_t *err);
static mcos_erid_t mfutex_wake_none(lockobj_t *lo);
static mcos_erid_t mfutex_wake_priority(lockobj_t *lo);

/*
 * Taken from posix_internal.h
 * Bitfield sem value structure
 * value    : 16    unsigned sem value
 * waiters  :  8    unsigned waiter count
 * named    :  8    named if nonzero
 */
typedef union semlock {
    uintptr_t val;
    struct semimpl {
        uint16_t val;
        uint8_t waiters;
        uint8_t nsem;
    } sem;
} semimpl_t;

#define SEM_INVALID_VALUE (~0ULL)

int32_t nsem_init(sem_t *sem, int32_t pshared, int32_t named, uint32_t value)
{
    int32_t ret = 0;
    semimpl_t semi;

    if (sem == NULL_PNTR)
    {
        errno = EINVAL;
        ret = -1;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if ((align != 0U) || (value > (uint32_t)SEM_VALUE_MAX))
        {
            errno = EINVAL;
            ret = -1;
        }
    }

    if (ret != -1)
    {
        (void)memset(sem, 0, sizeof(*sem));
        semi.val = (uintptr_t)0;
        semi.sem.val = (uint16_t)value;
        semi.sem.nsem = (uint8_t)named;
        sem->lock.val = semi.val;
    }
    return ret;
}

int32_t sem_init(sem_t* sem, int32_t pshared, uint32_t value)
{
    return nsem_init(sem, pshared, 0, value);
}

int32_t nsem_destroy(sem_t* sem)
{
    int32_t ret = 0;
    semimpl_t semi;

    if (sem == NULL_PNTR)
    {
        errno = EINVAL;
        ret = -1;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            errno = EINVAL;
            ret = -1;
        }
    }
    if (ret != -1)
    {
        uintptr_t newval = SEM_INVALID_VALUE;
        uint_fast8_t enable_break = 0U;
        bool comp;

        semi.val = atomic_load(&sem->lock.val);
        do
        {
            if (semi.val == newval)
            {
                enable_break = 1U;
            }
            else if (semi.sem.waiters > (uint8_t)0)
            {
                errno = EBUSY;
                ret = -1;
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
            else
            {
                comp = atomic_compare_exchange_strong(&sem->lock.val,
                                &semi.val, newval);
            }
        } while (comp == false);
    }
    return ret;
}

int32_t sem_destroy(sem_t* sem)
{
    int32_t ret = 0;
    semimpl_t semi;
    if (sem == NULL_PNTR)
    {
        errno = EINVAL;
        ret = -1;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            errno = EINVAL;
            ret = -1;
        }
    }

    if (ret != -1)
    {
        uintptr_t newval = SEM_INVALID_VALUE;
        uint_fast8_t enable_break = 0U;
        bool comp;
        semi.val = atomic_load(&sem->lock.val);
        do
        {
            if (semi.val == newval)
            {
                enable_break = 1U;
            }
            else if (semi.sem.nsem != (uint8_t)0)
            {
                errno = EINVAL;
                ret = -1;
                enable_break = 1U;
            }
            else if (semi.sem.waiters > (uint8_t)0)
            {
                errno = EBUSY;
                ret = -1;
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
            else
            {
                comp = atomic_compare_exchange_strong(&sem->lock.val,
                        &semi.val, newval);
            }
        } while (comp == false);
    }
    return ret;
}

int32_t sem_getvalue(sem_t* restrict sem, int32_t* restrict ptr)
{
    int32_t ret = 0;
    semimpl_t semi;

    if (sem == NULL_PNTR)
    {
        errno = EINVAL;
        ret = -1;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            errno = EINVAL;
            ret = -1;
        }
        else
        {
            if (ptr == NULL_PNTR)
            {
                errno = EINVAL;
                ret = -1;
            }
            else
            {
                align = CHECK_ALIGN(ptr, (uint32_t)sizeof(int32_t));
                if (align != 0U)
                {
                    errno = EINVAL;
                    ret = -1;
                }
            }
        }
    }

    if (ret != -1)
    {
        semi.val = atomic_load(&sem->lock.val);
        if (semi.val == SEM_INVALID_VALUE)
        {
            errno = EINVAL;
            ret = -1;
        }
        else if (semi.sem.waiters > (uint8_t)0)
        {
            ptr[0] = -(int32_t)semi.sem.waiters;
        }
        else
        {
            ptr[0] = (int32_t)semi.sem.val;
        }
    }
    return ret;
}

int32_t sem_post(sem_t* sem)
{
    int32_t ret = 0;
    semimpl_t semi;
    semimpl_t newsemi;

#if MCOS_CFG_PT_CHECK_ARGUMENTS
    if (sem == NULL_PNTR)
    {
        ret = EINVAL;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            ret = EINVAL;
        }
    }
    if (ret != EINVAL)
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
    {
        uint_fast8_t enable_break = 0U;
        semi.val = atomic_load(&sem->lock.val);
        do {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
            if (semi.val == SEM_INVALID_VALUE)
            {
                ret = EINVAL;
                enable_break = 1U;
            }
            else if (semi.sem.val == (uint16_t)SEM_VALUE_MAX)
            {
                ret = EOVERFLOW;
                enable_break = 1U;
            }
            else
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
            {
                newsemi.val = semi.val;
                newsemi.sem.val++;
            }

#if MCOS_CFG_PT_CHECK_ARGUMENTS
            if (enable_break != 0U)
            {
                break;
            }
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
            enable_break = atomic_compare_exchange_strong(&sem->lock.val,
                    &semi.val, newsemi.val);
        } while (enable_break == 0U);

        if ((ret == 0) && (semi.sem.waiters > (uint8_t)0))
        {
            /* An error return from mcos_mfutex_wake is acceptable and
             * may just mean that another sem_post was faster.
             */
            (void)mcos_mfutex_wake(&sem->lock,
                    (uint32_t)(MCOS_MFUTEX_ORDERING_PRIO | MCOS_MFUTEX_WAKE_ONE));
        }
    }
    if (ret != 0)
    {
        errno = ret;
        ret = -1;
    }
    return ret;
}

/* atomically decrement semaphore.
 * returns semaphore value if positive, or SEM_VALUE_MAX and an error in errno */
static uintptr_t sem_dec(sem_t *sem, _Bool waiter, int32_t *err)
{
    semimpl_t oldval;
    semimpl_t newval;
    int32_t ret;
    uint_fast8_t enable_break = (uint_fast8_t)0;
    oldval.val = (uintptr_t)atomic_load(&sem->lock.val);
    do {
        ret = 0;
        newval.val = oldval.val;
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        if (oldval.val == SEM_INVALID_VALUE)
        {
            ret = EINVAL;
            enable_break = 1U;
        }
        else
        {
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
            if (oldval.sem.val == (uint16_t)0)
            {
                /* always return EAGAIN even when we are going to decrease.
                * This is a good hint for the caller.
                */
                ret = EAGAIN;
                if (waiter)
                {
                    /* mark as waiter */
                    newval.sem.waiters++;
                }
                else
                {
                    enable_break = (uint_fast8_t)1;
                }
            }
            else
            {
                newval.sem.val--;
            }
#if MCOS_CFG_PT_CHECK_ARGUMENTS
        }
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */

        if (enable_break != (uint_fast8_t)0)
        {
            break;
        }
        enable_break = atomic_compare_exchange_strong(&sem->lock.val,
                &oldval.val, newval.val);
    } while (enable_break == (uint_fast8_t)0);
    err[0] = ret;
    return newval.val;
}

int32_t sem_trywait(sem_t* sem)
{
    int32_t ret = 0;
    int32_t err;

    if (sem == NULL_PNTR)
    {
        errno = EINVAL;
        ret = -1;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            errno = EINVAL;
            ret = -1;
        }
    }

    if (ret != -1)
    {
        err = 0;
        (void)sem_dec(sem, 0U, &err);
        if (err != 0)
        {
            errno = err;
            ret = -1;
        }
    }
    return ret;
}

static void sem_cleanup(void *arg)
{
    sem_t *sem = arg;
    uintptr_t oldval;
    semimpl_t newval;
    bool comp;
    oldval = atomic_load(&sem->lock.val);
    do {
        if (oldval == SEM_INVALID_VALUE)
        {
            break;
        }
        newval.val = oldval;
        newval.sem.waiters--;
        comp = atomic_compare_exchange_strong(&sem->lock.val, &oldval, newval.val);
    } while (comp == false);
}

static int32_t sem_timedwait_internal(sem_t* sem, const struct timespec* restrict abstmo,
        uint_fast8_t timed)
{
    int32_t ret = 0;
    int32_t err = 0;
    pttcb_t* pttcb;
    semimpl_t semval;
    mcos_erid_t mfret;
    uint_fast8_t loop;
    int32_t oldcancel;

#if MCOS_CFG_PT_CHECK_ARGUMENTS
    if (sem == NULL_PNTR)
    {
        err = EINVAL;
    }
    else
    {
        uint32_t align = CHECK_ALIGN(sem, (uint32_t)sizeof(uintptr_t));
        if (align != 0U)
        {
            err = EINVAL;
        }
    }
    if (err != EINVAL)
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
    {
        (void)sem_dec(sem, 0U, &err);

        /* if the semaphore was valid but busy */
        if (err == EAGAIN)
        {
#if MCOS_CFG_PT_CHECK_ARGUMENTS
            if (timed != 0U)
            {
                if (abstmo == NULL_PNTR)
                {
                    err = EINVAL;
                }
                else
                {
                    uint32_t align = CHECK_ALIGN(abstmo, (uint32_t)sizeof(abstmo->tv_nsec));
                    if (   (align != 0U)
                        || (abstmo->tv_nsec < 0L)
                        || (abstmo->tv_nsec >= 1000000000L))
                    {
                        err = EINVAL;
                    }
                }
            }
            if (err != EINVAL)
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
            {
                mcos_systime_t systime = {0,(uint32_t)0};
                if (timed == mcos_true)
                {
                    (void)pt_abstime_convert(abstmo, &systime);
                }

                /* Try again, marking as waiter */
                (void)pthread_setcanceltype((int32_t)PTHREAD_CANCEL_DEFERRED, &oldcancel);
                semval.val = sem_dec(sem, true, &err);
                if (err == EAGAIN)
                {
                    pthread_cleanup_push(&sem_cleanup, sem);
                    (void)pthread_setcanceltype(oldcancel, NULL_PNTR);

                    /* wait and try to decrement in a loop */
                    loop = (uint_fast8_t)1;
                    while (loop != (uint_fast8_t)0)
                    {
                        loop = (uint_fast8_t)0;
                        mfret = mcos_mfutex_wait(&sem->lock, semval.val, (uint32_t)0, &systime);
                        if ((mfret == 0) || (mfret == MCOS_MFUTEX_RET_NOTVALUE))
                        {
                            semval.val = sem_dec(sem, 0U, &err);
                            if (err == EAGAIN)
                            {
                                /* Someone else was faster.
                                 * Wait again.
                                 */
                                loop = (uint_fast8_t)1;
                            }
                            else /* EINVAL or 0 */
                            {
                                break;
                            }
                        }
                        /* handle signals if any */
                        else if (mfret == MCOS_MFUTEX_RET_SIGNAL)
                        {
                            pttcb = pt_get_pttcb();
                            pt_handle_interrupt(pttcb);
                            err = EINTR;
                        }
                        else if (mfret == MCOS_MFUTEX_RET_TIMEOUT)
                        {
                            /* handle timeout */
                            err = ETIMEDOUT;
                        }
                        else if (mfret == MCOS_EMACV)
                        {
                            err = EFAULT;
                        }
                        else
                        {
                            /* do nothing */
                        }
                    }
                    /* Remove ourselves from waiter count. */
                    pthread_cleanup_pop(1);
                }
                else
                {
                    (void)pthread_setcanceltype(oldcancel, NULL_PNTR);
                }
            }
        }
    }
#if MCOS_CFG_PT_CHECK_ARGUMENTS
    if (err != 0)
    {
        errno = err;
        ret = -1;
    }
#endif /* #if MCOS_CFG_PT_CHECK_ARGUMENTS */
    return ret;
}

int32_t sem_timedwait(sem_t* sem, const struct timespec* restrict abstmo)
{
    return sem_timedwait_internal(sem, abstmo, 1U);
}

int32_t sem_wait(sem_t* sem)
{
    return sem_timedwait_internal(sem, NULL_PNTR, 0U);
}

/* Scan and search for the highest priority (lowest number) of waiter */
uint8_t mfutex_waiter_get_highprio(lockobj_t *lo)
{
    /* ignore best waiter hint: it is impossible for one core to correctly find
     * the highest-priority waiter of all cores.
     */
    struct lockinfo local;
    uint8_t ret = 0U;
    uint_fast8_t i;
    for (i = 0U; i < (uint_fast8_t)MFUTEX_LOCK_CORE_COUNT; i++)
    {
        local = lo->percpu[i];
        if ((local.prio != (uint8_t)0) && ((local.prio < ret) || (ret == (uint8_t)0)))
        {
            ret = local.prio;
        }
    }
    return ret;
}
