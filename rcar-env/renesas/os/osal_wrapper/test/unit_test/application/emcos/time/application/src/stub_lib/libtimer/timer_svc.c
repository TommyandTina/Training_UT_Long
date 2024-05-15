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
[timer_svc.c] - SVC interface library (POSIX Timer)
****************************************************************************/

#include <svc/svc_if.h>
#include <svc/svc_if_timer.h>
#include "stub.h"

/* timer_create() API */
int32_t timer_create(clockid_t clockid, struct sigevent *restrict event, timer_t *restrict timerid)
{
    g_timer_create_count++;
    svc_packet_timer_create_t   p;

    p.id        = SVCID_TIMER_CREATE;
    p.clockid   = clockid;
    p.event     = event;
    p.timerid   = timerid;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* timer_delete() API */
int32_t timer_delete(timer_t timerid)
{
    svc_packet_timer_delete_t   p;

    p.id        = SVCID_TIMER_DELETE;
    p.timerid   = timerid;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* timer_gettime() API */
int32_t timer_gettime(timer_t timerid, struct itimerspec* value)
{
    svc_packet_timer_gettime_t  p;

    p.id        = SVCID_TIMER_GETTIME;
    p.timerid   = timerid;
    p.value     = value;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* timer_settime() API */
int32_t timer_settime(timer_t timerid, int32_t flags, const struct itimerspec* value, struct itimerspec* old)
{
    svc_packet_timer_settime_t  p;

    p.id        = SVCID_TIMER_SETTIME;
    p.timerid   = timerid;
    p.flags     = flags;
    p.value     = value;
    p.old       = old;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/* timer_getoverrun() API */
int32_t timer_getoverrun(timer_t timerid)
{
    svc_packet_timer_getoverrun_t   p;

    p.id        = SVCID_TIMER_GETOVERRUN;
    p.timerid   = timerid;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}
