
#ifndef OSAL_TEST_H
#define OSAL_TEST_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcar-xos/osal/r_osal.h"

/* Define */
#define OSAL_TEST_ALL_EXEC (100)
#define OSAL_TEST_APP_EXIT (999)

#define OSAL_TEST_OK (0)
#define OSAL_TEST_NG (1)

#define OSAL_TEST_OPT_INFO_BUFFER   (150)

/* enum */
/* OSAL Manager ID */
typedef enum
{
    OSAL_TEST_RETURN_CONFIRMATION = 11,
    OSAL_TEST_TIMER_ACCURACY,
    OSAL_TEST_ALL             = OSAL_TEST_ALL_EXEC,
    OSAL_TEST_EXIT            = OSAL_TEST_APP_EXIT,
    OSAL_TEST_PARENT_MAX      = OSAL_TEST_TIMER_ACCURACY,
} OSAL_TEST_PARENT_ID;

/* OSAL Manager API ID */
typedef enum
{
/* Return value confirmation */
    OSAL_TEST_RETURN_THREAD_ID = 1,
    OSAL_TEST_RETURN_THREAD_ID_WITHOUT_DEF,
    OSAL_TEST_RETURN_MUTEX_ID,
    OSAL_TEST_RETURN_MUTEX_ID_WITHOUT_DEF,
    OSAL_TEST_RETURN_COND_ID,
    OSAL_TEST_RETURN_COND_ID_WITHOUT_DEF,
    OSAL_TEST_RETURN_MQ_ID,
    OSAL_TEST_RETURN_MQ_ID_WITHOUT_DEF,
    OSAL_TEST_RETURN_DEVICE_ID,
    OSAL_TEST_RETURN_DEVICE_ID_WITHOUT_DEF,
#if !defined(NDEBUG) || defined(R_OSAL_ENABLE_DEBUG)
    OSAL_TEST_RETURN_MMNGR_DEBUG_PRINT,
    OSAL_TEST_RETURN_BUFFER_DEBUG_PRINT,
#endif
    OSAL_TEST_RETURN_MMNGR_OPEN = 13,
    OSAL_TEST_RETURN_MMNGR_OPEN_REGION_IDX,
    OSAL_TEST_RETURN_INT_NUM_ERROR_HANDLE,
    OSAL_TEST_RETURN_INT_NUM_BROKEN_HANDLE,
    OSAL_TEST_RETURN_IO_GET_AXIBUSNAME_OUT_RANGE,

/* Timer accuracy check */
    OSAL_TEST_TIMESTAMP_HIGH_RES = 1,
    OSAL_TEST_TIMESTAMP_HIGH_RES_FLOAT,
    OSAL_TEST_TIMESTAMP_HIGH_PRE,
    OSAL_TEST_TIMESTAMP_HIGH_PRE_FLOAT,
    OSAL_TEST_TIMEPERIOD_HIGH_RES,
    OSAL_TEST_TIMEPERIOD_HIGH_PRE,

/* Maximum ID of Manager */
    OSAL_TEST_RETURN_MAX     = OSAL_TEST_RETURN_IO_GET_AXIBUSNAME_OUT_RANGE,
    OSAL_TEST_TIMER_MAX      = OSAL_TEST_TIMEPERIOD_HIGH_PRE,
} OSAL_TEST_API_ID;

/* Macro */
/* ErrorLog output macro */
#define OSAL_TEST_ERR(fmt, ...) \
    fprintf(stderr, \
            "error: [%s][%s][line.%d] %02d_%03d_%03d: " fmt "\n", \
            __FILE__, __func__, __LINE__, __VA_ARGS__); \
            test_err = -1;

/* Additional information create macro for output of evaluation results. */
#define OSAL_TEST_MAKE_INFO(opt_info, exp_str, ret_str) \
    sprintf(opt_info, "Expectation value: %s, Return value: %s", exp_str, ret_str)

#define OSAL_TEST_MAKE_TIMER(opt_info, exp_time, res_time) \
    sprintf(opt_info, "Expectation time >= %d [nsec], Result time: %d [nsec]", exp_time, res_time)

/* prototype */
void osal_test_result_output(int parent, int api_id, int result, char *param);
void osal_test_result_timer_output(int parent, int api_id, int result, int count, char *param,
                                   st_osal_time_t *before_timestmp, st_osal_time_t *after_timestmp);
char *osal_ret_to_str(e_osal_return_t ret_val);

int osal_test(int parent_number, int child_number);
void osal_test_return_confirmation_display_menu(void);
void osal_test_return_confirmation_menu(int child);
void osal_test_timer_accuracy_check_display_menu(void);
void osal_test_timer_accuracy_check_menu(int child);

extern unsigned int test_pass;
extern unsigned int test_fail;
extern unsigned int test_skip;
extern int          test_err;

#endif  /* OSAL_TEST_H */
