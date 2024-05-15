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
 * IOVA structures
 */

#ifndef _IOVA_H_
#define _IOVA_H_

#include <stdint.h>
#include <sys/neutrino.h>
#include <private/rb_tree.h>
#include <pthread.h>

#define IOVA_RANGE_CACHE_MAX_SIZE 11 /* cached max size is 4MB(0x400000) */
#define IOVA_MAG_SIZE             128
#define MAX_GLOBAL_MAGS           32

typedef struct iova             iova_node_t;

struct iova {
    rb_tree_node_t              entry;
    unsigned long               pfn_hi;
    unsigned long               pfn_lo;
};

struct iova_magazine {
    unsigned long               size;
    unsigned long               pfns[IOVA_MAG_SIZE];
};

struct iova_cache {
    pthread_mutex_t             lock;
    struct iova_magazine        *loaded;
    struct iova_magazine        *prev;
    uint32_t                    depot_size;
    uint32_t                    reserved;
    struct iova_magazine        *depot[MAX_GLOBAL_MAGS];
};

struct iova_context {
    uint32_t                    start_addr;
    uint32_t                    reserved;
    uint64_t                    size;
    uint64_t                    alignment;

    pthread_mutex_t             rbtree_lock;
    rb_tree_t                   rbroot;
    struct iova                 *cached_node;
    unsigned long               iova_shift;
    unsigned long               start_pfn;
    unsigned long               max_alloc_size;
    unsigned long               dma_pfn;
    struct iova                 *anchor;
    struct iova_cache           caches[IOVA_RANGE_CACHE_MAX_SIZE];
};

struct iova_context *alloc_iova_context(uint32_t start_addr, uint64_t size, uint32_t alignment);
void free_iova_context(struct iova_context *ctx);
int alloc_iova(struct iova_context *ctx, size_t length, uint32_t *piova);
int free_iova(struct iova_context *ctx, uint32_t iova_addr, size_t length);

#endif


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.x_dev/trunk/lib/hw_vendor/renesas/rcar-gen4/ipmmu/iova.h $ $Rev: 972664 $")
#endif
