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
[ lfa.h ] - Lock-free allocator definitions
****************************************************************************/

#ifndef LFA_H
#define LFA_H

/* Lock-free allocator (deterministic heap)
 *
 * Optimizations to be done (in decreasing order of ROI):
 * - word-based traversal
 * - improve ordering constraints, and use load-acq/store-rel where possible
 * - move heuristics and non-const to beginning of heap to save on static base
 */
#include <stdint.h>

/* LFA implementation uses some private fields for internal management.
 * These reservations ensure the correct amount of memory is reserved.
 */
#define LFA_BGROUP_RESERVED_BYTES    (3+2*sizeof(uint_fast32_t)+2)
#define LFA_STRUCT_RESERVED_BYTES    (46)

#define LFA_LOG_SIZE                 0
#define LFA_ERROR_TRAP               0

typedef enum lfa_error_code
{
    LFA_ERROR_CHANGE_USAGE_FAILED = 1,
    LFA_ERROR_FREE_OF_UNKNOWN,
    LFA_ERROR_FREE_OF_ADMIN,
    LFA_ERROR_FREE_INSIDE_ALLOCATED,
    LFA_ERROR_DOUBLE_FREE,
    LFA_ERROR_INTERNAL_ERROR,
    LFA_ERROR_HEAP_FULL,
    LFA_ERROR_BAD_HEAP
} lfa_error_code_t;

typedef enum lfa_log_op
{
    LFA_LOG_FREE = 0,
    LFA_LOG_ALLOC = 1,
    LFA_LOG_REALLOC = 2,
    LFA_LOG_TAIL_FREE_IN_REALLOC = 3,
    LFA_LOG_CREATEBLOCK = 4
} lfa_log_op_t;

typedef struct lfa_log_entry
{
    lfa_log_op_t op; /* 0 FREE, 1 ALLOC, 2 REALLOC, 3 TAIL FREE IN REALLOC */
    uintptr_t p;
    uintptr_t size;
} lfa_log_entry_t;

typedef struct lfa_block
{
    /* block size */
    uint16_t size;
    /* word count of bitmap at the beginning of block group */
    uint8_t word_count;
    /* reserved range to cover private fields */
    uint8_t reserved_bytes[LFA_BGROUP_RESERVED_BYTES];
} lfa_bgroup_t;

typedef struct lfa lfa_t;

typedef struct lfa_opts
{
    /* set by application or system */

    /* called when the current memory usage including overhead goes above or
     * below a define threshold.
     */
    void (*reached_threshold)(lfa_t *heap, uintptr_t current_usage, uint8_t low_to_high);
    /* value to determine when to call reached_threshold when memory usage increases.
     * 0 if no threshold */
    uintptr_t low_to_high_threshold;
    /* value to determine when to call reached_threshold when memory usage decreases.
     * 0 if no threshold */
    uintptr_t high_to_low_threshold;

    /* log buffer entry count */
    uint32_t log_count;
    /* log array */
    lfa_log_entry_t *log_array;

    /* read-only for application */
    /* current log entry */
    uintptr_t log_current_entry;

    /* live memory usage tracking. Only if reached_threshold is != NULL */
    uintptr_t current_heap_usage;
} lfa_options_t;

struct lfa
{
    /* Start address of the heap */
    uintptr_t *start;
    /* Current end address of the heap */
    uintptr_t *end;
    /* Maximum end address of the heap, used during setup to determine page size */
    uintptr_t *max_end;
    /* requested page size logarithm (e.g. 12 for 1 << 12 == 4096) */
    uint8_t logpagesize;
    /* actual used block group count */
    uint8_t bgroup_count;
    /* Block groups characteristics */
    lfa_bgroup_t *bgroup_info;

    /* optional elements - can be NULL */
#if LFA_ERROR_TRAP != 0
    /* called when an error has been detected */
    void (*error)(lfa_t *heap, lfa_error_code_t code, uintptr_t arg);
#endif /* LFA_ERROR_TRAP != 0 */
    lfa_options_t *opts;

    /* These bytes cover private fields the lfa implementation use for internal
     * purposes.
     */
    uint8_t reserved[LFA_STRUCT_RESERVED_BYTES];
};

/* Behaves like aligned_alloc()
 * Currently, alignment must be below one page size.
 */
void *lfa_aligned_alloc(lfa_t *heap, size_t alignment, size_t size);
/* Behaves like free() */
void lfa_free(lfa_t *heap, void *ptr);
/* Behaves like realloc() */
void *lfa_realloc(lfa_t *heap, void *ptr, size_t size);
/* Must be called with a lfa_t structure with all public fields set.
 * Returns:
 * - 0 if OK
 * - EINVAL if lfa_t is misconfigured
 */
int32_t lfa_setup_heap(lfa_t *heap);

#endif /* LFA_H */
