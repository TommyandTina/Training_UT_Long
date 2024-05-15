#ifndef _PARAMS_H_
#define _PARAMS_H_
#include "testenv.h"
#include "base.h"

struct Backup {
	void *mem_backup;
	uint32_t mem_size;
};

struct Sequence {
	int32_t numbers[32];
};

struct TestParams {
	char *id;
	char func[128];
	char category[128];
	int32_t no;

	struct Backup	backup;
	struct Sequence pre_seq;
	struct Sequence post_seq;
};

struct TestParams *TEST_CreateParam(const char *name, const char *category, int32_t no);
void TEST_DestroyParam(struct TestParams *params);
void TEST_BackupInputParam(struct TestParams *params);

#endif
