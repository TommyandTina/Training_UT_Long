/*
    Copyright(C) 2021 eSOL Co.,Ltd. All rights reserved.

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
 uring_impl.h   Private impl header for uring
============================================================================*/

#ifndef URING_IMPL_H
#define URING_IMPL_H

#include <mcos/uring.h>

#define URING_WORD_BITS (8 * __SIZEOF_POINTER__)

#if URING_WORD_BITS == 32
    typedef uint32_t bitmap_t; /** depends on maximum size of posix atomic operation on the system */
    #define bitmap_c UINT32_C
    #define URING_FFS(word)      __builtin_ffsl(word) /** find first set */
    #define FULL (uint32_t) - 1
#else
    typedef uint64_t bitmap_t; /** depends on maximum size of posix atomic operation on the system */
    #define bitmap_c UINT64_C
    #define URING_FFS(word)      __builtin_ffsll(word) /** find first set */
    #define FULL (uint64_t) - 1
#endif

/* Entry types */
#define ENTRY_LOCAL (0U)                               /** Entry at a shared page prepared at initialization */
#define ENTRY_REQ_MAP (0x01U)                          /** for future function */
#define ENTRY_RES_MAP (0x02U)                          /** for future function */
#define ENTRY_TOT_MAP (ENTRY_REQ_MAP | ENTRY_RES_MAP)  /** For masking entry types */

#define MIN_BUFF_SIZE (sizeof(struct uring_e)) /** Buffer must at least contain the struct uring_e */
#define PAGEMASK (PAGESIZE - 1) /** mask a page knowing page size is a pow2 */

/* uring queue entry (item) */
struct uring_e
{
    sem_t lock;       /** To wait on this entry */
    uint8_t type;     /** uring entry's type, defined by ENTRY prefix */
    uint8_t sector;   /** sector where the entry is stored */
    bitmap_t bitmask; /** bitmask of this entry in the sector */
    size_t req_size;  /** Size of the request data in bytes */
    size_t res_size;  /** Size of the response data in bytes.
                       *  This is for future use for more generic way.
                       */
};

/* uring queue */
struct uring_q
{
    uint16_t head;             /** Pointer (offset) to first entry in ring buffer */
    volatile uint16_t tail;    /** Pointer (offset) to last entry in ring buffer */
    uint16_t tail_r;           /** Pointer (offset) to last entry for reservation */
    uint16_t max_entries;      /** Max number of entries in the ring buffer, must be pow(2)*/
    sem_t size;                /** Counting semaphore for queue size */
    /* ring buffer follows like: entry_t rb[max_entries] */
};

/* uring structure */
struct uring
{
    uint8_t n_sectors;                    /* Number of sectors in this ring */
    uint8_t opt;                          /* Uring option flags */
    struct uring_q queue;                 /* ring buffer */
    size_t size;                          /* uring size (including queue and data ) */
    uintptr_t queue_off;                  /* offset to data buffer (after uring structure) */
    uintptr_t data;                       /* offset to data buffer (after uring structure) */
    bitmap_t bitmap[MAX_N_SECTORS];       /* Bitmap of used data buffers inside each sector */
    uint8_t sector_buff_n[MAX_N_SECTORS]; /* Sector size: number of buffers in each sector */
    size_t sector_buff_sz[MAX_N_SECTORS]; /* Size of buffers inside each sector */
};

/** Get queue head index */
static inline uint16_t get_queue_head_idx(struct uring_q *q)
{
    return q->head & (q->max_entries - 1);
}

/** Get queue tail index */
static inline uint16_t get_queue_tail_idx(struct uring_q *q)
{
    return q->tail & (q->max_entries - 1);
}

/** Get queue tail index */
static inline uint16_t conv2idx(struct uring_q *q, uint16_t org)
{
    return org & (q->max_entries - 1);
}

/** Get ring buffer queue size */
static inline uint16_t get_queue_size(struct uring_q *q)
{
    return q->tail - q->head;
}

/** Test whether queue is empty or not*/
static inline _Bool queue_empty(struct uring_q *q)
{
    uint16_t queue_size = get_queue_size(q);
    _Bool ret = false;
    if (queue_size == 0)
    {
        ret = true;
    }
    return ret;
}

/** Test whether queue is full or not*/
static inline _Bool queue_full(struct uring_q *q)
{
    uint16_t queue_size = get_queue_size(q);
    _Bool ret = false;
    if (queue_size == q->max_entries)
    {
        ret = true;
    }
    return ret;
}

/** return 1 if n is pow(2) */
static inline _Bool is_pow(uint16_t n)
{
    return (n & (n - 1)) != 0 ? true : false;
}

/** uring queue is always after struct uring */
static inline entry_t *get_queue_head(struct uring *ring)
{
    return (entry_t *)((uintptr_t)ring + ring->queue_off);
}

/** uring data is after uring queue (ring->data is an offset from the start of the ring pointer) */
static inline uintptr_t get_data_page(struct uring *ring)
{
    return (uintptr_t)ring + ring->data;
}

#endif /* URING_IMPL_H */
