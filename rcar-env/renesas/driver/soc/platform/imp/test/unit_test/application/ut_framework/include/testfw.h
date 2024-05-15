#ifndef _TESTFW_H_
#define _TESTFW_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "testenv.h"

struct TestInfo
{
	const char *api;
	const char *category;
	struct TestCase *test_case;
};

void TEST_main(int argc, char *argv[], const char *env, const char *target, const struct TestInfo *testInfo);
int TEST_RunTest(const struct TestInfo *testInfo, int target);
int TEST_RunTestByName(const struct TestInfo *testInfo, const char *target);

#endif
