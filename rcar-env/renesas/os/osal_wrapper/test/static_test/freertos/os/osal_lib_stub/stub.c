#include "rcar-xos/memory_allocator/r_meml.h"

#include "FreeRTOS.h" 
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "event_groups.h"
#include "r_cpg_api.h"
//#include "target/linux/r_osal_dev_cfg_control.h"
//#include <linux/renesas_uioctl.h>
#include "r_pma.h"

#if 1
//void *memcpy(void *dest, const void *src, size_t n);

/* malloc */
#define STUB_MALLOC_ALIGN   (8)
#define STUB_MALLOC_SIZE    (0x100000)
#define STUB_MALLOC_COUNT   (1000)
typedef struct {
    size_t  size;
    void*   ptr;
} st_STUB_MALLOC_TBL;
union
{
    void* dummy;
    char  array[STUB_MALLOC_SIZE];
} stub_heap;
size_t  stub_malloc_pos = 0;
size_t  stub_malloc_count = 0;
st_STUB_MALLOC_TBL stub_malloc_tbl[STUB_MALLOC_COUNT];
#endif

// #ifndef WINAMS_STUB
// #define READ_LOWER()    (*(volatile uint32_t*)(0xF0000000UL+0x200ul))
// #define READ_UPPER()    (*(volatile uint32_t*)(0xF0000000UL+0x204ul))
// uint32_t g_start[2];
// uint32_t g_end[2];

// uint64_t g_stub_time_u, g_stub_time_l;

// void init_stub_time()
// {
	// g_stub_time_u = 0;
	// g_stub_time_l = 0;
// }

// uint64_t get_stub_time_u()
// {
	// return g_stub_time_u;
// }

// uint64_t get_stub_time_l()
// {
	// return g_stub_time_l;
// }

// void add_stub_time()
// {
    // if(g_end[1] > g_start[1])
    // {
        // g_stub_time_l += (0xFFFFFFFF - g_start[0] + 1) + g_end[0];
        // g_stub_time_u += g_end[1] - g_start[1];
    // }
    // else
    // {
        // g_stub_time_l += g_end[0] - g_start[0];
        // g_stub_time_u += g_end[1] - g_start[1];
    // }
// }

// #define START_TIMER do{g_start[1] = READ_UPPER();do{g_start[0] = READ_LOWER();}while (READ_UPPER() !=g_start[1]);}while(0);
// #define STOP_TIMER do{g_end[1] = READ_UPPER();do{g_end[0] = READ_LOWER();}while (READ_UPPER() !=g_end[1]);add_stub_time();}while(0);
// #else
// #define START_TIMER do{}while(0);
// #define STOP_TIMER do{}while(0);
// #endif

#define START_TIMER do{}while(0);
#define STOP_TIMER do{}while(0);


typedef void (*IrqHandlerFn)(void *data);
typedef unsigned long UBaseType_t;



#define STUB_FD_MAX     (100)
#define STUB_FD_CMEM    (0)
#define STUB_FD_ELSE    (100)
int stub_fd[STUB_FD_MAX];
int stub_fd_count = 0;

volatile uint32_t ulPortYieldRequired;
const void * SharedBase;
uint32_t ulPortInterruptNesting;
const void * SharedLimit;

uintptr_t stub_cmem_area;

uintptr_t * get_cmem_area()
{

	return &stub_cmem_area;
}

void R_OS_ASMIF_FlushAllCache_stub(size_t Start, size_t Size)
{
    (void)Start;
    (void)Size;
}   

void R_OS_ASMIF_FlushICache_stub(size_t Start, size_t Size)
{
    (void)Start;
    (void)Size;
}

void R_OS_ASMIF_FlushDCache_stub(size_t Start, size_t Size)
{
    (void)Start;
    (void)Size;
}

void R_OS_ASMIF_InvICache_stub(size_t Start, size_t Size)
{
    (void)Start;
    (void)Size;
}

void R_OS_ASMIF_InvDCache_stub(size_t Start, size_t Size)
{
    (void)Start;
    (void)Size;
}

void R_OS_ASMIF_InvAllCache_stub(size_t Start, size_t Size)
{
    (void)Start;
    (void)Size;
}

TickType_t xTaskGetTickCountFromISR( void )
{
    return (TickType_t)1;
}

#define DUMMY_TREAD_HANDLE (1)

BaseType_t xTaskGenericNotifyWait_stub(UBaseType_t uxIndexToWaitOn, uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, TickType_t xTicksToWait)
{
	(void)uxIndexToWaitOn;
	(void)ulBitsToClearOnEntry;
	(void)ulBitsToClearOnExit;
	(void)pulNotificationValue;
	(void)xTicksToWait;
	
	return pdTRUE;
}

BaseType_t xTaskGenericNotify_stub(TaskHandle_t xTaskToNotify, UBaseType_t uxIndexToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue)
{
	(void)xTaskToNotify;
	(void)uxIndexToNotify;
	(void)ulValue;
	(void)eAction;
	(void)pulPreviousNotificationValue;
	
	return pdTRUE;
}
BaseType_t xTaskCreate_stub(	TaskFunction_t pxTaskCode,
			const char * const pcName,	/*lint !e971 Unqualified char types are allowed for strings and single characters only. */
			const configSTACK_DEPTH_TYPE usStackDepth,
			void * const pvParameters,
			UBaseType_t uxPriority,
			TaskHandle_t * const pxCreatedTask )
{
    (void)pxTaskCode;
    (void)pcName;
    (void)usStackDepth;
    (void)pvParameters;
    (void)uxPriority;
    *pxCreatedTask = (TaskHandle_t)DUMMY_TREAD_HANDLE;
    return pdPASS;
}



void Irq_Setup_stub(void)
{
    ;
}

void Irq_SetupEntry_stub(unsigned int id, IrqHandlerFn Handler, void *Context)
{
    (void)id;
    (void)Handler;
    (void)Context;
}

void Irq_RemoveEntry_stub(unsigned int id)
{
    (void)id;
}

void Irq_Enable_stub(unsigned int id)
{
    (void)id;
}

void Irq_Disable_stub(unsigned int id)
{
    (void)id;
}

void Irq_SetPriority_stub(unsigned int id, unsigned int priority)
{
    (void)id;
    (void)priority;
}
#if 0
uint32_t R_CPG_DavehdClkEnable(r_cpg_ModuleName_t Module)
{
    (void)Module;
    return 0;
}
#endif

void *pvPortMalloc_stub( size_t xSize )
{
    size_t size_a;
    void *ptr;
START_TIMER
    if (xSize == 0)
    {
        return NULL;
    }
    size_a = ((xSize + STUB_MALLOC_ALIGN - 1) / STUB_MALLOC_ALIGN) * STUB_MALLOC_ALIGN;
    if ((stub_malloc_pos + size_a) >= STUB_MALLOC_SIZE)
    {
        return NULL;
    }
    if ((stub_malloc_count + 1) >= STUB_MALLOC_COUNT)
    {
        return NULL;
    }
    ptr = (void*)&stub_heap.array[stub_malloc_pos];
    stub_malloc_tbl[stub_malloc_count].size = xSize;
    stub_malloc_tbl[stub_malloc_count].ptr = ptr;
    stub_malloc_pos += size_a;
    stub_malloc_count++;
STOP_TIMER
    return ptr;
}

void vPortFree_stub( void *pv )
{
START_TIMER
    if (pv == NULL)
    {
STOP_TIMER
        return;
    }
    if (stub_malloc_count == 0)
    {
STOP_TIMER
        return;
    }
    stub_malloc_count--;
    if (stub_malloc_count == 0)
        stub_malloc_pos = 0;   // reset heap
STOP_TIMER
    return;
}

UBaseType_t uxQueueMessagesWaiting_stub( const QueueHandle_t xQueue )
{
    (void)xQueue;
    return (UBaseType_t)0;
}

UBaseType_t uxQueueSpacesAvailable_stub( const QueueHandle_t xQueue )
{
    (void)xQueue;
    return (UBaseType_t)1;
}

void vQueueDelete_stub( QueueHandle_t xQueue )
{
    (void)xQueue;
}

void vEventGroupDelete_stub( EventGroupHandle_t xEventGroup )
{
    (void)xEventGroup;
}

void vEventGroupSetBitsCallback_stub( void *pvEventGroup, const uint32_t ulBitsToSet )
{
    (void)pvEventGroup;
    (void)ulBitsToSet;
}

EventBits_t xEventGroupClearBits_stub( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear )
{
    (void)xEventGroup;
    (void)uxBitsToClear;
    return (EventBits_t)NULL;
}

EventGroupHandle_t xEventGroupCreate_stub( void )
{
    return (EventGroupHandle_t)1;
}

EventBits_t xEventGroupSetBits_stub( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet )
{
    return pdTRUE;
}

EventBits_t xEventGroupWaitBits_stub( EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToWaitFor, const BaseType_t xClearOnExit, const BaseType_t xWaitForAllBits, TickType_t xTicksToWait )
{
    (void)xEventGroup;
    (void)xClearOnExit;
    (void)xWaitForAllBits;
    (void)xTicksToWait;
    return uxBitsToWaitFor;
}

void vTaskDelay_stub( const TickType_t xTicksToDelay )
{
    (void)xTicksToDelay;
}

void vTaskDelete_stub( TaskHandle_t xTaskToDelete )
{
    (void)xTaskToDelete;
}

void vTaskPrioritySet_stub( TaskHandle_t xTask, UBaseType_t uxNewPriority )
{
    (void)xTask;
    (void)uxNewPriority;
}

void vTaskStartScheduler_stub( void )
{
    ;
}

QueueHandle_t xQueueCreateMutex_stub( const uint8_t ucQueueType )
{
    (void)ucQueueType;
    return (QueueHandle_t)1;
}

QueueHandle_t xQueueGenericCreate_stub( const UBaseType_t uxQueueLength, const UBaseType_t uxItemSize, const uint8_t ucQueueType )
{
    (void)uxQueueLength;
    (void)uxItemSize;
    (void)ucQueueType;
    return (QueueHandle_t)1;
}

BaseType_t xQueueGenericReset_stub( QueueHandle_t xQueue, BaseType_t xNewQueue )
{
    (void)xQueue;
    (void)xNewQueue;
    return pdPASS;
}

BaseType_t xQueueGenericSend_stub( QueueHandle_t xQueue, const void * const pvItemToQueue, TickType_t xTicksToWait, const BaseType_t xCopyPosition )
{
    (void)xQueue;
    (void)pvItemToQueue;
    (void)xTicksToWait;
    (void)xCopyPosition;
    return pdTRUE;
}

BaseType_t xQueueGenericSendFromISR_stub( QueueHandle_t xQueue, const void * const pvItemToQueue, BaseType_t * const pxHigherPriorityTaskWoken, const BaseType_t xCopyPosition )
{
    (void)xQueue;
    (void)pvItemToQueue;
    (void)pxHigherPriorityTaskWoken;
    (void)xCopyPosition;
    return pdTRUE;
}

TaskHandle_t xQueueGetMutexHolder_stub( QueueHandle_t xSemaphore )
{
    (void)xSemaphore;
    return (TaskHandle_t)2;
}

TaskHandle_t xQueueGetMutexHolderFromISR_stub( QueueHandle_t xSemaphore )
{
    (void)xSemaphore;
    return (TaskHandle_t)2;
}

BaseType_t xQueueGiveFromISR_stub( QueueHandle_t xQueue, BaseType_t * const pxHigherPriorityTaskWoken )
{
    (void)xQueue;
    (void)pxHigherPriorityTaskWoken;
    return pdTRUE; 
}

BaseType_t xQueueIsQueueEmptyFromISR_stub( const QueueHandle_t xQueue )
{
    (void)xQueue;
    return pdTRUE; 
}

BaseType_t xQueueIsQueueFullFromISR_stub( const QueueHandle_t xQueue )
{
    (void)xQueue;
    return pdTRUE; 
}

BaseType_t xQueueReceive_stub( QueueHandle_t xQueue, void * const pvBuffer, TickType_t xTicksToWait )
{
    (void)xQueue;
    (void)pvBuffer;
    (void)xTicksToWait;
    return pdTRUE; 
}

BaseType_t xQueueReceiveFromISR_stub( QueueHandle_t xQueue, void * const pvBuffer, BaseType_t * const pxHigherPriorityTaskWoken )
{
    (void)xQueue;
    (void)pvBuffer;
    (void)pxHigherPriorityTaskWoken;
    return pdTRUE; 
}

BaseType_t xQueueSemaphoreTake_stub( QueueHandle_t xQueue, TickType_t xTicksToWait )
{
    (void)xQueue;
    (void)xTicksToWait;
    return pdTRUE; 
}

BaseType_t xTaskGenericNotifyFromISR_stub( TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction, uint32_t *pulPreviousNotificationValue, BaseType_t *pxHigherPriorityTaskWoken )
{
    (void)xTaskToNotify;
    (void)ulValue;
    (void)eAction;
    (void)pulPreviousNotificationValue;
    (void)pxHigherPriorityTaskWoken;
    return pdTRUE; 
}

bool flg_xTaskGetCurrentTaskHandle;
TaskHandle_t xTaskGetCurrentTaskHandle_stub( void )
{
    TaskHandle_t ret;
    if (flg_xTaskGetCurrentTaskHandle)
    {
        ret = (TaskHandle_t)DUMMY_TREAD_HANDLE;
    }
    else
    {
        ret = (TaskHandle_t)(DUMMY_TREAD_HANDLE + 1);
    }
    return ret; 
}

TickType_t xTaskGetTickCount_stub( void )
{
    return (TickType_t)0;
}

BaseType_t xTaskNotifyWait_stub( uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, TickType_t xTicksToWait )
{
    (void)ulBitsToClearOnEntry;
    (void)ulBitsToClearOnExit;
    (void)pulNotificationValue;
    (void)xTicksToWait;
    return pdTRUE; 
}

BaseType_t xTimerPendFunctionCallFromISR_stub( PendedFunction_t xFunctionToPend, void *pvParameter1, uint32_t ulParameter2, BaseType_t *pxHigherPriorityTaskWoken )
{
    (void)xFunctionToPend;
    (void)pvParameter1;
    (void)ulParameter2;
    (void)pxHigherPriorityTaskWoken;
    return pdTRUE; 
}
