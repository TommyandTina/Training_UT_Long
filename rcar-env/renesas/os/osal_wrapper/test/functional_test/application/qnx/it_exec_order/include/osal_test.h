
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"

/* Define */
#define TOTAL_TEST_COUNT            (24)      // Total number of tests
#define OSAL_TEST_OK                (0)
#define OSAL_TEST_NG                (1)
#define OSAL_TEST_OPT_INFO_BUFFER   (150)

#define TEST_CALLBACK_MS_SLEEP_TIME         (1000000U)         // Sleep time for callback function wait
#define TEST_CALLBACK_ACTIVE_TIMEOUT        (1000000000L)      // Timeout value for callback function wait

/* Test Thread ID */
#define OSAL_TEST_THREAD_ID     (0x3000U)
/* Test Device ID */
#define TEST_INTCTX_DEVICE_1    "rtdmac_control0_00"    // 1st device used to generate interrupt and use for testing in interrupt context.
#define TEST_INTCTX_DEVICE_2    "rtdmac_descriptor0_00" // 2nd device used to generate interrupt.
/* Interrupt setting */
#define TEST_NUM_OF_BUFFERS                 (2U)                    // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    (64U)                   // the buffer allocation size
#define TEST_BUFFER_ALIGNMENT               (64U)                   // the buffer allocation aligment
#define TEST_DUMMY_BUFFER_DATA              (uint32_t[]){2908, 0}   // Dummy data that is the initialized value of allocated buffers
#define TEST_REG_RDMSAR                     (0x0000)                // source address
#define TEST_REG_RDMDAR                     (0x0004)                // destination address
#define TEST_REG_RDMTCR                     (0x0008)                // transfer count
#define TEST_REG_RDMCHCRB                   (0x001C)                // DMA Channel Control Register B
#define TEST_REG_RDMCHCR                    (0x000C)                // DMA channel control register for RTDMAC0
#define TEST_REG_RDMOR                      (0x0060)                // DMA operation register for RT-DMAC0
#define TEST_REG_CLEAR_REGISTER             (0x0100)                // Clear Register
#define TEST_TRANSFER_COUNT_VALUE           (0x00000001U)           // 1 transfer
#define TEST_TRANSFER_STEP_VALUE            (0x00000000U)           // 1 descriptor
#define TEST_TRANSFER_RDMCHCR_VALUE         (0x10040415U)           // setting for DMA channel control register
#define TEST_TRANSFER_RDMOR_VALUE           (0x0301U)               // setting for DMA operation register
#define TEST_CLEAR_REGISTER_VALUE           (0x00000001U)           // write value of Clear Register
#define TEST_MAX_TV_NSEC                    (1000000000L - 1L)      //Max value of nano second

/* Should be adjust the busy loop processing time is about 10[ms] as a guideline. */
#if defined(RCAR_S4)
#define TEST_CALLBACK_BUSY_LOOP_COUNT       (155000U)         // Busy loop count for user function
#define TEST_ISR_BUSY_LOOP_COUNT            (52000U)          // Busy loop count for callback function
#elif defined(RCAR_V4H)
#define TEST_CALLBACK_BUSY_LOOP_COUNT       (130000U)         // Busy loop count for user function
#define TEST_ISR_BUSY_LOOP_COUNT            (125000U)         // Busy loop count for callback function
#else    /* V4M */
#define TEST_CALLBACK_BUSY_LOOP_COUNT       (65000U)          // Busy loop count for user function
#define TEST_ISR_BUSY_LOOP_COUNT            (62500U)          // Busy loop count for callback function
#endif

/* enum for Call back function state for synchronization */
typedef enum e_function_state_t
{
    FUNCTION_RUN_BEFORE = 0,      /* User/Callback function is not already running. */
    FUNCTION_RUNNING,             /* User/Callback function is running. */
    FUNCTION_RUN_AFTER,           /* User/Callback function is already finished. */
} e_function_state_t;

/* enum for test case index */
typedef enum e_test_case_number_t
{
    TEST_CASE_01 = 1,
    TEST_CASE_02,
    TEST_CASE_03,
    TEST_CASE_04,
    TEST_CASE_05,
    TEST_CASE_06,
    TEST_CASE_07,
    TEST_CASE_08,
    TEST_CASE_09,
    TEST_CASE_10,
    TEST_CASE_11,
    TEST_CASE_12,
    TEST_CASE_13,
    TEST_CASE_14,
    TEST_CASE_15,
    TEST_CASE_16,
    TEST_CASE_17,
    TEST_CASE_18,
    TEST_CASE_19,
    TEST_CASE_20,
    TEST_CASE_21,
    TEST_CASE_22,
    TEST_CASE_23,
    TEST_CASE_24,
} e_test_case_number_t;

/* Macro */
/* Additional information create macro for output of evaluation results. */
#define OSAL_TEST_MAKE_INFO(opt_info, exp_str, ret_str) \
    sprintf(opt_info, "Expectation value: %s, Return value: %s", exp_str, ret_str)

/* ErrorLog output macro */
#define OSAL_TEST_ERR(fmt, ...) \
    fprintf(stderr, \
            "error: [%s][%s][line.%d][erro code: %s] " fmt "\n", \
            __FILE__, __func__, __LINE__, __VA_ARGS__);
