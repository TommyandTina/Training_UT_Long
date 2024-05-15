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
 uring.c   Implementation of uring functions
============================================================================*/

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <mcos/uring.h>
#include <mcos/pdev.h>
#include <sys/lfs.h>
#include <mcos/atomic.h>
//#include <posix_internal.h>
#include "uring_impl.h"

#include "stub.h"

/** Max number of buffer in a sector
 * The size size of the bitmap variable (of type bitmap_t)        \
 * depends on the maximum size of the variable on which           \
 * atomic operations are available. The maximum number of         \
 * buffers in a sector is therefore depends on the bitmap_t type. \
 */
#define MAX_SECTOR_BN (8 * __SIZEOF_POINTER__)

/**
 * @brief Get entry from head of the queue locklessly
 * @param[in] ring The uring object pointer
 * @param[out] e A new gotten entry
 * @retval 0 [Success]
 * @retval -1 with POSIX errono [Fail]
 * @un eMCOS Uring
 * @note
 * circular buffer design
 *  H:head, T:tail, F:free

 * Empty queue (H==T):
 * |  H T  |       |       |       |       |       |       |       | (pointers) |
 * |-------|-------|-------|-------|-------|-------|-------|-------|------------|
 * |       |       |       |       |       |       |       |       | (value)    |

 * One element queue:
 * |   H   |   T   |       |       |       |       |       |       |
 * |-------|-------|-------|-------|-------|-------|-------|-------|
 * |   1   |       |       |       |       |       |       |       |

 * Some elements:
 * |   T   |       |       |       |   H   |       |       |       |
 * |-------|-------|-------|-------|-------|-------|-------|-------|
 * |       |       |       |       |   5   |   6   |   7   |   8   |

 * One element empty:
 * |       |       |       |   T   |   H   |       |       |       |
 * |-------|-------|-------|-------|-------|-------|-------|-------|
 * |   9   |  10   |  11   |       |   5   |   6   |   7   |   8   |

 * Full queue (T-H == array_len): H is 4, T is 12
 * |       |       |       |       |  H T  |       |       |       |
 * |-------|-------|-------|-------|-------|-------|-------|-------|
 * |   9   |  10   |  11   |  12   |   5   |   6   |   7   |   8   |
 */
static int32_t uring_entry_get(struct uring *ring, struct uring_e **e);

/**
 @brief Check the gotten entry
 @param[in] ring The uring object pointer
 @param[in] e A new gotten entry
 @retval 0 [Success]
 @retval -1 [Fail]
 */
static int32_t uring_entry_check(struct uring *ring, struct uring_e *e);

static int32_t do_lock_uring_space(void *r, size_t ring_size, size_t ring_mng_size, _Bool used_from_intctx)
{
    int32_t ret;

    /* Lock all space but data area:
     * The send algorithm disables interrupt
     * during its processing for mutual exclusion.
     * Therefore, undesirable exception will happen when
     * page fault occurs while disabling interrupt. 
     * Locking the management memory area can guarantee
     * that page fault will never happen while
     * disabling interrupt because only the management
     * area is updated at that time.
     * The reason not to lock entire space is to
     * save physical memory because we assume
     * that a larger size may be allocated.
     * 
     * When a uring is used in interrupt context,
     * the further memory lock will be issued when
     * registering it as a channel between ISR thread and ISR.
     */
    ret = mlock(r, ring_mng_size);

    if (ret != 0)
    {
        ret = errno;
    }

    return ret;
}
    
/**
 * @note
 * Construct the following memory layout associated with the uring object:
 *  *
 *  *   uring_t               
 *  *   |                                                                                                
 *  *   `--> |--------------|------------------|--------------------|
 *  *        | Struct uring | Ring Buffer Area | Data Page          |
 *  *        |--------------|------------------|--------------------|
 *  *                       ^                  ^
 *  *                       |                  |
 *  *                       uring_queue()      uring_data()
 *  *   
 * Data page layout is the following:
 *  *
 *  *   uring_data()
 *  *   |                                                                                                
 *  *   `--> |------------|---------|------------|
 *  *        | sector 1   |  ....   | sector n   |
 *  *        |------------|---------|------------|
 *  * 
 * Each sector layout is the following:
 *  *   
 *  *        |------------|---------|------------|
 *  *        | buf 1      |  ....   | sector n   |
 *  *        |------------|---------|------------|
 *  *   
 *  Supplement
 *  - The member `struct uring_q` manages Ring buffer area.
 *  - The members, sector_buff_sz, sector_buff_n, bitmap manage Data page.
 *  - The ring buffer queue length will be round up if it is not power of two.
 */
static int32_t do_uring_create(uring_t *r, uring_par_t *par, _Bool used_from_intctx)
{
    int32_t sta = 0;

    /* Check input parameters */
    if ((r == NULL) || (par == NULL))
    {
        sta = EINVAL;
    }
    else if ((par->n_sectors == 0) || (par->n_sectors > MAX_N_SECTORS))
    {
        sta = EINVAL;
    }
    else if ((par->opt & ~OPT_MASK) != 0U)
    {
        sta = EINVAL;
    }
    else if (par->qlen == 0)
    {
        sta = EINVAL;
    }
    else
    {
        size_t queue_size;
        size_t ring_size;
        size_t data_size = 0U;
        size_t ring_mng_size;
        uintptr_t queue_head_offset;
        uintptr_t data_head_offset;
        _Bool qlen_is_pow = is_pow(par->qlen);

        if (qlen_is_pow == false)
        {
            /* Round up the queue length to the power of two for efficient index management */
            uint16_t ret = 1U;
            uint16_t n = par->qlen;
            while (n > 0)
            {
                ret <<= 1U;
                n >>= 1U;
            }
            par->qlen = ret;
        }
        queue_size = sizeof(entry_t) * par->qlen;

        /* Reserve management data area */
        for (uint8_t i = 0U; (i < par->n_sectors) && (sta == 0); i++)
        {
            par->sector_bsz[i] += MIN_BUFF_SIZE;
        }

        /* check parameters structure and compute data memspace size */
        for (uint8_t i = 0U; (i < par->n_sectors) && (sta == 0); i++)
        {
            if ((par->sector_bn[i] > MAX_SECTOR_BN) || (par->sector_bn[i] == 0))
            {
                /* check buffer number range (0-64) */
                sta = EINVAL;
            }
            else if ((i > 0) && (par->sector_bsz[i] < par->sector_bsz[i - 1]))
            {
                /* Check that buffers are in ascending order */
                sta = EINVAL;
            }
            else
            {
                data_size += par->sector_bsz[i] * par->sector_bn[i];
            }
        }

        /* If par struct was ok, allocate ring and init it */
        if (sta == 0)
        {
            /* Construct ring memory area size
             * with alignment for efficient accesses
             */

            queue_head_offset = sizeof(struct uring);
            if ((queue_head_offset % URING_WORD_BITS) != 0ULL)
            {
                /* sizeof(struct uring) + padding */
                queue_head_offset += (URING_WORD_BITS - (queue_head_offset % URING_WORD_BITS));
            }

            ring_mng_size = queue_head_offset + queue_size;
            if ((ring_mng_size % URING_WORD_BITS) != 0ULL)
            {
                /* sizeof(struct uring) + padding + queue_size + padding */
                ring_mng_size += (URING_WORD_BITS - (ring_mng_size % URING_WORD_BITS));
            }

            /* data_head_offset has already been aligned */
            data_head_offset = ring_mng_size;

            /* It is the entire uring size */
            ring_size = ring_mng_size + data_size;

            *r = mmap(NULL, ring_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
            if (*r == MAP_FAILED)
            {
                sta = ENOMEM;
            }
            else
            {
                sta = do_lock_uring_space(*r, ring_size, ring_mng_size, used_from_intctx);
            }
        }

        if ((sta == 0) && (*r != NULL))
        {
            struct uring *ring = (struct uring *)*r;
            uintptr_t uring_data;

            /* clear ring */
            (void)memset(ring, 0, sizeof(struct uring));

            /* set total ring size */
            ring->size = ring_size;

            /* set ring options */
            ring->opt = par->opt;

            /* save len of circular buffer */
            ring->queue.max_entries = par->qlen;

            /* Init (as 0) size counting semaphore */
            (void)sem_init(&(ring->queue.size), 1, 0U);

            /* save offset to queue memspace starting after struct uring + alignment */
            ring->queue_off = queue_head_offset;

            /* save offset to data memspace starting after struct uring + circular buffer + alignment */
            ring->data = data_head_offset;

            /* info section init and calcuate data size */
            ring->n_sectors = par->n_sectors;
            for (uint8_t i = 0U; (i < par->n_sectors) && (sta == 0); i++)
            {
                /* already checked the validity of the parameters */
                ring->sector_buff_sz[i] = par->sector_bsz[i];
                ring->sector_buff_n[i] = par->sector_bn[i];
            }

            /* Clear data memrange */
            uring_data = get_data_page(ring);
            (void)memset((void *)uring_data, 0, data_size);
        }
    }

    return sta;
}

int32_t uring_create(uring_t *r, uring_par_t *par)
{
    return do_uring_create(r, par, false);
}

int32_t uring_interrupt_create(uring_t *r, int32_t interrupts_count)
{
    int32_t sta = EINVAL;

    if (false == g_stub_start)
    {
        if ((interrupts_count > 0) && (interrupts_count <= (int32_t)URING_WORD_BITS))
        {
            uring_par_t par;
            par.qlen = (uint16_t)interrupts_count;
            par.n_sectors = 1U;
            par.sector_bn[0] = (uint8_t)interrupts_count;
            par.sector_bsz[0] = sizeof(struct lfs_devgetdone);
            par.opt = OPT_NONE;
            sta = do_uring_create(r, &par, true);
        }
    }
    else
    {
        sta = 0;
        *r = (uring_t*)&sta;
    }

    return sta;
}

int32_t uring_destroy(uring_t r)
{
    int32_t sta = EINVAL;

    if (false == g_stub_start)
    {
        if (r != NULL)
        {
            struct uring *ring = (struct uring *)r;
            size_t size = ring->size;
            sta = munmap(r, size);
            if (sta != 0)
            {
                sta = errno;
            }
        }
    }
    else
    {
        sta = 0;
    }

    return sta;
}

static int32_t uring_entry_get(struct uring *ring, struct uring_e **e)
{
    int32_t sta = 0;
    struct uring_q *q = &ring->queue;
    entry_t *uring_queue = get_queue_head(ring);
    entry_t eoff = 0;
    uint16_t head;
    _Bool empty;
    _Bool comp = false;

    do
    {
        uint16_t head_idx;
        uint16_t next_head;

        empty = queue_empty(q);
        if (empty != false)
        {
            break;
        }

        /* Load entry */
        head_idx = get_queue_head_idx(q);
        eoff = atomic_load(&uring_queue[head_idx]);

        /* Make that the entry is not observed by others */
        head = q->head;
        next_head = head + 1; /* do not mask, will count and overflow */
        comp = atomic_compare_exchange_strong(&q->head, &head, next_head);
    } while (comp != true);

    /* If comp != true, queue was empty */
    if (comp != true)
    {
        sta = EWOULDBLOCK;
    }
    else
    {
        /* Get entry pointer from entry offset */
        uintptr_t uring_data = get_data_page(ring);
        *e = (struct uring_e *)((uintptr_t)uring_data + eoff);
    }

    return sta;
}

static int32_t uring_entry_check(struct uring *ring, struct uring_e *e)
{
    int32_t sta = -1;
    uintptr_t uring_data = get_data_page(ring);

    if ((uintptr_t)e > ((uintptr_t)ring + ring->size - sizeof(struct uring_e)))
    {
        /* Entry is after ring memory */
    }
    else if ((uintptr_t)e < ((uintptr_t)uring_data))
    {
        /* Entry is before ring data memory */
    }
    else if ((e->type & ~ENTRY_TOT_MAP) != 0U)
    {
        /* Wrong entry type */
    }
    else if (e->sector >= ring->n_sectors)
    {
        /* Entry sector is out of range */
    }
    else if (e->bitmask == 0UL)
    {
        /* Entry bitmask is zero */
    }
    else
    {
        sta = 0;
    }

    return sta;
}

int32_t uring_interrupt_receive(uring_t r, void *data, size_t data_size)
{
    int32_t sta;

    if (false == g_stub_start)
    {
        if (r == NULL)
        {
            sta = EINVAL;
        }
        else if ((data == NULL) || (data_size == 0U))
        {
            sta = EINVAL;
        }
        else
        {
            struct uring *ring = (struct uring *)r; /* Get ring from server virtual pointer */
            struct uring_e *e = NULL;

            /* blocking */
            sta = sem_wait(&ring->queue.size);
            if (sta == 0)
            {
                sta = uring_entry_get(ring, &e);
            }
            if (sta == 0)
            {
                sta = uring_entry_check(ring, e);
                if (sta != 0)
                {
                    /* Fatal error */
                    sta = EBADMSG;
                }
                else
                {
                    void *req = (void *)((uintptr_t)e + sizeof(struct uring_e));
                    if (e->req_size != data_size)
                    {
                        /* Not desired message */
                        sta = ENOMSG;
                    }
                    else
                    {
                        (void)memcpy(data, req, data_size);
                    }

                    /* Release data buffer */
                    (void)atomic_fetch_and(&ring->bitmap[e->sector], ~(e->bitmask));
                }
            }
        }
    }
    else
    {
        sta = 0;
    }

    return sta;
}

int32_t uring_rr_receive_reply_sem_rel(uring_t r)
{
    int32_t sta = 0;
    struct uring *ring = (struct uring *)r;

    if (r == NULL)
    {
        sta = EINVAL;
    }
    else
    {
        /* Wait for the message */
        sta = sem_wait(&ring->queue.size);
    }

    return sta;
}
