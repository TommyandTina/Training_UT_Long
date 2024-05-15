/***********************************************************************************************************************
* Copyright [2021-2023] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
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
* File Name :    osal_sample_memory_main.c
* Version :      3.3.0
* Product Name : OSAL
* Device(s) :    R-Car
* Description :  Sample application to use OSAL Memory API
***********************************************************************************************************************/
#include "osal_sample_memory_common.h"

static int32_t alloc_use_main_task(void);
static int32_t init_obj_main_task(void);
#if defined(SUPPORT_MULTI_MEMORY_REGION)
static int32_t alloc_use_multi_memory_task(void);
#endif
static int32_t common_alloc_use(osal_memory_manager_handle_t memHandle, osal_memory_region_idx_t region_idx);
static int32_t memory_get_memory_configuration(osal_memory_manager_handle_t memHandle, st_osal_mmngr_config_t* memConfig);
static int32_t memory_get_debug_information_memory_handle(osal_memory_manager_handle_t memHandle);
static int32_t memory_setup_monitor_callback(osal_memory_manager_handle_t memHandle, st_osal_mmngr_config_t memConfig,
                                        fp_osal_memory_mngr_cb_t *monitor_handler);
static int32_t memory_allocate_buffers(osal_memory_manager_handle_t memHandle, size_t size, size_t align, size_t number_of_buffer, osal_memory_buffer_handle_t * buffHandle);
static int32_t memory_get_debug_information_buffer_handle (osal_memory_buffer_handle_t const * buffHandle, size_t number_of_buffer);
static int32_t memory_get_debug_information_memory_handle (osal_memory_manager_handle_t memHandle);
static int32_t memory_check_buffer(osal_memory_buffer_handle_t buffHandle);
static int32_t memory_get_size(osal_memory_buffer_handle_t buffHandle, size_t * getSize);
static int32_t memory_get_cpu_access(osal_memory_buffer_handle_t buffHandle, void **cpuPTR, const void **cpuConstPTR);
static int32_t memory_get_hardware_address(osal_memory_buffer_handle_t buffHandle, osal_axi_bus_id_t axi_id, uintptr_t *hwAddress);
static int32_t memory_synchronize_memory(osal_memory_buffer_handle_t buffHandle, size_t bufferSize, void* cpuPTR);
static int32_t memory_deallocate_buffers(osal_memory_manager_handle_t memHandle, st_osal_mmngr_config_t memConfig, osal_memory_buffer_handle_t const * buffHandle, size_t number_of_buffer);
static void monitorCb(void* const user_arg, e_osal_mmngr_event_t event, e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl);

/**
 * @defgroup osal_sample_memory OSAL Application: Sample for OSAL Memory Usage
 * @{
 */
/**
 * @brief The main function of OSAL Sample Memory application
 * - Call R_OSAL_GetVersion and output OSAL version
 * - Call R_OSAL_Initialize and call the memory_task function. Then call R_OSAL_Deinitialize
 * - The memory_task is divided into 3 main tasks as below:
 *      -# @link alloc_use_main_task @endlink describes the function of Memory allocate and use
 *      -# @link init_obj_main_task @endlink describes the initialization memory buffer and handle object APIS
 *      -# @link alloc_use_multi_memory_task @endlink describes the function of Memory allocate and use in multiple region
 * 
 * @return 0 on success
 * @return -1 on failure
 */
/* PRQA S 5209 1 # Since the function main() should have the type 'int' for the return value and the first argument, do not modify it */
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv; /* PRQA S 2956 # No problem because argv is not used */

#if defined(SAMPLE_LINUX)
    char const rmmod_cmemdrv[14] = "rmmod cmemdrv";
    char const insmod_cmemdrv[64] = "insmod /lib/modules/*/extra/cmemdrv.ko bsize=536870912,16777216";
    /* PRQA S 5126 1 # It is used to config the cmem driver in Linux */
    (void)system(rmmod_cmemdrv);
    /* PRQA S 5126 1 # It is used to config the cmem driver in Linux */
    (void)system(insmod_cmemdrv);
#endif

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_memory start ==================\n");
    /* local variable */
    e_osal_return_t osal_ret;
    /* PRQA S 5209 1 # This variable is used in the return value of the main function, do not modify it */
    int app_ret = 0;
    bool success = true;
    const st_osal_version_t *version;

    version = R_OSAL_GetVersion();
    printf("OSAL API version %u.%u.%u\n", version->api.major, version->api.minor, version->api.patch );
    printf("OSAL Wrapper version %u.%u.%u\n", version->wrapper.major, version->wrapper.minor, version->wrapper.patch );

    OSAL_SAMPLE_INFO("initialize osal\n");
    osal_ret = R_OSAL_Initialize();
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        success = false;
        OSAL_SAMPLE_ERR("R_OSAL_Initialize was failed, osal_ret = %d\n", osal_ret);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("execute alloc_use_main_task\n");
        app_ret = alloc_use_main_task();
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("alloc_use_main_task operation is completed\n");
        }
        else
        {
            success = false;
            OSAL_SAMPLE_ERR("alloc_use_main_task operation was failed\n");
        }


        OSAL_SAMPLE_INFO("execute init_obj_main_task\n");
        app_ret = init_obj_main_task();
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("init_obj_main_task operation is completed\n");
        }
        else
        {
            success = false;
            OSAL_SAMPLE_ERR("init_obj_main_task operation was failed\n");
        }

#if defined(SUPPORT_MULTI_MEMORY_REGION)
        OSAL_SAMPLE_INFO("execute alloc_use_multi_memory_task\n");
        app_ret = alloc_use_multi_memory_task();
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("alloc_use_multi_memory_task operation is completed\n");
        }
        else
        { 
            success = false;
            OSAL_SAMPLE_ERR("alloc_use_multi_memory_task operation was failed\n");
        }
#endif

        OSAL_SAMPLE_INFO("de-initialize osal\n");
        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            success = false;
            OSAL_SAMPLE_ERR("R_OSAL_Deinitialize was failed, osal_ret = %d\n", osal_ret);
        }

    }
    if (true == success)
    {
        OSAL_SAMPLE_INFO("osal_sample_memory run successfully\n");
    }
    else
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("osal_sample_memory was failed\n");
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== osal_sample_memory finish ==================\n");
    return app_ret;
}

/**
 * @defgroup alloc_use_main_task OSAL Memory allocation and use APIs in default region index
 * @{
 */
/**
 * @brief Alloc-use main task:<br>
 * (OSAL API UM reference: Chapter 2.9.1: allocate & use)
 * - Open a memory handle by using R_OSAL_MmngrOpen with max configuration
 * - Allocate and use memory by using @link common_alloc_use @endlink
 * - Close memory handle by using R_OSAL_MmngrClose
 * 
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t alloc_use_main_task(void)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    osal_memory_manager_handle_t memHandle = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    st_osal_mmngr_config_t memMaxConfig;
    bool open_success = true;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("\n================== alloc_use_main_task start ==================\n");

    OSAL_SAMPLE_INFO("Get the maximum memory configuration with the default region index (%u)\n", SAMPLE_VALUE_REGION_INDEX_DEFAULT);
    osal_ret = R_OSAL_MmngrGetOsalMaxConfig(&memMaxConfig);
    if (OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot get the memory max configuration, osal_ret = %d\n", osal_ret);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Open Memory Handle with the max configuration with the default region index (%u)\n", SAMPLE_VALUE_REGION_INDEX_DEFAULT);
        osal_ret = R_OSAL_MmngrOpen(&memMaxConfig, &memHandle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            open_success = false;
            OSAL_SAMPLE_ERR("Open memory handle failed , osal_ret = %d\n", osal_ret);
        }
    }
    
    if (0 == app_ret)
    {
        /* Allocation and use */
        app_ret = common_alloc_use(memHandle, SAMPLE_VALUE_REGION_INDEX_DEFAULT);
        if (0 != app_ret)
        {
            OSAL_SAMPLE_ERR("Common allocation usage failed\n");
        }
        else
        {
            OSAL_SAMPLE_INFO("Common allocation usage success\n");
        }
    }
    if (true == open_success)
    {
        OSAL_SAMPLE_INFO("Close memory handle\n");
        osal_ret = R_OSAL_MmngrClose(memHandle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Cannot close memory manager, osal_ret = %d\n", osal_ret);
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== alloc_use_main_task finish ==================\n\n");
    return app_ret;
}
/**@} alloc_use_main_task*/

/**
 * @defgroup common_alloc_use OSAL Memory Common allocation and use APIs
 * @{
 */
/**
 * @brief Memory allocation and use function:
 * - Initialize the memory configuration structure and get the current memory configuration by using @link memory_get_memory_configuration @endlink
 * - Get the debug information of memory handle by using @link memory_get_debug_information_memory_handle @endlink
 * - Setup the monitor callback for some events by using @link memory_setup_monitor_callback @endlink
 * - Allocate buffers which size and alignment are 64-byte by using @link memory_allocate_buffers @endlink
 * - Get the debug information of memory buffers by using @link memory_get_debug_information_memory_handle @endlink
 * - If allocation mode is OSAL_MMNGR_ALLOC_MODE_***_PROT, check memory buffer underflow or overflow by using @link memory_check_buffer @endlink
 * - Get buffer size of allocated buffer by using @link memory_get_size @endlink.<br>
 * The expected value of "bufferSize" is 64-byte
 * - Get CPU accessible pointer by using @link memory_get_cpu_access @endlink
 * - Get AXI bus domain specific address of buffer by using @link memory_get_hardware_address @endlink
 * - Modify CPU buffer and synchronizes to HW then synchronizes the buffer HW memory to the CPU memory by using @link memory_synchronize_memory @endlink
 * - Deallocate allocated buffer by using @link memory_deallocate_buffers @endlink
 *         In this case, sample app use the same order of the allocation. * 
 * @param[in] memHandle The opened OSAL memory manager handle
 * @param[in] region_idx The memory region index corresponding with memHandle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t common_alloc_use(osal_memory_manager_handle_t memHandle, osal_memory_region_idx_t region_idx)
{
    /* local variable */
    int32_t app_ret;
    st_osal_mmngr_config_t memConfig;
    osal_memory_buffer_handle_t buffHandle[NUM_OF_BUFFER] = {OSAL_MEMORY_BUFFER_HANDLE_INVALID,
                                                    OSAL_MEMORY_BUFFER_HANDLE_INVALID,
                                                        OSAL_MEMORY_BUFFER_HANDLE_INVALID};
    size_t bufferSize = 0;
    void *cpuPTR;
    const void *cpuConstPTR;
    uintptr_t hwAddress;

    /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
    OSAL_SAMPLE_INFO("Allocate and use the buffer of memory region index (%lu)\n", (unsigned long)region_idx);
    OSAL_SAMPLE_INFO("Get the memory configuration structure of the opened memory handle\n");
    app_ret = memory_get_memory_configuration(memHandle, &memConfig);
    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get memory configuration success\n");
    }
    else
    {
        OSAL_SAMPLE_ERR("Get memory configuration failed\n");
    }

    OSAL_SAMPLE_INFO("Dump the memory configuration information\n");
    switch (memConfig.mode)
    {
        case OSAL_MMNGR_ALLOC_MODE_STACK_PROT :
            OSAL_SAMPLE_INFO("Allocation with mode OSAL_MMNGR_ALLOC_MODE_STACK_PROT\n");
            break;
        case OSAL_MMNGR_ALLOC_MODE_FREE_LIST :
            OSAL_SAMPLE_INFO("Allocation with mode OSAL_MMNGR_ALLOC_MODE_FREE_LIST\n");
            break;
        case OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT :
            OSAL_SAMPLE_INFO("Allocation with mode OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT\n");
            break;
        default:
            OSAL_SAMPLE_INFO("Allocation with mode OSAL_MMNGR_ALLOC_MODE_STACK\n");
            break;
    }
    /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
    OSAL_SAMPLE_INFO("Memory limit: %lu\n", (unsigned long)memConfig.memory_limit);
    /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
    OSAL_SAMPLE_INFO("Maximum allowed allocations: %lu\n", (unsigned long)memConfig.max_allowed_allocations);
    /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
    OSAL_SAMPLE_INFO("Maximum number of registered monitors callbacks: %lu\n", (unsigned long)memConfig.max_registered_monitors_cbs);

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the debug information of memory handle and output log to stdout\n");
        app_ret = memory_get_debug_information_memory_handle(memHandle);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Register the monitor callbacks\n");
        app_ret = memory_setup_monitor_callback(memHandle, memConfig, monitorCb);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Setup monitor callback success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Setup monitor callback failed\n");
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Allocate 3 buffers with the same size (64-byte) and alignment (64-byte)\n");
        app_ret = memory_allocate_buffers(memHandle, ALLOC_SIZE, ALLOC_ALIGN, NUM_OF_BUFFER, buffHandle);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Allocate buffers success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Allocate buffers failed\n");
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get debug information memory buffers\n");
        app_ret = memory_get_debug_information_buffer_handle(buffHandle, NUM_OF_BUFFER);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the debug information of memory handle after finishing the buffer allocation and output log to stdout\n\n");
        app_ret = memory_get_debug_information_memory_handle(memHandle);
    }

    if (0 == app_ret)
    {
        if ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == memConfig.mode) || (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == memConfig.mode))
        {
            OSAL_SAMPLE_INFO("Check buffer underflow or overflow for the first allocated buffer (only available in OSAL_MMNGR_ALLOC_MODE_***_PROT mode)\n");
            OSAL_SAMPLE_INFO("No OSAL_MMNGR_EVENT_OVERFLOW_DETECTED or OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED event will be occur\n");
            app_ret = memory_check_buffer(buffHandle[0]);
            if (0 == app_ret)
            {
                OSAL_SAMPLE_INFO("Memory check buffer success\n");
            }
            else
            {
                OSAL_SAMPLE_ERR("Memory check buffer failed\n");
            }
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get buffer size of the first buffer. The returned size will be (%u)\n", ALLOC_SIZE);
        app_ret = memory_get_size(buffHandle[0], &bufferSize);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Memory get buffer size success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Memory get buffer size failed\n");
        }

        if (ALLOC_SIZE != bufferSize)
        {
            OSAL_SAMPLE_ERR("Operation failed\n");
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Memory get CPU accessible pointer\n");
        app_ret = memory_get_cpu_access(buffHandle[0], &cpuPTR, &cpuConstPTR);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Memory get CPU accessible pointer success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Memory get CPU accessible pointer failed\n");
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get AXI bus domain specific address which related to OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY\n");
        app_ret = memory_get_hardware_address(buffHandle[0], OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &hwAddress);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Get AXI bus domain specific address success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Get AXI bus domain specific address failed\n");
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Synchronizes CPU memory to HW then synchronizes HW memory to CPU with offset is 0, size is 64-byte\n");
        app_ret = memory_synchronize_memory(buffHandle[0], bufferSize, cpuPTR);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Synchronize memory success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Synchronize memory failed\n");
        }
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Show the information of the 1st allocated buffer\n");
        /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
        (void)printf("|%-12s|%-18s|%-18s|%-18s|%-11s|%-15s|\n", "Region Index", "CPU address", "CPU(Const) address", "HW address", "Buffer size", "Buffer alignment");
        /* PRQA S 5124,0306,0326 2 # It is used for the request to output the execution result of the sample.*/
        /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
        (void)printf("|%-12lu|0x%016x|0x%016x|0x%016x|%-11u|%-15u|\n", (unsigned long)region_idx, (unsigned int)((uintptr_t)cpuPTR), (unsigned int)((uintptr_t)cpuConstPTR), (unsigned int)hwAddress, ALLOC_SIZE, ALLOC_ALIGN);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Memory deallocate buffer\n");
        app_ret = memory_deallocate_buffers(memHandle, memConfig, buffHandle, NUM_OF_BUFFER);
        if (0 == app_ret)
        {
            OSAL_SAMPLE_INFO("Deallocate buffers success\n");
        }
        else
        {
            OSAL_SAMPLE_ERR("Deallocate buffers failed\n");
        }
    }

    return app_ret;
/* PRQA S 9107,9104 2 # Sample Application aims to show how to use the OSAL API and the order in which it is called. Splitting functions reduces readability.
    There are no significant side effects to the behavior of the sample app code without modification. */
}

/**
 * @brief Dummy monitor callback function
 * - Print the callback event for user
 */
/* PRQA S 3673 1 # Implemented in accordance with the definition of fp_osal_memory_mngr_cb_t. */
/* PRQA S 3673 2 # Implemented in accordance with the definition of fp_osal_memory_mngr_cb_t. */
static void monitorCb(void* const user_arg, e_osal_mmngr_event_t event,
    e_osal_return_t error, osal_memory_buffer_handle_t buffer_hndl)
{
    (void)user_arg;
    (void)error;
    (void)buffer_hndl;

    switch (event)
    {
        case OSAL_MMNGR_EVENT_ALLOCATE :
            OSAL_SAMPLE_INFO("====== EVENT: OSAL_MMNGR_EVENT_ALLOCATE ======\n");
            break;
        case OSAL_MMNGR_EVENT_DEALLOCATE :
            OSAL_SAMPLE_INFO("====== EVENT: OSAL_MMNGR_EVENT_DEALLOCATE ======\n");
            break;
        case OSAL_MMNGR_EVENT_OVERFLOW_DETECTED :
            OSAL_SAMPLE_INFO("====== EVENT: OSAL_MMNGR_EVENT_OVERFLOW_DETECTED ======\n");
            break;
        case OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED :
            OSAL_SAMPLE_INFO("====== EVENT: OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED ======\n");
            break;
        default:
            OSAL_SAMPLE_INFO("====== EVENT: OSAL_MMNGR_EVENT_INVALID ======\n");
            break;
    }
}

/**
 * @brief Get the memory configuration:
 * - Get the current memory configuration with below steps:
 *      -# Initialize the memory configuration structure by using R_OSAL_MmngrInitializeMmngrConfigSt
 *      -# Get the current memory configuration by using R_OSAL_MmngrGetConfig
 * @param[in] memHandle OSAL memory manager handle
 * @param[in,out] memConfig This parameter is to store the configuration of the input memory handle memHandle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_get_memory_configuration(osal_memory_manager_handle_t memHandle, st_osal_mmngr_config_t* memConfig)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    OSAL_SAMPLE_INFO("Initialize the memory configuration structure\n");
    osal_ret = R_OSAL_MmngrInitializeMmngrConfigSt(memConfig);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Memory configuration initialization failed, osal_ret = %d\n", osal_ret);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Get the current memory configuration of opened memory handle\n");
        osal_ret = R_OSAL_MmngrGetConfig(memHandle, memConfig);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Get the configuration of opened memory manager failed, osal_ret = %d\n", osal_ret);
        }
        else if((memConfig->mode != OSAL_MMNGR_ALLOC_MODE_STACK) && (memConfig->mode != OSAL_MMNGR_ALLOC_MODE_STACK_PROT)
                    && (memConfig->mode != OSAL_MMNGR_ALLOC_MODE_FREE_LIST) && (memConfig->mode != OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT))
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Allocation mode (%d) is invalid\n", memConfig->mode);
        }
        else
        {
        /* Do nothing */
        }
    }
    return app_ret;
}

/**
 * @brief Get the memory handle debug information:
 * - Get the memory handle debug information with below steps:
 *      -# Get debug information of memory handle by using R_OSAL_MmngrDebugMmngrPrint with stdout
 * @param[in] memHandle OSAL memory manager handle. This function shows its debug information.
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_get_debug_information_memory_handle(osal_memory_manager_handle_t memHandle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    osal_ret = R_OSAL_MmngrDebugMmngrPrint(memHandle, stdout);
    if((OSAL_RETURN_HANDLE_NOT_SUPPORTED == osal_ret) || (OSAL_RETURN_UNSUPPORTED_OPERATION == osal_ret))
    {
        OSAL_SAMPLE_INFO("Debugging feature is not supported in release mode, osal_ret = %d\n", osal_ret);
    }
    else if(OSAL_RETURN_IO == osal_ret)
    {
        OSAL_SAMPLE_INFO("Test environment does not support stdout as a standard output, osal_ret = %d\n", osal_ret);
    }
    else if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot print memory manager information, osal_ret = %d\n", osal_ret);
    }
    else
    {
        /* Don't process */
    }

    return app_ret;
}

/**
 * @brief Setup the memory monitor callback
 * - Setup the memory monitor callback with below steps:
 *      -# Register monitor callback function @link monitorCb @endlink for OSAL_MMNGR_EVENT_ALLOCATE event by using R_OSAL_MmngrRegisterMonitor
 *      -# Register monitor callback function @link monitorCb @endlink for OSAL_MMNGR_EVENT_DEALLOCATE event by using R_OSAL_MmngrRegisterMonitor
 *      -# If allocation mode is OSAL_MMNGR_ALLOC_MODE_***_PROT, register monitor callback function @link monitorCb @endlink for OSAL_MMNGR_EVENT_OVERFLOW_DETECTED event by using R_OSAL_MmngrRegisterMonitor.
 *      -# If allocation mode is OSAL_MMNGR_ALLOC_MODE_***_PROT, register monitor callback function @link monitorCb @endlink for OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED event by using R_OSAL_MmngrRegisterMonitor
 * @param[in] memHandle OSAL memory manager handle
 * @param[in] memConfig The configuration of memory handle memHandle
 * @param[in] monitor_handler The monitor callback function which will be triggered each time an OSAL memory event occurs
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_setup_monitor_callback(osal_memory_manager_handle_t memHandle, st_osal_mmngr_config_t memConfig,
                                        fp_osal_memory_mngr_cb_t *monitor_handler)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    /* Setup the monitor callback. 
    * OSAL_MMNGR_EVENT_OVERFLOW_DETECTED and OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED only be available in OSAL_MMNGR_ALLOC_MODE_***_PROT*/
    OSAL_SAMPLE_INFO("Setup the monitor callbacks for event OSAL_MMNGR_EVENT_ALLOCATE \n");
    osal_ret = R_OSAL_MmngrRegisterMonitor(memHandle, OSAL_MMNGR_EVENT_ALLOCATE, monitor_handler, NULL);
    if (OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Register monitor for OSAL_MMNGR_EVENT_ALLOCATE failed, osal_ret = %d\n", osal_ret);
    }

    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Setup the monitor callbacks for event OSAL_MMNGR_EVENT_DEALLOCATE \n");
        osal_ret = R_OSAL_MmngrRegisterMonitor(memHandle, OSAL_MMNGR_EVENT_DEALLOCATE, monitor_handler, NULL);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Register monitor for OSAL_MMNGR_EVENT_DEALLOCATE failed, osal_ret = %d\n", osal_ret);
        }
    }

    if ((0 == app_ret) && ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == memConfig.mode) || (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == memConfig.mode)))
    {
        OSAL_SAMPLE_INFO("Setup the monitor callbacks for event OSAL_MMNGR_EVENT_OVERFLOW_DETECTED \n");
        osal_ret = R_OSAL_MmngrRegisterMonitor(memHandle, OSAL_MMNGR_EVENT_OVERFLOW_DETECTED, monitor_handler, NULL);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Register monitor for OSAL_MMNGR_EVENT_OVERFLOW_DETECTED failed, osal_ret = %d\n", osal_ret);
        }
    }

    if ((0 == app_ret) && ((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == memConfig.mode) || (OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT == memConfig.mode)))
    {
        OSAL_SAMPLE_INFO("Setup the monitor callbacks for event OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED \n");
        osal_ret = R_OSAL_MmngrRegisterMonitor(memHandle, OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED, monitor_handler, NULL);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Register monitor for OSAL_MMNGR_EVENT_UNDERFLOW_DETECTED failed, osal_ret = %d\n", osal_ret);
        }
    }

    return app_ret;
}

/**
 * @brief Allocate the memory buffers
 * - Allocate buffers which size and alignment are 64-byte. Please refer to the steps at below:
 *      -# Use a "for" loop to allocate number of buffers by using R_OSAL_MmngrAlloc
 *      -# If there is any failed allocation, de-allocate previous allocated buffer by using R_OSAL_MmngrDealloc
 * @param[in] memHandle OSAL memory manager handle
 * @param[in] size The buffer size of OSAL memory buffer which is going to be allocated
 * @param[in] align The buffer alignment of OSAL memory buffer which is going to be allocated
 * @param[in] number_of_buffer The expected number of buffers that user want to allocate.<br>
 * This number needs to be less than or equal to the max_allowed_allocations in memory manager memHandle's configuration.
 * @param[in,out] buffHandle An address of OSAL memory buffer handle which represents for the allocated buffers
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_allocate_buffers(osal_memory_manager_handle_t memHandle, size_t size, size_t align, size_t number_of_buffer, osal_memory_buffer_handle_t * buffHandle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    int32_t allocated_buffer = 0;

    for (size_t i = 0; (i < number_of_buffer) && (0 == app_ret); i++)
    {
        osal_ret = R_OSAL_MmngrAlloc(memHandle, size, align, &buffHandle[i]);
        if (OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Memory allocation failed, osal_ret = %d\n", osal_ret);
        }
        else
        {
            allocated_buffer++;
        }
    }

    if ((0 != app_ret) && (0 < allocated_buffer))
    {
        for (int32_t i = allocated_buffer - (int32_t)1; i >= 0; i--)
        {
            osal_ret = R_OSAL_MmngrDealloc(memHandle, buffHandle[i]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_INFO("Memory de-allocation failed, osal_ret = %d\n", osal_ret);
            }
        }
    }

    return app_ret;
}

/**
 * @brief Get the memory buffer handle debug information
 * - Get the debug information of memory buffers with steps at below:
 *      -# Get debug information of 3 memory buffers by using R_OSAL_MmngrDebugBufferPrint with stdout
 * @param[in] buffHandle An address of OSAL memory buffer handle. This function shows debug information of its elements.
 * @param[in] number_of_buffer The number of elements (OSAL memory buffer handle) in buffHandle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_get_debug_information_buffer_handle(osal_memory_buffer_handle_t const * buffHandle, size_t number_of_buffer)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    for (size_t i = 0; i < number_of_buffer; i++)
    {
        osal_ret = R_OSAL_MmngrDebugBufferPrint(buffHandle[i], stdout);
        if((OSAL_RETURN_HANDLE_NOT_SUPPORTED == osal_ret) || (OSAL_RETURN_UNSUPPORTED_OPERATION == osal_ret))
        {
            OSAL_SAMPLE_INFO("Debugging feature is not supported in release mode, osal_ret = %d\n", osal_ret);
        }
        else if(OSAL_RETURN_IO == osal_ret)
        {
            OSAL_SAMPLE_INFO("Test environment does not support stdout as a standard output, osal_ret = %d\n", osal_ret);
        }
        else if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Cannot print memory manager information, osal_ret = %d\n", osal_ret);
        }
        else
        {
            /* Don't process */
        }
    }

    return app_ret;
}

/**
 * @brief Check the memory buffer underflow or overflow
 * - Check the memory buffer underflow or overflow with steps at below:
 *      -# Check memory buffer underflow or overflow by using R_OSAL_MmngrCheckBuffer.
 * @param[in] buffHandle OSAL memory buffer handle. This function checks the buffer overflow/underflow possibility of this memory buffer.
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_check_buffer(osal_memory_buffer_handle_t buffHandle)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    osal_ret = R_OSAL_MmngrCheckBuffer(buffHandle);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot check the underflow or overflow of allocated buffer, osal_ret = %d\n", osal_ret);
    }

    return app_ret;
}

/**
 * @brief Get the memory buffer size
 * - Check the memory buffer size with steps at below:
 *      -# Get buffer size of allocated buffer by using R_OSAL_MmngrGetSize.
 * @param[in] buffHandle OSAL memory buffer handle
 * @param[in,out] getSize This parameter is to store the size of memory buffer buffHandle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_get_size(osal_memory_buffer_handle_t buffHandle, size_t * getSize)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    osal_ret = R_OSAL_MmngrGetSize(buffHandle, getSize);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Get allocated buffer size failed, osal_ret = %d\n", osal_ret);
    }

    return app_ret;
}

/**
 * @brief Get CPU accessible (read/write and read only) pointers
 * - Get CPU accessible (read/write and read only) pointers with steps at below:
 *      -# Get CPU accessible (read/write) pointer by using R_OSAL_MmngrGetCpuPtr
 *      -# Get CPU accessible (read only) pointer by using R_OSAL_MmngrGetConstCpuPtr
 * @param[in] buffHandle OSAL memory buffer handle
 * @param[in,out] cpuPTR This parameter is to store the pointer of CPU accessible (read/write) of memory buffer buffHandle
 * @param[in,out] cpuConstPTR This parameter is to store the pointer of CPU accessible (read only) of memory buffer buffHandle
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_get_cpu_access(osal_memory_buffer_handle_t buffHandle, void **cpuPTR, const void **cpuConstPTR)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    OSAL_SAMPLE_INFO("Get CPU accessible (read/write) pointer to the first allocated buffer\n");
    osal_ret = R_OSAL_MmngrGetCpuPtr(buffHandle, cpuPTR);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot get the CPU accessible (read/write) pointer of buffer, osal_ret = %d\n", osal_ret);
    }

    OSAL_SAMPLE_INFO("Get CPU accessible (read only) pointer to the first allocated buffer\n");
    osal_ret = R_OSAL_MmngrGetConstCpuPtr(buffHandle, cpuConstPTR);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot get the CPU accessible (read only) pointer of buffer, osal_ret = %d\n", osal_ret);
    }

    return app_ret;
}

/**
 * @brief Get the AXI bus domain specific address of buffer
 * - Get the AXI bus domain specific address of buffer with steps at below:
 *      -# Get AXI bus domain specific address of buffer by using R_OSAL_MmngrGetHwAddr
 * @param[in] buffHandle OSAL memory buffer handle
 * @param[in] axi_id The AXI bus ID
 * @param[in,out] hwAddress This parameter is to store the hw/peripheral/axi bus domain specific address corrsponding with memory buffer buffHandle and AXI bus ID axi_id
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_get_hardware_address(osal_memory_buffer_handle_t buffHandle, osal_axi_bus_id_t axi_id, uintptr_t *hwAddress)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    osal_ret = R_OSAL_MmngrGetHwAddr(buffHandle, axi_id, hwAddress);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot get the AXI bus domain specific address, osal_ret = %d\n", osal_ret);
    }

    return app_ret;
}

/**
 * @brief Synchronizes to HW then synchronizes the buffer HW memory to the CPU memory
 * - Synchronizes to HW then synchronizes the buffer HW memory to the CPU memory with steps at below:
 *      -# Modify CPU buffer and synchronizes to HW by using R_OSAL_MmngrFlush
 *      -# Synchronizes the buffer HW memory to the CPU memory by using R_OSAL_MmngrInvalidate
 * @param[in] buffHandle OSAL memory buffer handle which is used for memory buffer synchronization
 * @param[in] bufferSize The expected buffer size which is going to be used for memory buffer synchronization
 * @param[in] cpuPTR The pointer of CPU accessible (read/write). This function uses it to modify the content of memory buffer from CPU side.
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_synchronize_memory(osal_memory_buffer_handle_t buffHandle, size_t bufferSize, void* cpuPTR)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    int8_t know_data = 0x1;

    OSAL_SAMPLE_INFO("Make a char array (64-byte) which all elements content is 0x1\n");
    OSAL_SAMPLE_INFO("Copy this array to first allocated buffer and synchronizes to HW memory with offset is 0, size is 64-byte\n");
    /* PRQA S 0314 1 # The 1st parameter must be (void *) and there is no side-effect in this case. */
    (void)memset(cpuPTR, know_data, bufferSize);
    osal_ret = R_OSAL_MmngrFlush(buffHandle, 0, bufferSize);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot synchronizes CPU memory to HW, osal_ret = %d\n", osal_ret);
    }

    OSAL_SAMPLE_INFO("Synchronizes the buffer HW memory to the CPU memory with offset is 0, size is 64-byte\n")
    osal_ret = R_OSAL_MmngrInvalidate(buffHandle, 0, bufferSize);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot synchronizes HW memory to CPU, osal_ret = %d\n", osal_ret);
    }

    return app_ret;
}

/**
 * @brief Deallocate allocated buffer
 * - Deallocate allocated buffer with the steps at blow:
 *      -# If allocation mode is OSAL_MMNGR_ALLOC_MODE_STACK_***, de-allocate the buffer in reverse order of the allocation by using R_OSAL_MmngrDealloc
 *      -# If allocation mode is OSAL_MMNGR_ALLOC_MODE_FREELIST_***, de-allocate the buffer any order by using R_OSAL_MmngrDealloc.<br>
 *         In this case, sample app uses the same order of the allocation.
 * @param[in] memHandle OSAL memory manager handle
 * @param[in] memConfig The configuration of memory manager handle memHandle
 * @param[in] buffHandle An address of OSAL memory buffer handle which is going to be deallocated
 * @param[in] number_of_buffer The number of buffers which are going to be deallocated
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t memory_deallocate_buffers(osal_memory_manager_handle_t memHandle, st_osal_mmngr_config_t memConfig, osal_memory_buffer_handle_t const * buffHandle, size_t number_of_buffer)
{
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;

    if((OSAL_MMNGR_ALLOC_MODE_STACK_PROT == memConfig.mode) || (OSAL_MMNGR_ALLOC_MODE_STACK == memConfig.mode))
    {
        OSAL_SAMPLE_INFO("Deallocate buffer. In case stack mode: Need to de-allocate in the reverse order of allocation\n")
        for (int32_t i = (int32_t)number_of_buffer - 1; i >= 0; i--)
        {
            osal_ret = R_OSAL_MmngrDealloc(memHandle, buffHandle[i]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Deallocation in stack mode failed, osal_ret = %d\n", osal_ret);
            }
        }
    }
    else /* OSAL_MMNGR_ALLOC_MODE_FREE_LIST_PROT or OSAL_MMNGR_ALLOC_MODE_FREE_LIST */
    {
        OSAL_SAMPLE_INFO("Deallocate buffer. In case free list mode: Can de-allocate in any order\n")
        for(size_t i = 0; i < number_of_buffer; i++)
        {
            osal_ret = R_OSAL_MmngrDealloc(memHandle, buffHandle[i]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Deallocation in free list mode failed, osal_ret = %d\n", osal_ret);
            }
        }
    }

    return app_ret;
}
/**@} common_alloc_use*/

/**
 * @defgroup init_obj_main_task OSAL Memory Object Initialization APIs
 * @{
 */
/**
 * @brief Memory object initialization main task:
 * - Initialize all members of st_osal_memory_manager_obj to invalid/defined values by using R_OSAL_MmngrInitializeMemoryManagerObj.
 * - Initializes all members of st_osal_memory_buffer_obj to invalid/defined values by using R_OSAL_MmngrInitializeMemoryBufferObj.
 * 
 * @return 0 on success
 * @return -1 on failure
 */
static int32_t init_obj_main_task(void)
{
    /* local variable */
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    osal_memory_manager_t memHandleObject;
    osal_memory_buffer_t memBufferObject;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("\n================== init_obj_main_task start ==================\n");
    OSAL_SAMPLE_INFO("Initializes all members of st_osal_memory_manager_obj to invalid/defined values\n");
    osal_ret = R_OSAL_MmngrInitializeMemoryManagerObj(&memHandleObject);
    if(OSAL_RETURN_OK != osal_ret)
    {
        app_ret = -1;
        OSAL_SAMPLE_ERR("Cannot initialize the memory handle object, osal_ret = %d\n", osal_ret);
    }
    if (0 == app_ret)
    {
        OSAL_SAMPLE_INFO("Initializes all members of st_osal_memory_buffer_obj to invalid/defined values\n");
        osal_ret = R_OSAL_MmngrInitializeMemoryBufferObj(&memBufferObject);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Cannot initialize the memory buffer object, osal_ret = %d\n", osal_ret);
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== init_obj_main_task finish ==================\n\n");
    return app_ret;
}
/**@} object init*/

/**
 * @defgroup alloc_use_multi_memory_task OSAL Memory allocation and use APIs in multi-memory regions
 * @{
 */
/**
 * @brief Alloc-use main task in multi-memory regions:<br>
 * (OSAL API UM reference: Chapter 2.9.6: Multi memory region)
 * - Open memory handle for all memory regions with the max configuration by using R_OSAL_MmngrOpenFromRegionIdx
 * - Allocate and use memory by using @link common_alloc_use @endlink
 * - Close all opened memory handles by using R_OSAL_MmngrClose
 *
 * @return 0 on success
 * @return -1 on failure
 */
#if defined(SUPPORT_MULTI_MEMORY_REGION)
static int32_t alloc_use_multi_memory_task(void)
{
    /* local variable */
    e_osal_return_t osal_ret;
    int32_t app_ret = 0;
    osal_memory_manager_handle_t memHandle[SAMPLE_NUM_OF_MEMORY_REGION];
    st_osal_mmngr_config_t memMaxConfig;

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("\n================== alloc_use_multi_memory_task start ==================\n");
    for(size_t region_idx = 0U; region_idx < SAMPLE_NUM_OF_MEMORY_REGION; region_idx++)
    {
        memHandle[region_idx] = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
        /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
        OSAL_SAMPLE_INFO("Get the maximum memory configuration from region index %lu\n", (unsigned long)region_idx);
        osal_ret = R_OSAL_MmngrGetOsalMaxConfigFromRegionIdx((osal_memory_region_idx_t)region_idx, &memMaxConfig);
        if(OSAL_RETURN_OK != osal_ret)
        {
            app_ret = -1;
            OSAL_SAMPLE_ERR("Cannot get the memory max configuration, osal_ret = %d\n", osal_ret);
        }

        if(0 == app_ret)
        {
            /* PRQA S 5209 1 # This casting to fix compiler-dependent. There is no side-effect in this case */
            OSAL_SAMPLE_INFO("Open memory handle for a memory region index (%lu) with the max configuration\n", (unsigned long)region_idx);
            osal_ret = R_OSAL_MmngrOpenFromRegionIdx(&memMaxConfig, region_idx, &memHandle[region_idx]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Open memory handle failed , osal_ret = %d\n", osal_ret);
            }
        }

        if(0 == app_ret)
        {
            /* Allocation and use */
            app_ret = common_alloc_use(memHandle[region_idx], region_idx);
            if(0 != app_ret)
            {
                OSAL_SAMPLE_ERR("Operation failed\n");
            }
        }
    }

    for(size_t region_idx = 0U; region_idx < SAMPLE_NUM_OF_MEMORY_REGION; region_idx++)
    {
        if(OSAL_MEMORY_MANAGER_HANDLE_INVALID != memHandle[region_idx])
        {
            OSAL_SAMPLE_INFO("Close memory handle\n");
            osal_ret = R_OSAL_MmngrClose(memHandle[region_idx]);
            if(OSAL_RETURN_OK != osal_ret)
            {
                app_ret = -1;
                OSAL_SAMPLE_ERR("Cannot close memory manager, osal_ret = %d\n", osal_ret);
            }
        }
    }

    /* PRQA S 5124 1 # It is used for the request to output the execution result of the sample. */
    (void)printf("================== alloc_use_multi_memory_task finish ==================\n\n");
    return app_ret;
}
#endif
/**@} alloc_use_multi_memory_task*/
/**@} osal_sample_memory*/
