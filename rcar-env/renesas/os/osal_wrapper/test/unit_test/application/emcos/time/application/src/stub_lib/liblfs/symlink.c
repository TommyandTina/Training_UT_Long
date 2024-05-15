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
[symlink.c] - create a symlink
****************************************************************************/
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

int32_t symlinkat(const char* content, int32_t fd, const char* linkpath)
{
    if (content == NULL)
    {
        errno = EINVAL;
        return -1;
    }
    size_t len = strlen(content);
    if (len > _POSIX_SYMLINK_MAX)
    {
        errno = ENAMETOOLONG;
        return -1;
    }
 /* Symlinks are world-accessible by default */
    int32_t lfd = openat(fd, linkpath, O_CREAT | O_EXCL | O_WRONLY,
                    /* To create symlink specify file type */
                       S_IRWXU | S_IRWXG | S_IRWXO | S_IFLNK);
    if (lfd < 0)
    {
        return lfd;
    }
 /* Do not write trailing nul */
    ssize_t res = write(lfd, content, len);
    int32_t err = errno;
    (void)close(lfd);
    int ret = 0;
    if (res != (ssize_t)len)
    {
        if (res >= 0)
        {
            err = EIO;
        }
        (void)unlinkat(fd, linkpath, 0);
        errno = err;
        ret = -1;
    }
    return ret;
}

int32_t symlink(const char* content, const char* linkpath)
{
    int32_t ret = symlinkat(content, AT_FDCWD, linkpath);
    return ret;
}
