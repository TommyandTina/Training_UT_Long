#define TEST_MSG_NAME "R_IMPDRV_GetVersion"

#include "it_common.h"

/* pragma qas cantata testscript start */
static void Device_get_version(struct test_params_t *tparam, struct test_result_t *tret);

static uint32_t testno;

int32_t it_GetVersion_Main(struct test_params_t *tparam, struct test_result_t *test_result)
{
	struct test_result_t tret = {0};

	testno = 2;

	Device_get_version(tparam, &tret);

	PrintTestResult(tret, test_result);

	return 0;
}

static void Device_get_version(struct test_params_t *tparam, struct test_result_t *tret)
{
	/* driver_get_version : output parameter */
	const st_impdrv_version_t *s_version = R_IMPDRV_GetVersion();
	
	SuccessTestPrint((s_version != NULL) && ((s_version->major == IMPDRV_VERSION_MAJOR) &&
			(s_version->minor == IMPDRV_VERSION_MINOR) &&
			(s_version->patch == IMPDRV_VERSION_PATCH)), *tret, "(No.%d) Get Version ", testno++);
}

/* pragma qas cantata testscript end */