
#include <stdlib.h>

#include "FreeRTOS.h"
#include "generic_api_memory.h"
#include "task.h"

#if (configSUPPORT_DYNAMIC_ALLOCATION == 0)
#    error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

/*-----------------------------------------------------------*/

void* pvPortMalloc(size_t xWantedSize)
{
    void* pvReturn = NULL;

    pvReturn = gf_Memalign(portBYTE_ALIGNMENT, xWantedSize);
    traceMALLOC(pvReturn, xWantedSize);

#if (configUSE_MALLOC_FAILED_HOOK == 1)
    {
        if (pvReturn == NULL)
        {
            extern void vApplicationMallocFailedHook(void);
            vApplicationMallocFailedHook();
        }
        else
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
#endif

    configASSERT((((size_t)pvReturn) & (size_t)portBYTE_ALIGNMENT_MASK) == 0);
    return pvReturn;
}
/*-----------------------------------------------------------*/

void vPortFree(void* pv)
{
    if (pv != NULL)
    {
        gf_Free(pv);
    }
}
