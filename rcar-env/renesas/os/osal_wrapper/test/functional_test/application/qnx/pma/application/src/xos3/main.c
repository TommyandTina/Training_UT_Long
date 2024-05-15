#include <stdio.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <hw/inout.h>

#include "r_pma_functionl_test_fw.h"
#include "r_pma_functionl_test_sequence.h"
#include "r_pma_functionl_test_reset_v4h.h"

extern st_pma_handle_t g_pma_handle;
/**********************************************************************************
  Function: main
    - description:
       main routine of test
**********************************************************************************/
int main(void)
{
    int result = TEST_OK;
    unsigned int fail_count = 0;
    unsigned int test_pass  = 0;
    unsigned int test_fail  = 0;
    unsigned int test_skip  = 0;

    TEST_LOG("main routine Start\n");

    /* Setup for Test */
    result = setup_test();

    if (TEST_OK == result)
    {
        result = pma_sequence_test();
        if (TEST_OK != result)
        {
            TEST_LOG("pma_sequence_test is failed\n");
            fail_count++;
        }

        result = pma_reset_test();
        if (TEST_OK != result)
        {
            TEST_LOG("pma_reset_test is failed\n");
            fail_count++;
        }
    }

    (void)cleanup_test();

    TEST_LOG("main routine Finish\n");
    if (0 != fail_count)
    {
        test_fail++;
    }
    else
    {
        test_pass++;
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return 0;
}
