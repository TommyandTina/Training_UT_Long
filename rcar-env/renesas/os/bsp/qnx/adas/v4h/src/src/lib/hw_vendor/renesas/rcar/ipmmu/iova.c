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
 * IOVA allocation/free implementation
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "iova.h"

#define ALIGN(x, a) (((x) + (typeof(x))(a) - 1) & ~((typeof(x))(a) - 1))

// #define VERBOSE
#ifdef VERBOSE
#define vdbg(x...) printf(x)
#else
#define vdbg(x...)
#endif


static inline int
__fls(unsigned int val) {
    int r = 32;

    if (!val) {
        return 0;
    }
    if (!(val & 0xffff0000u)) {
        val <<= 16;
        r -= 16;
    }
    if (!(val & 0xff000000u)) {
        val <<= 8;
        r -= 8;
    }
    if (!(val & 0xf0000000u)) {
        val <<= 4;
        r -= 4;
    }
    if (!(val & 0xc0000000u)) {
        val <<= 2;
        r -= 2;
    }
    if (!(val & 0x80000000u)) {
        val <<= 1;
        r -= 1;
    }

    return r;
}


static inline uint32_t
__ffs(uint32_t word) {
    int num = 0;

    if ((word & 0xffff) == 0) {
        num += 16;
        word >>= 16;
    }
    if ((word & 0xff) == 0) {
        num += 8;
        word >>= 8;
    }
    if ((word & 0xf) == 0) {
        num += 4;
        word >>= 4;
    }
    if ((word & 0x3) == 0) {
        num += 2;
        word >>= 2;
    }
    if ((word & 0x1) == 0) {
        num += 1;
    }

    return num;
}


static inline int
__ilog2(const uint32_t num) {
    return __fls(num) - 1;
}


static inline int
__order_base_2(const uint32_t num) {
    if (num > 1) {
        return __ilog2(num - 1) + 1;
    }
    return 0;
}


static int
iova_compare_nodes(void const * const vnode1, void const * const vnode2, rb_tree_t const * const tree) {
    (void)tree;
    iova_node_t const * const node1 = vnode1;
    iova_node_t const * const node2 = vnode2;
    return node1->pfn_lo - node2->pfn_lo;
}


static int
iova_compare_value(void const * const vnode, const uintptr_t value, rb_tree_t const * const tree) {
    (void)tree;
    iova_node_t const * const node = vnode;
    return node->pfn_lo - (unsigned long)value;
}


static inline struct iova *
alloc_iova_mem(void) {
    struct iova *const obj = malloc(sizeof(*obj));
    if (!obj) {
        return NULL;
    }
    memset(obj, 0, sizeof(*obj));
    return obj;
}


static inline void
free_iova_mem(struct iova *const obj) {
    free(obj);
}


static inline struct iova *
get_cached_node(const struct iova_context *const ctx) {
    return ctx->cached_node;
}


static inline void
cached_insert_update(struct iova_context *const ctx, struct iova *const obj) {
    ctx->cached_node = obj;
}


static void
cached_delete_update(struct iova_context *ctx, struct iova *const obj) {
    const struct iova *const cached_iova = get_cached_node(ctx);

    if ((cached_iova == obj) ||
            ((obj->pfn_hi < ctx->dma_pfn) &&
             (obj->pfn_lo >= cached_iova->pfn_lo))) {
        ctx->cached_node = rb_tree_next(&ctx->rbroot, &obj->entry);
        ctx->max_alloc_size = ctx->dma_pfn;
    }
}


static void
iova_insert_rbtree(struct iova_context *ctx, struct iova *const obj) {
    rb_tree_insert(&ctx->rbroot, &obj->entry);
}


static int
alloc_and_insert_iova_range(struct iova_context *ctx, const unsigned long size, struct iova *new) {
    struct iova *curr;
    unsigned long new_pfn;
    unsigned long limit_pfn;

    pthread_mutex_lock(&ctx->rbtree_lock);

    if (size >= ctx->max_alloc_size) {
        pthread_mutex_unlock(&ctx->rbtree_lock);
        return -ENOMEM;
    }

    curr = get_cached_node(ctx);
    limit_pfn = curr->pfn_lo;
    do {
        limit_pfn = min(limit_pfn, curr->pfn_lo);
        new_pfn = limit_pfn - size;
        curr = rb_tree_prev(&ctx->rbroot, &curr->entry);
    } while ((curr != NULL) && (new_pfn <= curr->pfn_hi));

    if ((limit_pfn < size) || (new_pfn < ctx->start_pfn)) {
        ctx->max_alloc_size = size;
        pthread_mutex_unlock(&ctx->rbtree_lock);
        return -ENOMEM;
    }

    new->pfn_lo = new_pfn;
    new->pfn_hi = new->pfn_lo + size;
    vdbg("%s: new 0x%x -- 0x%x\n", __func__, new->pfn_lo, new->pfn_hi);

    iova_insert_rbtree(ctx, new);
    cached_insert_update(ctx, new);

    pthread_mutex_unlock(&ctx->rbtree_lock);
    return 0;
}


static struct iova *
alloc_iova_deep(struct iova_context *const ctx, const unsigned long size) {
    struct iova *new_iova;
    int ret;

    new_iova = alloc_iova_mem();
    if (!new_iova) {
        return NULL;
    }

    ret = alloc_and_insert_iova_range(ctx, size, new_iova);
    if (ret) {
        free_iova_mem(new_iova);
        return NULL;
    }

    return new_iova;
}


static struct iova *
find_iova_locked(struct iova_context *const ctx, const unsigned long pfn) {
    struct iova *obj = rb_tree_first(&ctx->rbroot);
    struct iova *found = NULL;

    while ((obj != NULL) && (found == NULL)) {
        if (pfn < obj->pfn_lo) {
            obj = rb_tree_prev(&ctx->rbroot, &obj->entry);
        } else if (pfn >= obj->pfn_hi) {
            obj = rb_tree_next(&ctx->rbroot, &obj->entry);
        } else {
            found = obj;
        }
    }

    return found;
}


static struct iova *
find_iova(struct iova_context *const ctx, const unsigned long pfn) {
    struct iova *obj = NULL;
    pthread_mutex_lock(&ctx->rbtree_lock);
    obj = find_iova_locked(ctx, pfn);
    pthread_mutex_unlock(&ctx->rbtree_lock);

    return obj;
}


static void
free_iova_locked(struct iova_context *ctx, struct iova *const obj) {
    cached_delete_update(ctx, obj);
    rb_tree_remove(&ctx->rbroot, &obj->entry);
}


static void
free_iova_deep(struct iova_context *ctx, const unsigned long pfn) {
    struct iova *const obj = find_iova(ctx, pfn);

    if (obj != NULL) {
        pthread_mutex_lock(&ctx->rbtree_lock);
        vdbg("%s: iova 0x%x -- 0x%x\n", __func__, obj->pfn_lo, obj->pfn_hi);
        free_iova_locked(ctx, obj);
        pthread_mutex_unlock(&ctx->rbtree_lock);
    }
}


static inline struct iova_magazine *
iova_magazine_alloc(void) {
    struct iova_magazine *const m = malloc(sizeof(struct iova_magazine));
    if (m != NULL) {
        memset(m, 0, sizeof(*m));
    }
    return m;
}


static inline void
iova_magazine_free(struct iova_magazine *const mag) {
    free(mag);
}


static void
iova_magazine_free_pfns(struct iova_magazine *mag, struct iova_context *ctx) {
    unsigned long i;

    if (!mag) {
        return;
    }

    pthread_mutex_lock(&ctx->rbtree_lock);

    for (i = 0; i < mag->size; i++) {
        struct iova *const obj = find_iova_locked(ctx, mag->pfns[i]);

        if (obj != NULL) {
            free_iova_locked(ctx, obj);
        }
    }

    pthread_mutex_unlock(&ctx->rbtree_lock);
    mag->size = 0;
}


static inline bool
iova_magazine_full(struct iova_magazine *const mag) {
    return ((mag != NULL) && (mag->size == IOVA_MAG_SIZE));
}


static inline bool
iova_magazine_empty(struct iova_magazine *const mag) {
    return ((mag == NULL) || (mag->size == 0));
}


static bool
iova_magazine_pop(struct iova_magazine *mag, const unsigned long limit_pfn, unsigned long *pfn) {
    unsigned long i;

    if (iova_magazine_empty(mag)) {
        return false;
    }

    for (i = mag->size - 1; mag->pfns[i] > limit_pfn; i--) {
        if (i == 0) {
            return false;
        }
    }

    *pfn = mag->pfns[i];
    vdbg("%s: pfn 0x%x\n", __func__, *pfn);
    mag->pfns[i] = mag->pfns[--mag->size];

    return true;
}


static void
iova_magazine_push(struct iova_magazine *mag, const unsigned long pfn) {
    if (iova_magazine_full(mag)) {
        return;
    }

    mag->pfns[mag->size++] = pfn;
}


static void
init_iova_caches(struct iova_context *ctx) {
    struct iova_cache *cache;
    int i;

    for (i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; i++) {
        cache = &ctx->caches[i];
        pthread_mutex_init(&cache->lock, NULL);
        cache->depot_size = 0;
        cache->loaded = iova_magazine_alloc();
        cache->prev = iova_magazine_alloc();
    }
}


static bool
__iova_cache_insert(struct iova_context *const ctx, struct iova_cache *cache, const unsigned long iova_pfn) {
    struct iova_magazine *mag_to_free = NULL;
    bool can_insert = false;

    pthread_mutex_lock(&cache->lock);

    if (!iova_magazine_full(cache->loaded)) {
        can_insert = true;
    } else if (!iova_magazine_full(cache->prev)) {
        struct iova_magazine *const tmp = cache->prev;
        cache->prev = cache->loaded;
        cache->loaded = tmp;
        can_insert = true;
    } else {
        struct iova_magazine *const new_mag = iova_magazine_alloc();

        if (new_mag) {
            if (cache->depot_size < MAX_GLOBAL_MAGS) {
                cache->depot[cache->depot_size++] = cache->loaded;
            } else {
                mag_to_free = cache->loaded;
            }

            cache->loaded = new_mag;
            can_insert = true;
        }
    }

    if (can_insert) {
        iova_magazine_push(cache->loaded, iova_pfn);
    }

    pthread_mutex_unlock(&cache->lock);

    if (mag_to_free) {
        iova_magazine_free_pfns(mag_to_free, ctx);
        iova_magazine_free(mag_to_free);
    }

    return can_insert;
}


static bool
iova_cache_insert(struct iova_context *ctx, const unsigned long pfn, const uint32_t size) {
    const uint32_t log_size = __order_base_2(size);
    if (log_size >= IOVA_RANGE_CACHE_MAX_SIZE) {
        return false;
    }
    return __iova_cache_insert(ctx, &ctx->caches[log_size], pfn);
}


static bool
__iova_cache_get(struct iova_cache *cache, const unsigned long limit_pfn, unsigned long *const pfn) {
    bool has_pfn = false;

    pthread_mutex_lock(&cache->lock);

    if (!iova_magazine_empty(cache->loaded)) {
        has_pfn = true;
    } else if (!iova_magazine_empty(cache->prev)) {
        struct iova_magazine *const tmp = cache->prev;
        cache->prev = cache->loaded;
        cache->loaded = tmp;
        has_pfn = true;
    } else {
        if (cache->depot_size > 0) {
            iova_magazine_free(cache->loaded);
            cache->loaded = cache->depot[--cache->depot_size];
            has_pfn = true;
        }
    }

    if (has_pfn) {
        has_pfn = iova_magazine_pop(cache->loaded, limit_pfn, pfn);
    }

    pthread_mutex_unlock(&cache->lock);

    return has_pfn;
}


static bool
iova_cache_get(struct iova_context *ctx, const uint32_t size, const unsigned long limit_pfn, unsigned long *const pfn) {
    unsigned int log_size = __order_base_2(size);
    if (log_size >= IOVA_RANGE_CACHE_MAX_SIZE) {
        return false;
    }
    return __iova_cache_get(&ctx->caches[log_size], limit_pfn - size, pfn);
}


static void
free_iova_caches(struct iova_context *ctx) {
    struct iova_cache *cache;
    unsigned i, j;

    for (i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; ++i) {
        cache = &ctx->caches[i];
        iova_magazine_free(cache->loaded);
        iova_magazine_free(cache->prev);

        for (j = 0; j < cache->depot_size; ++j) {
            iova_magazine_free(cache->depot[j]);
        }
    }
}


static void
free_cached_iovas(struct iova_context *ctx) {
    struct iova_cache *cache;
    int i;

    for (i = 0; i < IOVA_RANGE_CACHE_MAX_SIZE; ++i) {
        cache = &ctx->caches[i];
        pthread_mutex_lock(&cache->lock);
        iova_magazine_free_pfns(cache->loaded, ctx);
        iova_magazine_free_pfns(cache->prev, ctx);
        pthread_mutex_unlock(&cache->lock);
    }
}


static int
__alloc_iova(struct iova_context *const ctx, const unsigned long size, unsigned long *pfn) {
    const struct iova *new_iova = NULL;
    bool flush_cache = true;

    if (iova_cache_get(ctx, (uint32_t)size, ctx->max_alloc_size, pfn)) {
        return 0;
    }

    while (new_iova == NULL) {
        new_iova = alloc_iova_deep(ctx, size);
        if (new_iova == NULL) {
            if (!flush_cache) {
                return -1;
            }

            flush_cache = false;
            free_cached_iovas(ctx);
        }
    }

    *pfn = new_iova->pfn_lo;
    return 0;
}


static void
__free_iova(struct iova_context *const ctx, const unsigned long pfn, const unsigned long size) {
    vdbg("%s: pfn 0x%x, size 0x%x\n", __func__, pfn, size);
    if (iova_cache_insert(ctx, pfn, (uint32_t)size)) {
        return;
    }

    free_iova_deep(ctx, pfn);
}


struct iova_context *
alloc_iova_context(const uint32_t start_addr, const uint64_t size, const uint32_t alignment) {
    struct iova_context *ctx = malloc(sizeof(*ctx));

    if (!ctx) {
        return NULL;
    }

    memset(ctx, 0, sizeof(*ctx));

    ctx->start_addr = start_addr;
    ctx->size = size;
    ctx->alignment = alignment;
    if (ctx->alignment == 0) {
        ctx->alignment = 4096; // at least, 4K base allocation
    }

    ctx->anchor = alloc_iova_mem();
    if (!ctx->anchor) {
        free(ctx);
        return NULL;
    }

    pthread_mutex_init(&ctx->rbtree_lock, NULL);

    static rb_tree_func_t const iova_tree_funcs = {
        .cmp_nodes = iova_compare_nodes,
        .cmp_value = iova_compare_value,
    };

    rb_tree_init(&ctx->rbroot, &iova_tree_funcs, offsetof(iova_node_t, entry));

    ctx->cached_node = ctx->anchor;
    ctx->iova_shift = __ffs((uint32_t)ctx->alignment);
    ctx->start_pfn = ctx->start_addr >> 12;
    ctx->dma_pfn = (ctx->start_addr + ctx->size) >> 12;
    ctx->max_alloc_size = ctx->dma_pfn;
    ctx->anchor->pfn_lo = ctx->dma_pfn;
    ctx->anchor->pfn_hi = ctx->dma_pfn;
    rb_tree_insert(&ctx->rbroot, &ctx->anchor->entry);

    init_iova_caches(ctx);

    vdbg("%s: iova_shift %d, dma_pfn 0x%x, max_alloc_size 0x%x, start_pfn 0x%x\n", __func__,
            ctx->iova_shift, ctx->dma_pfn, ctx->max_alloc_size, ctx->start_pfn);

    return ctx;
}


void
free_iova_context(struct iova_context *ctx) {
    free_iova_caches(ctx);

    pthread_mutex_lock(&ctx->rbtree_lock);
    while (1) {
        struct iova *const obj = rb_tree_first(&ctx->rbroot);
        if (obj == NULL) break;
        rb_tree_remove(&ctx->rbroot, &obj->entry);
        free_iova_mem(obj);
    }
    pthread_mutex_unlock(&ctx->rbtree_lock);

    free(ctx);
}


int
alloc_iova(struct iova_context *const ctx, const size_t length, uint32_t *piova) {
    const unsigned long aligned_size = ALIGN(length, ctx->alignment);
    unsigned long pfn;

    const int ret = __alloc_iova(ctx, aligned_size >> 12, &pfn);

    if (ret < 0) {
        return -ENOMEM;
    }

    *piova = (uint32_t)pfn << 12;
    vdbg("[ALLOC] context %p, iova 0x%x, len %ld\n", ctx, *piova, length);
    return 0;
}


int
free_iova(struct iova_context *const ctx, const uint32_t iova_addr, const size_t length) {
    __free_iova(ctx, iova_addr >> 12, length >> 12);
    vdbg("[FREE] context %p, iova 0x%x, len %ld\n", ctx, iova_addr, length);
    return 0;
}


#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL: http://svn.ott.qnx.com/product/branches/7.x_dev/trunk/lib/hw_vendor/renesas/rcar-gen4/ipmmu/iova.c $ $Rev: 982648 $")
#endif
