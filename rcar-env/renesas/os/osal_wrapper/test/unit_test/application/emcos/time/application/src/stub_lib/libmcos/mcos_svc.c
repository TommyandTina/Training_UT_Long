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
[mcos_svc.c] - SVC interface library (eMCOS Native)
****************************************************************************/

#include <svc/svc_if.h>
#include <svc/svc_if_mcos.h>

#include "stub.h"

int32_t *get_errno_addr(void)
{
    svc_packet_get_errno_addr_t p;
    int     *addr;

    p.id    = SVCID_GET_ERRNO_ADDR;
    p.addr  = &addr;

    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return *p.addr;
}

mcos_er_t mcos_thread_delay(mcos_reltime_t time)
{
    svc_packet_mcos_thread_delay_t  p;

    p.id    = SVCID_MCOS_THREAD_DELAY;
    p.time  = time;

    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_message_send(mcos_id_t tid, const void *data, uint32_t size, uint32_t flags)
{
    svc_packet_mcos_message_send_t  p;

    p.id    = SVCID_MCOS_MESSAGE_SEND;
    p.tid   = tid;
    p.data  = data;
    p.size  = size;
    p.flags = flags;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_erid_t mcos_message_receive(void *data, uint32_t *size, uint32_t flags)
{
    svc_packet_mcos_message_receive_t   p;

    p.id    = SVCID_MCOS_MESSAGE_RECEIVE;
    p.data  = data;
    p.size  = size;
    p.flags = flags;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_message_control(mcos_id_t tid, uint32_t command)
{
    svc_packet_mcos_message_control_t   p;

    p.id        = SVCID_MCOS_MESSAGE_CONTROL;
    p.tid       = tid;
    p.command   = command;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_rrmessage_send(
    mcos_id_t tid,
    uint64_t requestinf, const void *request, uint32_t requestsz,
    uint64_t *replyinf, void *reply, uint32_t replysz)
{
    svc_packet_mcos_rrmessage_send_t    p;

    p.id         = SVCID_MCOS_RRMESSAGE_SEND;
    p.tid        = tid;
    p.requestinf = requestinf;
    p.request    = request;
    p.requestsz  = requestsz;
    p.replyinf   = replyinf;
    p.reply      = reply;
    p.replysz    = replysz;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_rrmessage_receive(mcos_rrmsg_recv_t *recv)
{
    svc_packet_mcos_rrmessage_receive_t p;

    p.id        = SVCID_MCOS_RRMESSAGE_RECEIVE;
    p.recv      = recv;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_rrmessage_reply(uint64_t replyinf, uint32_t replysz, const mcos_rrmsg_recv_t *recv)
{
    svc_packet_mcos_rrmessage_reply_t   p;

    p.id        = SVCID_MCOS_RRMESSAGE_REPLY;
    p.replyinf  = replyinf;
    p.replysz   = replysz;
    p.recv      = recv;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_thread_sleep(void)
{
    svc_packet_mcos_thread_sleep_t  p;

    p.id        = SVCID_MCOS_THREAD_SLEEP;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

mcos_er_t mcos_thread_wakeup(mcos_id_t tid)
{
    svc_packet_mcos_thread_wakeup_t p;

    p.id        = SVCID_MCOS_THREAD_WAKEUP;
    p.tid       = tid;
    return (int)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

uint64_t mcos_debug_time_get(void)
{
    svc_packet_mcos_debug_time_get_t    p;
    uint64_t    nsec = 0;

    p.id        = SVCID_MCOS_DEBUG_TIME_GET;
    p.nsec      = &nsec;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return *p.nsec;
}

#include <stdio.h>
#include <stdarg.h>
void mcos_debug_printf(mcos_ds_str_t fmt, ... )
{
    svc_packet_mcos_debug_printf_t  p;
    char    buf[MCOS_CFG_DS_PRINT_BUFSIZE];
    va_list list;

    va_start(list, fmt);
    vsprintf(buf, fmt, list);
    va_end(list);

    p.id        = SVCID_MCOS_DEBUG_PRINTF;
    p.buf       = (void*)buf;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return;
}

/*
 *  Wakeup all threads based on mfutex
 * Input     :   lo       lockobj_t pointer
 *               flags    flags for describing behaviour
 * Output    :   none
 * Return    :   MCOS_EOK    all/one thread have/has been woken up
 *               MCOS_ENOEXS tid was not found
 * Note  :   none
 */
int16_t mcos_mfutex_wake(
    lockobj_t *lo,
    uint32_t flags)
{
    svc_packet_mcos_mfutex_wake_t   p;

    p.id        = SVCID_MCOS_MFUTEX_WAKE;
    p.lock      = lo;
    p.flags     = flags;

    return (mcos_er_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

/*
 * Wait on mfutex
 * Input     :   lo       lockobj_t pointer
 *               val      expected value in lockobj_t.val
 *               flags    flags for describing behaviour
 *               timeout  maximum wait time (NULL if infinite)
 * Output    :   none
 * Return    :   MCOS_EOK
 * Note  :   none
 */
int16_t mcos_mfutex_wait(lockobj_t *lo, uintptr_t val, uint32_t flags,
        const mcos_systime_t *timeout)
{
    svc_packet_mcos_mfutex_wait_t   p;

    p.id        = SVCID_MCOS_MFUTEX_WAIT;
    p.lock      = lo;
    p.val       = val;
    p.flags     = flags;
    p.abstmo    = timeout;

    return (int16_t)call_svc((void*)&p, GET_CALLER_ADDRESS());

}

/* invalidate data cache */
void bios_cache_invalidate(
    void *addr,
    size_t size)
{
    svc_packet_bios_cache_t     p;

    p.id        = SVCID_BIOS_CACHE_INVALIDATE;
    p.addr      = addr;
    p.size      = size;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return;
}

/* write-back data cache */
void bios_cache_writeback(
    void *addr,
    size_t size)
{
    svc_packet_bios_cache_t     p;

    p.id        = SVCID_BIOS_CACHE_WRITEBACK;
    p.addr      = addr;
    p.size      = size;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return;
}

/* purge data cache */
void bios_cache_purge(
    void *addr,
    size_t size)
{
    svc_packet_bios_cache_t     p;

    p.id        = SVCID_BIOS_CACHE_PURGE;
    p.addr      = addr;
    p.size      = size;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
    return;
}

/*
 * Uring Notification
 * Input     :   r         The uring object
 *               data      The sent data
 *               data_size The sent data size
 * Output    :   none
 * Return    :   0
 * Note  :   none
 */
int32_t uring_interrupt_notify(uring_t r, const void *data, size_t data_size)
{
    if (false == g_stub_start)
    {
        svc_packet_mcos_uring_notify_t p;

        p.id        = SVCID_MCOS_URING_NOTIFY;
        p.ring      = r;
        p.data      = data;
        p.data_size = data_size;

        return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
    }
    else
    {
        /* do nothing */
    }

    return 0;
}
