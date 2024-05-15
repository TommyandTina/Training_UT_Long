#include <stdio.h>
#include "testenv.h"
#include "testfw.h"

#include "file.h"
#include "logger.h"
#include "validator.h"

struct TestRunner g_test;
static bool g_autoRun = false;
static bool m_onlyCLTest = false;

static uint32_t m_testTarget = (TEST_TARGET_DEFAULT | TEST_TARGET_CHECKER);

static void TEST_RunAllTests(const struct TestInfo *testInfo);

static int TEST_ParseArges(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		const char *arg = argv[i];

		if (strncmp(arg, "--auto-run", strlen("--auto-run")) == 0)
		{
			g_autoRun = true;
		}
		else if (strncmp(arg, "--expected=", strlen("--expected=")) == 0)
		{
			const char *val = strstr(arg, "=");
			const char *path = &val[1];
			TEST_File_SetExpectedDirPath(path);
		}
		else if (strncmp(arg, "--output=", strlen("--output=")) == 0)
		{
			const char *val = strstr(arg, "=");
			const char *path = &val[1];
			TEST_File_SetOutputDirPath(path);
		}
		else if (strncmp(arg, "--cl=", strlen("--cl=")) == 0)
		{
			const char *val = strstr(arg, "=");
			const char *mode = &val[1];

			TEST_File_ClearCLOutputMode();

			while(1)
			{
				if (strncmp(mode, "binary", strlen("binary")) == 0)
				{
					TEST_File_SetCLOutputMode(CLOutputToBinaryFile);
				}
				else if (strncmp(mode, "text", strlen("text")) == 0)
				{
					TEST_File_SetCLOutputMode(CLOutputToTextFile);
				}
				else if (strncmp(mode, "console", strlen("console")) == 0)
				{
					TEST_File_SetCLOutputMode(CLOutputToConsole);
				}

				const char *m = strstr(mode, ",");
				if (m)
				{
					mode = &m[1];
				}
				else
				{
					break;
				}
			}
		}
		else if (strncmp(arg, "--no-log-color", strlen("--no-log-color")) == 0)
		{
			TEST_SetLogColorEnabled(false);
		}
		else if (strncmp(arg, "--log-timestamp", strlen("--log-timestamp")) == 0)
		{

		}
		else if (strncmp(arg, "--no-output-timestamp", strlen("--no-output-timestamp")) == 0)
		{
			TEST_File_SetOutputTimestampEnabled(false);
		}
		else if (strncmp(arg, "--cltest-only", strlen("--cltest-only")) == 0)
		{
			m_onlyCLTest = true;
		}
		else if (strncmp(arg, "--checker-test", strlen("--checker-test")) == 0)
		{
			m_testTarget = TEST_TARGET_CHECKER;
		}
		else if (strncmp(arg, "--no-checker-test", strlen("--no-checker-test")) == 0)
		{
			m_testTarget &= ~TEST_TARGET_CHECKER;
		}
		else if (strncmp(arg, "--skip-validator=", strlen("--skip-validator=")) == 0)
		{
			uint64_t skip_validator = 0x0000000000000000;
			const char *val = strstr(arg, "=");
			const char *v = &val[1];
			size_t length = strlen(v);
			if ((strncmp(v, "0x", 2) == 0) || (strncmp(v, "0X", 2) == 0)) {
				length -= 2;
				v += 2;
			}

			for (uint32_t j = 0; j < length; j++)
			{
				char n = v[j];
				int32_t s = 0;

				skip_validator <<= 4;

				if (('0' <= n) && (n <= '9')) {
					s = n - '0';
				} else if (('A' <= n) && (n <= 'F')) {
					s = (n - 'A') + 10;
				} else if (('a' <= n) && (n <= 'f')) {
					s = (n - 'a') + 10;
				} else {
					s = 0;
				}

				skip_validator |= s;
			}

			TEST_SetSkipValidator(skip_validator);
		}
	}

	return 0;
}

static const char *TEST_SupportedHardwareName()
{
	return TEST_SOC;
}

static bool TEST_IsValidTest(const char *target)
{
	const char *hwname = TEST_SupportedHardwareName();

	if (strlen(target) == 0)
	{
		return true;
	}
	else if (strcmp(target, "ALL") == 0)
	{
		return true;
	}
	else
	{
		char target_tmp[50] = {0};
		memcpy(target_tmp, target, strlen(target));
		char * tmp = strtok(target_tmp, "_");
        while (tmp != NULL)
        {
            if (strcmp(tmp, hwname) == 0)
			{
				return true;
			}
            tmp = strtok(NULL, "_");
        }
	}
	return false;
}

#ifdef USE_GTEST
static void TEST_SetFilterString(char *str, const char *env, const char *name)
{
	if (!m_onlyCLTest)
	{
		sprintf(str, "%s_%s_ALL.*:%s_%s_%s.*",
				env, name,
				env, name, TEST_SupportedHardwareName());
	}
	else
	{
		sprintf(str, "%s_%s_ALL.*_CL_*:%s_%s_%s.*_CL_*",
				env, name,
				env, name, TEST_SupportedHardwareName());
	}
}
#endif

void TEST_main(int argc, char *argv[], const char *env, const char *target, const struct TestInfo *testInfo)
{
	// Parse input arguments
	TEST_ParseArges(argc, argv);

	//TEST_File_Init();

#ifdef USE_GTEST
	int ret;
	char strbuf[255] = {0};
	char casename[255] = {0};
	int i;

	using ::testing::InitGoogleTest;

	while(1)
	{
		if (!g_autoRun)
		{
			printf("--- %s %s test menu ---\n", target, env);
			printf("<1> ALL Functional Test\n");
			printf("<2> Functional Test List Display \n");
			printf("<3> Execute by specifying a Function Name \n");
			printf("<a> ALL Functional Test and print CL coverage (exit after testing)\n");
			printf("<c> CL Coverage Output \n");
			printf("<9> TEST quit \n");
			printf("<q> TEST quit \n");
			fflush(0);

			strbuf[0] = 0;
			do {
				fgets(strbuf, sizeof(strbuf)-1, stdin);
			} while((strbuf[1] != 0x0a) && (strbuf[1] != 0x0d));
		}
		else
		{
			strbuf[0] = 'a';
		}

		switch (strbuf[0]) {

			case 'a':
				TEST_SetFilterString(strbuf, env, "*");
				::testing::GTEST_FLAG(filter) = strbuf;
			    InitGoogleTest(&argc, argv);
				ret = RUN_ALL_TESTS();
				TEST_File_PrintCLCoverage();
				goto cleanup;
				break;
			case '1':
				TEST_SetFilterString(strbuf, env, "*");
				::testing::GTEST_FLAG(filter) = strbuf;
			    InitGoogleTest(&argc, argv);
				ret = RUN_ALL_TESTS();
				break;
			case '2':
				::testing::GTEST_FLAG(list_tests) = true;
			    InitGoogleTest(&argc, argv);
				ret = RUN_ALL_TESTS();
				::testing::GTEST_FLAG(list_tests) = false;
				break;
			case '3':
				i = 0;
				while(1)
				{
					const char *api = testInfo[i].api;
					if (api == NULL)
					{
						break;
					}
					printf("\t%3d:%s_%s\n", i + 1, env, api);
					i++;
				}
				printf("Please enter a function name or number \n\n Func Name ->");
				fflush(0);
				scanf("%[^\n]%*c", &strbuf);
				if ((strlen(strbuf)!=0) && ((strbuf[strlen(strbuf) - 1] == 0x0a) || (strbuf[strlen(strbuf) - 1] == 0x0d)))
				{
					strbuf[strlen(strbuf) - 1] = 0;
				}

				if (strlen(strbuf) <= 3)	/* test case number */
				{
					i = atoi(strbuf);
					if (i != 0)
					{
						TEST_SetFilterString(strbuf, env, testInfo[i - 1].api);
					}
				}

				::testing::GTEST_FLAG(filter) = strbuf;
			    InitGoogleTest(&argc, argv);
				ret = RUN_ALL_TESTS();
				::testing::GTEST_FLAG(filter) = "*";
				break;
			case '9':
			case 'q':
				ret = 0;
				goto cleanup;
				break;
			case 'c':
				TEST_File_PrintCLCoverage();
				break;
			default:
				printf("not found %s\n", strbuf);
				break;
		}
	}
cleanup:
#else
	TEST_RunAllTests(testInfo);
#endif
    return;
}

int TEST_RunTest(const struct TestInfo *testInfo, int target)
{
	int32_t count = 0;
	char *test_case_name = NULL;

	const char *api_name = testInfo[target].api;
	struct TestCase *test_cases = testInfo[target].test_case;

	if (api_name == NULL)
	{
		return -1;
	}
	else if (test_cases == NULL)
	{
		return 0;
	}

	for (int j = 0; ; j++)
	{
		struct TestCase *test_case = &test_cases[j];
		if (test_case->func == NULL)
		{
			break;
		}

		if (test_case_name == NULL)
		{
			char str[128];
			memset(str, 0, sizeof(str));
			if (strlen(test_case->type) == 0)
			{
				sprintf(str, "%s", test_case->func_name);
			}
			else
			{
				sprintf(str, "%s_%s", test_case->type, test_case->func_name);
			}

			test_case_name = str;
		}

		if (!TEST_IsValidTest(test_case->target))
		{
			continue;
		}

		count++;
	}

	if ((test_case_name == NULL) || (count <= 0))
	{
		ILOG("%s() test case is NOT Found !!", api_name);
		return 0;
	}

	g_test.passed = 0;
	g_test.failed = 0;

	ILOG("[----------] %d tests from %s", count, test_case_name);
	for (int j = 0; ; j++)
	{
		struct TestCase *test_case = &test_cases[j];
		if (test_case->func == NULL)
		{
			break;
		}
		else if (!TEST_IsValidTest(test_case->target))
		{
			continue;
		}
		else
		{
			sprintf(g_test.runningTestName, "%s_%s_%s.%s", test_case->type, test_case->func_name, test_case->target, test_case->id);
			ILOG("[ RUN      ] %s", g_test.runningTestName);
			test_case->func();
		}
	}
	ILOG("[----------] %d tests from %s", count, test_case_name);

	if (g_test.passed > 0) {
		ILOG("[ PASSED   ] %d tests from %s", g_test.passed, test_case_name);
	}

	if (g_test.failed > 0) {
		ILOG("[ FAILED   ] %d tests from %s", g_test.failed, test_case_name);
	}
	FLOG("[ Result   ] %s : OK = %d, NG = %d, Skip = 0\n", test_case_name, g_test.passed, g_test.failed);
	
	return count;
}

int TEST_RunTestByName(const struct TestInfo *testInfo, const char *target)
{
	int index = -1;

	for (int i = 0; ; i++)
	{
		const char *funcName = testInfo[i].api;
		if (funcName == NULL)
		{
			ELOG("[  FAILED  ] Function is NOT Found !! %s", target);
			return false;
		}
		else if (strcmp(funcName, target) == 0)
		{
			index = i;
			break;
		}
	}

	return TEST_RunTest(testInfo, index);
}

static void TEST_PrintSummaryData(
	int		totalPassedCount,
	int		totalFailedCount
)
{
	int exitCode = 0;
	if (totalFailedCount > 0) { exitCode = -1; }
	printf("[SUMMARY-START]\n");
	printf("  [TOTAL-TC-DETECTED]           %d\n", (totalPassedCount + totalFailedCount));
	printf("    [TOTAL-TC-NOT-EXEC]         -\n");
	printf("      [TOTAL-TC-ARGS-FILTER]    -\n");
	printf("      [TOTAL-TC-ENV-FILTER]     -\n");
	printf("      [TOTAL-TC-SKIP]           -\n");
	printf("    [TOTAL-TC-EXEC]             %d\n", (totalPassedCount + totalFailedCount));
	printf("      [TOTAL-TC-PASS]           %d\n", totalPassedCount);
	printf("      [TOTAL-TC-FAIL]           %d\n", totalFailedCount);
	printf("      [TOTAL-TC-ERR]            -\n");
	printf("      [TOTAL-TC-INCONSISTENCY]  -\n");
	printf("  [TEST-DURATION-US]            -\n");
	printf("  [EXIT-CODE]                   %d\n", exitCode);
	printf("[SUMMARY-END]\n");
}

static void TEST_RunAllTests(const struct TestInfo *testInfo)
{
	int totalPassedCount = 0;
	int totalFailedCount = 0;
	// int g_count			 = 0;
	// char g_testName[500][100] = {{0}};
	ILOG("%s", "[==========] RunAllTests() START");
	for (int i = 0; ; i++)
	{
		int32_t count = TEST_RunTest(testInfo, i);
		if (count > 0)
		{
			totalPassedCount += g_test.passed;
			totalFailedCount += g_test.failed;
			// memcpy(&g_testName[i],testInfo[i].api,sizeof(g_testName[0]));
			// g_count++;
		}
		else if (count < 0)
		{
			break;
		}
	}

	ILOG("%s", "[==========] RunAllTests() END");
	ILOG("%s", "[==========] TEST SUMMARY");
	// ILOG("\n %d EXECUTED FUNCTION",g_count);
	// for (int i = 0; i < g_count ;i++)
	// {
	// 	ILOG("%s", g_testName[i]);
	// }
	TEST_PrintSummaryData(totalPassedCount, totalFailedCount);
}
