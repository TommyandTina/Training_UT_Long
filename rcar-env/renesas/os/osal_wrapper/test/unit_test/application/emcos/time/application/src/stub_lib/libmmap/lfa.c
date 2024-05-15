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
[ lfa.c ] - Lock-free allocator implementations
****************************************************************************/

/* Lock-free allocator (deterministic heap)
 *
 * The heap is organized as follows, from the beginning:
 * - page table: each 2-bit entry stores page status (FBDC = Free, Block, Data,
 *     Continue). An allocation bigger than a page follows the format BCCC/DCC.
 *     Allocation in the page table is done using word-based atomic operations
 *     with forward allocation, and backwards free.
 * - block group status: each 16-bit entry contains block status (4-bit block
 *     group type, 12-bit free slot count) for one page. The two pieces of
 *     information are grouped to allow atomic release of the block group.
 * - data pages
 *
 * Within a block group, the block group pages are divided in equal size blocks.
 * At the beginning of the block group, some blocks are reserved to hold a
 * bitmap of used/free (1/0) bits for each block. This means the first block is
 * always used.
 *
 * Memory overhead (please read kB as kiB):
 * - 18 bits per page (2 bits for page table, 16 bits for block group entry)
 * - 1 bit per block entry
 * e.g.:
 * - for a 1GB heap and 4kB pages:
 *   * 256k pages, 64kB page table, 576kB page table + block group status overhead
 *   * 256k*64/8=2MB bitmaps for 16-byte allocations (worst-case heap usage)
 *   -> 0.055% best case overhead (pages only), 0.157% worst-case overhead
 * - for a 16MB heap and 512B pages:
 *   * 32k pages, 8kB page table, 72kB page table + block group status overhead
 *   * 32k*64/8=256kB bitmaps for 16-byte allocations
 *   -> 0.43% best case overhead, 2% worst-case overhead
 * - for a 128kB small heap and 64B pages with 16-byte block groups:
 *   * 2k pages, 512B page table, 4.5kB page table + block group status overhead
 *   * 2k*8=16kB bitmaps for 16-byte allocations
 *   -> 3.5% best case overhead, 16% worst-case overhead
 * - for a 128kB small heap and 16B pages, no block groups:
 *   * 8k pages, 2kB page table, 2kB overhead
 *   -> 1.56% overhead
 * - for a 1kB tiny heap and 16B pages, no block groups:
 *   * 64 pages, 16B page table, 16B overhead
 *   -> 1.56% overhead
 * So, with a 128kB heap and below, it is preferable to skip the use of block
 * groups entirely. One may even argue there will not be more atomic operations
 * in typical cases.
 *
 * Free time for a page set is constant:
 * - one atomic write to its page table entry for each different page table word
 *     that needs an update.
 * Free time for a block entry is near-constant:
 * - two atomic operations (free bitmap entry, increase free slot count)
 * - if the block group becomes fully free, the corresponding pages are further
 *     freed
 *
 * Allocation time for a page set is bounded:
 * - worst case when no memory is available is a full page table traversal,
 *     reading 32 pages' status at a time on 64-bit system
 * - worst case when there is untracked memory available only just before the
 *     heuristic is a full page table traversal
 * - typical case for single page allocation is a single page table entry read,
 *     using heuristic tracking first free entry
 *
 * Allocation time for a block entry is bounded:
 * - worst case if heap is full and no block group of the type has free slots,
 *     and using single-page block group, is full page table traversal, reading
 *     full page table plus full block group status area
 * - worst case if heap is not full is traversal of used part of page table,
 *     reading appropriate entries in the block group status area
 * - typical case using the free block entry heuristic is single page read, single
 *     block group status check, plus allocation
 *
 * Optimizations to be done (in decreasing order of ROI):
 * - move heuristics and non-const to beginning of heap to save on static base
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "lfa.h"
#include "lfa_sysdepend.h"

/* Static assertion */
#ifndef STATIC_ASSERT
#define STATIC_ASSERT(COND,MSG) typedef char static_assertion_##MSG[(!!(COND))*2-1]
#endif

/* Calculate two's complement */
#define NEG_UINTPTR(val)    (~(uintptr_t)(val) + (uintptr_t)1U)

/* 2-bit value for representing one page current usage.
 * In the below code, F, D, B, C is used to refer to these usage states.
 * It is very important that USAGE_FREE is 0, because the page table is zeroed
 * at startup, and test against zero is used in many places.
 * It is very important that USAGE_CONTINUE is 3, making it all 1s, and xor'd
 * in many places, then used with LFA_CLZ() to find contiguous ranges.
 */
#define USAGE_FREE          ((uintptr_t)0x0U)
#define USAGE_BLOCK         ((uintptr_t)0x1U)
#define USAGE_DATA          ((uintptr_t)0x2U)
#define USAGE_CONTINUE      ((uintptr_t)0x3U)

#define USAGE_BITS          2U
#define USAGE_MASK          ((uintptr_t)0x3U)

/* Casting with 'uintptr_t' will make an appropriate bit-width according to the target environment. */
#define USAGE_BITPATTERN    ((uintptr_t)0x5555555555555555ULL)

/* assumes 8 bits per byte */
#define PAGE_PER_WORD       ((uint_fast8_t)LFA_WORD_BITS/USAGE_BITS)
#define PAGE_PER_BYTE       ((8U*sizeof(char))/USAGE_BITS)

/* block_def are 16-bits entries, packed in words */
#define BLOCKINFO_PER_WORD  ((uint_fast8_t)LFA_WORD_BITS/16U)

typedef uint_fast32_t page_id_t;
typedef uint_fast32_t page_count_t;
typedef uintptr_t block_usage_t;

typedef struct bgroup_info
{
    uint16_t id :4; /* block group ID, 1-based index, 0 meaning free */
    uint16_t free_count :12; /* free block count in the group, max 1024 */
} bgroup_info_t;

typedef union bgroup_word
{
    bgroup_info_t entries[BLOCKINFO_PER_WORD];
    uintptr_t word;
} bgroup_word_t;

typedef struct bgroup_def
{
    /* block size */
    uint16_t size;
    /* word count of bitmap at the beginning of block group */
    uint8_t word_count;
    /* number of blocks reserved in the bitmap for the bitmap itself */
    uint8_t reserved_blocks;
    /* block count per block group */
    uint16_t count;
    /* pages per block group */
    page_count_t page_count;
    /* Heuristic: current index of block group with a free entry */
    volatile page_id_t first_free;
} bgroup_def_t;

typedef struct lfaimpl
{
    /* public entries */
    uintptr_t *start;
    uintptr_t *end;
    uintptr_t *max_end;
    uint8_t logpagesize;
    uint8_t bgroup_count;
    bgroup_def_t *bgroup_info;

    /* optional element - can be NULL */
    /* called when an error has been detected */
#if LFA_ERROR_TRAP != 0
    void (*error)(lfa_t *heap, lfa_error_code_t code, uintptr_t arg);
#endif /* LFA_ERROR_TRAP != 0 */
    lfa_options_t *opts;

    /* private entries */
    /* page-table size in bytes */
    uintptr_t ptsize;
    /* heap total page count */
    page_count_t page_count;
    bgroup_word_t *bgroup_start;
    uintptr_t pagesize;
    page_id_t reserved_pages;

    /* heuristics for optimization */
    volatile page_id_t last_used_page;
    volatile page_id_t first_free_page;
} lfaimpl_t;

STATIC_ASSERT(sizeof(lfaimpl_t) == sizeof(lfa_t), adjust_LFA_STRUCT_RESERVED_BYTES);
STATIC_ASSERT(sizeof(bgroup_def_t) == sizeof(lfa_bgroup_t), adjust_LFA_BGROUP_RESERVED_BYTES);

LFA_PURE
LFA_INLINE uintptr_t round_up(uintptr_t val, uintptr_t round)
{
    return ((((val) + ((round) - 1U)) / (round)) * (round));
}

static void lfa_log(lfaimpl_t *heap, lfa_log_op_t op, void * p, uintptr_t size)
{
#if LFA_LOG_SIZE != 0
    uintptr_t new_entry, current_entry;
    lfa_log_entry_t *entry;
    if (heap->opts != NULL)
    {
        lfa_options_t *opts = heap->opts;
        if (opts->log_count > 0U)
        {
            current_entry = atomic_fetch_add(&opts->log_current_entry, 1U);
            new_entry = current_entry % opts->log_count;
            entry = (lfa_log_entry_t *)&opts->log_array[new_entry];
            entry->p = (uintptr_t)p;
            entry->size = size;
            entry->op = op;
        }
    }
#endif /* LFA_LOG_SIZE != 0 */
}

LFA_INLINE void lfa_error(lfaimpl_t *heap, lfa_error_code_t code, uintptr_t arg)
{
#if LFA_ERROR_TRAP != 0
    if (heap->error != NULL)
    {
        heap->error((lfa_t*)heap, code, arg);
    }
#endif /* LFA_ERROR_TRAP != 0 */
}

/* The function takes in mask one word of USAGE_BITS bitmap,
 * and returns a bitmap in the form 0b001111001100 where 0b11 notes an entry
 * of a series of 'count' USAGE_FREE entries.
 * 'count' must be >= 1.
 * based on https://geidav.wordpress.com/2014/03/06/on-finding-1-bit-sequences/ */
LFA_PURE
LFA_INLINE uintptr_t find_free_sequence(uintptr_t mask, page_count_t count)
{
    uintptr_t entry;
    uint_fast32_t n = count * USAGE_BITS;
    /* USAGE_FREE is 00, but the algorithm works on 11, reverse */
    entry = ~mask;
    /* do one iteration out of the loop */
    entry &= entry >> 1U;
    /* the algorithm really works on series of contiguous bits, but USAGE_BITS is
     * currently 2. Mask out the unneeded bits.
     */
    entry &= USAGE_BITPATTERN;
    /* The above mask removes all odd bits, and only remaining even bits are
     * those where a USAGE_FREE is described. Duplicate these bits.
     */
    entry |= entry << 1U;
    /* if entry is 0 right now, there is no free space, no need to enter the loop */
    if (entry != 0U)
    {
        /* The original algorithm at the above URL uses while (n > 1).
         * However, because we are actually looking for 2-bit USAGE_BITS entries,
         * so the last reduction to single-bit entries is not necessary.
         */
        while (n > 2U)
        {
            const uint_fast8_t shift = n >> 1U;
            entry &= entry >> shift;
            if (entry == 0U)
            {
                break;
            }
            n -= shift;
        }
    }
    return entry;
}

LFA_PURE
LFA_INLINE uintptr_t *page_address(lfaimpl_t *heap, page_id_t page)
{
    return (uintptr_t*) (((uintptr_t) heap->start) + ((uintptr_t) page << heap->logpagesize));
}

LFA_PURE
LFA_INLINE page_id_t get_page(lfaimpl_t *heap, void *ptr)
{
    uintptr_t addr = (uintptr_t) ptr;
    return (page_id_t)((addr - (uintptr_t) heap->start) >> heap->logpagesize);
}

LFA_PURE
LFA_INLINE block_usage_t get_usage(lfaimpl_t *heap, page_id_t page)
{
    uintptr_t ptentry = heap->start[page / PAGE_PER_WORD];
    const uint_fast8_t shift = (page % PAGE_PER_WORD) * USAGE_BITS;
    return (block_usage_t) ((ptentry >> shift) & USAGE_MASK);
}

/* count used pages in the 1st level page table
 * 'page' must be USAGE_DATA.
 */
LFA_PURE
LFA_INLINE page_count_t count_pages(lfaimpl_t *heap, page_id_t page)
{
    page_count_t count = 0U;
    uint_fast8_t start = page % PAGE_PER_WORD;
    const uintptr_t mask = USAGE_BITPATTERN * USAGE_CONTINUE;
    uintptr_t *pword = &heap->start[page / PAGE_PER_WORD];
    uintptr_t word = pword[0];

    /* make first entry all 0s */
    word ^= (uintptr_t) USAGE_DATA << (start * USAGE_BITS);
    /* make MSB USAGE_CONTINUE entries zero */
    if ((start + 1U) < PAGE_PER_WORD)
    {
        word ^= mask << ((start + 1U) * USAGE_BITS);
    }
    /* clz return the number of cleared high bits, which is what we want */
    uint_fast8_t clzword = LFA_CLZ(word) / USAGE_BITS;

    /* if the highest entry in the word is set, we need to continue to search
     * in the next word.
     */
    if (clzword >= (PAGE_PER_WORD - start))
    {
        /* count all end of this word */
        count = PAGE_PER_WORD - start;
        /* next word starts at 0 */
        start = 0U;
        /* find how many full words there are */
        for (;;)
        {
            pword++;
            word = pword[0];
            if (word != mask)
            {
                word ^= mask;
                break;
            }
            count += PAGE_PER_WORD;
        }
    }

    /* mask out all lower pages including start */
    word &= ~((1ULL << (start * USAGE_BITS)) - 1ULL);
    /* mask out high entries that are not USAGE_CONTINUE which is 11b */
    word ^= word - 1ULL;
    /* in the resulting word, non-USAGE_CONTINUE are zeroes, low two
     * bits are 1, and middle matching USAGE_CONTINUE are 11b.
     */
    clzword = LFA_CLZ(word);
    count += (PAGE_PER_WORD - 1U) - (clzword / USAGE_BITS) - start;
    return count;
}

static _Bool change_usage_sameword(
                lfaimpl_t *heap,
                page_id_t page,
                page_count_t count,
                block_usage_t before,
                block_usage_t after)
{
    _Bool ret = true;
    uintptr_t *ptentry = &heap->start[page / PAGE_PER_WORD];
    uintptr_t entry;
    uintptr_t newentry;
    uintptr_t new_field;
    uintptr_t old_field;
    uintptr_t mask;
    const uint_fast8_t shift_left = (page % PAGE_PER_WORD) * USAGE_BITS;

    /* construct a field and mask of the bits to change. For example, to change
     * pages 2 and 3 from DC to FF, build the following:
     * new_field = 0000FF00
     * old_field = 0000DC00
     * mask      = 0000FF00
     */
    if (count == 1U)
    {
        /* simple case, one page */
        mask = ((uintptr_t) USAGE_MASK) << shift_left;
        new_field = ((uintptr_t) after) << shift_left;
        old_field = ((uintptr_t) before) << shift_left;
    }
    else
    {
        /* complex case, multiple pages. The first page should be 'before', the
         * following pages should be USAGE_CONTINUE if 'before' != USAGE_FREE
         */
        uint_fast8_t shift_right = (PAGE_PER_WORD - count) * USAGE_BITS;
        uintptr_t usagemask = USAGE_BITPATTERN;
        uintptr_t contmask;
        usagemask >>= shift_right;
        usagemask <<= shift_left;
        mask = USAGE_MASK * usagemask;
        contmask = USAGE_BITPATTERN >> (shift_right + USAGE_BITS);
        contmask <<= shift_left + USAGE_BITS;
        contmask *= USAGE_CONTINUE;
        if (before == USAGE_FREE)
        {
            old_field = usagemask * USAGE_FREE;
        }
        else
        {
            old_field = contmask | ((uintptr_t) before << shift_left);
        }
        if (after == USAGE_FREE)
        {
            new_field = usagemask * USAGE_FREE;
        }
        else
        {
            new_field = contmask | ((uintptr_t) after << shift_left);
        }
    }

    /* now try to update word */
    _Bool cmpxchg_succeeded;
    entry = atomic_load(ptentry);
    do
    {
        if (((entry ^ old_field) & mask) != 0U)
        {
            /* if the contents we want to change have already changed, fail */
            ret = false;
            break;
        }
        newentry = entry & ~mask;
        newentry |= new_field;
        cmpxchg_succeeded = atomic_compare_exchange_strong(ptentry, &entry, newentry);
    } while (cmpxchg_succeeded == false);
    return ret;
}

static _Bool usage_set_free(lfaimpl_t *heap, page_id_t page, page_count_t count, block_usage_t before)
{
    _Bool ret;
    /* Entries to be freed should be set backwards to keep table consistent.
     * For example, consider the following set of events if we were freeing
     * forward, like for allocation:
     * 1) T1 allocates 4 pages across two words: FF_FF->DC_CC
     * 2) T1 starts freeing:                     DC_CC->FF_CC
     * 3) T2 starts allocating 2 pages:          FF_CC->DC_CC
     * 4) T2 frees its pages (in 2 steps):       DC_CC->FF_FF
     * 5) T1 tries to continue freeing, but sees an already freed range,
     * because T2 already (incorrectly) freed pages 3 and 4.
     * Instead, freeing backwards:
     * 1) T1 allocates 4 pages:           FF_FF->DC_CC
     * 2) T1 starts freeing:              DC_CC->DC_FF
     * 3) T2 allocates 2 pages:           DC_FF->DC_DC
     * 4) T2 frees 2 pages:               DC_DC->DC_FF
     * 5) T1 finishes its free operation: DC_FF->FF_FF
     */
    block_usage_t loopusage = USAGE_CONTINUE;
    page_id_t looppage = page + count;
    page_id_t nextpage = (looppage - 1U) & ~(PAGE_PER_WORD - 1U);
    do
    {
        if (nextpage <= page)
        {
            nextpage = page;
            loopusage = before;
        }
        ret = change_usage_sameword(heap, nextpage, looppage - nextpage, loopusage, USAGE_FREE);
        if (ret == false)
        {
            lfa_error(heap, LFA_ERROR_CHANGE_USAGE_FAILED, (uintptr_t)nextpage);
            break;
        }
        looppage = nextpage;
        nextpage -= PAGE_PER_WORD;
    } while (looppage > page);
    return ret;
}
static _Bool usage_set(
                lfaimpl_t *heap,
                page_id_t page,
                page_count_t count,
                block_usage_t before,
                block_usage_t after)
{
    _Bool ret;
    /* Try to allocate one or more pages within one or more words.
     * If atomic set failed in one word, rollback and let the caller decide
     * if it should retry.
     */
    block_usage_t loopusage = after;
    page_id_t looppage = page;
    const page_id_t endpage = page + count;

    do
    {
        page_id_t nextpage = (looppage & ~(PAGE_PER_WORD - 1U)) + PAGE_PER_WORD;
        if (nextpage > endpage)
        {
            nextpage = endpage;
        }
        ret = change_usage_sameword(heap, looppage, nextpage - looppage, before, loopusage);
        if (ret == false)
        {
            /* we failed to atomically reserve the range. Go out of the loop
             * and start a rollback.
             */
            break;
        }
        looppage = nextpage;
        loopusage = USAGE_CONTINUE;
    } while (looppage < endpage);

    if (looppage < endpage)
    {
        /* Reservation failed, so rollback here.
         * Like in the above free case, rollback in reverse.
         */
        while (looppage > page)
        {
            page_id_t nextpage = (page_id_t)round_up((uintptr_t)looppage - PAGE_PER_WORD, PAGE_PER_WORD);
            if (nextpage <= page)
            {
                nextpage = page;
                loopusage = after;
            }
            _Bool err = change_usage_sameword(heap, nextpage, looppage - nextpage, loopusage, USAGE_FREE);
            if (err == false)
            {
                /* now we have a serious issue, someone corrupted our own
                 * entries.
                 */
                lfa_error(heap, LFA_ERROR_CHANGE_USAGE_FAILED, (uintptr_t)nextpage);
                break;
            }
            looppage = nextpage;
        }
    }
    return ret;
}

/* returns a valid page ID successfully reserved with USAGE_DATA,
 * or 0 if no suitable page range could be found.
 */
static page_id_t page_alloc(lfaimpl_t *heap, page_count_t count)
{
    page_id_t first_free_page = heap->first_free_page;
    page_id_t startat = first_free_page / PAGE_PER_WORD;
    page_id_t endat = (heap->page_count - 1U) / PAGE_PER_WORD;
    page_id_t page_word = startat;
    intptr_t remaining = (intptr_t)count;
    page_id_t found = 0U;

    while (page_word <= endat)
    {
        const uintptr_t word = heap->start[page_word];
        if (word == (USAGE_FREE * USAGE_BITPATTERN))
        {
            /* easy case, nothing reserved */
            if (found == 0U)
            {
                found = page_word * PAGE_PER_WORD;
            }
            remaining -= (intptr_t)PAGE_PER_WORD;
        }
        else if (found == 0U)
        {
            /* Starting a new sequence, with a non-empty word.
             * Check if there is a potential candidate.
             */
            uint_fast8_t local_count;

            if (remaining > (intptr_t)PAGE_PER_WORD)
            {
                local_count = PAGE_PER_WORD;
            }
            else
            {
                local_count = (uint_fast8_t)remaining;
            }

            uintptr_t mask = find_free_sequence(word, local_count);
            if (mask != 0U)
            {
                /* There may be multiple proposed entries in the mask.
                 * Mask all but the LSB */
                mask ^= mask - 1U;
                /* Shift so the LSB is correctly placed */
                mask >>= USAGE_BITS;
                const uint_fast8_t bitcount = LFA_CLZ(mask) / USAGE_BITS;
                found = (page_word * PAGE_PER_WORD) + (PAGE_PER_WORD - bitcount);
                remaining -= (intptr_t)local_count;
            }
            else
            {
                /* Sequence is unusable. If there are leading free entries, we may be
                 * able to use this for a sequence beginning.
                 */
                const uint_fast8_t bitcount = LFA_CLZ(word) / USAGE_BITS;
                if (bitcount > 0U)
                {
                    found = (page_word * PAGE_PER_WORD) + (PAGE_PER_WORD - bitcount);
                    remaining -= (intptr_t)bitcount;
                }
                else
                {
                    found = 0U;
                    remaining = (intptr_t)count;
                }
            }
        }
        else
        {
            /* Continuing a new sequence, with a non-empty word */
            uint_fast8_t local_count = (LFA_FFS(word) - 1U) / USAGE_BITS;
            if ((intptr_t)local_count < remaining)
            {
                /* unsuitable sequence, restart */
                remaining = (intptr_t)count;
                found = 0U;
                /* The current word is unusable for the end of the sequence, but
                 * may be used for the beginning of a sequence, or there may be other
                 * available entries. So, try again on this word.
                 */
                uintptr_t mask = find_free_sequence(word, 1U);
                if (mask != 0U)
                {
                    page_word--;
                }
            }
            else
            {
                /* found a suitable end of sequence */
                remaining = 0;
            }
        }
        if (remaining <= 0)
        {
            /* found a location, try to reserve */
            _Bool err = usage_set(heap, found, count, USAGE_FREE, USAGE_DATA);
            if (err != false)
            {
                page_id_t last_used_page = heap->last_used_page;
                if (last_used_page < (found + count - 1U))
                {
                    /* while we do an atomic modification, we do not attempt to
                     * retry if we failed: if we failed, someone else must have
                     * changed last_used_page, most probably by increasing the
                     * value. */
                    atomic_compare_exchange_strong(&heap->last_used_page, &last_used_page, found + count - 1);
                }
                page_id_t local_first_free_page = heap->first_free_page;
                if (found == local_first_free_page)
                {
                    /* while we do an atomic modification, we do not attempt to
                     * retry if we failed: if we failed, someone else must have
                     * changed first_free_page, most probably by decreasing the
                     * value. */
                    atomic_compare_exchange_strong(&heap->first_free_page, &local_first_free_page, found + count);
                }
                break;
            }
            else
            {
                /* we failed reserving. Try again, starting from found, in case
                 * the same word has another available slot.
                 */
                page_word = (found / PAGE_PER_WORD) - 1U;
                found = 0U;
                remaining = (intptr_t)count;
            }
        }
        page_word++;

        /* scanning goes in 2 phases:
         * 1) from first_free_page to page_count
         * 2) from reserved_pages to first_free_page+count
         */
        if (page_word > endat)
        {
            /* end of first phase, we found nothing. Try to scan from beginning
             * of heap. */
            if ((startat > (heap->reserved_pages / PAGE_PER_WORD))
                            && (endat == ((heap->page_count - 1U) / PAGE_PER_WORD)))
            {
                if ((startat + (count / PAGE_PER_WORD)) < endat)
                {
                    endat = startat + (count / PAGE_PER_WORD);
                }
                startat = heap->reserved_pages / PAGE_PER_WORD;
                page_word = startat;
            }
            remaining = (intptr_t)count;
            found = 0U;
        }
    }
    if (remaining > 0)
    {
        found = 0U;
    }
    return found;
}

static void page_free(lfaimpl_t *heap, page_id_t page, page_count_t count)
{
    page_id_t last_used_page;
    page_id_t first_free_page;
    _Bool err = usage_set_free(heap, page, count, USAGE_DATA);
    if (err == false)
    {
        lfa_error(heap, LFA_ERROR_CHANGE_USAGE_FAILED, (uintptr_t) page_address(heap, page));
    }
    else
    {
        last_used_page = heap->last_used_page;
        if (last_used_page == (page + count - 1U))
        {
            /* while we do an atomic modification, we do not attempt to
             * retry if we failed: if we failed, someone else must have
             * changed last_used_page, most probably by increasing the
             * value.
             */
            atomic_compare_exchange_strong(&heap->last_used_page, &last_used_page, page - 1U);
        }
        first_free_page = heap->first_free_page;
        if (page < first_free_page)
        {
            /* while we do an atomic modification, we do not attempt to
             * retry if we failed: if we failed, someone else must have
             * changed first_free_page, most probably by decreasing the
             * value.
             */
            atomic_compare_exchange_strong(&heap->first_free_page, &first_free_page, page);
        }
    }
}

/* block_alloc() reserves an entry in the proposed block group.
 * Because the block group could be freed without locking, reservation must be
 * done in two steps: first decrease the free count in a fixed location, then
 * complete the reservation by flagging the bitmap inside the block group.
 */
static void *block_alloc(
                lfaimpl_t *heap,
                page_id_t page,
                bgroup_word_t *word,
                uint_fast8_t idx_in_block_id,
                uint8_t block_idx)
{
    bgroup_word_t atom;
    _Bool found = false;
    void *p = NULL;
    uintptr_t *block = page_address(heap, page);
    atom.word = atomic_load((uintptr_t* )word);
    if (((uint8_t)atom.entries[idx_in_block_id].id - 1U) == block_idx)
    {
        /* first stage: decrease free block count */
        while ((uint16_t)atom.entries[idx_in_block_id].free_count > 0U)
        {
            bgroup_word_t newatom = atom;
            uint16_t new_free_count = (uint16_t)newatom.entries[idx_in_block_id].free_count - 1U;
            newatom.entries[idx_in_block_id].free_count = new_free_count;
            /* In case the block is no longer the same as we expected,
             * refuse reservation. */
            block_usage_t usage = get_usage(heap, page);
            if ((usage != USAGE_BLOCK) || (((uint8_t)atom.entries[idx_in_block_id].id - 1U) != block_idx))
            {
                break;
            }
            _Bool cmpxchg_succeeded = atomic_compare_exchange_strong((uintptr_t* )word, &atom.word, newatom.word);
            if (cmpxchg_succeeded != false)
            {
                found = true;
                break;
            }
            else
            {
                if (((uint8_t)atom.entries[idx_in_block_id].id - 1U) != block_idx)
                {
                    /* if the block type no longer corresponds to the expected one,
                     * return and try another one. */
                    break;
                }
            }
        }
    }
    /* second stage: find where the free block is in the bitmap and reserve */
    if (found != false)
    {
        const uint8_t block_type = (uint8_t)atom.entries[idx_in_block_id].id - 1U;
        const uint_fast32_t word_count = heap->bgroup_info[block_type].word_count;
        uint_fast16_t idx = 0U;
        uintptr_t bit;
        /* This infinite loop is required for cases where fast threads always
         * steal free slots in the bitmap. This thread will scan words forward,
         * and if a forward scan led to no result, it should retry, because the
         * above free_count reservation guarantees there is a free entry ; we
         * just have to find it _and_ atomically reserve it.
         */
        for (;;)
        {
            uintptr_t oldword;
            uintptr_t newword;
            oldword = atomic_load(&block[idx]);
            _Bool cmpxchg_succeeded;
            do
            {
                /* set lowest cleared bit */
                newword = oldword | (oldword + 1U);
                /* isolate toggled bit if there is one */
                bit = newword ^ oldword;
                /* note that bit contains the location of the bit, not its index
                 * i.e. 0010000, not 4.
                 */
                if (bit == 0U)
                {
                    break;
                }
                /* try to set the bit */
                cmpxchg_succeeded = atomic_compare_exchange_strong(&block[idx], &oldword, newword);
            } while (cmpxchg_succeeded == false);
            if (bit != 0U)
            {
                /* we've found and set a bit, build back the pointer for it */
                const uint_fast8_t bitidx = ((uint_fast8_t)LFA_WORD_BITS - 1U) - LFA_CLZ(bit);
                const uint_fast16_t bsize = heap->bgroup_info[block_type].size;
                uint32_t ptr = ((idx * (uint_fast8_t)LFA_WORD_BITS) + bitidx) * bsize;
                p = &((uint8_t *) block)[ptr];
                break;
            }
            else if (idx < (word_count - 1U))
            {
                idx++;
            }
            else
            {
                idx = 0U;
            }

        }
    }
    return p;
}

static void block_free(lfaimpl_t *heap, void *ptr, uint_fast32_t page)
{
    uintptr_t *block = page_address(heap, page);
    bgroup_word_t *bgroup = &heap->bgroup_start[page / BLOCKINFO_PER_WORD];
    const uint_fast8_t bgroup_idx = page % BLOCKINFO_PER_WORD;
    const uintptr_t offset = (uintptr_t) ptr - (uintptr_t) block;
    const uint8_t id = (uint8_t)bgroup->entries[bgroup_idx].id - 1U;
    const uintptr_t size = heap->bgroup_info[id].size;
    const uint16_t maximum_free_count = heap->bgroup_info[id].count - heap->bgroup_info[id].reserved_blocks;
    uint_fast32_t idx = (uint_fast32_t)(offset / size);
    const uint_fast32_t word_idx = idx / (uint_fast32_t)LFA_WORD_BITS;
    uintptr_t free_block_index;
    uintptr_t bitmask;
    uintptr_t newbitmask;
    bgroup_word_t atom;
    bgroup_word_t newatom;
    _Bool free_bgroup = false;
    if ((idx * size) != offset)
    {
        lfa_error(heap, LFA_ERROR_FREE_INSIDE_ALLOCATED, (uintptr_t) ptr);
        return;
    }
    idx &= (uint_fast32_t)LFA_WORD_BITS - 1U;
    /* stage 1: release block in bitmask */
    bitmask = atomic_load(&block[word_idx]);
    _Bool cmpxchg_succeeded;
    do
    {
        newbitmask = bitmask & ~(1ULL << idx);
        if (bitmask == newbitmask)
        {
            lfa_error(heap, LFA_ERROR_DOUBLE_FREE, (uintptr_t) ptr);
            break;
        }
        /* loop in case someone else changed another bit in the word */
        cmpxchg_succeeded = atomic_compare_exchange_strong(&block[word_idx], &bitmask, newbitmask);
    } while (cmpxchg_succeeded == false);
    /* stage 2: increase freecount, or release block group.
     * If freecount == maximum_free_count - 1, then we're freeing the last used
     * entry. Instead of increasing free_count to maximum_free_count, set it
     * atomically to zero to reserve all entries at once, then release the
     * complete block group. This will help reducing fragmentation of the entire
     * heap, although there is a possibility of performance impact when the
     * application is just doing malloc/free loops at the border of block group
     * allocation.
     * Because the block ID is also in the same block_def, we additionally
     * atomically render the block Id invalid.
     */
    atom.word = atomic_load((uintptr_t* )bgroup);
    do
    {
        newatom = atom;
        if ((uint16_t)bgroup->entries[bgroup_idx].free_count == (maximum_free_count - 1U))
        {
            newatom.entries[bgroup_idx].id = 0U;
            newatom.entries[bgroup_idx].free_count = 0U;
            free_bgroup = true;
        }
        else
        {
            uint16_t new_free_count = (uint16_t)newatom.entries[bgroup_idx].free_count + 1U;
            newatom.entries[bgroup_idx].free_count = new_free_count;
        }
        cmpxchg_succeeded = atomic_compare_exchange_strong((uintptr_t* )bgroup, &atom.word, newatom.word);
    } while (cmpxchg_succeeded == false);

    /* We managed to do the change atomically.
     * If this was a full block reserve, release block group.
     */
    if (free_bgroup != false)
    {
        const page_count_t pcount = heap->bgroup_info[id].page_count;
        page_id_t last_used_page;
        page_id_t first_free_page;
        _Bool err = usage_set_free(heap, page, pcount, USAGE_BLOCK);
        if (err == false)
        {
            /* failing is a serious issue that should not happen since the previous atomic_cmpxchg
             * guarantees only one agent has responsible for freeing the regarding block page.
             */
            lfa_error(heap, LFA_ERROR_FREE_OF_UNKNOWN, (uintptr_t)page);
        }

        last_used_page = heap->last_used_page;
        if (last_used_page == (page + pcount - 1U))
        {
            /* while we do an atomic modification, we do not attempt to
             * retry if we failed: if we failed, someone else must have
             * changed last_used_page, most probably by increasing the
             * value.
             */
            atomic_compare_exchange_strong(&heap->last_used_page, &last_used_page, page - 1);
        }
        first_free_page = heap->first_free_page;
        if (page < first_free_page)
        {
            /* while we do an atomic modification, we do not attempt to
             * retry if we failed: if we failed, someone else must have
             * changed first_free_page, most probably by decreasing the
             * value.
             */
            atomic_compare_exchange_strong(&heap->first_free_page, &first_free_page, page);
        }
    }
    else
    {
        /* The block was not empty, but there is now space.
         * Change the advice if it is the lowest block group.
         */
        free_block_index = heap->bgroup_info[bgroup_idx].first_free;
        if (page < free_block_index)
        {
            /* while we do an atomic modification, we do not attempt to
             * retry if we failed: if we failed, someone else must have
             * changed first_free for the bgroup, most probably by decreasing the
             * value.
             */
            atomic_compare_exchange_strong(&heap->bgroup_info[bgroup_idx].first_free, &free_block_index, page);
        }
    }
}

static uintptr_t *create_blockgroup(
                lfaimpl_t *heap,
                uint8_t idx,
                bgroup_word_t **bgroup,
                uint_fast8_t *bgroup_idx,
                void **ptr)
{
    page_id_t page = page_alloc(heap, heap->bgroup_info[idx].page_count);
    uintptr_t *block = NULL;
    if (page != 0U)
    {
        bgroup_word_t atom;
        bgroup_word_t newatom;
        bgroup_word_t *bgw = &heap->bgroup_start[page / BLOCKINFO_PER_WORD];
        uint_fast8_t bgw_idx = page % BLOCKINFO_PER_WORD;
        /* reset the bitmap */
        block = page_address(heap, page);
        (void)memset(block, 0, heap->bgroup_info[idx].word_count * sizeof(uintptr_t));
        /* reserve some entries for the bitmap itself,
         * plus one block that we pre-reserve.
         */
        block[0] = (1ULL << (heap->bgroup_info[idx].reserved_blocks + 1U)) - 1ULL;
        /* prepare block ID and free_count */
        atom.word = atomic_load((uintptr_t* )&bgw[0]);
        _Bool cmpxchg_succeeded;
        do
        {
            newatom = atom;
            newatom.entries[bgw_idx].id = idx + 1U;
            /* -1 is for pre-removing the pre-reserved block */
            uint16_t new_free_count = heap->bgroup_info[idx].count - heap->bgroup_info[idx].reserved_blocks - 1U;
            newatom.entries[bgw_idx].free_count = new_free_count;
            cmpxchg_succeeded = atomic_compare_exchange_strong((uintptr_t* )bgw, &atom.word, newatom.word);
        } while (cmpxchg_succeeded == false);

        /* Compute the address of the first usable block that we reserved while setting up the group.
         * Because reserved_blocks is 0-based, no need to +1.
         */
        ptr[0] = (void*) ((uintptr_t)block + ((uintptr_t)heap->bgroup_info[idx].size * heap->bgroup_info[idx].reserved_blocks));

        /* make block available */
        _Bool err = usage_set(heap, page, 1U, USAGE_DATA, USAGE_BLOCK);
        if (err == false)
        {
            /* failing is a serious issue that should not happen, since we already
             * allocated the range successfully.
             */
            lfa_error(heap, LFA_ERROR_CHANGE_USAGE_FAILED, (uintptr_t)page);
        }

        /* update the advisory entry only if it was never initialized */
        {
            page_id_t reserved_pages = heap->reserved_pages;
            atomic_compare_exchange_strong(&heap->bgroup_info[idx].first_free, &reserved_pages, page);
        }

        lfa_log(heap, LFA_LOG_CREATEBLOCK, block, (uintptr_t)heap->bgroup_info[idx].size);
        *bgroup = bgw;
        *bgroup_idx = bgw_idx;
    }
    return block;
}

LFA_PURE
LFA_INLINE _Bool check_heap(lfaimpl_t *heap)
{
    _Bool ret = true;

    /* This function is called as each inter-module interface is executed. */
    /* For this reason, it is not appropriate to perform a complete validation check here. */
    /* In principle, as with other standard C functions, the pointer is only checked for non-NULL. */
    if (heap == NULL)
    {
        ret = false;
    }

    return ret;
}

static void account_heap_change(lfaimpl_t *heap, uintptr_t change)
{
    lfa_options_t *opts = heap->opts;
    if (opts != NULL)
    {
        uintptr_t current_heap_usage;
        uintptr_t new_heap_usage;
        current_heap_usage = atomic_fetch_add(&opts->current_heap_usage, change);
        if (opts->reached_threshold != NULL)
        {
            new_heap_usage = current_heap_usage + change;
            if ((current_heap_usage < opts->low_to_high_threshold)
                            && (new_heap_usage > opts->low_to_high_threshold))
            {
                opts->reached_threshold((lfa_t*)heap, new_heap_usage, 1U);
            }
            else if ((current_heap_usage > opts->high_to_low_threshold)
                            && (new_heap_usage < opts->high_to_low_threshold))
            {
                opts->reached_threshold((lfa_t*)heap, new_heap_usage, 0U);
            }
        }
    }
}

void *lfa_aligned_alloc(lfa_t *lheap, size_t alignment, size_t size)
{
    void *ptr = NULL;
    page_id_t page;
    lfaimpl_t *heap = (lfaimpl_t *) lheap;
    size_t used = 0U;
    int32_t new_errno = 0;
    _Bool is_valid_heap = check_heap(heap);
    if (size == 0U)
    {
        /* do nothing, return NULL */
    }
    else if (is_valid_heap == false)
    {
        new_errno = EINVAL;
    }
    else if ((alignment & (alignment - 1U)) != 0U)
    {
        new_errno = EINVAL;
    }
    else if (alignment > heap->pagesize)
    {
        /* we currently do not support alignment bigger than page size */
        new_errno = EINVAL;
    }
    else if (size > ((uintptr_t)heap->end - (uintptr_t)heap->start))
    {
        new_errno = EINVAL;
    }
    else if ((heap->bgroup_count == 0U) || (size > heap->bgroup_info[heap->bgroup_count - 1U].size))
    {
        /* page alloc */
        /* currently not looking at alignment */
        page_count_t count = (page_count_t)((size + heap->pagesize - 1U) >> heap->logpagesize);
        page = page_alloc(heap, count);
        if (page != 0U)
        {
            ptr = page_address(heap, page);
            used = (size_t)count * heap->pagesize;
        }
        else
        {
            new_errno = ENOMEM;
        }
    }
    else
    {
        /* block group alloc */
        uint8_t idx;
        uintptr_t *block;
        bgroup_word_t *bgroup;
        uint_fast8_t bgroup_idx;
        page_id_t first_free;
        const uint_fast8_t bgroup_count = heap->bgroup_count;
        /* find appropriate block size */
        for (idx = 0U; idx < bgroup_count; idx++)
        {
            if (size <= heap->bgroup_info[idx].size)
            {
                break;
            }
        }
        /* in case alignment is bigger than size, use bigger block */
        for (; idx < bgroup_count; idx++)
        {
            if ((heap->bgroup_info[idx].size & (alignment - 1U)) == 0U)
            {
                break;
            }
        }
        /* if block size in conjunction with alignment was not fitting any block
         * size, try to allocate a page.
         */
        if (idx == bgroup_count)
        {
            page = page_alloc(heap, 1U);
            if (page == 0U)
            {
                lfa_error(heap, LFA_ERROR_HEAP_FULL, 0U);
            }
            else
            {
                ptr = page_address(heap, page);
                used = heap->pagesize;
            }
        }
        /* first, try heuristic, which will succeed in most cases */
        if (ptr == NULL)
        {
            first_free = heap->bgroup_info[idx].first_free;
            block_usage_t usage = get_usage(heap, first_free);
            if (usage == USAGE_BLOCK)
            {
                bgroup = &heap->bgroup_start[first_free / BLOCKINFO_PER_WORD];
                ptr = block_alloc(heap, first_free, bgroup, first_free % BLOCKINFO_PER_WORD, idx);
                used = heap->bgroup_info[idx].size;
            }
        }

        /* Heuristic did not work, scan all fitting block groups */
        if (ptr == NULL)
        {
            page_id_t last_used_page = heap->last_used_page;
            page_id_t end_at = last_used_page / PAGE_PER_WORD;
            page_id_t start_at = first_free / PAGE_PER_WORD;
            page_id_t page_word = start_at;
            while (page_word <= end_at)
            {
                uintptr_t word = heap->start[page_word];
                /* find_free_sequence is designed to work on USAGE_FREE,
                 * swap USAGE_BLOCK appears as USAGE_FREE.
                 */
                word ^= (uintptr_t) USAGE_BLOCK * USAGE_BITPATTERN;

                uintptr_t mask = find_free_sequence(word, 1U);
                while (mask != 0U)
                {
                    const uint_fast8_t pageindex = LFA_FFS(mask);
                    page = (page_word * PAGE_PER_WORD) + (pageindex / USAGE_BITS);
                    bgroup = &heap->bgroup_start[page / BLOCKINFO_PER_WORD];
                    bgroup_idx = page % BLOCKINFO_PER_WORD;
                    if (((uint8_t)bgroup->entries[bgroup_idx].id - 1U) == idx)
                    {
                        ptr = block_alloc(heap, page, bgroup, bgroup_idx, idx);
                        if (ptr != NULL)
                        {
                            if (page < first_free)
                            {
                                /* Since we allocated from another block group
                                 * than the advertised lowest free, update the
                                 * lowest free if the value did not change.
                                 * Not checking if the cmpxchg succeeded, because
                                 * if someone else changed the value, they
                                 * probably know better.
                                 */
                                atomic_compare_exchange_strong(
                                                &heap->bgroup_info[idx].first_free,
                                                &first_free, page);
                            }
                            used = heap->bgroup_info[idx].size;
                            break;
                        }
                    }
                    /* mask out the current block group we tried, to try the
                     * next one in the word */
                    mask &= ~(USAGE_MASK << (pageindex - 1U));
                }
                if (ptr != NULL)
                {
                    break;
                }
                page_word++;
                if (page_word > end_at)
                {
                    /* first scan failed, try unadvised ranges */
                    page_id_t new_last_used_page = heap->last_used_page;
                    if (new_last_used_page > last_used_page)
                    {
                        /* if last page changed since we started scanning, try new ranges */
                        end_at = new_last_used_page / PAGE_PER_WORD;
                        start_at = last_used_page / PAGE_PER_WORD;
                        last_used_page = new_last_used_page;
                        page_word = start_at;
                    }
                    else if (start_at != (heap->reserved_pages / PAGE_PER_WORD))
                    {
                        /* try full heap search as fallback */
                        end_at = last_used_page / PAGE_PER_WORD;
                        start_at = heap->reserved_pages / PAGE_PER_WORD;
                        page_word = start_at;
                    }
                }
            }
        }

        /* traversal did not give anything useful, create a new block group */
        if (ptr == NULL)
        {
            block = create_blockgroup(heap, idx, &bgroup, &bgroup_idx, &ptr);
            if (block == NULL)
            {
                lfa_error(heap, LFA_ERROR_HEAP_FULL, 0U);
            }
            else
            {
                used = heap->bgroup_info[idx].size * (heap->bgroup_info[idx].reserved_blocks + 1UL);
            }
        }

        if (ptr == NULL)
        {
            /* Could not find a suitable location, note not enough memory */
            new_errno = ENOMEM;
        }
    }

    if (ptr != NULL)
    {
        lfa_log(heap, LFA_LOG_ALLOC, ptr, size);
        account_heap_change(heap, used);
    }
    else
    {
        if (new_errno != 0)
        {
            errno = new_errno;
        }
    }
    return ptr;
}

void lfa_free(lfa_t *lheap, void *ptr)
{
    lfaimpl_t *heap = (lfaimpl_t *) lheap;
    _Bool is_valid_heap = check_heap(heap);
    if (is_valid_heap == false)
    {
        lfa_error(heap, LFA_ERROR_BAD_HEAP, (uintptr_t) ptr);
    }
    else if (ptr != NULL)
    {
        if ((ptr < (void*) heap->start) || (ptr > (void*) heap->end))
        {
            lfa_error(heap, LFA_ERROR_FREE_OF_UNKNOWN, (uintptr_t) ptr);
        }
        else
        {
            page_id_t entrypage = get_page(heap, ptr);
            page_id_t firstpage = entrypage;
            block_usage_t usage = get_usage(heap, entrypage);
            if (usage == USAGE_CONTINUE)
            {
                /* someone called free in the middle of a block. It may be a
                 * block free, which is fine, or it may be a rogue free().
                 */
                firstpage--;
                while (firstpage > 0U)
                {
                    usage = get_usage(heap, firstpage);
                    if (usage != USAGE_CONTINUE)
                    {
                        break;
                    }
                    firstpage--;
                }
            }
            if (firstpage < heap->reserved_pages)
            {
                /* we reached the beginning of the heap, which contains the page
                 * table. This cannot possibly be correct and the heap is corrupted.
                 */
                lfa_error(heap, LFA_ERROR_FREE_OF_ADMIN, (uintptr_t) ptr);
            }
            else
            {
                switch (usage)
                {
                case USAGE_FREE:
                    lfa_error(heap, LFA_ERROR_DOUBLE_FREE, (uintptr_t) ptr);
                    break;
                case USAGE_BLOCK:
                {
                    /* Log before the actual free in case block_free releases the
                     * block group.
                     */
                    bgroup_word_t *bgroup = &heap->bgroup_start[firstpage / BLOCKINFO_PER_WORD];
                    const uint_fast8_t bgroup_idx = firstpage % BLOCKINFO_PER_WORD;
                    const uint8_t id = (uint8_t)bgroup->entries[bgroup_idx].id - 1U;
                    const uintptr_t bsize = heap->bgroup_info[id].size;
                    block_free(heap, ptr, firstpage);
                    lfa_log(heap, LFA_LOG_FREE, ptr, bsize);
                    account_heap_change(heap, NEG_UINTPTR(bsize));
                }
                    break;
                case USAGE_DATA:
                    if (entrypage != firstpage)
                    {
                        lfa_error(heap, LFA_ERROR_FREE_INSIDE_ALLOCATED, (uintptr_t) ptr);
                    }
                    else if (((uintptr_t) ptr & (heap->pagesize - 1U)) != 0U)
                    {
                        lfa_error(heap, LFA_ERROR_FREE_INSIDE_ALLOCATED, (uintptr_t) ptr);
                    }
                    else
                    {
                        page_count_t pagecount;
                        pagecount = count_pages(heap, entrypage);
                        page_free(heap, entrypage, pagecount);
                        account_heap_change(heap, NEG_UINTPTR(pagecount * heap->pagesize));
                        lfa_log(heap, LFA_LOG_FREE, ptr, pagecount * heap->pagesize);
                    }
                    break;
                case USAGE_CONTINUE:
                default:
                    /* cannot algorithmically happen */
                    break;
                }
            }
        }
    }
}

void *lfa_realloc(lfa_t *lheap, void *ptr, size_t size)
{
    void *p = NULL;
    lfaimpl_t *heap = (lfaimpl_t *) lheap;
    _Bool is_valid_heap = check_heap(heap);
    if (is_valid_heap == false)
    {
        errno = EINVAL;
    }
    else if (ptr == NULL)
    {
        /* Simple case, equivalent to malloc */
        p = lfa_aligned_alloc(lheap, sizeof(void*), size);
    }
    else if (size == 0U)
    {
        /* Simple case, equivalent to free */
        lfa_free(lheap, ptr);
    }
    else if (size > ((uintptr_t)heap->end - (uintptr_t)heap->start))
    {
        errno = EINVAL;
    }
    else
    {
        page_id_t entrypage = get_page(heap, ptr);
        page_id_t firstpage = entrypage;
        block_usage_t usage = get_usage(heap, entrypage);
        size_t old_size = 0U;
        if (usage == USAGE_CONTINUE)
        {
            /* someone called realloc in the middle of a block. It may be a
             * block realloc, which is fine, or it may be a rogue call.
             */
            firstpage--;
            while (firstpage > 0U)
            {
                usage = get_usage(heap, firstpage);
                if (usage != USAGE_CONTINUE)
                {
                    break;
                }
                firstpage--;
            }
        }
        /* we reached the beginning of the heap, which contains the page
         * table. This cannot possibly be correct and the heap is corrupted.
         */
        if (firstpage < heap->reserved_pages)
        {
            lfa_error(heap, LFA_ERROR_FREE_OF_ADMIN, (uintptr_t) ptr);
        }
        else
        {
            bgroup_word_t *bgw;
            bgroup_info_t bgroup;
            uint8_t bgroup_id;
            switch (usage)
            {
            case USAGE_FREE:
                lfa_error(heap, LFA_ERROR_DOUBLE_FREE, (uintptr_t) ptr);
                break;
            case USAGE_BLOCK:
                /* we are trying to resize a block. If it is bigger, we have
                 * to move, but if it is smaller, just leave the allocation
                 * as is to keep things fast.
                 */
                bgw = &heap->bgroup_start[firstpage / BLOCKINFO_PER_WORD];
                bgroup = bgw->entries[firstpage % BLOCKINFO_PER_WORD];
                bgroup_id = (uint8_t)bgroup.id;
                if (bgroup_id == 0U)
                {
                    lfa_error(heap, LFA_ERROR_INTERNAL_ERROR, (uintptr_t) ptr);
                }
                else if (size > heap->bgroup_info[bgroup_id - 1U].size)
                {
                    /* note that if this allocation fails, realloc() will
                     * correctly return NULL because the request failed,
                     * but we will not touch or free the original ptr.
                     */
                    p = lfa_aligned_alloc(lheap, sizeof(void*), size);
                    if (p != NULL)
                    {
                        lfa_log(heap, LFA_LOG_REALLOC, p, size);
                    }
                    old_size = heap->bgroup_info[bgroup_id - 1U].size;
                }
                else
                {
                    /* block size is bigger than necessary, but this is more
                     * efficient than alloc+memcpy+free.
                     */
                    p = ptr;
                }
                break;
            case USAGE_DATA:
                if (entrypage != firstpage)
                {
                    lfa_error(heap, LFA_ERROR_FREE_INSIDE_ALLOCATED, (uintptr_t) ptr);
                }
                else if (((uintptr_t) ptr & (heap->pagesize - 1U)) != 0U)
                {
                    lfa_error(heap, LFA_ERROR_FREE_INSIDE_ALLOCATED, (uintptr_t) ptr);
                }
                else
                {
                    page_count_t new_pagecount = (page_count_t)((size + heap->pagesize - 1U) >> heap->logpagesize);
                    page_id_t page;
                    page_count_t pagecount;
                    pagecount = count_pages(heap, entrypage);
                    if (pagecount < new_pagecount)
                    {
                        page = page_alloc(heap, new_pagecount);
                        if (page != 0U)
                        {
                            p = page_address(heap, page);
                            old_size = (size_t)pagecount << heap->logpagesize;
                            lfa_log(heap, LFA_LOG_REALLOC, p, (uintptr_t)new_pagecount << heap->logpagesize);
                            /* account for the new page allocation, the call
                             * to lfa_free() below will account for the
                             * previous memory location */
                            account_heap_change(heap, (uintptr_t)new_pagecount << heap->logpagesize);
                        }
                    }
                    else if (pagecount > new_pagecount)
                    {
                        /* The current allocation is bigger, try to reduce
                         * it to the minimum necessary by releasing tail
                         * pages.
                         */
                        page_count_t delta_pagecount = pagecount - new_pagecount;
                        _Bool err = usage_set_free(heap, entrypage + new_pagecount, delta_pagecount, USAGE_CONTINUE);
                        if (err == false)
                        {
                            /* now we have a serious issue, someone corrupted the tail of own continuation page.
                             */
                            lfa_error(heap, LFA_ERROR_CHANGE_USAGE_FAILED, (uintptr_t)entrypage + new_pagecount);
                        }
                        lfa_log(heap, LFA_LOG_TAIL_FREE_IN_REALLOC, page_address(heap, entrypage + new_pagecount),
                                        (uintptr_t)delta_pagecount << heap->logpagesize);
                        account_heap_change(heap, NEG_UINTPTR((uintptr_t)delta_pagecount << heap->logpagesize));
                        p = ptr;
                    }
                    else
                    {
                        /* nothing to do, already compatible size */
                        p = ptr;
                    }
                }
                break;
            case USAGE_CONTINUE:
            default:
                /* cannot happen */
                break;
            }
            if ((p != NULL) && (old_size > 0U))
            {
                (void)memcpy(p, ptr, old_size);
                lfa_free(lheap, ptr);
            }
            else if (p == NULL)
            {
                errno = ENOMEM;
            }
        }
    }
    return p;
}

int32_t lfa_setup_heap(lfa_t *lheap)
{
    lfaimpl_t *heap = (lfaimpl_t *) lheap;
    int32_t ret = 0;
    if (heap == NULL)
    {
        ret = EINVAL;
    }
    else if (heap->start == NULL)
    {
        ret = EINVAL;
    }
    else if (heap->end <= heap->start)
    {
        ret = EINVAL;
    }
    else if (heap->logpagesize == 0U)
    {
        ret = EINVAL;
    }
    /* Heap start must be aligned to the required page size */
    else if (((uintptr_t)heap->start & (((uintptr_t)1 << heap->logpagesize) - 1U)) != 0U)
    {
        ret = EINVAL;
    }
#if LFA_LOG_SIZE != 0
    else if ((heap->opts != NULL)
                    && (heap->opts->log_count > 0U)
                    && (heap->opts->log_array == NULL))
    {
        ret = EINVAL;
    }
#endif /* LFA_LOG_SIZE != 0 */
    else
    {
        uint_fast8_t i;
        uintptr_t heap_size;
        uintptr_t reserved;
        if (heap->max_end < heap->end)
        {
            heap_size = (uintptr_t)heap->end;
        }
        else
        {
            heap_size = (uintptr_t)heap->max_end;
        }
        heap_size -= (uintptr_t)heap->start;
        heap->pagesize = 1ULL << heap->logpagesize;
        heap->page_count = (page_id_t)(heap_size >> heap->logpagesize);
        heap->ptsize = round_up((uintptr_t)heap->page_count, PAGE_PER_BYTE) / PAGE_PER_BYTE;
        reserved = round_up(heap->ptsize, sizeof(uintptr_t));
        if (heap->bgroup_count > 0U)
        {
            heap->bgroup_start = (bgroup_word_t*)((uintptr_t)heap->start + reserved);
            reserved += heap->page_count * sizeof(bgroup_info_t);
        }
        (void)memset(heap->start, 0, reserved);
        heap->reserved_pages = (page_id_t)(round_up(reserved, heap->pagesize) >> heap->logpagesize);
        heap->first_free_page = heap->reserved_pages;
        heap->last_used_page = heap->first_free_page;
        _Bool err = usage_set(heap, 0U, heap->first_free_page, USAGE_FREE, USAGE_CONTINUE);
        if (err == false)
        {
            /* Failed to setup a heap due to concurrent setup. */
            ret = EBUSY;
        }

        /* If the size is not page word aligned, change to allocate the last unusable area. */
        if ((err != false) && ((heap->page_count % PAGE_PER_WORD) != 0U))
        {
            page_count_t invalid_page_count = PAGE_PER_WORD - (heap->page_count % PAGE_PER_WORD);
            err = usage_set(heap, heap->page_count, invalid_page_count, USAGE_FREE, USAGE_DATA);
            if (err == false)
            {
                /* Failed to setup a heap due to concurrent setup. */
                ret = EBUSY;
            }
        }

        if (ret == 0)
        {
            for (i = 0U; i < heap->bgroup_count; i++)
            {
                bgroup_def_t *def = &heap->bgroup_info[i];
                if ((def->size == 0U) || (def->word_count == 0U))
                {
                    ret = EINVAL;
                    break;
                }
                def->count = def->word_count * (uint16_t)LFA_WORD_BITS;
                uintptr_t block_page_size = (uintptr_t)def->count * def->size;
                def->page_count = (page_count_t)(block_page_size + heap->pagesize - 1U) >> heap->logpagesize;
                def->reserved_blocks = (uint8_t)(((def->word_count * sizeof(uintptr_t)) + def->size - 1U) / def->size);
                def->first_free = heap->reserved_pages;
            }
        }
    }
    if (ret == 0)
    {
        if (heap->opts != NULL)
        {
            heap->opts->current_heap_usage = (uintptr_t)heap->reserved_pages << heap->logpagesize;
#if LFA_LOG_SIZE != 0
            heap->opts->log_current_entry = 0U;
#endif /* LFA_LOG_SIZE != 0 */
        }
    }
    return ret;
}
