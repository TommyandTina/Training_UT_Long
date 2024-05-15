/********************************************************************************
    Atomic Library
     Copyright (C) 2016-2018 Renesas Electronics Corporation, all rights reserved.

    [File] params.c

********************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "params.h"

struct TestParams *TEST_CreateParam(const char *name, const char *category, int32_t no)
{
	struct TestParams *params = (struct TestParams *)heapMalloc(sizeof(struct TestParams));
	if (params == NULL)
	{
	}
	else if (params == (struct TestParams *)TEST_DATA_ID)
	{
		ASSERT(1);
	}
	else
	{
		(void)memset((void*)params, 0, sizeof(*params));
		strcpy(params->func, name);
		strcpy(params->category, category);
		params->no = no;
	}

	return params;
}

void TEST_DestroyParam(struct TestParams *params)
{
	if (params)
	{
		heapFree((void*)params);
	}
}

void TEST_BackupInputParam(struct TestParams *params)
{
}

