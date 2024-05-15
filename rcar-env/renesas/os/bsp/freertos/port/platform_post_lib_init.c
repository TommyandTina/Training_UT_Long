/* Standard includes. */
#include "FreeRTOS.h"
#include "assert.h"

#include "stdio.h"
#include "task.h"
#include <stdlib.h>

/* Xilinx includes. */
#include "rcar-xos/xil_common/xil_printf.h"
#include "rcar-xos/xil_scugic/xscugic.h"
#include "rcar-xos/xil_ttcps/xttcps.h"

/*-----------------------------------------------------------*/

extern int main(int argc, char* argv[]);

/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware(void);

/*-----------------------------------------------------------*/

/* The interrupt controller is initialised in this file, and made available to
other modules. */
XScuGic xInterruptController;

/*-----------------------------------------------------------*/

typedef struct
{
    int    argc;
    char** argv;
    int    ret;
} MainFunParameters;

static char       cmdArgString[256] = {0};   //!< Command Line argument as string, maximum characters 255. Set this via debugger
MainFunParameters mainFunParam      = {0, NULL, -1};

void main_entryTsk(void* pvArg)
{
    MainFunParameters* arg = (MainFunParameters*)pvArg;
    assert(pvArg != NULL);
    if (pvArg == NULL)
    {
        return;
    }
    vTaskDelay(100);
    arg->ret = main(arg->argc, arg->argv);
    vTaskEndScheduler(); /* FreeRTOS porting layer must support this */
    while (1)
    {
        vTaskDelete(NULL);
    }
}

/*-----------------------------------------------------------*/

void _platform_post_lib_init(void)
{
#ifdef __clang__
    extern void initMMU();
    initMMU();
#endif
    /* Configure the hardware ready to run the demo. */
    prvSetupHardware();

    /// Generate Argc, argv
    mainFunParam.ret  = -1;
    mainFunParam.argc = 1;

    //cnt arguments
    char* cptr = &cmdArgString[0];
    while (*cptr != 0)
    {
        if (*cptr == ' ')
        {
            mainFunParam.argc++;
        }
        cptr++;
    }
    mainFunParam.argv = (char**)malloc(sizeof(char*) * (unsigned int)mainFunParam.argc);
    assert(NULL != mainFunParam.argv);
    mainFunParam.argv[0] = "xosApplicaton";   //Set first string to archicture type since we do not have the application name here

    cptr                 = &cmdArgString[0];
    size_t i             = 1;
    mainFunParam.argv[i] = cptr;
    while (*cptr != 0)
    {
        if (*cptr != ' ')
        {
            cptr++;
            continue;
        }
        *cptr = 0;
        cptr++;
        i++;
        if (i < (size_t)mainFunParam.argc)
        {
            mainFunParam.argv[i] = cptr;
        }
    }

    // add a breakpoint to the next line to fill argv with the debugger
    configSTACK_DEPTH_TYPE stacksize = configMINIMAL_STACK_SIZE;
    if (configMINIMAL_STACK_SIZE < (4096 / sizeof(configSTACK_DEPTH_TYPE)))
    {
        stacksize = (4096 / sizeof(configSTACK_DEPTH_TYPE));
    }

    xTaskCreate(
        main_entryTsk,        /* Function that implements the task. */
        "main",               /* Text name for the task. */
        stacksize,            /* Stack size in words, not bytes. */
        (void*)&mainFunParam, /* Parameter passed into the task. */
        0,                    /* Priority at which the task is created. */
        NULL);                /* Used to pass out the created task's handle. */

    vTaskStartScheduler();
    exit(mainFunParam.ret); /* This will only work if FreeRTOS porting layer stops scheduler */
}
/*-----------------------------------------------------------*/

static void prvSetupHardware(void)
{
    int             xStatus;
    XScuGic_Config* pxGICConfig;

    /* Ensure no interrupts execute while the scheduler is in an inconsistent
    state.  Interrupts are automatically enabled when the scheduler is
    started. */
    taskDISABLE_INTERRUPTS();

    /* Obtain the configuration of the GIC. */
    pxGICConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);

    /* Install a default handler for each GIC interrupt. */
    xStatus = XScuGic_CfgInitialize(&xInterruptController, pxGICConfig, pxGICConfig->CpuBaseAddress);
    assert(xStatus == XST_SUCCESS);
    (void)xStatus; /* Remove compiler warning if configASSERT() is not defined. */

    /*
     * Connect the interrupt controller interrupt handler to the hardware
     * interrupt handling logic in the ARM processor.
     */
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                                 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                                 &xInterruptController);

    /*
     * Enable interrupts in the ARM
     */
    Xil_ExceptionEnable();

    taskENABLE_INTERRUPTS();
}
