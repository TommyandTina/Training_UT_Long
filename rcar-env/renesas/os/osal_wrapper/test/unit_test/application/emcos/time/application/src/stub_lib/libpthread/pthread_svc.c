/*
    Copyright (C) 2016 By eSOL Co.,Ltd. Tokyo, Japan

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
[pthread_create_svc.c] - SVC interface library (POSIX Thread)
****************************************************************************/

//#include <pt_api_impl.h>
#include <signal.h>
#include <sys/types.h>
#include <mcos/mcos.h>
#include <pthread.h>

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

#include <svc/svc_if.h>
#include <svc/svc_if_pthread.h>
#include <errno.h>

#include "stub.h"
#include "target/emcos/r_osal_common.h"

extern void* getextcbptr(void);
extern int pt_init_signal_entry(void);

/* Get own PT TCB from the process space */
pttcb_t* pt_get_pttcb(void)
{
    pttcb_t *pttcb;
    pttcb = getextcbptr();
    return pttcb;
}

/* Common PT thread entry for subthreads of a process */
void* pt_process_thread_entry(void *args)
{
    void    *(*start_routine)(void*);
    void    *arg;
    void    *ret;
    void    **param;

    /* Setup argument */
    param = (void**)args;
    start_routine = (void*(*)(void*))param[0];
    arg = (void*)param[1];

    /* Install signal handler entry */
    pt_init_signal_entry();

    /* Invoke user specified entry */
    ret = start_routine(arg);

    /* Exit thread */
    pthread_exit(ret);

    /* Never reach here */
    return ret;
}

/* pthread_create() API */
int pthread_create(pthread_t *restrict thread,
       const pthread_attr_t *restrict attr,
       void *(*start_routine)(void*), void *restrict arg)
{
    if (false == g_stub_start)
    {
        svc_packet_pthread_create_t p;

        p.id            = SVCID_PTHREAD_CREATE;
        p.thread        = thread;
        p.attr          = attr;
        p.start_routine = start_routine;
        p.arg           = arg;

        return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
    }
    else
    {
START_TIMER
        static uint32_t stub_pthread_create_count = 0U;
        uint32_t num_of_thread    = (uint32_t)R_OSAL_RCFG_GetNumOfThread();
        uint32_t num_of_interrupt = (uint32_t)R_OSAL_RCFG_GetNumOfInterruptThread();
        st_osal_interrupt_thread_control_t *interrupt_control;

        g_pthread_create_count++;
        *thread = 0x0U;    /* dummy of created thread */

        if (stub_pthread_create_count >= (num_of_thread+num_of_interrupt))
        {
            /* interrupt receive thread */
            /* do noting */
        }
        else if (stub_pthread_create_count >= num_of_thread)
        {
            /* interrupt user thread */
            interrupt_control = (st_osal_interrupt_thread_control_t *)arg;
            interrupt_control->thread_active = true;
        }
        else
        {
            /* thread control */
            /* do noting */
        }
        stub_pthread_create_count++;
STOP_TIMER
    }

    return 0;
}

/* pthread_exit() API */
void pthread_exit(void *value_ptr)
{
    svc_packet_pthread_exit_t   p;
    pttcb_t* pttcb;

    /* Cleanup pthread related resources */
    pttcb = pt_get_pttcb();
    pt_thread_cleanup(pttcb); /* No error returns even if pttcb is NULL */

    p.id            = SVCID_PTHREAD_EXIT;
    p.value_ptr     = value_ptr;

    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return;
}

/* pthread_join() API */
int pthread_join(pthread_t thread, void **value_ptr)
{
    if (false == g_stub_start)
    {
        svc_packet_pthread_join_t   p;

        p.id            = SVCID_PTHREAD_JOIN;
        p.thread        = thread;
        p.value_ptr     = value_ptr;

        return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
    }
    else
    {
        /* do nothing */
    }

    return 0;
}

/* pthread_self() API */
static pthread_t _pthread_self(void)
{
    svc_packet_pthread_self_t   p;

    p.id            = SVCID_PTHREAD_SELF;

    return (pthread_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

pthread_t pthread_self(void)
{
START_TIMER
    pthread_t   tid;
    pttcb_t     *pttcb;

    pttcb = pt_get_pttcb();
    if (pttcb != 0)
    {
        tid = pttcb->pthrid;
    }
    else
    {
        tid = _pthread_self();
    }
STOP_TIMER
    return tid;
}

int pthread_detach(pthread_t thread)
{
    svc_packet_pthread_detach_t p;

    p.id            = SVCID_PTHREAD_DETACH;
    p.thread        = thread;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int sched_yield(void)
{
    svc_packet_sched_yield_t    p;

    p.id            = SVCID_SCHED_YIELD;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_getschedparam(pthread_t thread, int *restrict policy, struct sched_param *restrict param)
{
    svc_packet_pthread_getschedparam_t    p;

    p.id        = SVCID_PTHREAD_GETSCHEDPARAM;
    p.thread    = thread;
    p.policy    = policy;
    p.param     = param;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param)
{
    svc_packet_pthread_setschedparam_t  p;

    p.id        = SVCID_PTHREAD_SETSCHEDPARAM;
    p.thread    = thread;
    p.policy    = policy;
    p.param     = param;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_setschedprio(pthread_t thread, int prio)
{
    if (false == g_stub_start)
    {
        svc_packet_pthread_setschedprio_t   p;

        p.id        = SVCID_PTHREAD_SETSCHEDPRIO;
        p.thread    = thread;
        p.prio      = prio;

        return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
    }
    else
    {
        /* do nothing */
    }

    return 0;
}

/*
 * Note: 'formutex' is not used here.
 * This 3rd argument is only for compatibility of the function.
 */
int32_t pthread_setschedprio_mutex(pthread_t tid, int32_t priority, int32_t formutex)
{
    svc_packet_pthread_setschedprio_mutex_t   p;

    p.id        = SVCID_PTHREAD_SETSCHEDPRIO_MUTEX;
    p.thread    = tid;
    p.prio      = priority;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int mfutex_sched_change(pthread_t thread)
{
    svc_packet_mfutex_sched_change_t   p;

    p.id        = SVCID_MFUTEX_SCHED_CHANGE;
    p.thread    = thread;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_sleep_np(void)
{
    svc_packet_pthread_sleep_np_t   p;

    p.id    = SVCID_PTHREAD_SLEEP_NP;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_timedsleep_np(const struct timespec* tspec)
{
    svc_packet_pthread_timedsleep_np_t  p;

    p.id    = SVCID_PTHREAD_TIMEDSLEEP_NP;
    p.tspec = tspec;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_wakeup_np(pthread_t tid)
{
    svc_packet_pthread_wakeup_np_t  p;

    p.id    = SVCID_PTHREAD_WAKEUP_NP;
    p.tid   = tid;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int pthread_cancel(pthread_t thread)
{
    svc_packet_pthread_cancel_t  p;

    p.id        = SVCID_PTHREAD_CANCEL;
    p.thread    = thread;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t get_thread_state(pthread_t tid)
{
    svc_packet_is_thread_blocked_t p;

    p.id  = SVCID_IS_THREAD_BLOCKED;
    p.tid = tid;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t get_process_state(pid_t pid)
{
    svc_packet_is_process_blocked_t p;

    p.id  = SVCID_IS_PROCESS_BLOCKED;
    p.pid = pid;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t sched_getscheduler(pid_t pid)
{
    svc_packet_sched_getscheduler_t  p;

    p.id        = SVCID_SCHED_GETSCHEDULER;
    p.pid       = pid;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t sched_setscheduler(pid_t pid, int32_t policy, const struct sched_param *param)
{
    svc_packet_sched_setscheduler_t  p;

    p.id        = SVCID_SCHED_SETSCHEDULER;
    p.pid       = pid;
    p.policy    = policy;
    p.param     = param;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t sched_getparam(pid_t pid, struct sched_param *param)
{
    svc_packet_sched_getparam_t  p;

    p.id        = SVCID_SCHED_GETPARAM;
    p.pid       = pid;
    p.param     = param;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t sched_setparam(pid_t pid, const struct sched_param *param)
{
    svc_packet_sched_setparam_t  p;

    p.id        = SVCID_SCHED_SETPARAM;
    p.pid       = pid;
    p.param     = param;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t sched_rr_get_interval(pid_t pid, struct timespec *interval)
{
    svc_packet_sched_rr_get_interval_t  p;

    p.id        = SVCID_SCHED_RR_GET_INTERVAL;
    p.pid       = pid;
    p.interval  = interval;

    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}


int32_t pthread_getname_np(pthread_t thread, char * name, size_t len)
{
    return call_svc(
        &(svc_packet_pthread_getname_np_t)
            { .id = SVCID_PTHREAD_GETNAME_NP, .thread = thread, .name = name, .len = len, },
        GET_CALLER_ADDRESS());
}

int32_t pthread_setname_np(pthread_t thread, const char * name)
{
    return call_svc(
        &(svc_packet_pthread_setname_np_t) { .id = SVCID_PTHREAD_SETNAME_NP, .thread = thread, .name = name, },
        GET_CALLER_ADDRESS());
}

int32_t pthread_getattr_np(pthread_t thread, pthread_attr_t * attr)
{
    return call_svc(
        &(svc_packet_pthread_getattr_np_t)
            { .id = SVCID_PTHREAD_GETATTR_NP, .thread = thread, .attr = attr, },
        GET_CALLER_ADDRESS());
}
