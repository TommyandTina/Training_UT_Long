/*
    Copyright (C) 2019 eSOL Co.,Ltd. All rights reserved.

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
[ lfa_sys.c ] - Lock-free allocator system integration
****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/mman.h>
#include "lfa.h"
#include "lfa_sysdepend.h"
#include "stub.h"

#if LFA_ERROR_TRAP != 0
static void defaultHeap_error(lfa_t *heap, lfa_error_code_t code, uintptr_t arg);
#endif /* LFA_ERROR_TRAP != 0 */

#if LFA_LOG_SIZE != 0
LFA_WEAK lfa_log_entry_t lfa_log_array[LFA_LOG_SIZE] = {0};
#endif /* LFA_LOG_SIZE != 0 */
LFA_WEAK lfa_options_t defaultHeapOpts = {
#if LFA_LOG_SIZE != 0
    .log_array = lfa_log_array,
#endif
    .log_count = LFA_LOG_SIZE,
};

static lfa_bgroup_t defaultHeapBgroupInfo[] = {
    /* declares that a block group of .size will be allocated with
     * groups of .word_count * LFA_WORD_BITS bytes, rounded up to the
     * next neareast page.
     */
    { .size = 16U,   .word_count = 8U },
    { .size = 32U,   .word_count = 8U },
    { .size = 64U,   .word_count = 8U },
    { .size = 96U,   .word_count = 4U },
    { .size = 128U,  .word_count = 4U },
    { .size = 192U,  .word_count = 4U },
    { .size = 256U,  .word_count = 2U },
    { .size = 384U,  .word_count = 2U },
    { .size = 512U,  .word_count = 1U },
    { .size = 768U,  .word_count = 1U },
    { .size = 1024U, .word_count = 1U },
};
LFA_WEAK lfa_t defaultHeap = {
    .logpagesize = 12U, /* 1 << 12 = 4096 */
    .bgroup_count = (uint8_t)(sizeof(defaultHeapBgroupInfo)/sizeof(lfa_bgroup_t)),
    .bgroup_info = &defaultHeapBgroupInfo[0],

#if LFA_ERROR_TRAP != 0
    .error = defaultHeap_error,
#endif /* LFA_ERROR_TRAP != 0 */
    .opts = &defaultHeapOpts
};

/* 2 bits per page */
#if UINTPTR_MAX > (1ULL << 32)
#define PAGE_PER_WORD 32U
#else
#define PAGE_PER_WORD 16U
#endif

#if LFA_ERROR_TRAP != 0
#if !defined(NDEBUG)
#include <pthread.h>
pthread_mutex_t lfa_log_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
static void defaultHeap_error(lfa_t *heap, lfa_error_code_t code, uintptr_t arg)
{
#if !defined(NDEBUG)
    pthread_mutex_lock(&lfa_log_mutex);
#endif
    uintptr_t heapsize = (uintptr_t)heap->max_end - (uintptr_t)heap->start;
    uintptr_t page_count = heapsize >> heap->logpagesize;

    fprintf(stderr, "LFA %p: code %d arg %p heapstart %p\n",
            (void*)heap, code, (void*)arg, (void*)heap->start);
#if !defined(NDEBUG)
    /* print pagetable */
    for (uint_fast32_t pt = 0U; pt < page_count; pt += 4U*PAGE_PER_WORD) {
        printf("0x%06"PRIxFAST32" %08"PRIXPTR" %08"PRIXPTR" %08"PRIXPTR" %08"PRIXPTR"\n",
                pt,
                heap->start[pt/PAGE_PER_WORD],
                heap->start[pt/PAGE_PER_WORD + 1U],
                heap->start[pt/PAGE_PER_WORD + 2U],
                heap->start[pt/PAGE_PER_WORD + 3U]);
    }
#if LFA_LOG_SIZE != 0
    printf("Log (last entry %"PRIu64"):\n", heap->opts->log_current_entry);
    for (uint_fast32_t logidx = 0; logidx < heap->opts->log_count; logidx++) {
        const char opstr[] = "FARTC";
        struct lfa_log_entry *lle = &heap->opts->log_array[logidx];
        printf("%c 0x%08"PRIXPTR" 0x%"PRIXPTR"\n", opstr[lle->op], lle->p, lle->size);
    }
    printf("Log end\n");
#endif /* LFA_LOG_SIZE != 0 */
    pthread_mutex_unlock(&lfa_log_mutex);
#endif
}
#endif /* LFA_ERROR_TRAP != 0 */

void *malloc(size_t size)
{
START_TIMER
    g_malloc_count++;
    void *p = lfa_aligned_alloc(&defaultHeap, sizeof(void*), size);
STOP_TIMER
    return p;
}

void *calloc(size_t nmemb, size_t size)
{
START_TIMER
    g_calloc_count++;
    void *p = lfa_aligned_alloc(&defaultHeap, sizeof(void*), nmemb * size);
    if (p != NULL) {
        (void)memset(p, 0, nmemb * size);
    }
STOP_TIMER
    return p;
}

void *realloc(void *ptr, size_t size)
{
START_TIMER
    g_realloc_count++;
    void *p = lfa_realloc(&defaultHeap, ptr, size);
STOP_TIMER
    return p;
}

void free(void *ptr)
{
START_TIMER
    lfa_free(&defaultHeap, ptr);
STOP_TIMER
}

int32_t posix_memalign(void **memptr, size_t alignment, size_t size)
{
    void *p;
    int32_t ret;

    if ((memptr == NULL) ||
        (alignment < sizeof(void*)) ||
        ((alignment & (alignment - 1U)) != 0U))
    {
        ret = EINVAL;
    }
    else
    {
        p = lfa_aligned_alloc(&defaultHeap, alignment, size);
        if (p == NULL)
        {
            ret = errno;
        }
        else
        {
            *memptr = p;
            ret = 0;
        }
    }

    return ret;
}

extern const size_t malloc_max_heap_size;

int32_t _clib_malloc_initialized = 0;

static int32_t _init_lfa_memalloc(void)
{
    const uintptr_t max_heap_size = (malloc_max_heap_size + LFA_MMU_PAGE_SIZE - 1U) & ~(LFA_MMU_PAGE_SIZE - 1U);
    void *heap_area;
    int32_t ret;

    heap_area = mmap(NULL, max_heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_HEAP, -1, 0);
    if (heap_area != MAP_FAILED)
    {
        defaultHeap.start = (uintptr_t*)heap_area;
        defaultHeap.end = (uintptr_t*)((uintptr_t)heap_area + max_heap_size);
        defaultHeap.max_end = defaultHeap.end;
        ret = lfa_setup_heap(&defaultHeap);
    }
    else
    {
        ret = EINVAL;
    }
    return ret;
}

int32_t _init_memalloc(void)
{
    int32_t ret;

    if (_clib_malloc_initialized == 0)
    {
        ret = _init_lfa_memalloc();
        if (ret == 0)
        {
            _clib_malloc_initialized = 1;
        }
    }
    else
    {
        ret = 0;
    }

    return ret;
}
