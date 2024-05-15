/*
    Copyright(C) 2019 eSOL Co.,Ltd. All rights reserved.

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
[libmmap_svc.c] - SVC interface library (Mmap)
****************************************************************************/

#include <svc/svc_if.h>
#include <svc/svc_if_mmap.h>
#include "stub.h"

void* mmap(void *addr, size_t len, int32_t prot, int32_t flags, int32_t fno, off_t offs)
{
    svc_packet_mmap_mmap_t  p;
    void    *map = NULL;
START_TIMER
    p.id    = SVCID_MMAP_MMAP;
    p.addr  = addr;
    p.len   = len;
    p.prot  = prot;
    p.flags = flags;
    p.fno   = fno;
    p.offs  = offs;
    p.map   = &map;
    (void)call_svc((void*)&p, GET_CALLER_ADDRESS());
STOP_TIMER
    return *p.map;
}

int32_t munmap(void *addr, size_t len)
{
    svc_packet_mmap_munmap_t    p;
START_TIMER
    p.id    = SVCID_MMAP_MUNMAP;
    p.addr  = addr;
    p.len   = len;
    int32_t ret = (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
STOP_TIMER
    return ret;
}

int32_t msync(void *addr, size_t len, int32_t flags)
{
    svc_packet_mmap_msync_t p;

    p.id    = SVCID_MMAP_MSYNC;
    p.addr  = addr;
    p.len   = len;
    p.flags = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t mlock(const void *addr, size_t len)
{
    svc_packet_mmap_mlock_t p;

    p.id    = SVCID_MMAP_MLOCK;
    p.addr  = addr;
    p.len   = len;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t munlock(const void *addr, size_t len)
{
    svc_packet_mmap_munlock_t   p;

    p.id    = SVCID_MMAP_MUNLOCK;
    p.addr  = addr;
    p.len   = len;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t mlockall(int32_t flags)
{
    svc_packet_mmap_mlockall_t  p;

    p.id    = SVCID_MMAP_MLOCKALL;
    p.flags = flags;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t munlockall(void)
{
    svc_packet_mmap_munlockall_t    p;

    p.id    = SVCID_MMAP_MUNLOCKALL;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t mprotect(void *addr, size_t len, int32_t prot)
{
    svc_packet_mmap_mprotect_t  p;

    p.id    = SVCID_MMAP_MPROTECT;
    p.addr  = addr;
    p.len   = len;
    p.prot  = prot;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t shm_open(const char *name, int32_t oflag, mode_t mode)
{
    svc_packet_mmap_shm_open_t  p;

    p.id    = SVCID_MMAP_SHM_OPEN;
    p.name  = name;
    p.oflag = oflag;
    p.mode  = mode;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}

int32_t shm_unlink(const char *name)
{
    svc_packet_mmap_shm_unlink_t    p;

    p.id    = SVCID_MMAP_SHM_UNLINK;
    p.name  = name;
    return (int32_t)call_svc((void*)&p, GET_CALLER_ADDRESS());
}
