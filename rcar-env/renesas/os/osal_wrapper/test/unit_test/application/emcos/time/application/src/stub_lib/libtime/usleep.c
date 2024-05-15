/*
    Copyright(C) 2017 eSOL Co., Ltd. All rights reserved.

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
/*============================================================================
 usleep.c   POSIX Layer usleep() library function
============================================================================*/
#include <unistd.h>
#include <time.h>
#include <errno.h>

/* Suspend execution for an interval */
int32_t usleep(useconds_t useconds)
{
    int32_t         sta;
    struct timespec rqtp;

    if (useconds < 1000000U)
    {
        rqtp.tv_sec  = 0;
        rqtp.tv_nsec = (int64_t)useconds * 1000U;
        sta = nanosleep(&rqtp, NULL);
    }
    else
    {
        sta = -1;
        errno = EINVAL;
    }

    return sta;
}
