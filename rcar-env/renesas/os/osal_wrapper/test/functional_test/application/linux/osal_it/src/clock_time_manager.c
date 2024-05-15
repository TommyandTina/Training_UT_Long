/**
 * @file clock_time_manager.c
 * @brief OSAL Clock&Time Manager evaluation file.
 * @author K.Takagi
 * @date 2019/11/12
 */

/* include */
#include "osal_test.h"
#include "osal_test_common.h"
#include "clock_time_manager.h"

/* prototype */
static void osal_test_R_OSAL_ClockTimeGetTimeStamp();
static void osal_test_R_OSAL_ClockTimeCalculateTimeDifference();

void osal_test_clock_time_manager_display_menu()
{
	printf("*** OSAL Clock & Time Manager ***\n");
	printf("***  04_001: R_OSAL_ClockTimeGetTimeStamp                ***\n");
	printf("***  04_002: R_OSAL_ClockTimeCalculateTimeDifference     ***\n");
	printf("***  04_100: clock & time manager module all exec ***\n\n");
}

void osal_test_clock_time_manager_menu(int child)
{
	int test_number = child;
	unsigned int test_count = 1;
	unsigned int loop_count = 0;

	/* ALL check */
	/* when select all */
	if (child == OSAL_TEST_ALL_EXEC)
	{
		test_number	= OSAL_TEST_CLOCKTIMEGETTIMESTAMP;
		test_count	= OSAL_TEST_CLOCKTIME_MAX;
	}

	for (loop_count = 0; loop_count < test_count; loop_count++)
	{
		switch (test_number)
		{
			case OSAL_TEST_CLOCKTIMEGETTIMESTAMP:
				osal_test_R_OSAL_ClockTimeGetTimeStamp();
				break;
			case OSAL_TEST_CLOCKTIMECALCULATETIMEDIFFERENCE:
				osal_test_R_OSAL_ClockTimeCalculateTimeDifference();
				break;
			default:
				printf("That menu is not available. \n\n");
				break;
		}
		test_number++;
	}
}

/* 04_001: R_OSAL_ClockTimeGetTimeStamp */
static void osal_test_R_OSAL_ClockTimeGetTimeStamp()
{
	// There are no test cases for this API.
	return;
}

/* 04_002: R_OSAL_ClockTimeCalculateTimeDifference */
static void osal_test_R_OSAL_ClockTimeCalculateTimeDifference()
{
	// There are no test cases for this API.
	return;
}
