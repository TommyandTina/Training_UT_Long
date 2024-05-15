/*
    Copyright(C) 2018 eSOL Co., Ltd. All rights reserved.

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
 clock.c    POSIX Layer clock() library function
============================================================================*/
#include <sys/time.h>

 /* Workaround for build error in forcibly_linked_funcs of eBinder */
clock_t clock(void)
{
    return (clock_t)((sizeof(clock_t)/sizeof(clock_t) << sizeof(clock_t) * 8U ) - sizeof(clock_t)/sizeof(clock_t));
}
