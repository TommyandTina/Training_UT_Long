/**
 * @file osal_test.c
 * @brief OSAL test application main process.
 */

/* include */
#include "osal_test.h"
#include "memory_manager.h"

int osal_test(int parent_number, int child_number)
{
	/* switch test case at manager number */
	switch (parent_number)
	{
		case OSAL_TEST_IO:
			osal_test_io_manager_menu(child_number);
			break;

		case OSAL_TEST_MEMORY:
			osal_test_memory_manager_menu(child_number);
			break;

		case OSAL_TEST_ALL:
			osal_test_io_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_memory_manager_menu(OSAL_TEST_ALL_EXEC);
			break;

		case OSAL_TEST_EXIT:
			printf("Quit.\n\n");
			return OSAL_TEST_OK;

		case OSAL_TEST_THREAD:
		case OSAL_TEST_SYNCHRONIZATION:
		case OSAL_TEST_MESSAGE:
		case OSAL_TEST_CLOCK_TIME:
		case OSAL_TEST_INTERRUPT:
		case OSAL_TEST_PM:
		case OSAL_TEST_COMMON_FUNCTION:
		default:
			printf("That menu is not available.\n\n");
			break;
	}

	return OSAL_TEST_OK;
}
