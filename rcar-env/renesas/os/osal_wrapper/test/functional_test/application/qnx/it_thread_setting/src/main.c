
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "rcar-xos/osal/r_osal.h"

int main(void)
{
    e_osal_return_t  osal_ret = OSAL_RETURN_OK;
    unsigned int    test_pass = 0;
    unsigned int    test_fail = 0;
    unsigned int    test_skip = 0;

    /* R_OSAL_Initialize */
    osal_ret = R_OSAL_Initialize();
    printf("Check R_OSAL_Initialize result\n");
    printf("Return value: %d\n", osal_ret);

    sleep(15);

    if (OSAL_RETURN_OK == osal_ret)
    {
        system("pidin -fabNpBhl");
        test_pass++;
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            printf("R_OSAL_Deinitialize() error.");
            test_pass--;
            test_fail++;
        }
    }
    else
    {
        test_fail++;
    }
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return 0;
}
