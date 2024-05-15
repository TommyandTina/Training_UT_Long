/* Scheduler include files. */
#include "FreeRTOS.h"
#include "rcar-xos/xil_common/xil_printf.h"
#include "task.h"

/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName);
void vApplicationTickHook(void);

void vApplicationMallocFailedHook(void)
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    xil_printf("%s: %s\n", __FILE__, __func__);
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;

    /* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
    xil_printf("%s: %s\n", __FILE__, __func__);
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
#if F_FS_THREAD_AWARE == 1
    {
        static BaseType_t xCreatedSampleFiles = pdFALSE;
        if (xCreatedSampleFiles == pdFALSE)
        {
            vCreateAndVerifySampleFiles();
            xCreatedSampleFiles = pdTRUE;
        }
    }
#endif
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
#if (mainSELECTED_APPLICATION == 1)
    {
        /* Only the comprehensive demo actually uses the tick hook. */
        extern void vFullDemoTickHook(void);
        vFullDemoTickHook();
    }
#endif
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
used by the Idle task. */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer, StackType_t** ppxIdleTaskStackBuffer, uint32_t* pulIdleTaskStackSize)
{
    /* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t  uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
	state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer, StackType_t** ppxTimerTaskStackBuffer, uint32_t* pulTimerTaskStackSize)
{
    /* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t  uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
	task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
	Note that, as the array is necessarily of type StackType_t,
	configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/*-----------------------------------------------------------*/

void FreeRTOS_Abort(void)
{
    const char* exceptionDescrPtr = NULL;
    uint64_t    esr, exClass;
    __asm__ volatile("mov %0, x0"
                     : "=r"(esr)
                     :
                     : "cc");
    __asm__ volatile("mov %0, x1"
                     : "=r"(exClass)
                     :
                     : "cc");
    xil_printf("FreeRTOS_Abort\n ESR Value: 0x%x\n Exception Class: 0x%x\n", esr, exClass);

    switch (exClass)
    {
        case 0:
            exceptionDescrPtr = "Unknown reason";
            break;
        case 1:
            exceptionDescrPtr = "Trapped WFI or WFE instruction execution. Conditional WFE and WFI instructions that fail their condition code check do not cause an exception.";
            break;
        case 3:
            exceptionDescrPtr = "Trapped MCR or MRC access with (coproc==1111) that is not reported using EC 0b000000.";
            break;
        case 4:
            exceptionDescrPtr = "Trapped MCRR or MRRC access with (coproc==1111) that is not reported using EC 0b000000.";
            break;
        case 5:
            exceptionDescrPtr = "Trapped MCR or MRC access with (coproc==1110).";
            break;
        case 6:
            exceptionDescrPtr = "Trapped LDC or STC access.\n		The only architected uses of these instructions are:\n		- An STC to write data to memory from DBGDTRRXint.\n		- An LDC to read data from memory to DBGDTRTXint.";
            break;
        case 7:
            exceptionDescrPtr = "Access to an Advanced SIMD or floating-point functionality trapped by CPACR_EL1.FPEN, CPTR_EL2.TFP, or CPTR_EL3.TFP control. Excludes exceptions resulting from CPACR_EL1 when the value of HCR_EL2.TGE is 1, or because Advanced SIMD and floating-point are not implemented. These are reported with EC value 0b000000 as described in Reporting the EC encoding when an exception is routed to EL2 on page D1-1535.";
            break;
        case 8:
            exceptionDescrPtr = "Trapped VMRS access, from ID group trap, that is not reported using EC 0b000111. This value is valid for ESR_EL2.";
            break;
        case 12:
            exceptionDescrPtr = "Trapped MRRC access with (coproc==1110).";
            break;
        case 14:
            exceptionDescrPtr = "Illegal Execution state.";
            break;
        case 17:
            exceptionDescrPtr = "SVC instruction execution in AArch32 state. This is reported in ESR_EL2 only when the exception is generated because the value of HCR_EL2.TGE is 1.";
            break;
        case 18:
            exceptionDescrPtr = "HVC instruction execution in AArch32 state, when HVC is not disabled.";
            break;
        case 19:
            exceptionDescrPtr = "SMC instruction execution in AArch32 state, when SMC is not disabled. This is reported in ESR_EL2 only when the exception is generated because the value of HCR_EL2.TSC is 1.";
            break;
        case 21:
            exceptionDescrPtr = "SVC instruction execution in AArch64 state.";
            break;
        case 22:
            exceptionDescrPtr = "HVC instruction execution in AArch64 state, when HVC is not disabled.";
            break;
        case 23:
            exceptionDescrPtr = "SMC instruction execution in AArch64 state, when SMC is not disabled. This is reported in ESR_EL2 only when the exception is generated because the value of HCR_EL2.TSC is 1.";
            break;
        case 24:
            exceptionDescrPtr = "Trapped MSR, MRS or System instruction execution in AArch64 state, that is not reported using EC 0b000000, 0b000001 or 0b000111. This include all instructions that cause exceptions that are part of the encoding space defined in System instruction class encoding overview on page C5-271, except for those exceptions reported using EC values 0b000000, 0b000001, or 0b000111.";
            break;
        case 31:
            exceptionDescrPtr = "IMPLEMENTATION DEFINED exception to EL3.";
            break;
        case 32:
            exceptionDescrPtr = "Instruction Abort from a lower Exception level. Used for MMU faults generated by instruction accesses and Synchronous external aborts, including synchronous parity or ECC errors. Not used for debug related exceptions.";
            break;
        case 33:
            exceptionDescrPtr = "Instruction Abort taken without a change in Exception level. Used for MMU faults generated by instruction accesses and Synchronous external aborts, including synchronous parity or ECC errors. Not used for debug related exceptions.";
            break;
        case 34:
            exceptionDescrPtr = "PC alignment fault exception.";
            break;
        case 36:
            exceptionDescrPtr = "Data Abort from a lower Exception level. Used for MMU faults generated by data accesses, alignment faults other than those caused by the Stack Pointer misalignment, and Synchronous external aborts, including synchronous parity or ECC errors. Not used for debug related exceptions.";
            break;
        case 37:
            exceptionDescrPtr = "Data Abort taken without a change in Exception level. Used for MMU faults generated by data accesses, alignment faults other than those caused by the Stack Pointer misalignment, and Synchronous external aborts, including synchronous parity or ECC errors. Not used for debug related exceptions. This value is valid for all described registers.";
            break;
        case 38:
            exceptionDescrPtr = "SP alignment fault exception.";
            break;
        case 40:
            exceptionDescrPtr = "Trapped floating-point exception taken from AArch32 state. Whether this Exception class is supported is IMPLEMENTATION DEFINED.";
            break;
        case 44:
            exceptionDescrPtr = "Trapped floating-point exception taken from AArch64 state. Whether this Exception class is supported is IMPLEMENTATION DEFINED.";
            break;
        case 47:
            exceptionDescrPtr = "SError interrupt.";
            break;
        case 48:
            exceptionDescrPtr = "Breakpoint exception from a lower Exception level.";
            break;
        case 49:
            exceptionDescrPtr = "Breakpoint exception taken without a change in Exception level.";
            break;
        case 50:
            exceptionDescrPtr = "Software Step exception from a lower Exception level.";
            break;
        case 51:
            exceptionDescrPtr = "Software Step exception taken without a change in Exception level.";
            break;
        case 52:
            exceptionDescrPtr = "Watchpoint exception from a lower Exception level.";
            break;
        case 53:
            exceptionDescrPtr = "Watchpoint exception taken without a change in Exception level.";
            break;
        case 56:
            exceptionDescrPtr = "BKPT instruction execution in AArch32 state.";
            break;
        case 58:
            exceptionDescrPtr = "Vector Catch exception from AArch32 state. The only case where a Vector Catch exception is taken to an Exception level that is using AArch64 is when the exception is routed to EL2 and EL2 is using AArch64.";
            break;
        case 60:
            exceptionDescrPtr = "BRK instruction execution in AArch64 state. This is reported in ESR_EL3 only if a BRK instruction is executed at EL3.";
            break;
        default:
            exceptionDescrPtr = "U0b100101ndefined or invalid exception class.";
            break;
    };

    xil_printf("Description:\n%s\n", exceptionDescrPtr);

    uint64_t iss = esr & 0x1FFFFFF;
    if (exClass == 36 || exClass == 37)   //Further Information can be provided for Data Abort
    {
        if ((iss & (0x1 << 24)) > 0)
        {
            const char* sasBitDesc[] = {"   SAS=00\tByte", "   SAS=01\tHalfword", "   SAS=10\tWord", "   SAS=11\tDoubleWord"};
            xil_printf("ISV  1\n");
            xil_printf("   SAS - Syndrome Access Size. When ISV is 1, indicates the size of the access attempted by the faulting operation.\n");
            xil_printf("   %s\n", sasBitDesc[((iss >> 22) & 0x3)]);
            xil_printf("   SSE - Syndrome Sign Extend. When ISV is 1, for a byte, halfword, or word load operation, indicates whether the data item must be sign extended.\n");
            xil_printf("      SSE = %s\n", (iss >> 21 & 0x1) ? "Data Item must be sign-extended" : "Sign-Extension not required");
            xil_printf("   SRT - Syndrome Register transfer. When ISV is 1, the register number of the Rt operand of the faulting instruction.\n");
            xil_printf("      SRT = %u\n", iss >> 16 & 15);
            xil_printf("   SF - Width of the register accessed by the instruction is Sixty-Four.\n");
            xil_printf("      SF = %u\t Instruction loads/stores a %u-bit wide register.\n", (iss >> 15 & 0x1), (iss >> 15 & 0x1) ? 64 : 32);
            xil_printf("   AR - Acquire/Release\n");
            if (iss & (0x1 << 14))
                xil_printf("      AR = 1\tInstruction did have acquire/release semantics.\n");
            else
                xil_printf("      AR = 0\tInstruction did not have acquire/release semantics.\n");
        }
        else
            xil_printf("ISV\t0\t\n");
        if (iss & 0x1 << 10)
            xil_printf("FnV=1 (FAR not Valid, for a Synchronous external abort other than a Synchronous external abort on a translation table walk.)\n");
        xil_printf("EA=%d (External abort type.)\n", (iss & (0x1 << 9)) > 0);
        xil_printf("CM=%d (Cache maintenance)\n", (iss & (0x1 << 8)) > 0);
        if (iss & (0x1 << 8))
            xil_printf("   CM=1\tThe Data Abort was generated by either the execution of a cache maintenance instruction or by a synchronous fault on the execution of an address translation instruction. The DC ZVA instruction is not classified as a cache maintenance instruction, and therefore its execution cannot cause this field to be set to 1.\n");
        xil_printf("S1PTW=%d \n", (iss & (0x1 << 7)) > 0);

        if (iss & (0x1 << 8))
            xil_printf("WnR=1\tAbort caused by a write instruction\n");
        else
            xil_printf("WnR=0\tAbort caused by a read instruction\n");

        xil_printf("DFSC=0x%x Data Fault Status Code\n", iss & 0x1F);
        if ((iss & 0x3C) == 0)
            xil_printf("   Address size fault, level %d\n", iss & 0x3);
        else if ((iss & 0x3C) == 0x1 << 2)
            xil_printf("   Translation fault, level %d\n", iss & 0x3);
        else if ((iss & 0x3C) == 0x2 << 2)
            xil_printf("   Access flag fault, level %d\n", iss & 0x3);
        else if ((iss & 0x3C) == 0x3 << 2)
            xil_printf("   Permission fault, level %d\n", iss & 0x3);
        else if ((iss & 0x3F) == 16)
            xil_printf("   Synchronous external abort, not on translation table walk\n");
        else if ((iss & 0x3F) == 24)
            xil_printf("   Synchronous parity or ECC error on memory access, not on translation table walk\n");
        else if ((iss & 0x3C) == 0x5 << 2)
            xil_printf("   Synchronous external abort, on translation table walk, level %d\n", iss & 0x3);
        else if ((iss & 0x3C) == 28)
            xil_printf("   Synchronous parity or ECC error on memory access on translation table walk, level %d\n", iss & 0x3);
        else if ((iss & 0x3F) == 33)
            xil_printf("   Alignment fault\n");
        else if ((iss & 0x3F) == 48)
            xil_printf("   TLB conflict abort\n");
        else if ((iss & 0x3F) == 52)
            xil_printf("   IMPLEMENTATION DEFINED fault (Lockdown fault)\n");
        else if ((iss & 0x3F) == 53)
            xil_printf("   IMPLEMENTATION DEFINED fault (Unsupported Exclusive access fault)\n");
        else if ((iss & 0x3F) == 61)
            xil_printf("   Section Domain Fault, used only for faults reported in the PAR_EL1\n");
        else if ((iss & 0x3F) == 62)
            xil_printf("   Page Domain Fault, used only for faults reported in the PAR_EL1\n");
        else
            xil_printf("   RESERVED\n");
    }

    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

//! Private function for Assert Error Output
void vMainAssertCalled(const char* pcFileName, uint32_t ulLineNumber)
{
    xil_printf("ASSERT!  Line %lu of file %s\r\n", ulLineNumber, pcFileName);
    taskENTER_CRITICAL();
    for (;;)
        ;
}
