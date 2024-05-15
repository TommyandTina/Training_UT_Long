#ifndef _TAURUS_MEMORY_BARRIER_H_
#define _TAURUS_MEMORY_BARRIER_H_

#include "rcar-xos/osal/r_osal.h"
#include "target/freertos/r_osal_common.h"
#include "r_print_api.h"

static inline void taurus_memory_cache_invalidate(void *addr, uint32_t size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_ret = R_OSAL_CacheInvalidate((size_t)addr, (size_t)size, R_OSAL_CACHE_D);
    if (OSAL_RETURN_OK != osal_ret) {
        R_PRINT_Log("[TaurusMemory]: Cache Invalidate failed(%d)\n", osal_ret);
    }
}

static inline void taurus_memory_cache_flush(void *addr, uint32_t size)
{
    e_osal_return_t osal_ret = OSAL_RETURN_OK;
    osal_ret = R_OSAL_CacheFlush((size_t)addr, (size_t)size, R_OSAL_CACHE_D);
    if (OSAL_RETURN_OK != osal_ret) {
        R_PRINT_Log("[TaurusMemory]: Cache Flush failed(%d)\n", osal_ret);
    }
}

#endif /* _TAURUS_MEMORY_BARRIER_H_ */

