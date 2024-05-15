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
[lchown.c] - change symlink owner/group
****************************************************************************/
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int32_t lchown(const char *path, uid_t owner, gid_t group)
{
    int32_t ret = fchownat(AT_FDCWD, path, owner, group, AT_SYMLINK_NOFOLLOW);
    return ret;
}
