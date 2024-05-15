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

#if 1 /* IPL workaround */
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <hw/inout.h>
#endif

static void osal_test_display_menu();

unsigned int test_pass;
unsigned int test_fail;
unsigned int test_skip;
int          test_err;

/* TP main */
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	int parent_number = 0;
	int child_number = 0;
	char ctr[] = "yes";
    int return_value = 0;

#if 1 /* IPL workaround */
    // Required for mmap
    // #include <hw/inout.h>
    // Required for register address
    // #include "r-car-gen3.h"
    uintptr_t cpg_vbase = (uintptr_t)MAP_FAILED;
    uint32_t  reg_mstpcr8 = 0;

    cpg_vbase = (uintptr_t)mmap_device_io(0x1000, 0xE6150000);
    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
        printf("CPG - %s : Unable to map CPG registers 0x%x\n", __func__, 0xE6150000);
        return 0;
    }

    /* read register */
    reg_mstpcr8 = in32(cpg_vbase + 0x980);
    if((reg_mstpcr8 & 0x7ff00000 ) == 0x7ff00000)
    {
        printf("RMSTPCR initial value OK!!\n");
    }
    else
    {
        printf("RMSTPCR initial value NG!!\n");

        printf("Set initial RMSTPCR8 value\n");
        out32(cpg_vbase + 0x980, reg_mstpcr8 | 0x7ff00000);
    }
    munmap_device_io(cpg_vbase, 0x1000);
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

        if(0 != test_fail)
        {
            return_value = -1;
        }
        if(-1 == test_err)
        {
            return_value = -2;
        }

		printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);
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
