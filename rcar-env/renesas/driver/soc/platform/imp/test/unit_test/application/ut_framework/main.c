#include <stdio.h>
#include "testenv.h"

#if defined(TEST_ENV_IT)
extern void TEST_IT_main(int argc, char *argv[]);
#define TEST_MAIN	TEST_IT_main

#elif defined(TEST_ENV_UT)
extern void TEST_UT_main(int argc, char *argv[]);
#define TEST_MAIN	TEST_UT_main

#else
#error "TEST_ENV Not Specified !!"
#endif

// for R-Car V3x Board Environment
#if defined(EXECUTED_BY_TASK)

#include "FreeRTOS.h"
#include "task.h"

static void rcvimpdrv_test_task(void *vParam);
extern void vTaskStartScheduler(void);
// call from /FreeRTOS_eagle_win/Demo/CORTEX_A53_64-bit_Eagle/RTOSDemo_A53/src/main.c
void DemoMain(void)
{
	BaseType_t ret;
	size_t   stack_depth  = 100000;
	void     *parameters  = NULL;
	uint64_t priority     = tskIDLE_PRIORITY +1;
	void     *task_handle = NULL;

	/* Initial Task Create */
	ret = xTaskCreate(rcvimpdrv_test_task,
				"rcvimpdrv_test_task",
				(StackType_t)((stack_depth + (sizeof(StackType_t) - (StackType_t) 1)) / sizeof(StackType_t)),
				parameters,
				priority,
				&task_handle);

	if(ret != pdPASS)
	{
		printf("[ERROR] xTaskCreate : %d\n",(int)ret);
		return;
	}

	/* Start the tasks scheduling (No return) */
	vTaskStartScheduler();
}

static void rcvimpdrv_test_task(void *vParam)
{
	// int argc;
	char *argv[] = { "rcvimpdrv_test.axf" };

	TEST_MAIN((sizeof(argv)/sizeof(argv[0])), argv);
}

// for WinAMS Environment
#elif __WINAMS__
void test_winams_main()
{
	// int argc;
	char *argv[] = { "rcvimpdrv_test.exe", "--cl=console" };

	TEST_MAIN((sizeof(argv)/sizeof(argv[0])), argv);
}

void main()
{
}

// for PC Environment
#else
int main(int argc, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	TEST_MAIN(argc, argv);
}
#endif
