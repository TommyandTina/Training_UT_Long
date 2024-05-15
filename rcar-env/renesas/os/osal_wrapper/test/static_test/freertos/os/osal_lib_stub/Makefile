OUTDIR	:= output
LIBNAME	:= libosal_test.a
TOPDIR	:= $(CURDIR)/../..
OSALDIR	:= $(TOPDIR)/os
MDWDIR	:= $(TOPDIR)/middleware
DRIDIR	:= $(TOPDIR)/drivers
OSDIR	:= $(TOPDIR)/os/freertos
TARGET	:= $(OSALDIR)/osal_lib_stub/$(OUTDIR)/$(LIBNAME)

# source files
SRC		+= $(wildcard $(OSALDIR)/osal_dev_cfg/src/*.c)
SRC		+= $(wildcard $(OSALDIR)/osal_usr_cfg/src/*.c)
SRC		+= $(wildcard $(OSALDIR)/osal_wrapper/src/target/freertos/*.c)
SRC		+= $(wildcard $(MDWDIR)/libraries/memory_allocator/src/*.c)
SRC		+= $(wildcard $(OSALDIR)/osal_lib_stub/*.c)
# include directories
INC_DIR	+= $(OSALDIR)/osal_lib_stub/include
INC_DIR	+= $(OSALDIR)/osal_lib_stub/include/arm-gnu-header
INC_DIR	+= $(OSALDIR)/osal/include
INC_DIR	+= $(OSALDIR)/osal_dev_cfg/include
INC_DIR	+= $(OSALDIR)/osal_dev_cfg/src/include
INC_DIR	+= $(OSALDIR)/osal_dev_cfg/src/include/config/freertos
INC_DIR	+= $(MDWDIR)/libraries/memory_allocator/include
INC_DIR	+= $(OSALDIR)/osal_usr_cfg/include
INC_DIR	+= $(OSALDIR)/osal_usr_cfg/src/include
INC_DIR	+= $(OSALDIR)/osal/include/rcar-xos/osal_wrapper/include/target/freertos
INC_DIR	+= $(OSALDIR)/osal_wrapper/src/include
INC_DIR	+= $(OSALDIR)/osal_wrapper/src/include/target/common/pma
INC_DIR	+= $(OSALDIR)/osal_wrapper/src/include/target/freertos
INC_DIR	+= $(OSALDIR)/osal_wrapper/src/target/freertos
INC_DIR	+= $(DRIDIR)/tick/lib
INC_DIR	+= $(DRIDIR)/cpg/lib
INC_DIR	+= $(DRIDIR)/prr/lib
INC_DIR	+= $(DRIDIR)/io/lib/
INC_DIR	+= $(DRIDIR)/delay/lib/
INC_DIR	+= $(DRIDIR)/print/lib/
INC_DIR	+= $(DRIDIR)/sysc/lib/
USER_INC := $(addprefix -I,$(INC_DIR))
ISYS_DIR	+= $(OSDIR)/FreeRTOS/Source/include
ISYS_DIR	+= $(OSDIR)/FreeRTOS/Demo/Common/include
ISYS_DIR	+= $(OSDIR)/FreeRTOS/Demo/R-Car_Gen3_CR7/common
ISYS_DIR	+= $(OSDIR)/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/Include
ISYS_DIR	+= $(OSDIR)/FreeRTOS-Plus/Source/FreeRTOS-Plus-Trace/config
ISYS_DIR	+= $(OSDIR)/FreeRTOS/Source/portable/GCC/ARM_CR7
ISYS_DIR	+= $(OSDIR)/FreeRTOS/Demo/R-Car_Gen3_CR7/CMSIS
USR_ISYS	+= $(addprefix -isystem,$(ISYS_DIR))
# command option
CC		:=gcc
AR		:= ar
EXTRA_CFLAGS += -DxTaskCreate=xTaskCreate_stub
EXTRA_CFLAGS += -DIrq_SetupEntry=Irq_SetupEntry_stub
EXTRA_CFLAGS += -DIrq_RemoveEntry=Irq_RemoveEntry_stub
EXTRA_CFLAGS += -DIrq_Enable=Irq_Enable_stub
EXTRA_CFLAGS += -DIrq_Disable=Irq_Disable_stub
EXTRA_CFLAGS += -DIrq_SetPriority=Irq_SetPriority_stub
EXTRA_CFLAGS += -DpvPortMalloc=pvPortMalloc_stub
EXTRA_CFLAGS += -DvPortFree=vPortFree_stub
EXTRA_CFLAGS += -DuxQueueMessagesWaiting=uxQueueMessagesWaiting_stub
EXTRA_CFLAGS += -DuxQueueSpacesAvailable=uxQueueSpacesAvailable_stub
EXTRA_CFLAGS += -DvQueueDelete=vQueueDelete_stub
EXTRA_CFLAGS += -DvEventGroupDelete=vEventGroupDelete_stub
EXTRA_CFLAGS += -DvEventGroupSetBitsCallback=vEventGroupSetBitsCallback_stub
EXTRA_CFLAGS += -DxEventGroupClearBits=xEventGroupClearBits_stub
EXTRA_CFLAGS += -DxEventGroupCreate=xEventGroupCreate_stub
EXTRA_CFLAGS += -DxEventGroupSetBits=xEventGroupSetBits_stub
EXTRA_CFLAGS += -DxEventGroupWaitBits=xEventGroupWaitBits_stub
EXTRA_CFLAGS += -DvTaskDelay=vTaskDelay_stub
EXTRA_CFLAGS += -DvTaskDelete=vTaskDelete_stub
EXTRA_CFLAGS += -DvTaskPrioritySet=vTaskPrioritySet_stub
EXTRA_CFLAGS += -DvTaskStartScheduler=vTaskStartScheduler_stub
EXTRA_CFLAGS += -DxQueueCreateMutex=xQueueCreateMutex_stub
EXTRA_CFLAGS += -DxQueueGenericCreate=xQueueGenericCreate_stub
EXTRA_CFLAGS += -DxQueueGenericReset=xQueueGenericReset_stub
EXTRA_CFLAGS += -DxQueueGenericSend=xQueueGenericSend_stub
EXTRA_CFLAGS += -DxQueueGenericSendFromISR=xQueueGenericSendFromISR_stub
EXTRA_CFLAGS += -DxQueueGetMutexHolder=xQueueGetMutexHolder_stub
EXTRA_CFLAGS += -DxQueueGetMutexHolderFromISR=xQueueGetMutexHolderFromISR_stub
EXTRA_CFLAGS += -DxQueueGiveFromISR=xQueueGiveFromISR_stub
EXTRA_CFLAGS += -DxQueueIsQueueEmptyFromISR=xQueueIsQueueEmptyFromISR_stub
EXTRA_CFLAGS += -DxQueueIsQueueFullFromISR=xQueueIsQueueFullFromISR_stub
EXTRA_CFLAGS += -DxQueueReceive=xQueueReceive_stub
EXTRA_CFLAGS += -DxQueueReceiveFromISR=xQueueReceiveFromISR_stub
EXTRA_CFLAGS += -DxQueueSemaphoreTake=xQueueSemaphoreTake_stub
EXTRA_CFLAGS += -DxTaskGetCurrentTaskHandle=xTaskGetCurrentTaskHandle_stub
EXTRA_CFLAGS += -DxTaskGetTickCount=xTaskGetTickCount_stub
EXTRA_CFLAGS += -DxTaskNotifyWait=xTaskNotifyWait_stub
EXTRA_CFLAGS += -DxTimerPendFunctionCallFromISR=xTimerPendFunctionCallFromISR_stub
EXTRA_CFLAGS += -DR_PMA_GetLockHwa=R_PMA_GetLockHwa_stub
EXTRA_CFLAGS += -DR_PMA_UnlockHwa=R_PMA_UnlockHwa_stub
EXTRA_CFLAGS += -DR_PMA_RequestHwa=R_PMA_RequestHwa_stub
EXTRA_CFLAGS += -DR_PMA_ReleaseHwa=R_PMA_ReleaseHwa_stub
EXTRA_CFLAGS += -DR_PMA_SetPostClock=R_PMA_SetPostClock_stub
EXTRA_CFLAGS += -DR_PMA_SetPostClock=R_PMA_SetPostClock_stub
EXTRA_CFLAGS += -DR_PMA_GetLowestPowerState=R_PMA_GetLowestPowerState_stub
EXTRA_CFLAGS += -DR_PMA_GetReset=R_PMA_GetReset_stub
EXTRA_CFLAGS += -DR_PMA_ReleaseResetHwa=R_PMA_ReleaseResetHwa_stub
EXTRA_CFLAGS += -DR_PMA_ApplyResetHwa=R_PMA_ApplyResetHwa_stub
EXTRA_CFLAGS += -DR_PMA_GetPowerPolicy=R_PMA_GetPowerPolicy_stub
EXTRA_CFLAGS += -DR_PMA_SetPowerPolicy=R_PMA_SetPowerPolicy_stub
EXTRA_CFLAGS += -DR_PMA_GetHwaState=R_PMA_GetHwaState_stub
EXTRA_CFLAGS += -DR_PMA_Quit=R_PMA_Quit_stub
EXTRA_CFLAGS += -DR_PMA_Init=R_PMA_Init_stub
EXTRA_CFLAGS += -DR_OS_ASMIF_FlushAllCache=R_OS_ASMIF_FlushAllCache_stub
EXTRA_CFLAGS += -DR_OS_ASMIF_FlushICache=R_OS_ASMIF_FlushICache_stub
EXTRA_CFLAGS += -DR_OS_ASMIF_FlushDCache=R_OS_ASMIF_FlushDCache_stub
EXTRA_CFLAGS += -DR_OS_ASMIF_InvICache=R_OS_ASMIF_InvICache_stub
EXTRA_CFLAGS += -DR_OS_ASMIF_InvDCache=R_OS_ASMIF_InvDCache_stub
EXTRA_CFLAGS += -DR_OS_ASMIF_InvAllCache=R_OS_ASMIF_InvAllCache_stub
EXTRA_CFLAGS += -DxTaskGenericNotifyWait=xTaskGenericNotifyWait_stub
EXTRA_CFLAGS += -DxTaskGenericNotify=xTaskGenericNotify_stub
#-------------------------------------------------------------------------------
all: 
	$(CC) $(CFLAGS) $(USER_INC) $(USR_ISYS) $(EXTRA_CFLAGS) -g -O -c $(SRC)
	$(AR) -crs $(TARGET) *.o
.PHONY: all
