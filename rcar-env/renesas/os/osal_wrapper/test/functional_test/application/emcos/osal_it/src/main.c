/**
 * @file main.c
 * @brief OSAL test application main function.
 * @author Khanh Hoang
 * @date Aug 04, 2020
 */
#include "osal_test.h"
#include "thread_manager.h"
#include "synchronization_manager.h"
#include "message_manager.h"
#include "clock_time_manager.h"
#include "interrupt_manager.h"
#include "io_manager.h"
#include "pm_manager.h"
#include "common_function.h"
#include "memory_manager.h"

static void osal_test_display_menu();

unsigned int test_pass;
unsigned int test_fail;
unsigned int test_skip;
int          test_err;

/* TP main */
int main(int argc, char *argv[])
{
	int parent_number = 0;
	int child_number = 0;
	char ctr[10] = "yes";
	char buff[256] = "\0";
    int return_value = 0;

	parent_number = 100;
	child_number = 1;

	if(argc == 3)
	{
		strcpy(buff, argv[1]);
		parent_number = atoi(buff);
		
		strcpy(buff, argv[2]);
		child_number = atoi(buff);
	}
	if((argc == 3) || (argc == 2))
	{
		while(!strcmp(ctr,"yes"))
		{
			test_pass	= 0;
			test_fail	= 0;
			test_skip	= 0;
			osal_test_display_menu();
			printf("%d %d\n", parent_number, child_number);

			//scanf("%d", &parent_number);
			//parent_number = 100;
			if (parent_number == 100)
			{
				osal_test(parent_number, child_number);
				printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
			}
			else
			{
				//scanf("%d", &child_number);
				if (parent_number >= OSAL_TEST_THREAD && parent_number <= OSAL_TEST_PARENT_MAX)
				{
					if ((child_number >= 1 && child_number <= 20) || (child_number == 100))
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
			//scanf("%s", ctr);
			ctr[0] = 'n';
			ctr[1] = 'o';
			ctr[2] = '\r';
			ctr[3] = '\r';
			ctr[4] = '\n';
			ctr[5] = '\0';
		}
	}
	else
	{
		test_pass	= 0;
		test_fail	= 0;
		test_skip	= 0;
		osal_test_display_menu();

		parent_number = 100;
		child_number = 1;
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
	osal_test_thread_common_display_menu();
	osal_test_memory_manager_display_menu();

	printf("***  100: select all       ***\n\n");
	printf("***  999: application exit ***\n\n\n");

	printf("input number : ");
}
