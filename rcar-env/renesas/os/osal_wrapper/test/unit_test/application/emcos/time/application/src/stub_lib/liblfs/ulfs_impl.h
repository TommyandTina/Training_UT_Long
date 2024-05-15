/*
    Copyright(C) 2021 eSOL Co.,Ltd. All rights reserved.

    This software is protected by the law and the agreement cspinlockrning
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
[ulfs_impl.h] - User-mode fs management: private types and functions
****************************************************************************/
#ifndef ULFS_IMPL_H
#define ULFS_IMPL_H

int32_t call_svc_mkfs(const char *name, int32_t pflags, struct lfs_cmd_channel *uchannel);
int32_t call_svc_rmfs(const char *name);

#endif /* ULFS_IMPL_H */
