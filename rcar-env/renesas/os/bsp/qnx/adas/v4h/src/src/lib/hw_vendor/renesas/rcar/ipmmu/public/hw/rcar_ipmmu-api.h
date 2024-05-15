/*
 * Copyright (c) 2023, BlackBerry Limited. All rights reserved.
 *
 * BlackBerry Limited and its licensors retain all intellectual property and
 * proprietary rights in and to this software and related documentation. Any
 * use, reproduction, disclosure or distribution of this software and related
 * documentation without an express license agreement from BlackBerry Limited
 * is strictly prohibited.
 */

/**
 * @file
 * libipmmu API
 */

#ifndef _IPMMU_API_H
#define _IPMMU_API_H

#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

int ipmmu_open(const char *const group_name, const int channel_id);
void ipmmu_close(const int handle);
int ipmmu_map(const int handle, const uint64_t pa, const size_t len, uint32_t *pva);
int ipmmu_unmap(const int handle, const uint64_t pa, const uint32_t va, const size_t len);

#endif

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.x_dev/trunk/lib/hw_vendor/renesas/rcar-gen4/ipmmu/public/hw/rcar_ipmmu-api.h $ $Rev: 974525 $")
#endif
