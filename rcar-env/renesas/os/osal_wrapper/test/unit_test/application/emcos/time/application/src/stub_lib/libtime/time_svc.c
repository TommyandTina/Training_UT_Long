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
[time_svc.c] - SVC interface library (POSIX Clock/Time)
****************************************************************************/

#include <svc/svc_if.h>
#include <svc/svc_if_time.h>

#include "stub.h"

/* nanosleep() API */
int32_t nanosleep(const struct timespec *rqtp, struct timespec *rmtp)
{
    svc_packet_time_nanosleep_t p;

    p.id        = SVCID_TIME_NANOSLEEP;
    p.rqtp      = rqtp;
    p.rmtp      = rmtp;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* sleep() API */
unsigned int sleep(unsigned int seconds)
{
    svc_packet_time_sleep_t p;

    p.id        = SVCID_TIME_SLEEP;
    p.seconds   = seconds;
    return (unsigned int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* clock_nanosleep() API */
int32_t clock_nanosleep(clockid_t clock_id, int32_t flags, const struct timespec *rqtp, struct timespec *rmtp)
{
    if (false == g_stub_start)
    {
        svc_packet_time_clock_nanosleep_t   p;

        p.id        = SVCID_TIME_CLOCK_NANOSLEEP;
        p.clock_id  = clock_id;
        p.flags     = flags;
        p.rqtp      = rqtp;
        p.rmtp      = rmtp;
        return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
    }
    else
    {
        /* do nothing */
    }

    return 0;
}

/* clock_getres() API */
int32_t clock_getres(clockid_t clock_id, struct timespec *res)
{
    svc_packet_time_clock_getres_t  p;

    p.id        = SVCID_TIME_CLOCK_GETRES;
    p.clock_id  = clock_id;
    p.res       = res;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* clock_gettime() API */
int32_t clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    svc_packet_time_clock_gettime_t p;

    p.id        = SVCID_TIME_CLOCK_GETTIME;
    p.clock_id  = clock_id;
    p.tp        = tp;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* clock_settime() API */
int32_t clock_settime(clockid_t clock_id, const struct timespec *tp)
{
    svc_packet_time_clock_settime_t p;

    p.id        = SVCID_TIME_CLOCK_SETTIME;
    p.clock_id  = clock_id;
    p.tp        = tp;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* clock_getcpuclockid() API */
int clock_getcpuclockid(pid_t pid, clockid_t *clock_id)
{
    svc_packet_time_clock_getcpuclockid_t   p;

    p.id        = SVCID_TIME_CLOCK_GETCPUCLOCKID;
    p.pid       = pid;
    p.clock_id  = clock_id;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* gettimeofday() API */
int gettimeofday(struct timeval * restrict tp, struct timezone * restrict zp)
{
    svc_packet_time_gettimeofday_t  p;

    p.id    = SVCID_TIME_GETTIMEOFDAY;
    p.tp    = tp;
    p.zp    = zp;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}
