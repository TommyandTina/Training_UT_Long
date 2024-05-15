/********************************************************************************
    Atomic Library
     Copyright (C) 2016-2018 Renesas Electronics Corporation, all rights reserved.

    [File] allocator.c

********************************************************************************/
#include <stdlib.h>
#include "base.h"
#include "logger.h"

#if defined(__CYGWIN__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
#define MAX_HEAP_ALLOCATE_SIZE  0xffffffff

#elif defined(__WINAMS__)

//refs. sbrk.c
#define HEAP_BLOCK_SIZE			(1200*1024)	// 1.2M
#define HEAP_BLOCK_COUNT		(4)
#define MAX_HEAP_ALLOCATE_SIZE  (HEAP_BLOCK_SIZE * HEAP_BLOCK_COUNT)

#else
#include "g_memdef.h"

//refs. configTOTAL_HEAP_SIZE @ FreeRTOSConfig.h
#define HEAP_SIZE               (512 * 1024 * 1024)
#define MAX_HEAP_ALLOCATE_SIZE  (HEAP_SIZE - (500 * 1024 * 1024))
#endif

#if defined(__WINAMS__)
static bool m_heapInitialized = false;
struct HeapArea {
	uint32_t area[HEAP_BLOCK_COUNT][HEAP_BLOCK_SIZE/sizeof(uint32_t)];
	uint32_t assigned;
};

static struct HeapArea m_heapArea[32];
static void *allocMemory(size_t __size)
{
	if (!m_heapInitialized)
	{
		for (int i = 0; i < ARRAY_COUNT(m_heapArea); i++)
		{
			m_heapArea[i].assigned = 0U;
		}

		m_heapInitialized = true;
	}

	if (__size < HEAP_BLOCK_SIZE)
	{
		for (int i = 0; i < ARRAY_COUNT(m_heapArea); i++)
		{
			for (int j = 0; j < HEAP_BLOCK_COUNT; j++)
			{
				if ((m_heapArea[i].assigned & (1 << j)) == 0U)
				{
					m_heapArea[i].assigned |= (1 << j);
					return &m_heapArea[i].area[j][0];
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < ARRAY_COUNT(m_heapArea); i++)
		{
			if (m_heapArea[i].assigned == 0U)
			{
				m_heapArea[i].assigned = 1U;
				return &m_heapArea[i].area[0][0];
			}
		}
	}

	return NULL;
}

static void freeMemory(void * p)
{
	for (int i = 0; i < ARRAY_COUNT(m_heapArea); i++)
	{
		for (int j = 0; j < HEAP_BLOCK_COUNT; j++)
		{
			if ((uintptr_t)p == (uintptr_t)&m_heapArea[i].area[j][0])
			{
				m_heapArea[i].assigned &= ~(1U << j);
			}
		}
	}
}

#endif


#if defined(__CYGWIN__) || defined(__WINAMS__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
#else
#define MEM_ALIGN	(256)
void* freeRtosAllocate(int32_t size, int32_t align)
{
	void *org_addr, *addr = NULL;

	if ((align & (align - 1)) != 0)
	{
		return NULL; /* error: align is not 2^n */
	}

	if (align < (int64_t)sizeof(void*))
	{
		align = (int64_t)sizeof(void*);
	}

	size +=  (int64_t)sizeof(void*) + align - 1;

#if 1
	org_addr = (void*)rg_malloc((size_t)size, MEM_REGION_CACHE);
#else
	org_addr = pvPortMalloc(size);
#endif

	if (org_addr != NULL)
	{
		addr = (void*)(((uintptr_t)org_addr + sizeof(void*) + (uint64_t)align - 1) & ~(uintptr_t)(align - 1));
		*((void**)addr - 1) = org_addr;
	}

//	ILOG("freeRtosAllocate() addr=%p, org_addr=%p size=%d, align=%d", addr, org_addr, size, align);

	return addr;
}

void freeRtosFree(void* addr)
{
	if (addr != NULL)
	{
		void* org_addr = *((void**)addr - 1);
#if 1
		rg_free(org_addr);
#else
		vPortFree(org_addr);
#endif
	}
}
#endif

void *heapMalloc(size_t __size)
{
	if (__size >= MAX_HEAP_ALLOCATE_SIZE)
	{
//        WLOG("heapMalloc() size is too large !! fix %d ===> %d", __size, MAX_HEAP_ALLOCATE_SIZE);
        __size = MAX_HEAP_ALLOCATE_SIZE;
	}

#if defined(__CYGWIN__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
	return malloc(__size);
#elif defined(WIN32)
	void* m = malloc(__size);
	memset(m, 0, __size);
	return m;
#elif defined(__WINAMS__)
	return allocMemory(__size);
#else
	return freeRtosAllocate((int32_t)__size, MEM_ALIGN);
#endif
}

void heapFree(void * p)
{
#if defined(__CYGWIN__) || defined(WINDOWS_OS) || defined(QNX_OS) || defined(LINUX_OS)
	free(p);
#elif defined(__WINAMS__)
	freeMemory(p);
#else
	freeRtosFree(p);
#endif
}

