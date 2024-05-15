
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "rcar-xos/osal/r_osal.h"

#define TEST_INTCTX_DEVICE_1    "rtdmac_control0_00"    // 1st device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2    "rtdmac_descriptor0_00" // 2nd device used to generate interrupt.
#define TEST_INTCTX_DEVICE_3    "rtdmac_control1_00"    // device use for testing in interrupt context.

/* Interrupt setting */
#define TEST_NUM_OF_BUFFERS                 2U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               64U // the buffer allocation aligment
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){2908, 0} // Dummy data that is the initialized value of allocated buffers
#define TEST_REG_RDMSAR                     0x0000 // source address
#define TEST_REG_RDMDAR                     0x0004 // destination address
#define TEST_REG_RDMTCR                     0x0008 // transfer count
#define TEST_REG_RDMCHCRB                   0x001C // DMA Channel Control Register B
#define TEST_REG_RDMCHCR                    0x000C // DMA channel control register for RTDMAC0
#define TEST_REG_RDMOR                      0x0060 // DMA operation register for RT-DMAC0
#define TEST_REG_CLEAR_REGISTER             0x0100 // Clear Register
#define TEST_TRANSFER_COUNT_VALUE           0x00000001U // 1 transfer
#define TEST_TRANSFER_STEP_VALUE            0x00000000U // 1 descriptor
#define TEST_TRANSFER_RDMCHCR_VALUE         0x10040415U // setting for DMA channel control register
#define TEST_TRANSFER_RDMOR_VALUE           0x0301U // setting for DMA operation register
#define TEST_CLEAR_REGISTER_VALUE           0x00000001U // write value of Clear Register
#define TEST_CALLBACK_MS_SLEEP_TIME         1000000U //Sleep time for callback function wait
#define TEST_CALLBACK_ACTIVE_TIMEOUT        1000000000L //Timeout value for callback function wait
#define TEST_COUNT                          10U // Total number of tests
/* Should be adjust the busy loop processing time is about 300[ms] as a guideline. */
#if defined(RCAR_S4)
#define TEST_CALLBACK_BUSY_LOOP_COUNT       400000U  //Busy loop count for callback function(Device1)
#elif defined(RCAR_V4H)   /* V4H */
#define TEST_CALLBACK_BUSY_LOOP_COUNT       4000000U //Busy loop count for callback function(Device1)
#else   /* V4M */
#define TEST_CALLBACK_BUSY_LOOP_COUNT       2000000U //Busy loop count for callback function(Device1)
#endif

typedef enum e_callback_state_t
{
    CALLBACK_RUN_BEFORE          = 0,      /* Callback function is not already running. */
    CALLBACK_RUNNING             = 1,      /* Callback function is running. */
    CALLBACK_RUN_AFTER           = 2,      /* Callback function is already finished. */
} e_callback_state_t;

extern int64_t g_osal_interrupt_enable_timeout;
e_callback_state_t g_callback_active; //Call back function state for synchronization

/* Definition irq_arg */
typedef struct st_callback_arg
{
    osal_cond_handle_t  cond_handle;
    osal_mutex_handle_t mtx_handle;
    osal_mq_handle_t    mq_handle;
    int32_t             send_msg;
}st_callback_arg_t;

/* Interrupt test */
int osal_test_interrupt(void);

/* Allocate Memory */
e_osal_return_t allocate_setup_data_memory(osal_memory_manager_handle_t* hndl_mmngr,
                                           uint32_t *hw_addr,
                                           osal_memory_buffer_handle_t* p_buffer_obj);

/* Deallocate Memory */
e_osal_return_t deallocate_data_memory(osal_memory_manager_handle_t hndl_mmngr,
                                       osal_memory_buffer_handle_t* buffer_obj);

/* CallBack Function */
void osal_isr_test_for_dev1(osal_device_handle_t dev_handle,
                            uint64_t irq_channel,
                            void *irq_arg)
{
    uintptr_t offset    = TEST_REG_CLEAR_REGISTER;
    uint32_t write_data = TEST_CLEAR_REGISTER_VALUE;
    uint64_t dummy;
    st_callback_arg_t* local_arg = (st_callback_arg_t*)irq_arg;
    uint32_t             init_clear_data;

    /* Start of call back function */
    g_callback_active = CALLBACK_RUNNING;

    /* Print to notify the start of the callback function */
    printf("Start callback function(Device1)\n");

    /* Disable interrupt */
    (void)R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);

    /* Clear the status register */
    (void)R_OSAL_IoWrite32(dev_handle, offset, write_data);

    /* Busy Loop */
    for (uint32_t i = 0U; TEST_CALLBACK_BUSY_LOOP_COUNT > i; i++)
    {
        (void)R_OSAL_IoRead32(dev_handle, TEST_REG_CLEAR_REGISTER, &init_clear_data);
    }

    /* End of call back function */
    g_callback_active = CALLBACK_RUN_AFTER;
}

void osal_isr_test_for_dev3(osal_device_handle_t dev_handle,
                            uint64_t irq_channel,
                            void *irq_arg)
{
    (void) irq_arg;

    /* Disable interrupt */
    (void)R_OSAL_InterruptDisableIsr(dev_handle, irq_channel);
}

/* Main Function */
int main(void)
{
    int         api_ret = 0;
    uint32_t    loop_count;
    unsigned int test_pass   = 0;
    unsigned int test_fail   = 0;
    unsigned int test_skip   = 0;

    /* Repeat several times because Interrupt timing changes for each execution */
    for (loop_count = 0U; (0 == api_ret) && (TEST_COUNT > loop_count); loop_count++)
    {
        printf("----- Start interrupt test (No.%u) -----\n", loop_count + 1);
        api_ret = osal_test_interrupt();
    }

    if (0 == api_ret)
    {
        test_pass++;
    }
    else
    {
        test_fail++;
    }
    printf("< OSAL Tests OK = %d, NG = %d, NT = %d>\n", test_pass, test_fail, test_skip);

    return api_ret;
}

/* Interrupt test */
int osal_test_interrupt(void)
{
    int                  api_ret = -1;
    e_osal_return_t      osal_ret;
    osal_device_handle_t device_handle;
    osal_device_handle_t device_handle2;
    osal_device_handle_t device_handle3;
    st_callback_arg_t    arg;
    uint32_t             hw_addr[TEST_NUM_OF_BUFFERS];
    osal_memory_manager_handle_t hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    osal_memory_buffer_handle_t  buffer_obj[TEST_NUM_OF_BUFFERS];

    int              os_ret;
    struct timespec  time_stamp;
    uint32_t         elapsed_time = 0U;

    /* Initialize callback state  */
    g_callback_active = CALLBACK_RUN_BEFORE;

    /* Set 500ms to the timeout value of R_OSAL_InterruptEnableIsr */
    g_osal_interrupt_enable_timeout = 500;

    osal_ret = R_OSAL_Initialize();
    if (OSAL_RETURN_OK == osal_ret)
    {
        /* Device Open */
        osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_1, &device_handle);
        if (OSAL_RETURN_OK != osal_ret)
        {
            printf("R_OSAL_IoDeviceOpen(DEVICE1) error: %d\n", osal_ret);
        }
        else
        {
            osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_2, &device_handle2);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle);
                printf("R_OSAL_IoDeviceOpen(DEVICE2) error: %d\n", osal_ret);
            }
            else
            {
                osal_ret = R_OSAL_IoDeviceOpen(TEST_INTCTX_DEVICE_3, &device_handle3);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    printf("R_OSAL_IoDeviceOpen(DEVICE3) error: %d\n", osal_ret);
                }
            }
        }

        /* SetUp */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_REQUIRED, true);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_IoDeviceClose(device_handle3);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                printf("R_OSAL_PmSetRequiredState() error: %d\n", osal_ret);
            }
        }

        /* Allocate read/write addresses */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = allocate_setup_data_memory(&hndl_mmngr, &hw_addr[0], &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle3);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
            }
        }

        /* Enable Device1 Interrupt */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = R_OSAL_InterruptRegisterIsr(device_handle, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test_for_dev1, (void *)&arg);
            if (OSAL_RETURN_OK != osal_ret)
            {
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle3);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                printf("R_OSAL_InterruptRegisterIsr(Device1) error: %d\n", osal_ret);
            }
            else
            {
                osal_ret = R_OSAL_InterruptEnableIsr(device_handle, 0);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle3);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    printf("R_OSAL_InterruptEnableIsr(Device1) error: %d\n", osal_ret);
                }
            }
        }

        /* SetUp HW register */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Config DMA operation register */
            (void)R_OSAL_IoWrite16(device_handle2, TEST_REG_RDMOR, TEST_TRANSFER_RDMOR_VALUE);
            /* Config transfer src address */
            (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMSAR, hw_addr[0]);
            /* Config transfer dst address */
            (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMDAR, hw_addr[1]);
            /* Config transfer count */
            (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMTCR, TEST_TRANSFER_COUNT_VALUE);
            /* Config transfer steps */
            (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCRB, TEST_TRANSFER_STEP_VALUE);
            /* Config DMA channel control register */
            (void)R_OSAL_IoWrite32(device_handle,  TEST_REG_RDMCHCR, TEST_TRANSFER_RDMCHCR_VALUE);
        }

        /* Enable Device3 Interrupt */
        if (OSAL_RETURN_OK == osal_ret)
        {
            if (CALLBACK_RUN_AFTER == g_callback_active)
            {
                (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
                (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle3);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
                (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                osal_ret = OSAL_RETURN_FAIL;
                printf("Call back function(Device1) is already finished.\n");
            }
            else
            {
                /* Main thread waits for callback function(Device1) to be called. */
                while ((CALLBACK_RUN_BEFORE == g_callback_active) && (OSAL_RETURN_OK == osal_ret) && (TEST_CALLBACK_ACTIVE_TIMEOUT > elapsed_time))
                {
                    os_ret = clock_gettime(CLOCK_MONOTONIC, &time_stamp);
                    if (0 != os_ret)
                    {
                        (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
                        (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                        (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                        (void)R_OSAL_IoDeviceClose(device_handle3);
                        (void)R_OSAL_IoDeviceClose(device_handle2);
                        (void)R_OSAL_IoDeviceClose(device_handle);
                        (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                        osal_ret = OSAL_RETURN_FAIL;
                    }
                    else
                    {
                        time_stamp.tv_nsec += (long)TEST_CALLBACK_MS_SLEEP_TIME;
                        elapsed_time++;
                        if (1000000000L < time_stamp.tv_nsec)
                        {
                            time_stamp.tv_sec++;
                            time_stamp.tv_nsec = time_stamp.tv_nsec - (1000000000L);
                        }
                        os_ret = clock_nanosleep(CLOCK_MONOTONIC, (int)TIMER_ABSTIME, &time_stamp, NULL);
                        if (0 != os_ret)
                        {
                            (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
                            (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                            (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                            (void)R_OSAL_IoDeviceClose(device_handle3);
                            (void)R_OSAL_IoDeviceClose(device_handle2);
                            (void)R_OSAL_IoDeviceClose(device_handle);
                            (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                            osal_ret = OSAL_RETURN_FAIL;
                        }
                    }
                }
                if (TEST_CALLBACK_ACTIVE_TIMEOUT <= elapsed_time)
                {
                    (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle3);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    osal_ret = OSAL_RETURN_FAIL;
                    printf("Timeout error: Call back function(Device1) is not already running.\n");
                }
            }

            if (OSAL_RETURN_OK == osal_ret)
            {
                osal_ret = R_OSAL_InterruptRegisterIsr(device_handle3, 0, OSAL_INTERRUPT_PRIORITY_TYPE0, osal_isr_test_for_dev3, (void *)&arg);
                if (OSAL_RETURN_OK != osal_ret)
                {
                    (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
                    (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                    (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                    (void)R_OSAL_IoDeviceClose(device_handle3);
                    (void)R_OSAL_IoDeviceClose(device_handle2);
                    (void)R_OSAL_IoDeviceClose(device_handle);
                    (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    printf("R_OSAL_InterruptRegisterIsr(Device3) error: %d\n", osal_ret);
                }
                else
                {
                    osal_ret = R_OSAL_InterruptEnableIsr(device_handle3, 0);
                    if (OSAL_RETURN_OK != osal_ret)
                    {
                        api_ret = -1;
                        printf("[INFO]Error R_OSAL_InterruptEnableIsr():%d\n", osal_ret);
                        (void)R_OSAL_InterruptUnregisterIsr(device_handle3, 0, osal_isr_test_for_dev3);
                        (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
                        (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
                        (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                        (void)R_OSAL_IoDeviceClose(device_handle3);
                        (void)R_OSAL_IoDeviceClose(device_handle2);
                        (void)R_OSAL_IoDeviceClose(device_handle);
                        (void)deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
                    }
                    else
                    {
                        api_ret = 0;
                        printf("[INFO]Success R_OSAL_InterruptEnableIsr()\n");
                    }
                }
            }
        }

        /* Disable Device1/3 Interrupt */
        if (OSAL_RETURN_OK == osal_ret)
        {
            /* Wait for callback function to finish */
            (void)sleep(10);

            (void)R_OSAL_InterruptDisableIsr(device_handle3, 0);
            (void)R_OSAL_InterruptUnregisterIsr(device_handle3, 0, osal_isr_test_for_dev3);
            (void)R_OSAL_InterruptDisableIsr(device_handle, 0);
            (void)R_OSAL_InterruptUnregisterIsr(device_handle, 0, osal_isr_test_for_dev1);
        }

        /* Deallocate read/write addresses */
        if (OSAL_RETURN_OK == osal_ret)
        {
            osal_ret = deallocate_data_memory(hndl_mmngr, &buffer_obj[0]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                api_ret = -1;
                (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
                (void)R_OSAL_IoDeviceClose(device_handle3);
                (void)R_OSAL_IoDeviceClose(device_handle2);
                (void)R_OSAL_IoDeviceClose(device_handle);
            }
        }

        /* TearDown */
        if (OSAL_RETURN_OK == osal_ret)
        {
            (void)R_OSAL_PmSetRequiredState(device_handle2, OSAL_PM_REQUIRED_STATE_RELEASED, true);
            (void)R_OSAL_IoDeviceClose(device_handle3);
            (void)R_OSAL_IoDeviceClose(device_handle2);
            (void)R_OSAL_IoDeviceClose(device_handle);
        }

        osal_ret = R_OSAL_Deinitialize();
        if (OSAL_RETURN_OK != osal_ret)
        {
            api_ret = -1;
            printf("R_OSAL_Deinitialize() error: %d\n", osal_ret);
        }
    }
    else
    {
        printf("R_OSAL_Initialize() error: %d\n", osal_ret);
    }

    return api_ret;
}

/* Allocate Memory */
e_osal_return_t allocate_setup_data_memory(osal_memory_manager_handle_t* hndl_mmngr,
                                           uint32_t *hw_addr,
                                           osal_memory_buffer_handle_t* p_buffer_obj)
{
    e_osal_return_t osal_ret;
    osal_memory_buffer_handle_t local_buffer_obj = OSAL_MEMORY_BUFFER_HANDLE_INVALID;
    osal_memory_manager_handle_t local_hndl_mmngr = OSAL_MEMORY_MANAGER_HANDLE_INVALID;
    uint32_t data;
    void *cpu_ptr[TEST_NUM_OF_BUFFERS];
    size_t size = 0;
    uintptr_t local_hw_addr = 0;
    st_osal_mmngr_config_t config;
    size_t num_of_max_entry = 0;

    /* Open Memory manager */
    (void)R_OSAL_MmngrGetOsalMaxConfig(&config);
    osal_ret = R_OSAL_MmngrOpen(&config, &local_hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("R_OSAL_MmngrOpen() error: %d\n", osal_ret);
    }
    else
    {
        *hndl_mmngr = local_hndl_mmngr;

        /* Allocate buffer for Display list */
        for(size_t i = 0; i < TEST_NUM_OF_BUFFERS; i++)
        {
            osal_ret = R_OSAL_MmngrAlloc(local_hndl_mmngr, TEST_BUFFER_SIZE, TEST_BUFFER_ALIGNMENT, &local_buffer_obj);
            if (OSAL_RETURN_OK != osal_ret)
            {
                printf("R_OSAL_MmngrAlloc() error: %d\n", osal_ret);
                break;
            }
            *(p_buffer_obj + i) = local_buffer_obj;

            /* Get HW address of buffer */
            osal_ret = R_OSAL_MmngrGetHwAddr(local_buffer_obj, OSAL_AXI_BUS_ID_IPA_MAIN_MEMORY, &local_hw_addr);
            if (OSAL_RETURN_OK != osal_ret)
            {
                printf("R_OSAL_MmngrGetHwAddr() error: %d\n", osal_ret);
                break;
            }
            hw_addr[i] = (uint32_t)local_hw_addr;

            /* Get CPU accessible pointer of the buffer */
            cpu_ptr[i] = NULL;
            osal_ret = R_OSAL_MmngrGetCpuPtr(local_buffer_obj, &cpu_ptr[i]);
            if (OSAL_RETURN_OK != osal_ret)
            {
                printf("R_OSAL_MmngrGetCpuPtr() error: %d\n", osal_ret);
                break;
            }

            /* Write display list data to the beginning of the buffer */
            data = TEST_DUMMY_BUFFER_DATA[i];
            (void)memcpy(cpu_ptr[i], (void*)&data, sizeof(uint32_t));
            osal_ret = R_OSAL_MmngrGetSize(local_buffer_obj, &size);
            if (OSAL_RETURN_OK != osal_ret)
            {
                printf("R_OSAL_MmngrGetSize() error: %d\n", osal_ret);
                break;
            }

            osal_ret = R_OSAL_MmngrFlush(local_buffer_obj, 0 , size);
            if (OSAL_RETURN_OK != osal_ret)
            {
                printf("R_OSAL_MmngrFlush() error: %d\n", osal_ret);
                break;
            }
        }

        if (OSAL_RETURN_OK != osal_ret)
        {
            (void)R_OSAL_MmngrClose(local_hndl_mmngr);
        }
    }

    return osal_ret;
}

/* Deallocate Memory */
e_osal_return_t deallocate_data_memory(osal_memory_manager_handle_t hndl_mmngr,
                                       osal_memory_buffer_handle_t* buffer_obj)
{
    e_osal_return_t osal_ret;

    for(int i = TEST_NUM_OF_BUFFERS - 1; i >= 0; i--)
    {
        osal_ret = R_OSAL_MmngrDealloc(hndl_mmngr, buffer_obj[i]);
        if (OSAL_RETURN_OK != osal_ret)
        {
            printf("R_OSAL_MmngrDealloc() error: %d\n", osal_ret);
            break;
        }
    }

    osal_ret = R_OSAL_MmngrClose(hndl_mmngr);
    if (OSAL_RETURN_OK != osal_ret)
    {
        printf("R_OSAL_MmngrClose() error: %d\n", osal_ret);
    }

    return osal_ret;
}
