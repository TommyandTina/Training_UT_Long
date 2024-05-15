/***********************************************************************************************************************
* Copyright [2021] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    osal_memory_sample.c
* Version :      2.0.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Sample application to use OSAL Memory for Linux
***********************************************************************************************************************/

/* Includes */
#include <string.h>
#include <stdio.h>

#define CHECK_STACK_PROT
//#define CHECK_STACK
#define ABNORMAL

/* OSAL API*/
#include "rcar-xos/osal/r_osal.h"
#include "rcar-xos/osal/r_osal_memory_impl.h"

#include "osal_memory_sample.h"

#include "rcar-xos/memory_allocator/r_meml.h"

FILE *check_buffer;
FILE *check_buffer_r;
FILE *check_mmngr;
FILE *check_mmngr_r;

e_osal_return_t osal_mmngr_init(e_osal_mmngr_allocator_mode_t mode,
                                osal_memory_manager_handle_t *mmngr_hndl,
                                osal_memory_buffer_handle_t *mmngr_buf);
e_osal_return_t osal_mmngr_quit(osal_memory_manager_handle_t mmngr_hndl,
                                osal_memory_buffer_handle_t *mmngr_buf);

void test_monitor(void* user_arg, e_osal_mmngr_event_t event,
    e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    void *va;
    e_osal_return_t ret_osal;
    OSAL_TEST_INFO("\n============ MMNGR TEST CALLBACK ==========EVENT = %d\n", event);
    OSAL_TEST_INFO("MONITOR: arg = %p; event = %d;\n", user_arg, event);

    OSAL_TEST_INFO("debug call back buffer_handl (%p)\n", (void *)buffer_hndl);

    ret_osal = R_OSAL_MmngrGetCpuPtr(buffer_hndl, &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_INFO ("R_OSAL_MmngrGetCpuPtr in callback is fail %d\n",ret_osal);
    }
    OSAL_TEST_INFO("MONITOR: va = %p, error = %d\n", va, error);
    OSAL_TEST_INFO("\n============ MMNGR TEST CALLBACK ==========EVENT = %d END\n", event);
}


int main(void){

    OSAL_TEST_INFO("\n\n<TEST START>\n\n");

    /* OSAL initialize */
    (void)R_OSAL_Initialize();

    e_osal_return_t ret_osal;

    OSAL_TEST_INFO("==================== MMNGR TEST =========================\n");

    st_osal_mmngr_config_t        mmngr_config;
    st_osal_mmngr_config_t        max_config;
    osal_memory_manager_handle_t      mmngr_hndl;
    osal_memory_buffer_handle_t       mmngr_buf[6];
#ifndef OSAL_NOT_TEST_QNX 
    osal_memory_buffer_handle_t       tmp_mmngr_buf;
#endif
    void                                *va = NULL;
    void                                *pa = NULL;
    const void        *va_const = NULL;
#ifndef OSAL_NOT_TEST_QNX /* TODO: Make sure to use this variable when using R_OSAL_MmngrGetHwAddr in sample */
    uintptr_t hw_addr;
#endif
    size_t                          size[6];

    int i;

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetOsalMaxConfig ------------\n");
    ret_osal = R_OSAL_MmngrGetOsalMaxConfig(&max_config);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetOsalMaxConfig is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("Memory Max Configuration: \
            \nmode:%d \
            \nmemory_limit:%ld \
            \nmax_allowed_allocation:%ld \
            \nmax_registered_monitors_cbs:%ld\n",
            max_config.mode,
            max_config.memory_limit,
            max_config.max_allowed_allocations,
            max_config.max_registered_monitors_cbs);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetOsalMaxConfig OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrInitializeMmngrConfigSt ------------\n");
    ret_osal = R_OSAL_MmngrInitializeMmngrConfigSt(&mmngr_config);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInitializeMngrConfigSt is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrInitializeMmngrConfigSt OK------------\n");

/***********************************************************************/
#ifdef CHECK_STACK_PROT
    mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK_PROT;
#else
#ifdef CHECK_STACK
    mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
#else
    mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_FREE_LIST;
#endif
#endif
    mmngr_config.memory_limit = 100 * 1024 * 1024; // 100Mb
    mmngr_config.max_allowed_allocations = 6;
    mmngr_config.max_registered_monitors_cbs = 4;

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrOpen ------------\n");
    ret_osal = R_OSAL_MmngrOpen(&mmngr_config, &mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrOpen is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrOpen OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetConfig ------------\n");
    ret_osal = R_OSAL_MmngrGetConfig(mmngr_hndl, &max_config);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetConfig is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("Memory Configuration: \
            \nmode:%d \
            \nmemory_limit:%ld \
            \nmax_allowed_allocation:%ld \
            \nmax_registered_monitors_cbs:%ld\n",
            max_config.mode,
            max_config.memory_limit,
            max_config.max_allowed_allocations,
            max_config.max_registered_monitors_cbs);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetConfig OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrRegisterMonitor ------------\n");
    ret_osal = R_OSAL_MmngrRegisterMonitor(
            mmngr_hndl, OSAL_MMNGR_EVENT_ALLOCATE, test_monitor, NULL);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrRegisterMonitor for EVENT_ALLOCATE is fail %d\n",
              ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrRegisterMonitor(
            mmngr_hndl, OSAL_MMNGR_EVENT_DEALLOCATE, test_monitor, NULL);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrRegisterMonitor for OSAL_MMNGR_EVENT_DEALLOCATE is fail %d\n",
            ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrRegisterMonitor(
            mmngr_hndl, OSAL_MMNGR_EVENT_OVERFLOW_DETECTED, test_monitor, NULL);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrRegisterMonitor for OSAL_MMNGR_EVENT_OVERFLOW_DETECTED \
            is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrRegisterMonitor(
            mmngr_hndl, OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED, test_monitor, NULL);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrRegisterMonitor for OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrRegisterMonitor OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrAlloc ------------\n");

    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 25 * 1024 * 1024, 32, &mmngr_buf[0]);

    OSAL_TEST_INFO("debug alloc[0] buffer_handl (%p)\n", (void *)mmngr_buf[0]);

    if (OSAL_RETURN_CONF != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 25M, align 32 is NOT fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 25 * 1024 * 1024, 63, &mmngr_buf[0]);

    OSAL_TEST_INFO("debug alloc[0] buffer_handl (%p)\n", (void *)mmngr_buf[0]);

    if (OSAL_RETURN_CONF != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 25M, align 63 is NOT fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 25 * 1024 * 1024, 64, &mmngr_buf[0]);

    OSAL_TEST_INFO("debug alloc[0] buffer_handl (%p)\n", (void *)mmngr_buf[0]);
    OSAL_TEST_INFO("debug alloc[0] buffer_handl->p_ptr_self (%p)\n", (void *)mmngr_buf[0]->p_ptr_self);
    OSAL_TEST_INFO("debug alloc[0] &buffer_handl->p_ptr_self (%p)\n", (void *)&mmngr_buf[0]->p_ptr_self);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 25M, align 64 is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 100 * 1024 * 1024, 64, &mmngr_buf[1]);

    OSAL_TEST_INFO("debug alloc[1] buffer_handl (%p)\n", (void *)mmngr_buf[1]);

    if (OSAL_RETURN_MEM != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc Over max_allowed_allocations is NOT fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrAlloc OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetSize ------------\n");
    ret_osal = R_OSAL_MmngrGetSize(mmngr_buf[0], &size[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetSize 25M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("mmngr_buf[0] has size = %ld\n", size[0]);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetSize OK------------\n");

/***********************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugBufferPrint ------------\n");
    check_buffer = fopen("check_buffer.txt", "a");
    ret_osal = R_OSAL_MmngrDebugBufferPrint(mmngr_buf[0], check_buffer);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrDebugBufferPrint 25M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    fflush(check_buffer);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugBufferPrint OK------------\n");

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugBufferPrint (Failre arguments)------------\n");
    check_buffer_r = fopen("check_buffer.txt", "ra");
    ret_osal = R_OSAL_MmngrDebugBufferPrint(mmngr_buf[0], check_buffer_r);
    if (OSAL_RETURN_IO != ret_osal)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrDebugBufferPrint is OK(expect: io) %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugBufferPrint (Failre arguments) Pass------------\n");
#endif

/***********************************************************************/
    OSAL_TEST_INFO ("\nUSE CASE: Aloc with multible size and aligment ------------\n");
    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 10 * 1024 * 1024, 4096, &mmngr_buf[1]);

    OSAL_TEST_INFO("debug alloc[1] buffer_handl (%p)\n", (void *)mmngr_buf[1]);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 10M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrGetSize(mmngr_buf[1], &size[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetSize 10M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("mmngr_buf[1] has size = %ld\n", size[1]);
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    ret_osal = R_OSAL_MmngrDebugBufferPrint(mmngr_buf[1], check_buffer);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
              "R_OSAL_MmngrDebugBufferPrint 10M, align 4K is fail %d\n",
              ret_osal);
        return -1;
    }
    fflush(check_buffer);
#endif

    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 1037, 4096, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 1037B, align 4K is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 4096+32, 4096, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 4096+32, align 4K is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 4096+63, 4096, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 4096+63, align 4K is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 0, 4096, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 0, align 4K is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 4096, 0, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 4096, align 0 is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 4096, 96, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 4096, align 96 is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 4096, 63, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 4096, align 63 is OK(expect: fail) %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 4096, 64, &mmngr_buf[2]);

    OSAL_TEST_INFO("debug alloc[2] buffer_handl (%p)\n", (void *)mmngr_buf[2]);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 4096, align 64 is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrGetSize(mmngr_buf[2], &size[2]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetSize 4096, align 64 is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("mmngr_buf[2] has size = %ld\n", size[2]);
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    ret_osal = R_OSAL_MmngrDebugBufferPrint(mmngr_buf[2], check_buffer);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrDebugBufferPrint 4096, align 64 is fail %d\n",
            ret_osal);
        return -1;
    }
    fflush(check_buffer);
#endif

    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 10 * 1024 * 1024, 128, &mmngr_buf[3]);

    OSAL_TEST_INFO("debug alloc[3] buffer_handl (%p)\n", (void *)mmngr_buf[3]);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 10M, align 128 is fail %d\n",ret_osal);
        /* Note: Current implement haven't supported to check for aligment */
        return -1;
    }
    ret_osal = R_OSAL_MmngrGetSize(mmngr_buf[3], &size[3]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetSize 10M, align 128 is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("mmngr_buf[3] has size = %ld\n", size[3]);
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    ret_osal = R_OSAL_MmngrDebugBufferPrint(mmngr_buf[3], check_buffer);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrDebugBufferPrint 10M, align 128 is fail %d\n",
            ret_osal);
        return -1;
    }
    fflush(check_buffer);
#endif
    OSAL_TEST_INFO ("\nUSE CASE: Aloc with multible size and aligment OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetCpuPtr ------------\n");
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[3], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetCpuPtr 10M, align 128 is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR VA (1 align): %p\n", va);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetCpuPtr OK------------\n");

/***********************************************************************/
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetHwAddr ------------\n");
    /*limit: R_OSAL_MmngrGetHwAddr not get hw_addr via axi bus */
    ret_osal = R_OSAL_MmngrGetHwAddr(
            mmngr_buf[3], OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &hw_addr);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetHwAddr 10M, align 128 is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR HW (1 align): %lx\n", hw_addr);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetHwAddr OK------------\n");
#endif
/***********************************************************************/

    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 10 * 1024 * 1024, 4096, &mmngr_buf[4]);

    OSAL_TEST_INFO("debug alloc[3] buffer_handl (%p)\n", (void *)mmngr_buf[4]);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 10M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrGetSize(mmngr_buf[4], &size[4]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetSize 10M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("mmngr_buf[4] has size = %ld\n", size[4]);
/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetConstCpuPtr ------------\n");
    ret_osal = R_OSAL_MmngrGetConstCpuPtr(mmngr_buf[4], &va_const);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetConstCpuPtr 10M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR VA (4K align): %p\n", va_const);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrGetConstCpuPtr OK------------\n");

/***********************************************************************/
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    /*limit: R_OSAL_MmngrGetHwAddr not get hw_addr via axi bus */
    ret_osal = R_OSAL_MmngrGetHwAddr(
            mmngr_buf[4], OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &hw_addr);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetHwAddr 10M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR HW (4K align): %lx\n", hw_addr);
#endif
/***********************************************************************/
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    OSAL_TEST_INFO ("\nUSE CASE: Re-allocate buffer (mmngr_buf[4]) ------------\n");
    /*Allocate with the same mmngr_buf[4]. It was overwriten information*/
    tmp_mmngr_buf = mmngr_buf[4];
    ret_osal = R_OSAL_MmngrAlloc(
            mmngr_hndl, 10 * 1024 * 1024, 4096, &mmngr_buf[4]);

    OSAL_TEST_INFO("debug alloc[4] buffer_handl (%p)\n", (void *)mmngr_buf[4]);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "Actual: R_OSAL_MmngrAlloc at the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[4], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR VA (4K align) second: %p\n", va);
    /*limit: R_OSAL_MmngrGetHwAddr not get hw_addr via axi bus */
    ret_osal = R_OSAL_MmngrGetHwAddr(
            mmngr_buf[4], OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &hw_addr);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetHwAddr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR HW (4K align) second: %lx\n", hw_addr);
    OSAL_TEST_INFO ("\nUSE CASE: Re-allocate buffer (mmngr_buf[4]) Finished------------\n");
    OSAL_TEST_INFO ("There is no error for re-allocate but it can lead to later issues\n");
#endif
/***********************************************************************/
#ifdef ABNORMAL
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    OSAL_TEST_INFO ("ABNORMAL USE CASE: Not deallocate the last buffer ------------\n");

    OSAL_TEST_INFO("debug dealloc[0] buffer_handl (%p)\n", (void *)mmngr_buf[0]);

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_INFO (
            "Actual: R_OSAL_MmngrDealloc mmngr_buf[0] (not last buffer) \
              fail %d\n",ret_osal);
        OSAL_TEST_INFO (
            "Expect: R_OSAL_MmngrDealloc to return ret_osal = 2 (OSAL_RETURN_MEM)\n");
    }
    OSAL_TEST_INFO ("ABNORMAL USE CASE: Not deallocate the last buffer Finished------------\n");
#endif
#endif
/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDealloc ------------\n");

    OSAL_TEST_INFO("debug dealloc[4] buffer_handl (%p)\n", (void *)mmngr_buf[4]);

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[4]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc mmngr_buf[4] fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDealloc ------------\n");

/***********************************************************************/
#ifdef ABNORMAL
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    OSAL_TEST_INFO ("Isse of USE CASE: Re-allocate buffer (mmngr_buf[4]) ------------\n");

    OSAL_TEST_INFO("debug dealloc[3] buffer_handl (%p)\n", (void *)mmngr_buf[3]);

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[3]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_INFO (
            "Actual: R_OSAL_MmngrDealloc mmngr_buf[3] fail %d\n",ret_osal);
        OSAL_TEST_INFO (
            "Expect: R_OSAL_MmngrDealloc to return ret_osal = 2 (OSAL_RETURN_MEM)\n");
        OSAL_TEST_INFO (\
            "From now on, cannot deallocate any buffer because user missed the last\n \
            buffer's information. Call close is required if used up all allocation\n");
    }
    OSAL_TEST_INFO ("------------------------------------------------------\n");
#endif
#endif
/***********************************************************************/
#if 0 
    OSAL_TEST_INFO ("\nUSE CASE: Continue allocate after deallocate ------------\n");
    ret_osal = R_OSAL_MmngrAlloc(
          mmngr_hndl, 5 * 1024 * 1024, 4096, &mmngr_buf[4]);

    OSAL_TEST_INFO("debug alloc[4] buffer_handl (%p)\n", (void *)mmngr_buf[4]);

    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 5M, 4K align is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nUSE CASE: Continue allocate after deallocate OK------------\n");
#endif
/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrCheckBuffer ------------\n");
    ret_osal = R_OSAL_MmngrCheckBuffer(mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrCheckBuffer 25M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrCheckBuffer OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nUSE CASE: Check underflow event ------------\n");
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[1], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr to check underflow/overflow is fail %d\n",
            ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR VA monitor test: %p\n", va);
    ((char *)va)[-1] = 0x0;

    OSAL_TEST_INFO("debug: check buffer buffer_handl (%p)\n", (void *)mmngr_buf[1]);
    OSAL_TEST_INFO("debug: check buffer buffer_handl->self (%p)\n", (void *)mmngr_buf[1]->p_ptr_self);
    OSAL_TEST_INFO("debug: check buffer &buffer_handl->self (%p)\n", (void *)&mmngr_buf[1]->p_ptr_self);

    ret_osal = R_OSAL_MmngrCheckBuffer(mmngr_buf[1]);
    if (OSAL_RETURN_MEM != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrCheckBuffer to check underflow is fail %d\n",
        ret_osal);
        return -1;
    }
    ((char *)va)[-1] = 0xaa;
    OSAL_TEST_INFO ("\nUSE CASE: Check underflow event Finished------------\n");
    OSAL_TEST_INFO ("Expect: Callback will be called with event is 4\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nUSE CASE: Check overflow event ------------\n");
    ((char *)va)[10 * 1024 * 1024] = 0x0;

    OSAL_TEST_INFO("debug: check buffer buffer_handl (%p)\n", (void *)mmngr_buf[1]);

    ret_osal = R_OSAL_MmngrCheckBuffer(mmngr_buf[1]);
    if (OSAL_RETURN_MEM != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrCheckBuffer to check overflow is fail %d\n",
        ret_osal);
        return -1;
    }
    ((char *)va)[10 * 1024 * 1024] = 0xaa;
    OSAL_TEST_INFO ("\nUSE CASE: Check overflow event Finished------------\n");
    OSAL_TEST_INFO ("Expect: Callback will be called with event is 3\n");

/***********************************************************************/
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    /*limit: R_OSAL_MmngrGetHwAddr not get hw_addr via axi bus */
    ret_osal = R_OSAL_MmngrGetHwAddr(
            mmngr_buf[0], OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &hw_addr);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetHwAddr 25M, align 4K is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("MMNGR HW (first buffer): %lx\n", hw_addr);
#endif

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrFlush ------------\n");
    /* Check flush and Invalidate */
    ret_osal = R_OSAL_MmngrFlush(mmngr_buf[0], 0, 64);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrFlush offset 0, 64 bytes is fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrFlush (Failre arguments)------------\n");
    ret_osal = R_OSAL_MmngrFlush(mmngr_buf[0], 32, 64);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrFlush offset 32, 64 bytes is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrFlush(mmngr_buf[0], 96, 64);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrFlush offset 96, 64 bytes is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrFlush(mmngr_buf[0], 0, 63);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrFlush offset 0, 63 bytes is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrFlush (Failre arguments) Pass------------\n");

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrFlush OK------------\n");

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrInvalidate ------------\n");
    ret_osal = R_OSAL_MmngrInvalidate(mmngr_buf[0], 0, 64);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInvalidate offset 0, 64 bytes is fail %d\n",
            ret_osal);
        return -1;
    }

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrInvalidate (Failre arguments)------------\n");
    ret_osal = R_OSAL_MmngrInvalidate(mmngr_buf[0], 32, 64);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInvalidate offset 32, 64 bytes is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrInvalidate(mmngr_buf[0], 96, 64);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInvalidate offset 96, 64 bytes is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrInvalidate(mmngr_buf[0], 0, 63);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInvalidate offset 0, 63 bytes is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrInvalidate (Failre arguments) Pass------------\n");

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrInvalidate OK------------\n");

/***********************************************************************/
#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    OSAL_TEST_INFO (
        "\nUSE CASE: Check size and offset when flush and invalidate------------\n");
    /* Todo: should check offset + size must < buffer size ?
     * There isn't this requirement in R_OSAL_MmngrFlush() */
    ret_osal = R_OSAL_MmngrFlush(mmngr_buf[0], size[0], size[0]);
    if (OSAL_RETURN_PAR != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrFlush offset & size are size of buffer %d\n",
            ret_osal);
        return -1;
    }
    /* Todo: should check offset + size must < buffer size ?
     * There isn't this requirement in R_OSAL_MmngrInvalidate() */
    ret_osal = R_OSAL_MmngrInvalidate(mmngr_buf[0], size[0], size[0]);
    if (OSAL_RETURN_PAR != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInvalidate offset & size are size of buffer %d\n",
            ret_osal);
        return -1;
    }
    OSAL_TEST_INFO (
        "\nUSE CASE: Check size and offset when flush and invalidate Finished------------\n");
#endif

/***********************************************************************/
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugMmngrPrint ------------\n");
    check_mmngr = fopen("check_mmngr.txt", "a");
    ret_osal = R_OSAL_MmngrDebugMmngrPrint(mmngr_hndl, check_mmngr);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDebugMmngrPrint is fail %d\n",ret_osal);
        return -1;
    }
    fflush(check_buffer);
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugMmngrPrint OK------------\n");

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugMmngrPrint (Failre arguments)------------\n");
    check_mmngr_r = fopen("check_mmngr.txt", "ra");
    ret_osal = R_OSAL_MmngrDebugMmngrPrint(mmngr_hndl, check_mmngr_r);
    if (OSAL_RETURN_IO != ret_osal)
    {
        OSAL_TEST_ERR("R_OSAL_MmngrDebugMmngrPrint is OK(expect: io) %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrDebugMmngrPrint (Failre arguments) Pass------------\n");
#endif

/***********************************************************************/
#ifdef ABNORMAL
    OSAL_TEST_INFO ("\nABNORMAL USE CASE: Allocate ouf of range------------\n");
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 5 * 1024 * 1024, 4096, &mmngr_buf[4]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("Actual: R_OSAL_MmngrAlloc mmngr_buf[4] fail %d\n",ret_osal);
        OSAL_TEST_ERR ("Expect: R_OSAL_MmngrAlloc to return ret_osal = 1029 (OSAL_RETURN_PROHIBITED)\n");
    }
    OSAL_TEST_INFO ("\nABNORMAL USE CASE: Allocate ouf of range Finished------------\n");
#endif

/***********************************************************************/
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrClose ------------\n");

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[4]);
#ifndef OSAL_NOT_TEST_QNX 
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, tmp_mmngr_buf);
#endif
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[3]);
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[2]);
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[1]);
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[0]);

    ret_osal = R_OSAL_MmngrClose(mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrClose is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrClose OK------------\n");

    OSAL_TEST_INFO("==================== END MMNGR TEST ====================\n");

    /* OSAL deinitialize */
    (void)R_OSAL_Deinitialize();

#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    fclose(check_buffer);
    fclose(check_buffer_r);
    fclose(check_mmngr);
    fclose(check_mmngr_r);
#endif

    OSAL_TEST_INFO("\n\n<TEST END>\n\n");

#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */

    OSAL_TEST_INFO("==================== Add FreeList debug ====================\n");

    /* OSAL initialize */
    (void)R_OSAL_Initialize();


    ret_osal = R_OSAL_MmngrInitializeMmngrConfigSt(&mmngr_config);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInitializeMngrConfigSt is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrGetOsalMaxConfig(&max_config);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrGetOsalMaxConfig is fail %d\n",ret_osal);
        return -1;
    }

    mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_FREE_LIST;


    mmngr_config.memory_limit = 115 * 1024 * 1024; // 115Mb
    mmngr_config.max_allowed_allocations = 4;
    mmngr_config.max_registered_monitors_cbs = 4;

    ret_osal = R_OSAL_MmngrOpen(&mmngr_config, &mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrOpen is fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== Alloc 3 buffers ====================\n");
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 20 * 1024 * 1024, 64, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_0 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 20 * 1024 * 1024, 64, &mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_1 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 20 * 1024 * 1024, 64, &mmngr_buf[2]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_2 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== print VA of 3 buffers ====================\n");
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[0], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[0] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[1], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[1] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[2], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[2] VA: %p\n", va);

    OSAL_TEST_INFO("check VA Order 0->1->2\n");

    OSAL_TEST_INFO("==================== Dealloc mmngr_buf[1]  ====================\n");
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_1 fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== print VA of 3 buffers / expect:mmngr_buf[1] is failer ====================\n");
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[0], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[0] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[1], &va);
    if (OSAL_RETURN_OK == ret_osal)
    {
        OSAL_TEST_ERR (
            "failer expect:  %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[2], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[2] VA: %p\n", va);

    OSAL_TEST_INFO("==================== Re-alloc mmngr_buf[1]  ====================\n");
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 30 * 1024 * 1024, 64, &mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_1 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== alloc mmngr_buf[3]  ====================\n");
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 10 * 1024 * 1024, 64, &mmngr_buf[3]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_3 25M, align 64 is fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== print VA of 4 buffers ====================\n");

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[0], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[0] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[1], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[1] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[2], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[2] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[3], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[3] VA: %p\n", va);

    OSAL_TEST_INFO("check VA Order 0->3->2->1\n");


    OSAL_TEST_INFO("==================== Dealloc mmngr_buf[0->3]  ====================\n");
    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_0 fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_1 fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[2]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_2 fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[3]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_3 fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== Re-Alloc 3 buffers ====================\n");
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 20 * 1024 * 1024, 64, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_0 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 20 * 1024 * 1024, 64, &mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_1 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 20 * 1024 * 1024, 64, &mmngr_buf[2]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_2 20M, align 64 is fail %d\n",ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== print VA of 3 buffers ====================\n");
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[0], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[0] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[1], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[1] VA: %p\n", va);

    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[2], &va);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer \
            is fail %d\n",ret_osal);
        return -1;
    }
    OSAL_TEST_INFO("mmngr_buf[2] VA: %p\n", va);

    OSAL_TEST_INFO("check VA Order 0->1->2\n");

    OSAL_TEST_INFO ("\nAPI Check - R_OSAL_MmngrClose------------\n");
    ret_osal = R_OSAL_MmngrClose(mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrClose is fail %d\n",ret_osal);
        return -1;
    }

    (void)R_OSAL_Deinitialize();

    OSAL_TEST_INFO("\n\n<TEST END>\n\n");

#endif

#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
    OSAL_TEST_INFO("==================== Add Alignment check ====================\n");

    mmngr_buf[0] = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    mmngr_buf[1] = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    mmngr_buf[2] = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    mmngr_buf[3] = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    mmngr_buf[4] = OSAL_MEMORY_BUFFER_HANDLE_INVALID;

    OSAL_TEST_INFO("==================== Add Alignment check(OSAL_MMNGR_ALLOC_MODE_STACK) ====================\n");
    ret_osal = osal_mmngr_init(OSAL_MMNGR_ALLOC_MODE_STACK, &mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_init is fail %d\n", ret_osal);
        return -1;
    }
    for (i = 0; i < 5; i++)
    {
        ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[i], &va);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        ret_osal = R_OSAL_MmngrGetHwAddr(mmngr_buf[i], 0, &pa);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        OSAL_TEST_INFO("buffer[%d]  CPU addr; %p, HW addr; %p\n", i, va, pa);
    }
    ret_osal = osal_mmngr_quit(mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_quit is fail %d\n", ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== Add Alignment check(OSAL_MMNGR_ALLOC_MODE_STACK_PROT) ====================\n");
    ret_osal = osal_mmngr_init(OSAL_MMNGR_ALLOC_MODE_STACK_PROT, &mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_init is fail %d\n", ret_osal);
        return -1;
    }
    for (i = 0; i < 5; i++)
    {
        ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[i], &va);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        ret_osal = R_OSAL_MmngrGetHwAddr(mmngr_buf[i], 0, &pa);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        OSAL_TEST_INFO("buffer[%d]  CPU addr; %p, HW addr; %p\n", i, va, pa);
    }
    ret_osal = osal_mmngr_quit(mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_quit is fail %d\n", ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== Add Alignment check(OSAL_MMNGR_ALLOC_MODE_FREE_LIST) ====================\n");
    ret_osal = osal_mmngr_init(OSAL_MMNGR_ALLOC_MODE_FREE_LIST, &mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_init is fail %d\n", ret_osal);
        return -1;
    }
    for (i = 0; i < 5; i++)
    {
        ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[i], &va);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        ret_osal = R_OSAL_MmngrGetHwAddr(mmngr_buf[i], 0, &pa);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        OSAL_TEST_INFO("buffer[%d]  CPU addr; %p, HW addr; %p\n", i, va, pa);
    }
    ret_osal = osal_mmngr_quit(mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_quit is fail %d\n", ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("==================== Add Alignment check(OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT) ====================\n");
    ret_osal = osal_mmngr_init(OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT, &mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_init is fail %d\n", ret_osal);
        return -1;
    }
    for (i = 0; i < 5; i++)
    {
        ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[i], &va);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        ret_osal = R_OSAL_MmngrGetHwAddr(mmngr_buf[i], 0, &pa);
        if (OSAL_RETURN_OK != ret_osal)
        {
            OSAL_TEST_ERR (
                "R_OSAL_MmngrGetCpuPtr of case the same with previous buffer[%d] \
                is fail %d\n", i, ret_osal);
            return -1;
        }
        OSAL_TEST_INFO("buffer[%d]  CPU addr; %p, HW addr; %p\n", i, va, pa);
    }
    ret_osal = osal_mmngr_quit(mmngr_hndl, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "osal_mmngr_quit is fail %d\n", ret_osal);
        return -1;
    }

    OSAL_TEST_INFO("\n\n<TEST END>\n\n");
#endif


#ifndef OSAL_NOT_TEST_QNX /* TODO: Review API and tests */
#ifdef MEML_DEBUG

    OSAL_TEST_INFO ("\n--- Memory Allocator Check ---\n");

    (void)R_OSAL_Initialize();

    st_meml_manage_t hdl;
    uintptr_t alloc_addr[10];
    uintptr_t tmp;

    e_meml_return_t meml_ret;

    ret_osal = R_OSAL_MmngrGetOsalMaxConfig(&max_config);

    mmngr_config.mode = OSAL_MMNGR_ALLOC_MODE_STACK;
    mmngr_config.memory_limit = max_config.memory_limit - 4096;
    mmngr_config.max_allowed_allocations = 6;
    mmngr_config.max_registered_monitors_cbs = 6;

    ret_osal = R_OSAL_MmngrOpen(&mmngr_config, &mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrOpen is fail %d\n",ret_osal);
        return -1;
    }

    ret_osal = R_OSAL_MmngrAlloc(mmngr_hndl, 1024, 0, &mmngr_buf[0]);
    if (OSAL_RETURN_CONF != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc 1024, align 0 is NOT fail %d\n",ret_osal);
        return -1;
    }
    ret_osal = R_OSAL_MmngrGetCpuPtr(mmngr_buf[0], &va);

    OSAL_TEST_INFO("--- CMEM area %p, %zu\n",
            (void *)va,
            mmngr_config.memory_limit);

    if (OSAL_MEMORY_MANAGER_CFG_UNLIMITED != mmngr_config.memory_limit)
        R_MEML_Initialize(&hdl, (uintptr_t)va, mmngr_config.memory_limit, MEML_ALLOC_MODE_STACK);
    else
        R_MEML_Initialize(&hdl, (uintptr_t)va, 70*1024*1024, MEML_ALLOC_MODE_STACK);
    
    OSAL_TEST_INFO ("--- R_MEML_Alloc %d\n",ret_osal);
    alloc_addr[0] = R_MEML_Alloc(&hdl, 6*1024*1024, 0);
    alloc_addr[1] = R_MEML_Alloc(&hdl, 6*1024*1024, 2);
    alloc_addr[2] = R_MEML_Alloc(&hdl, 6*1024*1024, 4);
    alloc_addr[3] = R_MEML_Alloc(&hdl, 6*1024*1024, 8);
    alloc_addr[4] = R_MEML_Alloc(&hdl, 6*1024*1024, 16);
    alloc_addr[5] = R_MEML_Alloc(&hdl, 6*1024*1024, 32);
    alloc_addr[6] = R_MEML_Alloc(&hdl, 6*1024*1024, 64);
    alloc_addr[7] = R_MEML_Alloc(&hdl, 6*1024*1024, 128);
    alloc_addr[8] = R_MEML_Alloc(&hdl, 6*1024*1024, 256);
    alloc_addr[9] = R_MEML_Alloc(&hdl, 6*1024*1024, 1024);

    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    OSAL_TEST_INFO ("--- R_MEML_AllocR_MEML_Alloc(&hdl, %ld, 0)\n", mmngr_config.memory_limit/2);
    tmp = R_MEML_Alloc(&hdl, mmngr_config.memory_limit-9*1024, 64);
    OSAL_TEST_INFO("over size  addr %p\n", (void *)tmp);

    OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[1]))\n");
    meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[1]);
    OSAL_TEST_INFO("dealloc fail(expect 0)  %d\n", meml_ret);

    OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[7]))\n");
    meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[7]);
    OSAL_TEST_INFO("dealloc fail  %d\n", meml_ret);


    for (i=0; i<10; i++)
    {
        OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[%d]) \n",9-i);
        meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[9-i]);
        OSAL_TEST_INFO("dealloc OK  %d\n", meml_ret);
    }

    OSAL_TEST_INFO ("--- R_MEML_Alloc align \n");
    alloc_addr[0] = R_MEML_Alloc(&hdl, 6*1024*1024, 4096);
    alloc_addr[1] = R_MEML_Alloc(&hdl, 6*1024*1024, 2048);
    alloc_addr[2] = R_MEML_Alloc(&hdl, 6*1024*1024, 1024);
    alloc_addr[3] = R_MEML_Alloc(&hdl, 6*1024*1024, 512);
    alloc_addr[4] = R_MEML_Alloc(&hdl, 6*1024*1024, 256);
    alloc_addr[5] = R_MEML_Alloc(&hdl, 6*1024*1024, 128);
    alloc_addr[6] = R_MEML_Alloc(&hdl, 6*1024*1024, 64);
    alloc_addr[7] = R_MEML_Alloc(&hdl, 6*1024*1024, 32);
    alloc_addr[8] = R_MEML_Alloc(&hdl, 6*1024*1024, 16);
    alloc_addr[9] = R_MEML_Alloc(&hdl, 6*1024*1024, 0);

    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    meml_ret = R_MEML_Deinitialize(&hdl);
    OSAL_TEST_INFO("deinit fail  %d\n", meml_ret);

    for (i=0; i<10; i++)
    {
        meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[9-i]);
        OSAL_TEST_INFO("%d  dealloc OK  %d\n", i, meml_ret);
    }

    meml_ret = R_MEML_Deinitialize(&hdl);
    OSAL_TEST_INFO("deinit OK  %d\n", meml_ret);


    OSAL_TEST_INFO ("\n\n==== (LIST) ====\n");

    meml_ret = R_MEML_Initialize(&hdl, (uintptr_t)va, mmngr_config.memory_limit, MEML_ALLOC_MODE_FREE_LIST);
    OSAL_TEST_INFO ("--- R_MEML_Init(LIST) %d\n",meml_ret);

    OSAL_TEST_INFO ("--- R_MEML_Alloc(LIST) %d\n",ret_osal);
    alloc_addr[0] = R_MEML_Alloc(&hdl, 6*1024*1024, 0);
    alloc_addr[1] = R_MEML_Alloc(&hdl, 6*1024*1024, 2);
    alloc_addr[2] = R_MEML_Alloc(&hdl, 6*1024*1024, 4);
    alloc_addr[3] = R_MEML_Alloc(&hdl, 6*1024*1024, 8);
    alloc_addr[4] = R_MEML_Alloc(&hdl, 6*1024*1024, 16);
    alloc_addr[5] = R_MEML_Alloc(&hdl, 6*1024*1024, 32);
    alloc_addr[6] = R_MEML_Alloc(&hdl, 6*1024*1024, 64);
    alloc_addr[7] = R_MEML_Alloc(&hdl, 6*1024*1024, 128);
    alloc_addr[8] = R_MEML_Alloc(&hdl, 6*1024*1024, 256);
    alloc_addr[9] = R_MEML_Alloc(&hdl, 6*1024*1024, 1024);

    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    OSAL_TEST_INFO ("--- R_MEML_AllocR_MEML_Alloc(&hdl, %ld, 0)\n", mmngr_config.memory_limit/2);
    tmp = R_MEML_Alloc(&hdl, mmngr_config.memory_limit-9*1024, 0);
    OSAL_TEST_INFO("over size  addr %p\n", (void *)tmp);

    OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[1])\n");
    meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[1]);
    OSAL_TEST_INFO("dealloc fail(expect 0)  %d\n", meml_ret);
    alloc_addr[1] = (uintptr_t)NULL;
    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    OSAL_TEST_INFO ("--- R_MEML_AllocR_MEML_Alloc(&hdl, %d, 0)\n", 3*1024*1024);
    alloc_addr[1] = R_MEML_Alloc(&hdl, 3*1024*1024, 64);
    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[9])  %p\n", (void *)alloc_addr[9]);
    meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[9]);
    alloc_addr[9] = (uintptr_t)NULL;
    OSAL_TEST_INFO("dealloc OK  %d\n", meml_ret);
    OSAL_TEST_INFO ("--- R_MEML_AllocR_MEML_Alloc(&hdl, %d, 0)\n", 1*1024*1024);
    alloc_addr[9] = R_MEML_Alloc(&hdl, 1*1024*1024, 64);
    OSAL_TEST_INFO ("Add totyu? %p\n", (void *)alloc_addr[9]);

    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    OSAL_TEST_INFO ("--- R_MEML_AllocR_MEML_Alloc(&hdl, %d, 0)\n", 20*1024*1024);
    alloc_addr[3]  = R_MEML_Alloc(&hdl, 20*1024*1024, 64);
    OSAL_TEST_INFO ("Add tail? %p\n", (void *)tmp);

    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    for (i=0; i<10; i++)
    {
        OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[%d])  %p\n", i, (void *)alloc_addr[i]);
        meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[i]);
        OSAL_TEST_INFO("%d  dealloc OK  %d\n", i, meml_ret);
    }

    OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, tmp)  %p\n", (void *)tmp);
    meml_ret = R_MEML_Dealloc(&hdl, tmp);
    OSAL_TEST_INFO("%d  dealloc OK  %d\n", i, meml_ret);

    OSAL_TEST_INFO ("--- R_MEML_Alloc align \n");
    alloc_addr[0] = R_MEML_Alloc(&hdl, 6*1024*1024, 4096);
    alloc_addr[1] = R_MEML_Alloc(&hdl, 6*1024*1024, 2048);
    alloc_addr[2] = R_MEML_Alloc(&hdl, 6*1024*1024, 1024);
    alloc_addr[3] = R_MEML_Alloc(&hdl, 6*1024*1024, 512);
    alloc_addr[4] = R_MEML_Alloc(&hdl, 6*1024*1024, 256);
    alloc_addr[5] = R_MEML_Alloc(&hdl, 6*1024*1024, 128);
    alloc_addr[6] = R_MEML_Alloc(&hdl, 6*1024*1024, 64);
    alloc_addr[7] = R_MEML_Alloc(&hdl, 6*1024*1024, 32);
    alloc_addr[8] = R_MEML_Alloc(&hdl, 6*1024*1024, 16);
    alloc_addr[9] = R_MEML_Alloc(&hdl, 6*1024*1024, 0);

    for (i=0; i<10; i++)
        OSAL_TEST_INFO("%d addr %p\n", i, (void *)alloc_addr[i]);

    for (i=0; i<10; i++)
    {
        OSAL_TEST_INFO ("--- R_MEML_Dealloc(&hdl, alloc_addr[%d])  %p\n", i, (void *)alloc_addr[i]);
        meml_ret = R_MEML_Dealloc(&hdl, alloc_addr[i]);
        OSAL_TEST_INFO("%d  dealloc OK  %d\n", i, meml_ret);
    }

    meml_ret = R_MEML_Deinitialize(&hdl);
    OSAL_TEST_INFO("deinit OK  %d\n", meml_ret);

    (void)R_OSAL_Deinitialize();

#endif
#endif
}

e_osal_return_t osal_mmngr_init(e_osal_mmngr_allocator_mode_t mode,
                                osal_memory_manager_handle_t *mmngr_hndl,
                                osal_memory_buffer_handle_t *mmngr_buf)
{

    st_osal_mmngr_config_t      mmngr_config;
    e_osal_return_t             ret_osal;

    /* OSAL initialize */
    (void)R_OSAL_Initialize();

    ret_osal = R_OSAL_MmngrInitializeMmngrConfigSt(&mmngr_config);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR (
            "R_OSAL_MmngrInitializeMngrConfigSt is fail %d\n",ret_osal);
        return ret_osal;
    }

    mmngr_config.mode = mode;
    mmngr_config.memory_limit = 115 * 1024 * 1024; // 115Mb
    mmngr_config.max_allowed_allocations = 5;
    mmngr_config.max_registered_monitors_cbs = 4;

    ret_osal = R_OSAL_MmngrOpen(&mmngr_config, mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrOpen is fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrAlloc(*mmngr_hndl, 10 * 1024, 1 * 1024 * 1024, &mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_0 is fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrAlloc(*mmngr_hndl, 20 * 1024, 2 * 1024 * 1024, &mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_1 is fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrAlloc(*mmngr_hndl, 40 * 1024, 4 * 1024 * 1024, &mmngr_buf[2]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_2 is fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrAlloc(*mmngr_hndl, 64, 8 * 1024 * 1024, &mmngr_buf[3]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_3 is fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrAlloc(*mmngr_hndl, 128, 16 * 1024 * 1024, &mmngr_buf[4]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrAlloc_4 is fail %d\n",ret_osal);
        return ret_osal;
    }

    return ret_osal;
}

e_osal_return_t osal_mmngr_quit(osal_memory_manager_handle_t mmngr_hndl,
                                osal_memory_buffer_handle_t *mmngr_buf)
{
    e_osal_return_t             ret_osal;

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[4]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_4 fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[3]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_3 fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[2]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_2 fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[1]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_1 fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrDealloc(mmngr_hndl, mmngr_buf[0]);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrDealloc_0 fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_MmngrClose(mmngr_hndl);
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_MmngrClose is fail %d\n",ret_osal);
        return ret_osal;
    }

    ret_osal = R_OSAL_Deinitialize();
    if (OSAL_RETURN_OK != ret_osal)
    {
        OSAL_TEST_ERR ("R_OSAL_Deinitialize is fail %d\n", ret_osal);
        return ret_osal;
    }

    return ret_osal;
}
