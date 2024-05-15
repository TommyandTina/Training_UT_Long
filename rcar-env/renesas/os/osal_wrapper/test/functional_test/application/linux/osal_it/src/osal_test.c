/**
 * @file osal_test.c
 * @brief OSAL test application main process.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */

/* include */
#include "osal_test.h"
#include "thread_manager.h"
#include "synchronization_manager.h"
#include "message_manager.h"
#include "clock_time_manager.h"
#include "interrupt_manager.h"
#include "io_manager.h"
#include "pm_manager.h"
#include "common_function.h"

int osal_test(int parent_number, int child_number)
{
	/* switch test case at manager number */
	switch (parent_number)
	{
		case OSAL_TEST_COMMON_FUNCTION:
			osal_test_thread_common_menu(child_number);
			break;

		case OSAL_TEST_THREAD:
			osal_test_thread_manager_menu(child_number);
			break;

		case OSAL_TEST_SYNCHRONIZATION:
			osal_test_synchronization_manager_menu(child_number);
			break;

		case OSAL_TEST_MESSAGE:
			osal_test_message_manager_menu(child_number);
			break;

		case OSAL_TEST_CLOCK_TIME:
			osal_test_clock_time_manager_menu(child_number);
			break;

		case OSAL_TEST_INTERRUPT:
			osal_test_interrupt_manager_menu(child_number);
			break;

		case OSAL_TEST_IO:
			osal_test_io_manager_menu(child_number);
			break;

		case OSAL_TEST_PM:
			osal_test_pm_manager_menu(child_number);
			break;

		case OSAL_TEST_MEMORY:
			osal_test_memory_manager_menu(child_number);
			break;

		case OSAL_TEST_ALL:
			osal_test_thread_common_menu(OSAL_TEST_ALL_EXEC);
			osal_test_thread_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_synchronization_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_message_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_clock_time_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_interrupt_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_io_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_pm_manager_menu(OSAL_TEST_ALL_EXEC);
			osal_test_memory_manager_menu(OSAL_TEST_ALL_EXEC);
			break;
		case OSAL_TEST_EXIT:
			printf("Quit.\n\n");
			return OSAL_TEST_OK;
		default:
			printf("That menu is not available.\n\n");
			break;
	}

	return OSAL_TEST_OK;
}
