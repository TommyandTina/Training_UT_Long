/* include */
#include "osal_test.h"
#include "osal_test_common.h"
#include "common_function.h"

/* prototype */
static void osal_test_R_OSAL_Initialize();
static void osal_test_R_OSAL_Deinitialize();
static void osal_test_R_OSAL_GetVersion();

void osal_test_thread_common_display_menu()
{
	printf("*** OSAL Thread Common Function ***\n");
	printf("***  09_001: R_OSAL_Initialize                  ***\n");
	printf("***  09_002: R_OSAL_Deinitialize                ***\n");
	printf("***  09_100: Common function all exec           ***\n\n");
	return;
}

void osal_test_thread_common_menu(int child)
{
    int test_number = child;
	unsigned int test_count = 1;
	unsigned int loop_count = 0;

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number	= OSAL_TEST_INITIALIZE;
		test_count	= OSAL_TEST_COMMON_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number) 
		{
			case OSAL_TEST_INITIALIZE:
				osal_test_R_OSAL_Initialize();
				break;
			case OSAL_TEST_DEINITIALIZE:
				osal_test_R_OSAL_Deinitialize();
				break;
			case OSAL_TEST_GETVERSION:
				osal_test_R_OSAL_GetVersion();
				break;
			default:
				printf("That menu is not available.\n\n");
				break;
		}
		test_number++;
	}
	return;
}

/* 08_001: R_OSAL_Initialize */
static void osal_test_R_OSAL_Initialize()
{
	// There are no test cases for this API.
	return;
}

/* 08_002: R_OSAL_Deinitialize */
static void osal_test_R_OSAL_Deinitialize()
{
	// There are no test cases for this API.
	return;
}

/* 08_003: R_OSAL_GetVersion */
static void osal_test_R_OSAL_GetVersion()
{
	// There are no test cases for this API.
	return;
}
