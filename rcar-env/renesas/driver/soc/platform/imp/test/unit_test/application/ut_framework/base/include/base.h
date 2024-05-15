#ifndef _BASE_H_
#define _BASE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdint.h>
#include <string.h>
#include "drv_def.h"

#define TRUE 				(1U)
#define FALSE 				(0U)

#define TEST_DATA_ID		0xFAF00000
#define TEST_ADDR_ID		(TEST_DATA_ID | 0x000A0000)

#define TEST_DATA(a)		(((uint32_t)TEST_DATA_ID + ((uint32_t)(a) & 0xFFFF)))
#define TEST_ADDR(a)		((uint32_t)TEST_ADDR_ID + ((uint32_t)(a) & 0xFFFF))

/* include test macros using in target test environment */
#include "macro.h"

#define ARRAY_COUNT(a)		(int)(sizeof(a)/sizeof(a[0]))

#define ASSERT(v)	if (v) { while(1); }

void *heapMalloc(size_t __size);
void heapFree(void * p);

#endif

