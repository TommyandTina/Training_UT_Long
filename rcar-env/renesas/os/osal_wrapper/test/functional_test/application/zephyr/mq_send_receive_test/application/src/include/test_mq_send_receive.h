#ifndef TEST_MQ_SEND_RECEIVE_H
#define TEST_MQ_SEND_RECEIVE_H

#include <stdio.h>
#include "rcar-xos/osal/r_osal.h"

/**********************************************************/
/*                     Definition                         */
/**********************************************************/
#define OSAL_MQ_TEST_RESULT_OK              0
#define OSAL_MQ_TEST_RESULT_FAIL            1

#define OSAL_MQ_SEND_MSG_DATA               0x1111
#define OSAL_MQ_MSG_DATA_LEN                4

#define OSAL_MQ_PROCESS_NUM                 4

/* MQ test function */
#define OSAL_MQ_FUNCTION_RECEIVE            0
#define OSAL_MQ_FUNCTION_SEND               1
#define OSAL_MQ_FUNCTION_RESET              2
#define OSAL_MQ_FUNCTION_MAX                3

#define OSAL_MQ_PROCESS_RECEIVE_THREAD      0
#define OSAL_MQ_PROCESS_RECEIVE_ISR         1
#define OSAL_MQ_PROCESS_SEND_THREAD         2
#define OSAL_MQ_PROCESS_SEND_ISR            3
#define OSAL_MQ_PROCESS_RESET               4
#define OSAL_MQ_PROCESS_MAX                 5

/* MQ test context */
#define OSAL_MQ_CONTEXT_THREAD_HIGH         0
#define OSAL_MQ_CONTEXT_THREAD_MIDDLE       1
#define OSAL_MQ_CONTEXT_THREAD_LOW          2
#define OSAL_MQ_CONTEXT_ISR_HIGH            3
#define OSAL_MQ_CONTEXT_ISR_MIDDLE          4
#define OSAL_MQ_CONTEXT_ISR_LOW             5
#define OSAL_MQ_CONTEXT_MAX                 6

#define TEST_IMR_REG_DLSAR                  0x0030 // DL Start Address Register
#define TEST_IMR_REG_IMR                    0x0018 // Interrupt Mask Register
#define TEST_IMR_REG_CR                     0x0008 // Control Register
#define TEST_REG_CLEAR_REGISTER             0x0010 // Status Clear Register

#define TEST_IMR_IMR_VALUE                  0x00000000U // write value of Interrupt Mask Register
#define TEST_IMR_CR_VALUE                   0x00000001U // write value of Control Register
#define TEST_CLEAR_REGISTER_VALUE           0x00000007U // write value of Status Clear Register

#define TEST_NUM_OF_BUFFERS                 4U // the number of buffers need to be allocate for HW interrupt generation
#define TEST_BUFFER_SIZE                    64U
#define TEST_BUFFER_ALIGNMENT               64U
#define TEST_DUMMY_BUFFER_DATA_1            0x8f000000U
#define TEST_DUMMY_BUFFER_DATA_2            0U
#define TEST_BUFFER_ADDR_INDEX_1            0U
#define TEST_BUFFER_ADDR_INDEX_2            8U

#define TIMEOUT_MS                          5 // 1000 millisecond
#define THREAD_STACK_SIZE                   0x1000 // 4KB

#define OSAL_RESOURCE_ID                    0xf000U
#define RESORCE_ID_IMR                      0x1000U
#define RESORCE_ID_ICCOM                    0x3000U
#define RESORCE_ID_OSAL                     0xf000U
/* mutex id */
#define OSAL_MUTEX_OSAL_00                  (RESORCE_ID_OSAL  +  0U)
#define OSAL_MUTEX_OSAL_01                  (RESORCE_ID_OSAL  +  1U)
/* condition variable id */
#define OSAL_COND_OSAL_00                   (RESORCE_ID_OSAL +  0U)
#define OSAL_COND_OSAL_01                   (RESORCE_ID_OSAL +  1U)
/* message queue id */
#define OSAL_MQ_MSG_ID_01                   OSAL_RESOURCE_ID + 0U
/* thread id */
#define OSAL_THREAD_OSAL_00                 (RESORCE_ID_OSAL  +  0U)
#define OSAL_THREAD_OSAL_01                 (RESORCE_ID_OSAL  +  1U)
#define OSAL_THREAD_OSAL_02                 (RESORCE_ID_OSAL  +  2U)
#define OSAL_THREAD_OSAL_03                 (RESORCE_ID_OSAL  +  3U)
#define OSAL_THREAD_OSAL_04                 (RESORCE_ID_OSAL  +  4U)
#define OSAL_THREAD_OSAL_05                 (RESORCE_ID_OSAL  +  5U)
#define OSAL_THREAD_OSAL_06                 (RESORCE_ID_OSAL  +  6U)
#define OSAL_THREAD_OSAL_07                 (RESORCE_ID_OSAL  +  7U)
#define OSAL_THREAD_OSAL_08                 (RESORCE_ID_OSAL  +  8U)
#define OSAL_THREAD_OSAL_09                 (RESORCE_ID_OSAL  +  9U)
#define OSAL_THREAD_IMR_TASK00              (RESORCE_ID_IMR   +  0U)
#define OSAL_THREAD_IMR_TASK01              (RESORCE_ID_IMR   +  1U)
#define OSAL_THREAD_IMR_TASK02              (RESORCE_ID_IMR   +  2U)

/**********************************************************/
/*                     Structure                         */
/**********************************************************/
typedef struct st_arg_mq_context {
    osal_milli_sec_t time;
    osal_mq_handle_t mq_handle;
    int function;
    int index;
    int priority;
} st_arg_mq_context_t;

typedef struct st_arg_main_thread {
    uint32_t idx_process[OSAL_MQ_PROCESS_NUM];
    uint32_t idx_pri_context[OSAL_MQ_PROCESS_NUM];
    uint32_t priority_tbl[OSAL_MQ_PROCESS_NUM];
} st_arg_main_thread_t;

#endif
