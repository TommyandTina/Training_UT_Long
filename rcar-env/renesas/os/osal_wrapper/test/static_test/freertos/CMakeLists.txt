#######################################################################################################################
# Copyright [2022] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
#
# The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
# and/or its licensors ("Renesas") and subject to statutory and contractual protections.
#
# Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
# display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
# purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
# SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
# WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
# NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
# INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
# OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
# be subject to different terms.
#######################################################################################################################
#----------------------------------------------------------------------------------------------------------------------
# History   Version   DD.MM.YYYY    Description
#            2.0.0     22.02.2022    Create New
#            2.1.0     09.03.2022    Add PMA source files
#----------------------------------------------------------------------------------------------------------------------

#----------------------------------------------------------------------------------------------------------------------
# Set module information
#----------------------------------------------------------------------------------------------------------------------
set(app_name        "osal_wrapper_freerots" )
set(app_version     "2.1.0")

#----------------------------------------------------------------------------------------------------------------------
# Common Source/Header configuration
#----------------------------------------------------------------------------------------------------------------------
# Interface source files that always needs to be compiled with the application and not during artifact generation
# Example: UDF sources
set(interface_source
)

# Public Header that are exported and can be used by application or other modules
set(public_header
)

# Private Header files that are not exported and cannot be used by other components or modules
set(private_header
)

#----------------------------------------------------------------------------------------------------------------------
# OS/SoC or environment specific Source/Header configuration
# Any specifics you may need add a suffix (lower letters) to the variable defined in the common section
# E.g. Specific sources for FreeRTOS:           source_freertos
# E.g. Specific sources for Linux:              source_linux
# E.g. Specific sources for HIL Env:            source_hil
# E.g. Specific sources for V3H1 SoC:           source_v3h1
# E.g. Specific sources for FreeRTOS and V3H1:  source_freertos_v3h1
#----------------------------------------------------------------------------------------------------------------------

add_definitions(-DxTaskCreate=xTaskCreate_stub)
add_definitions(-DIrq_SetupEntry=Irq_SetupEntry_stub)
add_definitions(-DIrq_RemoveEntry=Irq_RemoveEntry_stub)
add_definitions(-DIrq_Enable=Irq_Enable_stub)
add_definitions(-DIrq_Disable=Irq_Disable_stub)
add_definitions(-DIrq_SetPriority=Irq_SetPriority_stub)
add_definitions(-DpvPortMalloc=pvPortMalloc_stub)
add_definitions(-DvPortFree=vPortFree_stub)
add_definitions(-DuxQueueMessagesWaiting=uxQueueMessagesWaiting_stub)
add_definitions(-DuxQueueSpacesAvailable=uxQueueSpacesAvailable_stub)
add_definitions(-DvQueueDelete=vQueueDelete_stub)
add_definitions(-DvEventGroupDelete=vEventGroupDelete_stub)
add_definitions(-DvEventGroupSetBitsCallback=vEventGroupSetBitsCallback_stub)
add_definitions(-DxEventGroupClearBits=xEventGroupClearBits_stub)
add_definitions(-DxEventGroupCreate=xEventGroupCreate_stub)
add_definitions(-DxEventGroupSetBits=xEventGroupSetBits_stub)
add_definitions(-DxEventGroupWaitBits=xEventGroupWaitBits_stub)
add_definitions(-DvTaskDelay=vTaskDelay_stub)
add_definitions(-DvTaskDelete=vTaskDelete_stub)
add_definitions(-DvTaskPrioritySet=vTaskPrioritySet_stub)
add_definitions(-DvTaskStartScheduler=vTaskStartScheduler_stub)
add_definitions(-DxQueueCreateMutex=xQueueCreateMutex_stub)
add_definitions(-DxQueueGenericCreate=xQueueGenericCreate_stub)
add_definitions(-DxQueueGenericReset=xQueueGenericReset_stub)
add_definitions(-DxQueueGenericSend=xQueueGenericSend_stub)
add_definitions(-DxQueueGenericSendFromISR=xQueueGenericSendFromISR_stub)
add_definitions(-DxQueueGetMutexHolder=xQueueGetMutexHolder_stub)
add_definitions(-DxQueueGetMutexHolderFromISR=xQueueGetMutexHolderFromISR_stub)
add_definitions(-DxQueueGiveFromISR=xQueueGiveFromISR_stub)
add_definitions(-DxQueueIsQueueEmptyFromISR=xQueueIsQueueEmptyFromISR_stub)
add_definitions(-DxQueueIsQueueFullFromISR=xQueueIsQueueFullFromISR_stub)
add_definitions(-DxQueueReceive=xQueueReceive_stub)
add_definitions(-DxQueueReceiveFromISR=xQueueReceiveFromISR_stub)
add_definitions(-DxQueueSemaphoreTake=xQueueSemaphoreTake_stub)
add_definitions(-DxTaskGetCurrentTaskHandle=xTaskGetCurrentTaskHandle_stub)
add_definitions(-DxTaskGetTickCount=xTaskGetTickCount_stub)
add_definitions(-DxTaskNotifyWait=xTaskNotifyWait_stub)
add_definitions(-DxTimerPendFunctionCallFromISR=xTimerPendFunctionCallFromISR_stub)
add_definitions(-DR_OS_ASMIF_FlushAllCache=R_OS_ASMIF_FlushAllCache_stub)
add_definitions(-DR_OS_ASMIF_FlushICache=R_OS_ASMIF_FlushICache_stub)
add_definitions(-DR_OS_ASMIF_FlushDCache=R_OS_ASMIF_FlushDCache_stub)
add_definitions(-DR_OS_ASMIF_InvICache=R_OS_ASMIF_InvICache_stub)
add_definitions(-DR_OS_ASMIF_InvDCache=R_OS_ASMIF_InvDCache_stub)
add_definitions(-DR_OS_ASMIF_InvAllCache=R_OS_ASMIF_InvAllCache_stub)
add_definitions(-DxTaskGenericNotifyWait=xTaskGenericNotifyWait_stub)
add_definitions(-DxTaskGenericNotify=xTaskGenericNotify_stub)

# Linux specific
set(source
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_dev_cfg/src/target/freertos/r_osal_dev_cfg.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_dev_cfg/src/target/freertos/r_osal_dev_cfg_tbl.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_usr_cfg/src/target/freertos/r_osal_usr_cfg.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_os_cache.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_common.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_interrupt.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_io.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_memory.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_message.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_power.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_thread.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_threadsync.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_timeclock.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/r_osal_version.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/pma/r_pma_hwdepend.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/pma/r_pma_if.c
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos/pma/r_pma_statemachine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/memory_allocator/src/r_meml_allocator.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os/osal_lib_stub/stub.c
    ${CMAKE_CURRENT_SOURCE_DIR}/os/osal_lib_stub/test.c
)

set(header)

set(link_lib
    rt
    pthread
    #mmngr
)

set(include_dir
    ${CMAKE_CURRENT_SOURCE_DIR}/os/osal/include

    ${CMAKE_SOURCE_DIR}/renesas/os/osal_dev_cfg/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_dev_cfg/src/target/freertos/include

    ${CMAKE_SOURCE_DIR}/renesas/os/osal_usr_cfg/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_usr_cfg/src/target/freertos/include
    
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/freertos/pma
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/include/target/freertos
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_wrapper/src/target/freertos

    ${CMAKE_SOURCE_DIR}/renesas/os/osal_lib_stub/include
    ${CMAKE_SOURCE_DIR}/renesas/os/osal_lib_stub/src/include

    ${CMAKE_CURRENT_SOURCE_DIR}/memory_allocator/include

    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/tick/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/cpg/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/prr/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/io/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/delay/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/print/lib/
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/sysc/lib

    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS/Source/include
    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS/Demo/Common/include
    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS/Demo/R-Car_Gen3_CR7/common
    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include
    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config
    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS/Source/portable/GCC/ARM_CR7
    ${CMAKE_SOURCE_DIR}/renesas/os/bsp/freertos/h3/FreeRTOS/Demo/R-Car_Gen3_CR7/CMSIS

)

set(public_include_dir_linux
    ${CMAKE_SYSROOT}/usr/local/include
)


unset(CMAKE_C_STANDARD)
set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")


#======================================================================================================================
# DO NOT CHANGE ANYTHING BELOW THIS LINE, except you know what to do ;)
#======================================================================================================================
rcar_configure_application()
