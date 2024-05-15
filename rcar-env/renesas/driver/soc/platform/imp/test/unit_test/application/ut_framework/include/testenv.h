#ifndef _TESTENV_H_
#define _TESTENV_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "drv_def.h"
#include "functions.h"

#define TEST_IMP_V3M2			(0x00000001U)
#define TEST_IMP_V3H1			(0x00000002U)
#define TEST_IMP_V3H2			(0x00000003U)
#define TEST_IMP_V3U			(0x00000004U)

#define TEST_TARGET_DEFAULT		(0x00000001U)
#define TEST_TARGET_CHECKER		(0x00000002U)

#if defined(WIN32)
#define snprintf _snprintf
#endif

// #ifdef __cplusplus

struct TestRunner {
	int passed;
	int failed;
	char runningTestName[256];
};

extern struct TestRunner g_test;

typedef void (*TEST_FUNC)(void);

struct TestCase {
	const char *type;
	const char *func_name;
	const char *target;
	const char *id;
	TEST_FUNC  func;
};

// #ifdef USE_GTEST

// #include <gtest/gtest.h>

// class TestBase : public ::testing::Test
// {

// };

// #define TEST_CASE_F(type, func, target, id)\
// 	GTEST_TEST(type##_##func##_##target, id)

// #define TEST_CASE_T(type, func, target, id)\
// 	{ "", "", "", "", NULL	}

// #define TEST_CASE_END	\
// 	{ "", "", "", "", NULL	}

// // ToDo: remove
// #define TEST_CASE(test_case_name, test_name)	\
// 	{ "", "", NULL	}

// // not USE_GTEST
// #else

#ifdef __WINAMS__
#define EXPECT_EQ(exp, act)	\
	{\
		bool val1 = exp;	\
		bool val2 = act;	\
		if (val1 == val2)	\
		{\
			g_test.passed++;\
		}\
		else\
		{\
			while(1) {};\
		}\
	}
#else
#define EXPECT_EQ(exp, act)	\
	{\
		bool val1 = exp;	\
		bool val2 = act;	\
		if (val1 == val2)	\
		{\
			printf("[          OK ] %s\n", g_test.runningTestName);	g_test.passed++;\
		}\
		else\
		{\
			printf("%s:%d: Failure\n", __FILE__, __LINE__);\
			printf("\tExpected: %s\n", (val1) ? "true" : "false");\
			printf("\tWhich is: %s\n", (val2) ? "true" : "false");\
			printf("[          NG ] %s\n", g_test.runningTestName);	g_test.failed++;\
		}\
	}
#endif

#define TEST_FUNC_NAME(test_case_name, target, test_name)\
	test_case_name##_##test_name##_##target##_Test

#define TEST_CASE_F(type, func, target, id)\
	void type##_##func##_##id##_##target##_Test()

#define TEST_CASE_T(type, func, target, funcid, id)\
	{ #type, #func, #target, #id, TEST_FUNC_NAME(type##_##func, target, funcid)	}

#define TEST_CASE_END	\
	{ "", "", "", "", NULL	}


// ToDo remove
#define GTEST_TEST(test_case_name, test_name)\
	void TEST_FUNC_NAME(test_case_name, ALL, test_name)()

#define TEST_CASE(test_case_name, test_name)\
	{ "", #test_case_name, "ALL", #test_name, TEST_FUNC_NAME(test_case_name, ALL, test_name)	}

// #endif

// #endif /* __cplusplus */

#endif
