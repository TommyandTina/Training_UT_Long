/*
    Copyright (C) 2021 By eSOL Co.,Ltd. All rights reserved.

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
[readlink.c] - read a symlink
****************************************************************************/
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

ssize_t readlinkat(int32_t fd, const char* restrict path, char* restrict buf, size_t size)
{
 /* O_CREAT | O_SEARCH plus file type will open symlink for reading */
    int32_t lfd = openat(fd, path, O_CREAT | O_SEARCH,
                      /* To open symlink also specify file type */
                         S_IRWXU | S_IRWXG | S_IRWXO | S_IFLNK);
    if (lfd < 0)
    {
        return lfd;
    }
    ssize_t ret = read(lfd, buf, size);
    int32_t err = errno;
    (void)close(lfd);
    errno = err;
    return ret;
}

ssize_t readlink(const char* restrict path, char* restrict buf, size_t size)
{
    ssize_t ret = readlinkat(AT_FDCWD, path, buf, size);
    return ret;
}
