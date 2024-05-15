/**
 * @file main.c
 * @brief OSAL test application main function.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */

#include <glob.h>

#include "osal_test.h"
#include "thread_manager.h"
#include "synchronization_manager.h"
#include "message_manager.h"
#include "clock_time_manager.h"
#include "interrupt_manager.h"
#include "io_manager.h"
#include "pm_manager.h"
#include "common_function.h"

static void osal_test_display_menu();

unsigned int	test_pass;
unsigned int	test_fail;
unsigned int	test_skip;
int				test_err;

/* TP main */
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	int parent_number = 0;
	int child_number = 0;
	char ctr[] = "yes";
	glob_t glob_buffer;
	int linux_ret;
	int count = 0;
	int return_value = 0;

#if defined(RCAR_V3H1)
	printf("=== OSAL_SOC_V3H1\n");
#elif defined(RCAR_V3U)
	printf("=== OSAL_SOC_V3U\n");
#elif defined(RCAR_V3H2)
	printf("=== OSAL_SOC_V3H2\n");
#elif defined(RCAR_V3M2)
	printf("=== OSAL_SOC_V3M2\n");
#elif defined(RCAR_V4H)
	printf("=== OSAL_SOC_V4H\n");
#elif defined(RCAR_V4M)
	printf("=== OSAL_SOC_V4M\n");
#elif defined(RCAR_S4)
	printf("=== OSAL_SOC_S4\n");
#else
#error  Device is not defined
#endif

	linux_ret = glob("/dev/*", 0, NULL, &glob_buffer);
	if (0 == linux_ret)
	{
		printf("=== Device entries of /dev/*\n");
		while (count < glob_buffer.gl_pathc)
		{
			printf("%04d : %s\n", count, glob_buffer.gl_pathv[count]);
			count++;
		}
	}

	system("rmmod cmemdrv");
#if defined(RCAR_V4H) || defined(RCAR_V4M)
	system("insmod /lib/modules/*/extra/cmemdrv.ko bsize=536870912");
#else
	system("insmod /lib/modules/*/extra/cmemdrv.ko bsize=536870912,6291456");
#endif

	if(argc == 1)
	{
		while(!strcmp(ctr,"yes"))
		{
			test_pass	= 0;
			test_fail	= 0;
			test_skip	= 0;
			osal_test_display_menu();

			scanf("%d", &parent_number);
			if (parent_number == 100)
			{
				osal_test(parent_number, child_number);
				printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
			}
			else
			{
				scanf("%d", &child_number);
				if (parent_number >= 1 && parent_number <= 8)
				{
					if ((child_number >= 1 && child_number <= 24) || (child_number == 100))
					{
						osal_test(parent_number, child_number);
						printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
					}
					else
					{
						printf("< Invalid input>\n");
					}
				}
				else
				{
					printf("< Invalid input>\n");
				}
			}
			if(0 != test_fail)
			{
				return_value = -1;
			}
			if(-1 == test_err)
			{
				return_value = -2;
			}
			printf("< Testing complete>\n");
			printf("< Continue excuting other test cases ? (yes/no)>\n");
			scanf("%s", ctr);
		}
	}
	else
	{
		test_pass	= 0;
		test_fail	= 0;
		test_skip	= 0;
		osal_test_display_menu();

		parent_number = 100;
		child_number = 0;
		osal_test(parent_number, child_number);
		printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);

		if(0 != test_fail)
		{
			return_value = -1;
		}
		if(-1 == test_err)
		{
			return_value = -2;
		}
	}

	printf("< OSAL Program end>\n");

	return return_value;
}

static void osal_test_display_menu()
{
	printf("*** please select test subcomponents and module. ***\n\n");

	osal_test_thread_manager_display_menu();
	osal_test_synchronization_manager_display_menu();
	osal_test_message_manager_display_menu();
	osal_test_clock_time_manager_display_menu();
	osal_test_interrupt_manager_display_menu();
	osal_test_io_manager_display_menu();
	osal_test_pm_manager_display_menu();
	osal_test_memory_manager_display_menu();
	osal_test_thread_common_display_menu();

	printf("***  100: select all       ***\n\n");
	printf("***  999: application exit ***\n\n\n");

	printf("input number : ");
}
