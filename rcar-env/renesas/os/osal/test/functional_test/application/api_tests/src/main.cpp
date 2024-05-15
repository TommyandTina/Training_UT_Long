/***********************************************************************************************************************
* Copyright [2020-2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    main.cpp
* Version :      3.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  main of OSAL API Functional Test
***********************************************************************************************************************/
/***********************************************************************************************************************
 * History   Version   DD.MM.YYYY    Description
 *           2.0.0     29.10.2021    Create New
 *           3.0.0     14.07.2022    Add creation command of /dev/cmem0 and /dev/cmem1 on Linux HIL
***********************************************************************************************************************/
#if !defined(OSAL_TEST_FRAMEWORK)
#include "gtest/gtest.h"
#else
#include "osal_test_framework.hpp"
#endif
#include "rcar-xos/osal/r_osal.h"

#if TEST_QNX /* IPL workaround */
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/mman.h>
#include <hw/inout.h>
#endif

#if defined(TEST_QNX) && defined(TEST_AFFINITY)
#include <sys/neutrino.h>
#endif

#ifdef __cplusplus 
extern "C" 
{ 
#endif 

#if defined(TEST_QNX) && defined(TEST_AFFINITY)
extern uint64_t g_osal_main_thread_cpu_affinity;
#endif

int main(int argc, char** argv)
{
    int return_val;
#if !defined(OSAL_TEST_FRAMEWORK)
    ::testing::InitGoogleTest(&argc, argv);
#else
    return_val = InitOSALTestFramework(&argc, argv);
    if(0 != return_val)
    {
        LOG_ERROR("OSAL Test Framework initialization was failed\n");
        return return_val;
    }
#endif

#if defined(TEST_LINUX)
    char rmmod_cmemdrv[] = "rmmod cmemdrv";
    char insmod_cmemdrv[] = "insmod /lib/modules/*/extra/cmemdrv.ko bsize=536870912,16777216";
    system(rmmod_cmemdrv);
    system(insmod_cmemdrv);
#endif

#if defined(TEST_QNX) /* IPL workaround */
    // Required for mmap
    // #include <hw/inout.h>
    // Required for register address
    // #include "r-car-gen3.h"
    uintptr_t cpg_vbase = (uintptr_t)MAP_FAILED;
    uint32_t  reg_mstpcr8 = 0;

    cpg_vbase = (uintptr_t)mmap_device_io(0x1000, 0xE6150000);
    if (cpg_vbase  == (uintptr_t)MAP_FAILED) {
        printf("Unable to map CPG registers\n");
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
    R_OSAL_Initialize();
#if defined(TEST_QNX) && defined(TEST_AFFINITY)
    if (0x0U != g_osal_main_thread_cpu_affinity)
    {
        /* Change the CPU Affinity of the main thread. */
        ThreadCtl(_NTO_TCTL_RUNMASK, (void *)g_osal_main_thread_cpu_affinity);
    }
#endif
    return_val = RUN_ALL_TESTS();
    R_OSAL_Deinitialize();

#if defined(OSAL_TEST_FRAMEWORK)
    printf("================== osal_functional_test_non_gtest finish ==================\n");
#endif

    return return_val;
}

#ifdef __cplusplus
}    
#endif
