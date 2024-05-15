
/***********************************************************************************************************************
* Copyright [2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
*
* The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
* and/or its licensors ("Renesas") and subject to statutory and contractual protections.
*
* Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
* display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
* purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
* SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
* WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
* NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
* INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
* OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
* be subject to different terms.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name :    main.c
* Version :      0.2.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  main of check about OSAL Device Configuration
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           0.1.0     10.02.2023    Create New
 *           0.2.0     10.11.2023    Add definition for V4M
***********************************************************************************************************************/
#include "osal_test_dev_conf.h"

/* TP main */
int main(int argc, char *argv[])
{
    (void)argv;
    (void)argc;
    int ret = 0;
    unsigned int test_pass   = 0;
    unsigned int test_fail   = 0;
    unsigned int test_skip   = 0;
#if defined(RCAR_V4H) || defined(RCAR_V4M) || defined(RCAR_S4)
    OSAL_CHECK_INFO("< OSAL Program start>\n");
    ret = simple_dev_conf_test();
    OSAL_CHECK_INFO("< OSAL Program end>\n");
    if (0 != ret)
    {
        test_fail++;
    }
    else
    {
        test_pass++;
    }
#else
    OSAL_CHECK_ERR("Don't Test Target SoC\n");
    test_skip++;
#endif
    printf("< OSAL Tests OK = %u, NG = %u, NT = %u>\n", test_pass, test_fail, test_skip);

    return ret;
}
